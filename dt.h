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
// MAC = Ts = 2.2609e+06, 2.26196e+06
#ifndef DT_H
#define DT_H

#include <algorithm>
#include "image.h"
#include <time.h>
#include<omp.h>
#define INF 1E20

#define THREADS 8
#define CHUNKSIZE 100

/* dt of 1d function using squared distance */
static float *dt(float *f, int n) {
  float *d = new float[n];
  int *v = new int[n];
  float *z = new float[n+1];
  int k = 0;
  v[0] = 0;
  z[0] = -INF;
  z[1] = +INF;

  float s = 0.0;
  for (int q = 1; q <= n-1; q++) {
    s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
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
  omp_set_num_threads(THREADS);

  int x =0,y=0;
	#pragma omp parallel num_threads(THREADS)
	{	
	//cols
		int tID = omp_get_thread_num();
			auto time = omp_get_wtime();
//		#pragma omp for schedule(dynamic,CHUNKSIZE)	
		for (int x = 0; x < width; x++) {
			float * f = new float[height];
			
			for (int y = 0; y < height; y++) {	//access im
				f[y] = imRef(im, x, y);
			}	
		
			float * d = dt(f, height);					//transform
		
			for (int y = 0; y < height; y++) {	//update im
				imRef(im, x, y) = d[y];
			}		
				
			delete [] d;
		  delete f;
		}
		
		//row
		
		#pragma omp for schedule(dynamic,CHUNKSIZE)	
		for (y = 0; y < height; y++) {			
			float * g = new float[width];
			
		  for (x = 0; x < width; x++) { //access im
		  	g[x] = imRef(im, x, y);
		  }
				
		
			//printf("%lf\n",omp_get_wtime() - time);	// 1


			float * e = dt(g, width);			//transform
			time = omp_get_wtime();	
		 	for (int x = 0; x < width; x++) {//update im
		  	imRef(im, x, y) = e[x];
			}
			printf("%i %lf\n",tID,omp_get_wtime() - time);	// 8		
			delete [] e;
		  delete g;
		}
	//	printf("%i %lf\n",tID,omp_get_wtime() - time);			
	}

}

/* dt of binary image using squared distance */
static image<float> *dt(image<uchar> *im, uchar on = 1) {
  int width = im->width();
  int height = im->height();

  image<float> *out = new image<float>(width, height, false);
  for(int y = 0; y < height; y++) {
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

#endif
