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
//#include "dt.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
    return 1;
  }

  char *input_name = argv[1];
  char *output_name = argv[2];

  
  float *temp_arr;
  temp_arr = new float[1];
  temp_arr[0] = 5;
  //threshold(temp_arr);
  // load input
  image<uchar> *input = loadPGM(input_name);


  int width = input->width();
  int height = input->height();
  image<uchar> *out = new image<uchar>(width, height, true);
   int i,j;
// for(i=0; i<height; i++) 
// {  for(j=0; j<width; j++)
//    { 
//      int temp = imRef(input,i,j);
//      imRef(out,i,j)=imRef(input,j,i);
//      imRef(out,j,i) = temp;
     
//    }
// }

// for n = 0 to N - 2
//     for m = n + 1 to N - 1
//         swap A(n,m) with A(m,n)

for(i=0; i<height-2; i++) 
{  for(j=i; j<width-1; j++)
   { 
     uchar temp = input->data[i*width + j];
     uchar temp2 = input->data[j*width+i];
     input->data[i*width+j]=temp2;
     input->data[j*width+i] = temp;
     
   }
}

  // // compute dt
  // image<float> *out = dt(input);

  // //threshold(input->data);
  // // // take square roots
  //  for (int y = 0; y < input->height(); y++) {
  //    for (int x = 0; x < out->width(); x++) {
  //      imRef(out, x, y) = sqrt(imRef(out, x, y));
  //    }
  //  }

  // // convert to grayscale
  // image<uchar> *gray = imageFLOATtoUCHAR(out);


  // for(int i=40;i<50;i++)
  // {
  //   for(int j=40;j<50;j++)
  //   {
  //     printf("%d ", imRef(gray,i,j));
  //   }
  // }

  // save output
  savePGM(input, output_name);

  delete input;
 delete out;
  // delete gray;
}
