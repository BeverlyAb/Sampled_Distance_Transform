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
//#include "dt_orig.h"
#include "dt.h"


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

  // load input
  image<uchar> *input = loadPGM(input_name);

 image<float> *out = dt(input);

  printf("Main - final Res\n");
  print_values(out,3,3);


 
  //transpose_UCHAR(input);






  // //threshold(input->data);
  // // take square roots
   for (int y = 0; y < input->height(); y++) {
     for (int x = 0; x < out->width(); x++) {
       imRef(out, x, y) = sqrt(imRef(out, x, y));
     }
   }

  // convert to grayscale
  printf("Values_before_gray_After_root\n");
  print_values(out,3,3);


  image<uchar> *gray = imageFLOATtoUCHAR(out,0,112);


  printf("\nGray - final  Res\n");
  print_values_UCHAR(gray,3,3);

  savePGM(gray, output_name);

  delete input;
  delete out;
  delete gray;
}

/*
DT_ORIG

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
    return 1;
  }

  char *input_name = argv[1];
  char *output_name = argv[2];

  
 
  image<uchar> *input = loadPGM(input_name);



 image<float> *out = dt(input);


  // // take square roots
   for (int y = 0; y < input->height(); y++) {
     for (int x = 0; x < out->width(); x++) {
       imRef(out, x, y) = sqrt(imRef(out, x, y));
     }
   }

  // convert to grayscale
  image<uchar> *gray = imageFLOATtoUCHAR(out);



  // save output
  savePGM(output, output_name);

  delete input;
 delete out;
  // delete gray;
}


*/