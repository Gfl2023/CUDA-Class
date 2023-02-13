//nvcc SimpleJuliaSetGPU.cu -o SimpleJuliaSetGPU -lglut -lGL -lm
// This is a simple Julia set which is repeated iterations of 
// Znew = Zold + C whre Z and Care imaginary numbers.
// After so many tries if Zinitial escapes color it black if it stays around color it red.

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "./CUDAERROR.h"


#define A  -0.8789  //real
#define B  -0.11234009   //imaginary
#define N 1024

unsigned int window_width = 1024;
unsigned int window_height = 1024;

float xMin = -2.0;
float xMax =  2.0;
float yMin = -2.0;
float yMax =  2.0;
dim3 BlockSize;
dim3 GridSize;
float stepSizeX = (xMax - xMin)/((float)window_width);
float stepSizeY = (yMax - yMin)/((float)window_height);
float *pixels;
float *pixels_GPU; 

void SetUpCudaDevices()
{
	BlockSize.x = 1024;
	BlockSize.y = 1;
	BlockSize.z = 1;
	
	GridSize.x = 1024;
	GridSize.y = 1;
	GridSize.z = 1;
}

void AllocateMemory()
{					
	//Allocate Device (GPU) Memory
	cudaMalloc(&pixels_GPU,N*3*N*sizeof(float));
	
	//Allocate Host (CPU) Memory
	pixels = (float*)malloc(N*3*N*sizeof(float));

	
	
	
}

__device__ float color (float x, float y) 
{
	float mag,maxMag,temp;
	float maxCount = 200;
	float count = 0;
	maxMag = 10;
	mag = 0.0;
	
	while (mag < maxMag && count < maxCount) 
	{
		// Zn = Zo*Zo + C
		// or xn + yni = (xo + yoi)*(xo + yoi) + A + Bi
		// xn = xo*xo - yo*yo + A (real Part) and yn = 2*xo*yo + B (imagenary part)
		temp = x; // We will be changing the x but weneed its old value to hind y.	
		x = x*x - y*y + A;
		y = (2.0 * temp * y) + B;
		mag = sqrt(x*x + y*y);
		count++;
	}
	if(count < maxCount) 
	{
		return(0.0);
	}
	else
	{
		return(1.0);
	}
}

__global__ void initialization(float *g, float stepSizeX, float stepSizeY, float xmin, float ymin) 
{ 
	float x,y;

	int id = (blockDim.x*blockIdx.x + threadIdx.x)*3;
	y = ymin + blockIdx.x*stepSizeY;
	x = xmin + threadIdx.x*stepSizeX;

	g[id] = color(x,y); //red 1.0 on 0.0 off
	g[id=1] = 0.0; // blue off
	g[id=2] = 0.0; // green off

	
}

void display(void)
{
	 initialization<<<GridSize,BlockSize>>>(pixels_GPU,stepSizeX,stepSizeY,xMin,yMin);
	 cudaMemcpyAsync(pixels,pixels_GPU,N*3*N*sizeof(float),cudaMemcpyDeviceToHost);
	 errorCheck(__FILE__, __LINE__);
	 glDrawPixels(window_width,window_height, GL_RGB, GL_FLOAT, pixels);
	 glFlush();
}

int main(int argc, char** argv)
{ 
	
	SetUpCudaDevices();
	AllocateMemory();
	cudaMemcpyAsync(pixels_GPU,pixels,N*3*N*sizeof(float),cudaMemcpyHostToDevice);
	errorCheck(__FILE__, __LINE__);
   	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Fractals man, fractals.");
   	glutDisplayFunc(display);
   	glutMainLoop();
}
