/// 
//  mandel.c
//  Based on example code found here:
//  https://users.cs.fiu.edu/~cpoellab/teaching/cop4610_fall22/project3.html
//  Converted to use jpg instead of BMP and other minor changes
//  Edited by: Alex Toma
//  Assignment: Lab 11: Multiprocessing
//  Section: 121
//  Run instructions (For my setup image):
//  ./mandel -x .21 -y -.555 -s .01 -m 1000
//  ffmpeg -i mandel%02d.jpg -r 50 -q:v 5 mandel.mpg
///

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>
#include "jpegrw.h"

// local routines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max );
static void show_help();


int main( int argc, char *argv[] )
{
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.
	const char *outfile = "mandel";
	double xcenter = 0;
	double ycenter = 0;
	double xscale = 4;
	double yscale = 0; // calc later
	int    image_width = 1000;
	int    image_height = 1000;
	int    max = 1000;
	int    processes = 8; // number of children
	int    imageNum = 50;

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"x:y:s:W:H:m:o:c:h"))!=-1) {
		switch(c) 
		{
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				xscale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'c':
			    processes = atoi(optarg);
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

    // create child processes to handle image generation concurrently
    for (int i = 0; i < processes; i++) 
    {
        pid_t pid = fork(); 

        if (pid == 0) 
        {
            // calculate the range of images this child will process
            int frameStart = i * (imageNum / processes);
            int frameEnd;
            if (i == processes - 1) 
            {
                // assign the remaining frames to the last process
                frameEnd = imageNum;
            } 
            else 
            {
                // divide frames evenly among the processes
                frameEnd = frameStart + (imageNum / processes);
            }

            // generate the Mandelbrot images for this process's assigned range
            for (int j = frameStart; j < frameEnd; j++) 
            {
                // adjust the scale for the current frame
                double scale = xscale * pow(4.0, (double)j / imageNum);

                // derive the corresponding y-axis scale based on image dimensions
                yscale = scale / image_width * image_height;

                // allocate memory for the image data structure
                imgRawImage* img = initRawImage(image_width, image_height);

                // set all pixels to black
                setImageCOLOR(img, 0);

                // perform Mandelbrot set computations and fill the image
                compute_image(img, xcenter - scale / 2, xcenter + scale / 2, 
                              ycenter - yscale / 2, ycenter + yscale / 2, max);

                // construct the output filename with their sequential numbers
                char output[100] = "";
                sprintf(output, "%s%02d.jpg", outfile, j + 1);
                storeJpegImageFile(img, output);

                // log details about the generated frame
                printf("mandel: x=%lf y=%lf xscale=%lf yscale=%lf max=%d outfile=%s\n", 
                       xcenter, ycenter, scale, yscale, max, output);

                // release allocated memory to prevent leaks
                freeRawImage(img);
            }

            // exit the child process after completing its task
            exit(1);
        }
    }

    // parent process waits for all child processes to finish execution
    int status;
    for (int i = 0; i < processes; i++) 
    {
        wait(&status);
    }
    printf("All images generated!\n");
    return 0;

}

/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max )
{
	int i,j;

	int width = img->width;
	int height = img->height;

	// For every pixel in the image...

	for(j=0;j<height;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			setPixelCOLOR(img,i,j,iteration_to_color(iters,max));
		}
	}
}


/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max )
{
	int color = 0xB4A7D6*iters/(double)max;
	return color;
}


// Show help message
void show_help()
{
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the image in pixels. (default=1000)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}
