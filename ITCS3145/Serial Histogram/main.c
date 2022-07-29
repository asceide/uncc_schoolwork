/**
 * Student: Patrick Alarcon
 * ID: 800792239
 * Homework: Mini Project: Fundamentals in Parallel Computing
 * The purpose of this program is demonstrate the usage of pointers and memory, I think, and to create a histogram program based
 * on RNG numbers with user-defined parameters.
 * In order to demonstrate pointers, in lieu of normal array functions, pointer math is used.
 **/

#include <stdio.h>
#include <stdlib.h>
#include "./my_rand.h"

void make_data(int data_count, float min_meas, float max_meas, float* data);//Function to generate the data based on random numbers
void make_bins(int bin_count, float min_meas, float max_meas, float* bin_max, int* num_bin);//Function to make the bins
int bin_alloc(float data, float min_meas, int bin_count, float* bin_max);//Function to allocate the elements in each individual bin

int main() {
    int data_count=0, bin_count=0, bin=0;//Vars for user input
    float min_meas=0, max_meas=0;
    float* data;//An array are pointers pointing at memory locations that hold a value, so this declares a pointer that will hold the array with a user defined array length that will be allocated later.
    float* bin_max;//An array to hold the maximum value of the bin
    int* num_bin;//An array to hold the number of elements contained per bin.
    printf("Please input the number of data: ");
    scanf("%d",&data_count);
    printf("\nPlease input the min data measurement you want: ");
    scanf("%f",&min_meas);
    printf("\nPlease input the max data measurement you want: ");
    scanf("%f",&max_meas);
    printf("\nPlease input the number of bins: ");
    scanf("%d",&bin_count);

    data=malloc(data_count*sizeof(float));//Allocates the number of memory locations needed for the array based on the user input. For large numbers or user-defined memory length use malloc/heap or linked list, otherwise use stack/normal array.
    bin_max=malloc(bin_count* sizeof(float));//^
    num_bin=malloc(bin_count*sizeof(int));//^^
    make_data(data_count,min_meas,max_meas,data);//calls method to populate data array with RNG numbers
    make_bins(bin_count,min_meas,max_meas,bin_max,num_bin);//calls method to determine bin ranges, number of bins etc.
    for(int i=0; i<data_count; i++){//Determines which each element of the data belongs to which bins, and updates the number of elements per bin.
        bin=bin_alloc(*(data+i), min_meas, bin_count, bin_max);//Determines bin via binary search
        *(num_bin+bin)+=1;//adds to the number of elements in that bin.
    }
    for(int i=0; i<bin_count; i++){//output to notify number of elements per bin range
        float min, max=0;
        if(i==0){//for the first iteration the minimum measurement
            min=min_meas;
        } else//otherwise it would be the max measurement for the previous bin
            min=*(bin_max+(i-1));
        max=*(bin_max+i);//max measurement for that bin.
        printf("\nFor Bin number: %d, ranges %.1f-%.1f, there are: %d values in it. ",i+1 ,min, max, *(num_bin+i));

    }
    /*for(int j=0; j<bin_count; j++){
        printf("\nBin number: %d has %d values in it.", j, *(num_bin+j));
    }*/
    free(data);//Frees up the memory allocated for the array
    free(bin_max);//^
    free(num_bin);//^^
    return 0;
}
/*
 * This method is called when an array needs to be populated with random numbers
 */
void make_data(int data_count, float min_meas, float max_meas, float* data){
    unsigned seed=1;//unsigned seed variable for the rng
    for(int i=0; i<data_count; i++){//fills up the array with RNG
        *(data+i)=min_meas+(max_meas-min_meas)*my_drand(&seed);//Gets the RNG number by adding the difference between the min and max measurements, multiplying that by a random seed number and adding that to the minimum measurement.n
        printf("\n%.1f", *(data+i));
    }

}
/*
 * This method creates the bins
 */
void make_bins(int bin_count, float min_meas, float max_meas, float* bin_max, int* num_bin){
    float bin_width=0;//Holds the range of numbers for each bin.
    bin_width=(max_meas-min_meas)/bin_count;//Range is calculated by take the difference between the min and max measurements and dividing it by the number of bins
    for(int i=0; i<bin_count; i++){//For loop to set the maxes of each bin in an array
        *(bin_max+i)=min_meas+(i+1)*bin_width;//Bin maxes are calculated by adding the minimum measurement to the product of the iteration count(1,2,3...n) + 1 and the bin width
        *(num_bin+i)=0;//This will hold the number of elements per bin, so the array is populated now.
    }
}
/*
 * This method determines the bin for which a certain element belongs to. Binary Search is used since the bin _max array is already sorted
 */
int bin_alloc(float data, float min_meas, int bin_count, float* bin_max){
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
