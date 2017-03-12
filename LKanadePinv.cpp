/*
 * LKanadePinv.cpp
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#include "LKanadePinv.h"

LKanadePinv::LKanadePinv(int vecindad,cv::Mat* img_t, cv::Mat* img_t1):LKanade(vecindad,img_t, img_t1) {

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;
	this->matrix= Mat::zeros(r,c,CV_32FC1); //Sumatorio Ix

}

void LKanadePinv::Calcula_UV(){

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	//calculo sumatorios
	this->Calcula_sumatorios();

	#pragma omp parallel for simd collapse (2)
	for (int i=this->vecindad;i<=(r-this->vecindad);i++){
		for (int j=this->vecindad;j<=(c-this->vecindad);j++){

			float a=this->Iy2i.at<float>(i,j);
			float b=this->IxiIti.at<float>(i,j);
			float c=this->IyiIxi.at<float>(i,j);
			float d=this->IyiIti.at<float>(i,j);
			float e=this->Ix2i.at<float>(i,j);

			/*this->matrix.at<float>(0,0)=e;
			this->matrix.at<float>(0,1)=c;
			this->matrix.at<float>(1,0)=c;
			this->matrix.at<float>(1,1)=a;*/


			Mat C = (Mat_<float>(2,2) << e, c, c, a);
			Mat D = (Mat_<float>(2,1) << -b, -d);

			invert(C,this->matrix, DECOMP_SVD);
			Mat UV= this->matrix*D;

			this->U.at<float>(i,j)=UV.at<float>(0,0);
			this->V.at<float>(i,j)=UV.at<float>(1,0);

			//cout<<UV<<"\n";
			//cout<<"--------------------\n";
		}
	}

}





LKanadePinv::~LKanadePinv() {
	// TODO Auto-generated destructor stub
}

