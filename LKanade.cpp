/*
 * LKanade.cpp
 *
 *  Created on: 7 mar. 2017
 *      Author: jsa
 */

#include "LKanade.h"

LKanade::LKanade(int vecindad,cv::Mat* img_t, cv::Mat* img_t1):Calculaflujos(img_t,img_t1) {

	this->vecindad=vecindad;
	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->Ixi= Mat::zeros(r,c,CV_32F);
	this-> Iyi= Mat::zeros(r,c,CV_32F);
	this->Ix2i= Mat::zeros(r,c,CV_32F);
	this->Iy2i= Mat::zeros(r,c,CV_32F);
	this->Iti= Mat::zeros(r,c,CV_32F);
	this->IxiIti= Mat::zeros(r,c,CV_32F);
	this->IyiIti= Mat::zeros(r,c,CV_32F);
	this->Ix2iIy2i= Mat::zeros(r,c,CV_32F);

}

void LKanade::Calcula_UV(){

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

    //#pragma omp parallel for
	for (int i=0;i++;i<r){
		for (int j=0;j++;j<c){



		}
	}


}

LKanade::~LKanade() {
	// TODO Auto-generated destructor stub
}
