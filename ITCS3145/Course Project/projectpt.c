/*
Convert an image to grayscale.

The code is written for a mini-project of ITCS 5145 Parallel Programming at UNCC.(Q.C.)

To compile the code, we use
        gcc -g -Wall -o projectpt stb_image/stb_image.h stb_image/stb_image_write.h projectpt.c -lm -lpthreads

To run the code, type
        ./projectpt ${input color image} ${output grayscale image} ${image type} ${num_threads}

        The format of images depends on its types.
        To specify image type, we have ${image type} as follows:
            1 is for .png file
            2 is for .jpg file
        
        For example,
        ./projectpt lena1.png lena2.png 1 4
        ./projectpt lizard1.jpg lizard2.jpg 2 4
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

void* colorToGrayscale(void *rank);
const int IS_PNG = 1;
const int IS_JPG = 2;
const int DESIRED_CHANNELS = 3;
const int MAX_NAME_LENGTH = 500;
int counter=0;
int width, height;
pthread_mutex_t mutex;
pthread_cond_t cond_var;
unsigned char *gray_img;
unsigned char *color_img;
int thread_count=0;
int main(int argc, char *argv[]) {
    if (argc < 5){
        printf("Usage: color2Grayscale ${input color image file} ${output grayscale image file} ${image type}\n Image Types:\n\t1: PGN\n\t2: JPG");
	exit(-1);
    }
    long thread;
    int channels, type;
    char  in_name[MAX_NAME_LENGTH], out_name[MAX_NAME_LENGTH];
    pthread_t* threads;
    strcpy(in_name, argv[1]);
    strcpy(out_name, argv[2]);
    type = atoi(argv[3]);
    thread_count=strtol(argv[4], NULL, 10);//gets the # of threads to get used from the CML
    threads=malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);
    color_img = stbi_load(in_name, &width, &height, &channels, 0); // load and conver the image to 3 channels (ignore the transparancy channel)
    if(color_img == NULL) {
        printf("Error in loading the image\n");
        exit(-1);
    }
    printf("Loaded image %s with a width of %dpx, a height of %dpx and %d channels\n", in_name, width, height, channels);

    // Convert the input image to gray
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;
    
    gray_img = (unsigned char *)malloc(gray_img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }
    printf("Create a image array with a width of %dpx, a height of %dpx and %d channels\n", width, height, gray_channels);

    for(thread=0; thread<thread_count; thread++){
        pthread_create(&(*(threads+thread)), NULL, colorToGrayscale, (void*) thread);
    }
    for(thread=0; thread<thread_count; thread++){
        pthread_join((*(threads+thread)),NULL);
    }

    if (type == IS_PNG)
    	stbi_write_png(out_name, width, height, gray_channels, gray_img, width * gray_channels);
    else
        if (type == IS_JPG)
            stbi_write_jpg(out_name, width, height, gray_channels, gray_img, 100); //The last parameter of the stbi_write_jpg function is a quality parameter that goes from 1 to 100. Since JPG is a lossy image format, you can chose how much data is dropped at save time. Lower quality means smaller image size on disk and lower visual image quality.
    printf("Wrote image %s with a width of %dpx, a height of %dpx and %d channels\n", out_name, width, height, channels);

    stbi_image_free(gray_img); 
    free(threads);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
}

void* colorToGrayscale(void *rank){
    long my_rank=(long) rank;
    unsigned char pixel[DESIRED_CHANNELS];
    long long my_n=height/thread_count;
    long long first_ele=my_n*my_rank;
    long long last_ele=first_ele+my_n;
    if(my_rank>=(thread_count-1)){
        last_ele=height;
    }
    pthread_mutex_lock(&mutex);
    counter++;
    if(counter==thread_count){
        counter=0;
        pthread_cond_broadcast(&cond_var);
    }else
    {
        while(pthread_cond_wait(&cond_var,&mutex)!=0);
    }
    
    pthread_mutex_unlock(&mutex);

    for (int row = first_ele; row < last_ele; ++row)
    {
       
        for (int col = 0; col < width; ++col)
        {
            int greyOffset = row * width + col;
            int rgbOffset = greyOffset * DESIRED_CHANNELS;
            pixel[0] = color_img[rgbOffset];
            pixel[1] = color_img[rgbOffset + 1];
            pixel[2] = color_img[rgbOffset + 2];
            pthread_mutex_lock(&mutex);
            gray_img[greyOffset] = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
            pthread_mutex_unlock(&mutex);
            
        }
    }
    return NULL;
}
