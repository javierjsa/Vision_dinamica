/*
 * LKanade.cpp
 *
 *  Created on: 7 mar. 2017
 *      Author: jsa
 */

#include "LKanade.h"
#include <math.h>
LKanade::LKanade(int vecindad,cv::Mat* img_t, cv::Mat* img_t1):Calculaflujos(img_t,img_t1) {

	this->vecindad=vecindad;
	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->Ixi= Mat::zeros(r,c,CV_32F); //Sumatorio Ix
	this->Iyi= Mat::zeros(r,c,CV_32F); // Sumatorio Iy
	this->Ix2i= Mat::zeros(r,c,CV_32F); // Sumatorio Ix al cuadrado
	this->Iy2i= Mat::zeros(r,c,CV_32F); // Sumatorio Iy al cuadrado
	this->Iti= Mat::zeros(r,c,CV_32F);  // Sumatorio It
	this->IxiIti= Mat::zeros(r,c,CV_32F); //Sumatorio Ixi* Iti
	this->IyiIti= Mat::zeros(r,c,CV_32F); //Sumatorio  Iyi* Iti
	this->Ix2iIy2i= Mat::zeros(r,c,CV_32F); // Sumatior Ix cuadrado * Iy cuadrado

}

void LKanade::Calcula_UV(){

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	#pragma omp parallel for collapse (2)
	for (int i=this->vecindad;i<=(r-this->vecindad);i++){
		for (int j=this->vecindad;j<=(c-this->vecindad);j++){
			//float pixel = this->Ixi.at<float>(i,j);

			for (int swi=(i-this->vecindad);swi<=(i+this->vecindad);swi++){
				for (int swj=(j-this->vecindad);swj<=(j+this->vecindad);swj++){

					this->Ixi.at<float>(i,j)+=this->Ix.at<float>(swi,swj);
					this->Iyi.at<float>(i,j)+=this->Iy.at<float>(swi,swj);
					this->Ix2i.at<float>(i,j)+=(this->Ix.at<float>(swi,swj))*(this->Ix.at<float>(swi,swj));
					this->Iy2i.at<float>(i,j)+=(this->Iy.at<float>(swi,swj))*(this->Iy.at<float>(swi,swj));
					this->Iti.at<float>(i,j)+=this->It.at<float>(swi,swj);
					this->IxiIti.at<float>(i,j)+=(this->Ix.at<float>(swi,swj))*(this->It.at<float>(swi,swj));
					this->IyiIti.at<float>(i,j)+=(this->Iy.at<float>(swi,swj))*(this->It.at<float>(swi,swj));
				}
			}


		}
	}
	namedWindow( "Ix2i", WINDOW_AUTOSIZE );
	imshow( "Ix2i", this->IyiIti );
	waitKey(0);
	destroyAllWindows();

}

LKanade::~LKanade() {
	// TODO Auto-generated destructor stub
}
