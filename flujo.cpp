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
#include "opencv2/videoio.hpp"

#endif



#include "Calculaflujos.h"
#include "LKanade.h"
#include "LKanadePinv.h"
#include "HShunck.h"





using namespace cv;
using namespace std;


int main(int argc, char** argv) {

	Mat img_a;
	Mat img_b;

		/*img_a = imread(argv[1], CV_LOAD_IMAGE_COLOR);
		img_b = imread(argv[2], CV_LOAD_IMAGE_COLOR);

		if((! img_a.data ) || (! img_b.data ))
			    {
			        cout <<  "Could not open or find the image" << std::endl ;
			        return -1;
			    }



		//cvtColor(InputArray src, OutputArray dst, int code, int dstCn=0
		*/


		cv::VideoCapture cap =cv::VideoCapture(argv[1]);


		int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));

		Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),
		                  (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

		VideoWriter outputVideo;
		outputVideo.open(argv[2], ex, 30, S, true);

		if (!outputVideo.isOpened())
		    {
		        cout  << "Could not open the output video for write: " << argv[1] << endl;
		        return -1;
		    }

		cout<<"\n";

		Mat img_c;
		Mat img_d;


		while(true){

			cap>>img_a;

			cap>>img_b;
			if (img_b.empty()) break;

			cvtColor(img_a,img_c,CV_RGB2GRAY,0);
			cvtColor(img_b,img_d,CV_RGB2GRAY,0);

			img_c.convertTo(img_c, CV_32F, 1.f/255);
			img_d.convertTo(img_d, CV_32F, 1.f/255);

			//Calculaflujos a = Calculaflujos(&img_a,&img_b);
			//a.Calcula_gradiente();
			cerr<<".";
			//LKanade b = LKanade(20,&img_c,&img_d);
			LKanadePinv b = LKanadePinv(40,&img_c,&img_d);
			b.Calcula_gradiente();
			b.Calcula_UV();
			b.pintaVector(&img_a);
			b.pintaVector(&img_b);
			//HShunck c =HShunck(1,&img_a,&img_b);
			//c.Calcula_gradiente();

			//cv::Mat* U=b.get_U();
			//cv::Mat* V=b.get_V();

			//namedWindow( "Vector", WINDOW_AUTOSIZE );
			//imshow( "Vector", img_a);
			//waitKey(0);

			outputVideo<<img_a;
			outputVideo<<img_b;
		}

	cout<<"\n";
	return 0;
}


