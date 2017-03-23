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


#define D_TYPE CV_32F


using namespace cv;
using namespace std;


int main(int argc, char** argv) {

	Mat img_a;
	Mat img_b;



	cv::VideoCapture cap =cv::VideoCapture(argv[1]);


	int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));

	Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),
					  (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	VideoWriter outputVideo;
	outputVideo.open(argv[2], ex, 30, S, true);

	if (!outputVideo.isOpened())
		{
			cout  << "No se puede salvar vídeo: " << argv[1] << endl;
			return -1;
		}

	int step=5;
	int win=40;

	if (argc==4){

		win= atoi(argv[3]);
		cout<<win;
	}

	if (argc==5){

		win= atoi(argv[3]);
		step= atoi(argv[4]);
	}



	cout<<"\n";

	Mat img_c;
	Mat img_d;

	cap>>img_a;
	cap>>img_b;

	cvtColor(img_a,img_c,CV_RGB2GRAY,0);
	cvtColor(img_b,img_d,CV_RGB2GRAY,0);

	img_c.convertTo(img_c, D_TYPE, 1.f/255);
	img_d.convertTo(img_d, D_TYPE, 1.f/255);

	LKanade b = LKanade(win,step,&img_c,&img_d);
	//LKanadePinv b = LKanadePinv(win,step,&img_c,&img_d);

		while(true){

			if (img_b.empty()) break;

			cerr<<".";

			b.Calcula_UV(&img_c,&img_d);
			b.pintaVector(&img_a);
			b.pintaVector(&img_b);

			outputVideo<<img_a;
			outputVideo<<img_b;

			cap>>img_a;
			cap>>img_b;

			cvtColor(img_a,img_c,CV_RGB2GRAY,0);
			cvtColor(img_b,img_d,CV_RGB2GRAY,0);

			img_c.convertTo(img_c, D_TYPE, 1.f/255);
			img_d.convertTo(img_d, D_TYPE, 1.f/255);

		}

	cout<<"\n";
	return 0;
}


