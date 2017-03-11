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

	if((! img_a.data ) || (! img_b.data ))
		    {
		        cout <<  "Could not open or find the image" << std::endl ;
		        return -1;
		    }



	//cvtColor(InputArray src, OutputArray dst, int code, int dstCn=0

	Mat img_c;
	Mat img_d;

	cvtColor(img_a,img_c,CV_RGB2GRAY,0);
	cvtColor(img_b,img_d,CV_RGB2GRAY,0);

	//Calculaflujos a = Calculaflujos(&img_a,&img_b);
	//a.Calcula_gradiente();

	LKanade b = LKanade(1,&img_c,&img_d);
	b.Calcula_gradiente();
	b.Calcula_UV();
	b.pintaVector(&img_a);
	//HShunck c =HShunck(1,&img_a,&img_b);
	//c.Calcula_gradiente();

    //cv::Mat* U=b.get_U();
    //cv::Mat* V=b.get_V();

	namedWindow( "Vector", WINDOW_AUTOSIZE );
	imshow( "Vector", img_a);
	waitKey(0);
	destroyAllWindows();



	return 0;
}


