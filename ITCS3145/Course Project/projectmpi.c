/*
Convert an image to grayscale.

The code is written for a mini-project of ITCS 5145 Parallel Programming at UNCC.(Q.C.)

To compile the code, we use
        mpicc -g -Wall -o projectmpi stb_image/stb_image.h stb_image/stb_image_write.h projectmpi.c -lm

To run the code, type
        mpiexec --hostfile hostfile -n 2 projectmpi lena1.png lena2.png 1 **If you run a system that needs a hostfile, otherwise*
        mpiexec -n 2 projectmpi lena1.png lena2.png 1

        mpiexec --hostfile hostfile -n 2 projectmpi lizard1.jpg lizard2.jpg 2
        mpiexec -n 2 projectmpi lizard1.jpg lizard2.jpg 2

        mpiexec --hostfile hostfile -n ${number of threads} ${name of program} ${input image} ${output image} ${type of image}

        The format of images depends on its types.
        To specify image type, we have ${image type} as follows:
            1 is for .png file
            2 is for .jpg file
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height, int my_rank);

const int IS_PNG = 1;
const int IS_JPG = 2;
const int DESIRED_CHANNELS = 3;
const int MAX_NAME_LENGTH = 500;
int main(int argc, char *argv[]) {
    if (argc < 4){
        printf("Usage: color2Grayscale ${input color image file} ${output grayscale image file} ${image type}\n Image Types:\n\t1: PGN\n\t2: JPG");
	exit(-1);
    }

    int width, height, channels, type, csize, rank, gray_channels, c_size, g_size;
    char  in_name[MAX_NAME_LENGTH], out_name[MAX_NAME_LENGTH];
    unsigned char *color_img;
    unsigned char *gray_img;
    strcpy(in_name, argv[1]);
    strcpy(out_name, argv[2]);
    type = atoi(argv[3]);
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank==0){
        color_img = stbi_load(in_name, &width, &height, &channels, 0); // load and conver the image to 3 channels (ignore the transparancy channel)
        if(color_img == NULL) {
            printf("Error in loading the image\n");
            exit(-1);
        }
        printf("Loaded image %s with a width of %dpx, a height of %dpx and %d channels\n", in_name, width, height, channels);

        // Convert the input image to gray
        gray_channels = channels == 4 ? 2 : 1;
        size_t gray_img_size = width * height * gray_channels;
    
        gray_img = (unsigned char *)malloc(gray_img_size);
        if(gray_img == NULL) {
            printf("Unable to allocate memory for the gray image.\n");
            exit(1);
        }
        printf("Create a image array with a width of %dpx, a height of %dpx and %d channels\n", width, height, gray_channels);
        c_size=(width*height*channels)/csize;
        g_size=(width*height*gray_channels)/csize;
    }
    MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&c_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&g_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    unsigned char *local_carr=(unsigned char*)malloc(c_size);
    unsigned char *local_garr=(unsigned char*)malloc(g_size);
    MPI_Scatter(color_img,c_size,MPI_UNSIGNED_CHAR,local_carr,c_size,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
    if(csize%2==0 && csize%4!=0){
        colorToGrayscale(local_garr, local_carr, width/2, height, rank);
    }else
    {
        colorToGrayscale(local_garr, local_carr, width/2, height/2, rank);
    }

    MPI_Gather(local_garr,g_size,MPI_UNSIGNED_CHAR,gray_img,g_size,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
    if(rank==0){
        if (type == IS_PNG)
    	    stbi_write_png(out_name, width, height, gray_channels, gray_img, width * gray_channels);
        else
            if (type == IS_JPG)
                stbi_write_jpg(out_name, width, height, gray_channels, gray_img, 100); //The last parameter of the stbi_write_jpg function is a quality parameter that goes from 1 to 100. Since JPG is a lossy image format, you can chose how much data is dropped at save time. Lower quality means smaller image size on disk and lower visual image quality.
        printf("Wrote image %s with a width of %dpx, a height of %dpx and %d channels\n", out_name, width, height, channels);

        stbi_image_free(gray_img); 
    }
    MPI_Finalize();
}

void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height, int my_rank){
    unsigned char pixel[DESIRED_CHANNELS];
    for (int row = 0; row < height; ++row)
    {   
        for (int col = 0; col < width; ++col)
        {
           //If the input image has a transparency channel this will be simply copied to the second channel of the gray image, while the first channel of the gray image will contain the gray pixel values. If the input image has three channels, the output image will have only one channel with the gray data.

            int greyOffset = row * width + col;
            int rgbOffset = greyOffset * DESIRED_CHANNELS;
            pixel[0] = color_img[rgbOffset];
            pixel[1] = color_img[rgbOffset + 1];
            pixel[2] = color_img[rgbOffset + 2];

            gray_img[greyOffset] = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
        }
    }
}
