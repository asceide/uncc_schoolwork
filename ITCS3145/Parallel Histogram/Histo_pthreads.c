/*
*   Student: Patrick Alarcon
*   ID: 800792239
*   Homework: Mini Project 2
*   The purpose of this program is to show the parallelization of the serial histogram program that we had written in the previous mini
*   project.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //For threading
#include <time.h> //In order to use the time as a seed for the RNG
#include "./my_rand.h" //To generate random numbers

pthread_mutex_t mutex;//Used for locking
pthread_cond_t cond_var;//Used for the barrier method
const int MAX_THREADS=64;//Max # of threads allowed.
int thread_count=0, data_count=0, bin_count=0, counter=0; //shared variables, counter is used for the barrier
float min_meas=0, max_meas=0;
int* bins;//Arrays to hold the #of bins, generated data and the bin widths
float* data;
float* bin_max;

void Usage();//If the user inputs invalid numbers an error function is called.
void* make_data(void* arg);
void make_bins(long rank);
void bin_alloc(long rank);
int find_bin(float data_point);
//a struct object is made in order to send multiple parameters to the thread function
struct rand_func{
    long rank;
    unsigned seed;
};


int main(int argc, char* argv[]){
    long thread; //holds the thread number
    unsigned seed; //used for the initial RNG seed
    unsigned randSeed; //used for the seeds that will be used for the RNG that will be used by the threads
    float min, max=0;
 
    struct rand_func *params; //an array of the struct will have to be made since all the threads are to have their own seeds
    pthread_t* threads; //threads to generate data
   
    
    //If the number of arguments sent in the CML isn't two (program + # of threads)
    if(argc!=2){
        Usage();
    }
    thread_count=strtol(argv[1], NULL, 10);//gets the # of threads to get used from the CML

    //If the number of threads sent is <=0 or >=64 an error is thrown
    if(thread_count <=0 || thread_count>=MAX_THREADS){
        Usage();
    }
    printf("How many data points are to be used? ");
    scanf("%d",&data_count);
    printf("What will be your minimum measurement? ");
    scanf("%f", &min_meas);
    printf("What will be your maximum measurement? ");
    scanf("%f", &max_meas);
    printf("How many bins will the histogram use? ");
    scanf("%d",&bin_count);
    //If the data points sent is <=0;
    if(data_count<=0 || max_meas<=min_meas){
       Usage();
    }
    //Memory locations are allocated for various arrays and certain arrays are initialized.
    params=malloc(thread_count*sizeof(*params));
    data=calloc(data_count,sizeof(float));
    bins=calloc(bin_count,sizeof(int));
    bin_max=calloc(bin_count,sizeof(float));
    threads=malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);//Mutex and barrier variables are initialized.
    pthread_cond_init(&cond_var, NULL);
    
    seed=(unsigned) time(0);//uses Computer Time as the random seed
    randSeed=my_rand(&seed);//seeds the initial RNG number
  
    // Thread creation, threads start with filling up the data array with random digits.
    for(thread=0; thread<thread_count; thread++){
        randSeed=1+my_rand(&seed);//creates an rng number for each thread, so that each thread doesn't have the same seed and thus the same data
        (*(params+thread)).rank=thread;//sets the thread number in the struct to be sent to as a parameter with the thread
        (*(params+thread)).seed=randSeed;//same but for seed
        pthread_create(&(*(threads+thread)), NULL, make_data, (void *) &(*(params+thread)));//creates a thread and calls the make_data function, sending the threads location on the struct array as an arg
    }
    // Joins and terminates the threads
    for(thread=0; thread<thread_count; thread++){
        pthread_join((*(threads+thread)),NULL);
    }
    /*
     * Following three loops are to output the data from the histogram
    */
    printf("\nData Points: ");
    for(int i=0; i<data_count; i++){
        printf(" %.1f,", *(data+i));
    }
   
    printf("\nBin Maxes: ");
    for(int i=0; i<bin_count; i++){
        printf(" %.1f,", *(bin_max+i));
    }
    for(int i=0; i<bin_count; i++){//output to notify number of elements per bin range
        if(i==0){//for the first iteration the minimum measurement
            min=min_meas;
        } else//otherwise it would be the max measurement for the previous bin
            min=*(bin_max+(i-1));
        max=*(bin_max+i);//max measurement for that bin.
        printf("\nFor Bin number: %d, ranges %.1f-%.1f, there are: %d values in it. ",i+1 ,min, max, *(bins+i));

    }
    // Frees the memory allocated to the arrays/threads and sets their pointers to NULL.

    free(params);
    free(data);
    free(bin_max);
    free(threads);
    params=NULL;
    data=NULL;
    threads=NULL;
    bin_max=NULL;
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
}
/*
*   This function is called to throw an error if the user made an invalid input
*/
void Usage(){
    fprintf(stderr, "\nYou have made an invalid input, please check the # of args, # of threads, # of data points, or the max and min mesaurements. Goodbye.");
    exit(0);
}

/*
*   This function is used to create randomized data that will be used to create the data
*/
void* make_data(void *arg){
   struct rand_func *params = arg;//gets the struct arg in order to access the thread variables
   long my_rank=params->rank;//sets the rank to the value located within the struct 
   unsigned my_seed=params->seed;//sets the seed that will be used for the thread with the seed gotten from the struct
   long long my_n=data_count/thread_count;//basically, decides how many locations in the array the threads will be responsible for. I.e. a data count of 28 and a thread count of 7 means each thread will be responsible for 4 array locations each.
   long long first_ele=my_n*my_rank;//sets the first array location that this thread will be responsible for according to the number of locations each thread is responsible for and the number of thread itself.
   long long last_ele=first_ele+my_n;//sets the last element of the array to be accessed by the thread as the first element plus the number of locations the threads are responsible for.
 
   // However, if the current thread is the last thread, then the last element has to each to the length of the array, otherwise a seg fault will occur trying to access a location of the memory that isn't allocated for that array
   if(my_rank>=(thread_count-1)){
       last_ele=data_count;
   }
   /*
   *  The threads will create random numbers and store it into the array. Since each thread is accessing different location indice within 
   *  the data array, there is no need to synchronize or have mutex/semaphores.
   */
  
   for(int i=first_ele; i<last_ele; i++){
       *(data+i)=min_meas+(max_meas-min_meas)*my_drand(&my_seed);//Random data is generated.
       
   }
   //Once the thread is finished creating and inputting its data, it goes to create the bins. The threads do not have to be synchronized for this so no barrier is needed. The thread number is sent as a parameter
   make_bins(my_rank);
   return NULL;

}
/*
*   This function is used to determine the range that each bin will consist of, and to create the bins themselves. The thread number is sent as a parameter.
*/
void make_bins(long rank){
    float bin_width=0;
    long my_rank=(long) rank;
    long long my_n=bin_count/thread_count;
    long long first_ele=my_n*my_rank;
    long long last_ele=first_ele+my_n;
    bin_width=(max_meas-min_meas)/bin_count;//the range of numbers each bin will consist of
    //Same as in make_data, but with the bin_count as the array length
    if(my_rank>=(thread_count-1)){
        last_ele=bin_count;
    }
    /*
    *   Each thread will determine the width of each bin and insert the max number into an array that holds those number
    */
    for(int i=first_ele; i<last_ele; i++){
        *(bin_max+i)=min_meas+(i+1)*bin_width;
    } 

    /*
    *   This time however, You want all the threads to finish calculating the bin maxes before calling the bin_alloc function. So a barrier mechanism using condition variable and a counter is used. The counter is used to make sure that all the threads are finished prior to
    *   calling the function
    */
    pthread_mutex_lock(&mutex);
    counter++;
    if(counter == thread_count){
        counter=0;
        pthread_cond_broadcast(&cond_var);//If the counter reaches the number of threads, then the counter is reset and the fact that all the threads are woken up
    }
    else
    {
        while(pthread_cond_wait(&cond_var,&mutex)!=0);//Otherwise, the threads are put to sleep while the condition isn't reached.
    }
    pthread_mutex_unlock(&mutex);
    bin_alloc(my_rank);
}
/*
*   This function is to be used to find in what bin does each data point belong to.
*/
void bin_alloc(long rank){
    int bin_num=0;//holds the index of the bin that the data point belongs to.
    long my_rank=(long) rank;
    long long my_n=data_count/thread_count;
    long long first_ele=my_n*my_rank;
    long long last_ele=first_ele+my_n;

    if(my_rank>=(thread_count-1)){
        last_ele=data_count;
    }

    for(int i=first_ele; i<last_ele; i++){
        bin_num=find_bin(*(data+i));
        pthread_mutex_lock(&mutex); //Since there is the chance that two threads maybe accessing the same index in an array, a mutex lock is placed in order to avoid any conflicts between threads and ensure that each thread has a chance to update the bins with their data
     //   printf("\n%ld",my_rank);
        *(bins+bin_num)+=1;
        pthread_mutex_unlock(&mutex);
    }

}
/*
*   This function is a binary search function in order to determine what bin each data point belongs to. Returns the bin index.
*/
int find_bin(float data_point){
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