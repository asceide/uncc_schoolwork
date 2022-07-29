#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int numOne=0, numTwo=0, flag=0;

void* calc(void* rank);

int main(){
    pthread_t threads;
    threads=malloc(4*sizeof(pthread_t));
    printf("Input the first number: ");
    scanf("%d",&numOne);
    printf("\nInput the second number: ");
    scanf("%d",&numTwo);
    for(int i=0; i<4; i++){
        pthread_create(&threads[i], NULL, calc, (void *) i);
    }
    for(int i=0; i<4; i++){
        pthread_join(&threads[i],NULL);
    }
    free(threads);
    threads=NULL;
    return 0;
}
/*
 * Displays +- and div/mult operations on the two numbers gotten from the user
 */ 
void* calc(void* rank){
    long thread=(long) rank;
    float cal=0;
    while(flag!=thread);//busy waiting to synchronize the threads
    switch(thread%4){
        case 0:
            cal=numOne+numTwo;
            printf("\nAddition: %.1f",cal);
            break;
        case 1:
            cal=numOne-numTwo;
            printf("\nSubtraction: %.1f",cal);
            break;
        case 2:
            cal=(float) numOne/(float) numTwo;
            printf("\nDivision: %.1f",cal);
            break;
        case 3:
            cal=numOne*numTwo;
            printf("\nMultiplication:  %.1f",cal);
            break;
        default:
            break;
    }
    flag=(flag+1)%4;
    return NULL;
}