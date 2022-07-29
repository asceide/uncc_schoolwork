/*
*   This is an MPI implementation of the parallel histogram that we wrote earlier this semester. I can make this code less cluttered by adding a user input function sure, and maybe some less Bcast and barriers. But it werks now.
* 
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> //For threading
#include <time.h> //In order to use the time as a seed for the RNG
#include <math.h>
#include "./my_rand.h" //To generate random numbers

void make_data(float min_meas, float max_meas, int rank, int* localc, int* locald, float* dataf);//Function to generate the data based on random numbers
void calc_mpi_data(int* recv_counts, int* displ, int comm, int datac);
void make_bins(float min_meas, float max_meas, int binCount, float* binm);
void bin_alloc(int rank, int data_count, int thread_count, int bin_count, float min_meas, float* data, int* bins, float* binmaxes);
int find_bin(float data_point, float min_meas, int bin_count, float* bin_max);
int main(){
    int my_rank, comm_size, data_count, bin_count;
    float minme, maxme;
    int* recv_cnt;
    int* disp;
    
    float* data;
    int* bins;
    float* bin_max;


    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    //We only need the first rank to ask the user for input. Otherwise this would be spammed
    if(my_rank==0){
        printf("Please input the number of data: \n");
        scanf("%d",&data_count);
        printf("Please input the min data measurement you want: \n");
        scanf("%f",&minme);
        printf("Please input the max data measurement you want: \n");
        scanf("%f",&maxme);
        printf("Please input the number of bins: \n");
        scanf("%d",&bin_count);
    }
    //Unfortunately this means that only the root process would have the needed information from the user, so this information needs to be broadcasted to all the other processes.
    MPI_Bcast(&data_count, 1, MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&minme,1,MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Bcast(&maxme,1,MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Bcast(&bin_count,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    //Allocate the arrays needed for this program
    recv_cnt=calloc(comm_size,sizeof(int));//For the array length for each thread
    disp=calloc(comm_size,sizeof(int));//Memory displacement for Gatherv
    data=calloc(data_count,sizeof(float));
    bin_max=calloc(bin_count,sizeof(float));
    bins=calloc(bin_count,sizeof(int));

    MPI_Barrier(MPI_COMM_WORLD);

    if(my_rank==0){
        calc_mpi_data(recv_cnt,disp,comm_size, data_count);//Calculate the array lengths and displacements needed for Gatherv and scatterv
    }
    MPI_Bcast(recv_cnt, comm_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(disp, comm_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    make_data(minme,maxme,my_rank,recv_cnt,disp,data);
    MPI_Bcast(data, data_count, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if(my_rank==0){
        make_bins(minme,maxme,bin_count,bin_max);
    }
    MPI_Bcast(bin_max, bin_count, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    bin_alloc(my_rank, data_count, comm_size, bin_count, minme, data, bins, bin_max);
    MPI_Barrier(MPI_COMM_WORLD);
    if(my_rank==0){
        printf("\nData Points: ");
        for(int i=0; i<data_count; i++){
           printf(" %.1f,", *(data+i));
       }
   
        printf("\nBin Maxes: ");
        for(int i=0; i<bin_count; i++){
            printf(" %.1f,", *(bin_max+i));
        }
        float min;
        float max;
        for(int i=0; i<bin_count; i++){//output to notify number of elements per bin range
            if(i==0){//for the first iteration the minimum measurement
                min=minme;
            } else//otherwise it would be the max measurement for the previous bin
            min=*(bin_max+(i-1));
            max=*(bin_max+i);//max measurement for that bin.
            printf("\nFor Bin number: %d, ranges %.1f-%.1f, there are: %d values in it. ",i+1 ,min, max, *(bins+i));
        }
    }
    free(disp);
    free(data);
    free(bin_max);
    free(bins);
    MPI_Finalize();    
}

/*
 * This method is called when an array needs to be populated with random numbers
 */
void make_data(float min_meas, float max_meas, int rank, int* localc, int* locald, float* dataf){
    float* datal;//Holds the locally produced data for this thread
    unsigned seed, randSeed;//unsigned seed variable for the rng
    datal=calloc(*(localc+rank),sizeof(float));//The length of each local array that will be used by this thread is according to the segment of the array that the thread is responsible for.
    seed=(unsigned) time(0)+rank;
    randSeed=my_rand(&seed);
    randSeed=my_rand(&randSeed);
    for(int i=0; i<*(localc+rank); i++){//fills up the array with RNG, the upper bound is the length of the array that this thread is responsible for (for example in an array of 10, threads 0-2 would be responsible for 3 each, the last one being responsible for 1.)
        *(datal+i)=min_meas+(max_meas-min_meas)*my_drand(&randSeed);//Gets the RNG number by adding the difference between the min and max measurements, multiplying that by a random seed number and adding that to the minimum measurement.n
    }
    MPI_Gatherv(datal, *(localc+rank), MPI_FLOAT, dataf, localc, locald, MPI_FLOAT, 0, MPI_COMM_WORLD); //Puts together the portions the overall array that each thread was responsible for into a single array Data using gatherv(since the data points may not be divisible by the comm size)
    MPI_Barrier(MPI_COMM_WORLD);
    free(datal);

}

/*
*   This method is used to calculate the bin maxes used for the Histogram. I was trying to parallelize this method but honestly I couldn't come up with a viable solution currently, especially since if the bin count is less than the number of threads working, its probably more efficient to just use the main thread to do this.
*
*/
void make_bins(float min_meas, float max_meas, int binCount, float* binm){
    float bin_width=(max_meas-min_meas)/binCount;
    for(int i=0;i<binCount;i++){
        *(binm+i)=bin_width*(i+1)+min_meas;
    }
}

void bin_alloc(int rank, int data_count, int thread_count, int bin_count, float min_meas, float* data, int* bins, float* binmaxes){
    int bin_num=0;//holds the index of the bin that the data point belongs to.
    int* local_bins=calloc(bin_count,sizeof(int));//holds the locally produced bin allocations for this thread
    int my_n=data_count/thread_count;
    int first_ele=my_n*rank;
    int last_ele=first_ele+my_n;

    if(rank>=(thread_count-1)){
        last_ele=data_count;
    }
    for(int i=first_ele; i<last_ele; i++){
        bin_num=find_bin(*(data+i), min_meas, bin_count, binmaxes);
        local_bins[bin_num]++;
    }
    MPI_Reduce(local_bins, bins, bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);//Sums the different arrays that each thread was responsible. I.E. Rank 0 Array would be [0,2,3], Rank 1 Array would be [2,3,1], overall array would be [2,5,4]. I was having trouble with this method while excluding MPI Reduce.
    free(local_bins);
    

}
/*
*   This function is a binary search function in order to determine what bin each data point belongs to. Returns the bin index.
*/
int find_bin(float data_point, float min_meas, int bin_count, float* bin_max){
    int lower=0, middle=0, upper=0;
    float min=0, max=0;
    
    upper=bin_count-1;
    while(lower<=upper){
        middle=(lower+upper)/2;
        max=*(bin_max+middle);
        if(middle==0){
            min=min_meas;
        }else
        {
            min=*(bin_max+(middle-1));
        }
        if(data_point>=max){
            lower=middle+1;
        }else if(data_point<min){
            upper=middle-1;
        }else
        {
            return middle;
        }           
    }
    return middle;
}
/*
*   This method calculates the sizes of the array and displacements in order to use MPI_Gatherv
*/
void calc_mpi_data(int* recv_counts, int* displ, int comm, int datac){
    if(datac%comm!=0){//If the comm size isn't a factor of the data count
        for(int i=0; i<comm; i++){
            if(i==comm-1){
                recv_counts[i]=datac%((datac/comm)+1);//Last thread would take the remainder
            }
            else
            {
                recv_counts[i]=(datac/comm)+1;//All the lengths for all the other threads would be the ceiling of the divison. I.E. Data Count=10, Threads=4 would result in the counts being 3,3,3,1.
                
            }
            
        }
    }else//Otherwise if the commsize is a factor of the data count.
    {
        for(int i=0; i<comm; i++){
            recv_counts[i]=datac/comm;
        }
    }
    displ[0]=0;
    for(int i=1; i<comm; i++){
       displ[i]=displ[i-1]+recv_counts[i-1];
    }
}
