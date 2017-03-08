/*
 * flujo.cpp
 *
 *  Created on: 8 mar. 2017
 *      Author: jsa
 */


#ifndef INCLUDES
#define INCLUDES

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>


#endif



#include "Calculaflujos.h"
#include "LKanade.h"
#include "HShunck.h"





using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	Mat img_a;
	Mat img_b;

	img_a = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	img_b = imread(argv[2], CV_LOAD_IMAGE_COLOR);
	Calculaflujos a = Calculaflujos(&img_a,&img_b);
	//a.Calcula_gradiente();

	LKanade b = LKanade(1,&img_a,&img_b);
	//b.Calcula_gradiente();

	HShunck c =HShunck(1,&img_a,&img_b);
	c.Calcula_gradiente();

	if((! img_a.data ) || (! img_a.data ))
	    {
	        cout <<  "Could not open or find the image" << std::endl ;
	        return -1;
	    }


	return 0;
}


