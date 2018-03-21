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

/*
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

  // printf("Main - final Res\n");
  // print_values(out,3,3);


 
  // transpose_UCHAR(input);
  // printf("sizeuchar= %d\n",sizeof(uchar));
   int width = input->width();
   int height = input->height();
  // //transpose(input,input + (height-1)*width + width-1,1);





  // //threshold(input->data);
  // // take square roots
   for (int y = 0; y < input->height(); y++) {
     for (int x = 0; x < out->width(); x++) {
       imRef(out, x, y) = sqrt(imRef(out, x, y));
     }
   }

  // convert to grayscale
  printf("Values_before_gray_After_root\n");
 // print_values(out,3,3);


  image<uchar> *gray = imageFLOATtoUCHAR(out,0,112);


  printf("\nGray - final  Res\n");
  //print_values_UCHAR(gray,3,3);

  // image<uchar> *gray_trans = new image<uchar>(height,width,false);


  // for(int i=0;i<width;i++)
  // {
  //   for(int j=0;j<height;j++)
  //   {
  //     gray_trans->data[i*height + j] = gray->data[j*width + i];
  //   }
  // }

  // for(int i=0;i<width;i++)
  // {
  //   for(int j=0;j<height;j++)
  //   {
  //     gray->data[j*width + i]=gray_trans->data[i*height + j]; 
  //   }
  // }

  

  //savePGM(gray, output_name);
  savePGM(gray, output_name);

  //delete input;
  //delete out;
  //delete gray;
}
*/

//DT_ORIG
/*
int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
    return 1;
  }

  char *input_name = argv[1];
  char *output_name = argv[2];

  
 
  image<uchar> *input = loadPGM(input_name);



 image<float> *out = dt_i(input);


  // // take square roots
   for (int y = 0; y < input->height(); y++) {
     for (int x = 0; x < out->width(); x++) {
       imRef(out, x, y) = sqrt(imRef(out, x, y));
     }
   }

  // convert to grayscale
  image<uchar> *gray = imageFLOATtoUCHAR(out);



  // save output
  savePGM(gray, output_name);

  delete input;
 delete out;
  // delete gray;
}
*/

//New version
int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
    return 1;
  }

  char *input_name = argv[1];
  char *output_name = argv[2];

  
 
  image<uchar> *input = loadPGM(input_name);
   int width = input->width();
   int height = input->height();


  image<float> *in_f = imageUCHARtoFLOAT(input);

  dt_i_only_row(in_f);

  image<float> *trans_out = new image<float>(height,width,false); //Note : Reverse here

  for(int i=0;i<width;i++)
  {
    for(int j=0;j<height;j++)
    {
      trans_out->data[i*height + j] = in_f->data[j*width + i];
    }
  }

  dt_i_only_row(trans_out);

  image<float> *out_f = new image<float>(width,height,false); 
  for(int i=0;i<width;i++)
  {
    for(int j=0;j<height;j++)
    {
      out_f->data[j*width + i]=trans_out->data[i*height + j]; 
    }
  }

  
 


  // // take square roots
   for (int y = 0; y < input->height(); y++) {
     for (int x = 0; x < input->width(); x++) {
       imRef(out_f, x, y) = sqrt(imRef(out_f, x, y));
     }
   }

  // convert to grayscale
  image<uchar> *gray = imageFLOATtoUCHAR(out_f);



  // save output
  savePGM(gray, output_name);

  delete input;
 //delete out;
  // delete gray;
}

