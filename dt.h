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

/* distance transform */

#ifndef DT_H
#define DT_H

#include <algorithm>
#include "image.h"

//#define INF 1E20
#define INF 1E20


#include <iterator>  // std::ostream_iterator
#include <algorithm> // std::swap (until C++11)
#include <vector>
#include <omp.h>
#define THREADS 8

template<class RandomIterator>
void transpose(RandomIterator first, RandomIterator last, int m)
{
    const int mn1 = (last - first - 1);
    const int n   = (last - first) / m;
    std::vector<bool> visited(last - first);
    RandomIterator cycle = first;
    while (++cycle != last) {
        if (visited[cycle - first])
            continue;
        int a = cycle - first;
        do  {
            a = a == mn1 ? mn1 : (n * a) % mn1;
            std::swap(*(first + a), *cycle);
            visited[a] = true;
        } while ((first + a) != cycle);
    }
}





void transpose_UCHAR(image<uchar> *input)
{

  int width = input->width();
  int height = input->height();
  for(int i=0; i<height-2; i++) 
  {  for(int j=i; j<width-1; j++)
    { 
      uchar temp = input->data[i*width + j];
      uchar temp2 = input->data[j*width+i];
      input->data[i*width+j]=temp2;
      input->data[j*width+i] = temp;
      
    }
  }

}

void transpose_FLOAT(image<float> *input)
{

  int width = input->width();
  int height = input->height();
  for(int i=0; i<height-2; i++) 
  {  for(int j=i; j<width-1; j++)
    { 
      float temp = input->data[i*width + j];
      float temp2 = input->data[j*width+i];
      input->data[i*width+j]=temp2;
      input->data[j*width+i] = temp;
      printf("%0.1f %0.1f",temp,temp2);
      
    }
    printf("\n");
  }

}

void print_values(image<float> *input,int num_rows,int num_cols)
{
  int width = input->width();
  int height = input->height();
  num_rows = num_rows > height/2 ? height/2 : num_rows;
  num_cols = num_rows > width/2 ? height/2 : num_cols;
  for(int i=height/2;i<(height/2)+num_rows;i++)
  {
    for(int j=width/2;j<(width/2)+num_cols;j++)
    {
        printf("%0.1f ",input->data[i*width+j]);
    }
    printf("\n");
  }


}

void print_values_UCHAR(image<uchar> *input,int num_rows,int num_cols)
{
  int width = input->width();
  int height = input->height();
  num_rows = num_rows > height/2 ? height/2 : num_rows;
  num_cols = num_rows > width/2 ? height/2 : num_cols;
  for(int i=height/2;i<(height/2)+num_rows;i++)
  {
    for(int j=width/2;j<(width/2)+num_cols;j++)
    {
        printf("%d ",input->data[i*width+j]);
    }
    printf("\n");
  }


}



/* dt of 1d function using squared distance */
static float *dt(float *f, int n) {
  float *d = new float[n];
  int *v = new int[n];
  float *z = new float[n+1];
  int k = 0;
  v[0] = 0;
  z[0] = -INF;
  z[1] = +INF;
  for (int q = 1; q <= n-1; q++) {
    float s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    while (s <= z[k]) {
      k--;
      s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    }
    k++;
    v[k] = q;
    z[k] = s;
    z[k+1] = +INF;
  }

  k = 0;
  for (int q = 0; q <= n-1; q++) {
    while (z[k+1] < q)
      k++;
    d[q] = square(q-v[k]) + f[v[k]];
  }

  delete [] v;
  delete [] z;
  return d;
}

/* dt of 2d function using squared distance */
static void dt(image<float> *im) {
  int width = im->width();
  int height = im->height();
  float *f = new float[std::max(width,height)];

  //printf("Beginning\n");
  // print_values(im,3,3);
  // for(int p=0;p<10;p++)printf("___________________-----------\n\n\n\n\n\n\n");
  //printf(" %0.1f ",im->data[100*200+100]);

 // printf("One value above ---\n");


  // transform along rows
	#pragma omp parallel for schedule(dynamic, 90)
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      f[x] = imRef(im, x, y);
    }
    float *d = dt(f, width);
    for (int x = 0; x < width; x++) {
      imRef(im, x, y) = d[x];
    }
    delete [] d;
  }
  int method_new = 1;
  if(method_new == 1)
  {
  //  printf("--------NEW METHOD------\n");

  //  printf("Before_ trans");
  //  print_values(im,3,3);
	#pragma omp parallel
	{
		#pragma omp for schedule(dynamic, 90)
  	for(int i=0; i<height-2; i++) 
    {  for(int j=i; j<width-1; j++)
      { 
        float temp = im->data[i*width + j];
        float temp2 = im->data[j*width+i];
        im->data[i*width+j]= temp2;
        im->data[j*width+i] = temp;
        
      }
    }

    //printf("Before_ trans");
    // print_values(im,3,3);
    //    transpose_FLOAT(im);
  //  printf("After_ trans");
   // print_values(im,3,3);

    // transform along rows
		#pragma omp for schedule(dynamic,90)
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        f[x] = imRef(im, x, y);
      }
      float *d = dt(f, width);
      for (int x = 0; x < width; x++) {
        imRef(im, x, y) = d[x];
      }
      delete [] d;
    }
	}

    //     for(int i=0; i<height-2; i++) 
    // {  for(int j=i; j<width-1; j++)
    //   { 
    //     float temp = im->data[i*width + j];
    //     float temp2 = im->data[j*width+i];
    //     im->data[i*width+j]= temp2;
    //     im->data[j*width+i] = temp;
        
    //   }
    // }




  /*  printf("After_row-andtrans");
    print_values(im,3,3);
    printf("---END NEW----\n"); */
  }
  else
  {
      // transform along columns
		#pragma omp parallel for schedule(dynamic,90)
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
        f[y] = imRef(im, x, y);
      }
      float *d = dt(f, height);
      for (int y = 0; y < height; y++) {
        imRef(im, x, y) = d[y];
      }
      delete [] d;
    }

  }

/*
  printf("Final Res\n");
    print_values(im,3,3);


printf("\nAfter\n");
  // print_values(im,3,3);
  // for(int p=0;p<10;p++)printf("___________________-----------\n\n\n\n\n\n\n");
  printf(" %0.1f \n\n",im->data[100*200+100]);


*/
  delete f;
}


/* dt of binary image using squared distance */
static image<float> *dt(image<uchar> *im, uchar on = 1) {
  int width = im->width();
  int height = im->height();

  image<float> *out = new image<float>(width, height, false);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (imRef(im, x, y) == on)
	imRef(out, x, y) = 0;
      else
	imRef(out, x, y) = INF;
    }
  }

  dt(out);
  return out;
}
//---------------Slight diff - inplace function------------//

void dt_i(float *f, int n) {
  float *d = new float[n];
  int *v = new int[n];
  float *z = new float[n+1];
  int k = 0;
  v[0] = 0;
  z[0] = -INF;
  z[1] = +INF;
  for (int q = 1; q <= n-1; q++) {
    float s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    while (s <= z[k]) {
      k--;
      s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    }
    k++;
    v[k] = q;
    z[k] = s;
    z[k+1] = +INF;
  }

  k = 0;
  for (int q = 0; q <= n-1; q++) {
    while (z[k+1] < q)
      k++;
    d[q] = square(q-v[k]) + f[v[k]];
  }

  for(int q=0;q<n;q++)
  { 
    f[q] = d[q];
  }

  delete [] v;
  delete [] z;
  delete [] d;
}
void dt_i_only_row(image<float> *im)
{
   int width = im->width();
  int height = im->height();
  float *f = new float[std::max(width,height)];

// transform along rows
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      f[x] = imRef(im, x, y);
    }
    dt_i(f, width);
    for (int x = 0; x < width; x++) {
      imRef(im, x, y) = f[x];
    }
    //delete [] d;
  
}
}
void dt_i(image<float> *im) {
  int width = im->width();
  int height = im->height();
  float *f = new float[std::max(width,height)];

// transform along rows
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      f[x] = imRef(im, x, y);
    }
    dt_i(f, width);
    for (int x = 0; x < width; x++) {
      imRef(im, x, y) = f[x];
    }
    //delete [] d;
  }



  // transform along columns
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      f[y] = imRef(im, x, y);
    }
    dt_i(f, height);
    for (int y = 0; y < height; y++) {
      imRef(im, x, y) = f[y];
    }
    //delete [] d;
  }

  

  delete f;
}

static image<float> *dt_i(image<uchar> *im, uchar on = 1) {
  int width = im->width();
  int height = im->height();

  image<float> *out = new image<float>(width, height, false);
	#pragma omp parallel for schedule(dynamic, 90)
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (imRef(im, x, y) == on)
	imRef(out, x, y) = 0;
      else
	imRef(out, x, y) = INF;
    }
  }

  dt_i(out);
  return out;
}


#endif

