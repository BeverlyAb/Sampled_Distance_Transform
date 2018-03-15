/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "pnmfile.h"
#include "imconv.h"
#include "dt.h"
#include "cuda_utils.h"
typedef uchar dtype;
#define MAX_THREADS 256


//kernel0 (dtype *input, dtype *output, unsigned int n)
__global__ void
kernel_threshold(uchar *im_ptr,uchar *out_ptr, int height,int width)
{
  //Idea is for each thread to do the dt computation save back in scratch and then 

  __shared__  dtype scratch[height * width];

  unsigned int bid = gridDim.x * blockIdx.y + blockIdx.x;
  unsigned int i = bid * blockDim.x + threadIdx.x;

  if(i<height)
  {
    for(int j=0;j<width;j++)
    {
      if(j<200)
      {
        scratch[i*width +j] = 90;
      }
      else
      {
        scratch[i*width +j] = *(im_ptr + i*width+j);
      }
     // scratch[i*width +j] = *(im_ptr + i*width+j);
      
    }
  }


  __syncthreads ();

  // for(int i=0;i<244;i++)
  // {
  //   for(int j=0;j<200;j++)
  //   {
  //     *(im_ptr + 400*i + j) = 90;
  //   }
  // }

  
  if(i<height)
  {
    for(int j=0;j<width;j++)
    {
      *(out_ptr + i*width +j) = scratch[i*width +j]; 
    }
  }


  printf("value= %f", im_ptr[0]);
}



int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
    return 1;
  }

  char *input_name = argv[1];
  char *output_name = argv[2];

  dtype *h_idata, *h_odata, h_cpu;
  dtype *d_idata, *d_odata;

  float *temp_arr;
  temp_arr = new float[1];
  temp_arr[0] = 5;
  //threshold(temp_arr);
  // load input
  image<uchar> *input = loadPGM(input_name);

  // compute dt
  //image<float> *out = dt(input);

  threshold(input->data);
  int N = input->height() * input->width();


  int blocks = ceil(input->height()/MAX_THREADS);
  h_idata = (dtype*) malloc (N * sizeof (dtype));
  CUDA_CHECK_ERROR (cudaMalloc (&d_idata, N * sizeof (dtype)));
  CUDA_CHECK_ERROR (cudaMalloc (&d_odata, N * sizeof (dtype)));

  // /* Initialize array */
  // srand48(time(NULL));
  // for(i = 0; i < N; i++) {
  //   h_idata[i] = drand48() / 100000;
  // }
  CUDA_CHECK_ERROR (cudaMemcpy (d_idata, input->data, N * sizeof (dtype), 
				cudaMemcpyHostToDevice));


  kernel_threshold <<<gb, tb>>> (d_idata, d_odata, input->height(),input->width());



  CUDA_CHECK_ERROR (cudaMemcpy (h_odata, d_odata, sizeof (dtype), 
  cudaMemcpyDeviceToHost));
  // // take square roots
  // for (int y = 0; y < input->height(); y++) {
  //   for (int x = 0; x < out->width(); x++) {
  //     imRef(input, x, y) = sqrt(imRef(out, x, y));
  //   }
  // }

  // convert to grayscale
//  image<uchar> *gray = imageFLOATtoUCHAR(out);
  input->data = h_odata;
  // save output
  savePGM(input, output_name);

  delete input;
 // delete out;
 // delete gray;
}
