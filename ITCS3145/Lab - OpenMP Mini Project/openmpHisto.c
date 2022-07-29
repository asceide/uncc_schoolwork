/**
 * Student: Patrick Alarcon
 * ID: 800792239
 * Homework: OpenMP Mini Project
 * This project is meant to demonstrate OpenMP usage while adapting a histogram program. I will be using the original serial histogram program that we 
 * developed earlier this semester and parallelizing  it using openmp.
 * 
 * Compiling: gcc -g -Wall -fopenmp -o openmpHiso openmpHisto.c my_rand.c
 * Executing: ./openmpHisto <# of threads>
 **/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "./my_rand.h"

void Usage();
void make_data(int data_count, float min_meas, float max_meas, float* data, int tcount);//Function to generate the data based on random numbers
void make_bins(int bin_count, float min_meas, float max_meas, float* bin_max, int t_count);//Function to make the bins
int find_bin(float data, float min_meas, int bin_count, float* bin_max);//Function to allocate the elements in each individual bin
void get_user_input(int* d_count, float* minme, float* maxme, int* b_count);
void bin_alloc(int d_count, float* data, float min_meas, int b_count, int* num_bin, float* bin_maxes, int t_count);
void print_info(int b_count, float* bin_maxes, int* num_bin, float min_meas);
int main(int argc, char* argv[]) {
    int thread_count=0, data_count=0, bin_count=0;//Vars for user input
    float min_meas=0, max_meas=0;
    float* data;//An array are pointers pointing at memory locations that hold a value, so this declares a pointer that will hold the array with a user defined array length that will be allocated later.
    float* bin_max;//An array to hold the maximum value of the bin
    int* num_bin;//An array to hold the number of elements contained per bin.
    thread_count=strtol(argv[1], NULL, 10);//Gets the number of threads from the command line.
    if(thread_count<=0){//Throws an error if the number of threads given is less than or equal to 0 and quits
        Usage();
    }
    get_user_input(&data_count, &min_meas, &max_meas, &bin_count);//Gets the user input
    data=malloc(data_count*sizeof(float));//Allocates the number of memory locations needed for the array based on the user input. For large numbers or user-defined memory length use malloc/heap or linked list, otherwise use stack/normal array.
    bin_max=malloc(bin_count* sizeof(float));//^
    num_bin=calloc(bin_count, sizeof(int));//^^
    make_data(data_count,min_meas,max_meas,data, thread_count);//calls method to populate data array with RNG numbers
    make_bins(bin_count,min_meas,max_meas,bin_max, thread_count);//calls method to determine bin ranges, number of bins etc.
    bin_alloc(data_count, data, min_meas, bin_count, num_bin, bin_max, thread_count);//Determines which bins the data set belongs to
    print_info(bin_count, bin_max, num_bin, min_meas);//prints the Histogram
    free(data);//Frees up the memory allocated for the array
    free(bin_max);//^
    free(num_bin);//^^
    return 0;
}
/*
*   This method is called if there is an error with the user input, whether it be because of threads or otherwise
*/
void Usage(){
    fprintf(stderr, "\nYou have made an invalid input, please check the # of args, # of threads, # of data points. Goodbye.");
    exit(0);
}
/*
*   This method gets the user inputs.
*/
void get_user_input(int* d_count, float* minme, float* maxme, int* b_count){
    printf("Please input the number of data: ");
    scanf("%d",d_count);
    printf("\nPlease input the min data measurement you want: ");
    scanf("%f",minme);
    printf("\nPlease input the max data measurement you want: ");
    scanf("%f",maxme);
    printf("\nPlease input the number of bins: ");
    scanf("%d",b_count);
    //Checks just in case the minimum number given is greater than the maximum number given and swaps them
    if(*minme>*maxme){
        float* temp=maxme;
        maxme=minme;
        minme=temp;
    }else if(*d_count<=0||*b_count<=0){//If the data count or data is less or equal to 0, or 
        Usage();
    }
}

/*
 * This method is called when an array needs to be populated with random numbers
 */
void make_data(int data_count, float min_meas, float max_meas, float* data, int t_count){
    unsigned seed;//unsigned seed variable for the rng
    unsigned randSeed;
    int i;
    seed=(unsigned)time(0);
    randSeed=my_rand(&seed);
#pragma omp parallel for num_threads(t_count) \
    default(none) private(i) \
    shared(data_count, min_meas, max_meas, data, randSeed)
    for(i=0; i<data_count; i++){//fills up the array with RNG
        *(data+i)=min_meas+(max_meas-min_meas)*my_drand(&randSeed);//Gets the RNG number by adding the difference between the min and max measurements, multiplying that by a random seed number and adding that to the minimum measurement.n
        printf("%.1f ", *(data+i));
    }

}
/*
 * This method creates the bins
 */
void make_bins(int bin_count, float min_meas, float max_meas, float* bin_max, int t_count){
    float bin_width=0;//Holds the range of numbers for each bin.
    bin_width=(max_meas-min_meas)/bin_count;//Range is calculated by take the difference between the min and max measurements and dividing it by the number of bins
    int i;
#   pragma omp parallel for num_threads(t_count) \
    default(none) private(i)\
    shared(bin_count, bin_width, min_meas, bin_max)
    for(i=0; i<bin_count; i++){//For loop to set the maxes of each bin in an array
        *(bin_max+i)=min_meas+(i+1)*bin_width;//Bin maxes are calculated by adding the minimum measurement to the product of the iteration count(1,2,3...n) + 1 and the bin width    }
    }
}

/*
*   This method finds which bins each data point belongs to
*/
void bin_alloc(int d_count, float* data, float min_meas, int b_count, int* num_bin, float* bin_maxes, int t_count){
    int bin=0;
    int i;
#   pragma omp parallel for num_threads(t_count)\
    default(none) reduction(+: num_bin[:b_count]) private(i, bin) \
    shared(b_count, min_meas, bin_maxes, data, d_count)
    for(i=0; i<d_count; i++){//Determines which each element of the data belongs to which bins, and updates the number of elements per bin.
        bin=find_bin(*(data+i), min_meas, b_count, bin_maxes);//Determines bin via binary search
        num_bin[bin]+=1;//adds to the number of elements in that bin. More over the new OpenMP allows you to use reductions on arrays.
    }
}
/*
 * This method determines the bin for which a certain element belongs to. Binary Search is used since the bin _max array is already sorted
 */
int find_bin(float data, float min_meas, int bin_count, float* bin_max){
    int lower=0, middle=0, upper=0;
    float min=0, max=0;

    upper=bin_count-1;
    while(lower<=upper){
        middle=(lower+upper)/2;
        max=*(bin_max+middle);
        if(middle==0){
            min=min_meas;
        }else
            min=*(bin_max+(middle-1));
        if(data>=max){
            lower=middle+1;
        }else if(data<min){
            upper=middle-1;
        } else
            return middle;
    }
    return middle;
}
/*
*   This method prings the histogram.
*/
void print_info(int b_count, float* bin_maxes, int* num_bin, float min_meas){
    for(int i=0; i<b_count; i++){//output to notify number of elements per bin range
        float min, max=0;
        if(i==0){//for the first iteration the minimum measurement
            min=min_meas;
        } else//otherwise it would be the max measurement for the previous bin
            min=*(bin_maxes+(i-1));
        max=*(bin_maxes+i);//max measurement for that bin.
        printf("\nFor Bin number: %d, ranges %.1f-%.1f, there are: %d values in it. ",i+1 ,min, max, *(num_bin+i));
    }
}