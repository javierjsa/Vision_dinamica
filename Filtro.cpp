/*
 * Filtro.cpp
 *
 *  Created on: 31 mar. 2017
 *      Author: jsa
 */



#ifndef INCLUDES
#define INCLUDES

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/videoio.hpp"
#include <ctime>
#endif



#include "FiltroParticulas.h"


#define D_TYPE CV_32F


using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	char* imageName = argv[1];
	cout<<argv[1];
	//Mat c =Mat::zeros(800,600,CV_32FC1);
	Mat c = imread(imageName,1);

	namedWindow(  argv[1], CV_WINDOW_AUTOSIZE );
	imshow( argv[1], c );
	waitKey(0);

	if( !c.data )
	 {
	   printf( " No image data \n " );
	   return -1;
	 }


	Mat gray_image;
	cvtColor( c, gray_image, CV_BGR2GRAY );
	FiltroParticulas a = FiltroParticulas(20,gray_image);
	a.GenerarParticulas(gray_image);
	a.CalcularPesos(gray_image);
	a.SeleccionarParticulas();

}
