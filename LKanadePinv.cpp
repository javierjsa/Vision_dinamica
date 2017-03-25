/*
 * LKanadePinv.cpp
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#include "LKanadePinv.h"

#define D_TYPE CV_32F

LKanadePinv::LKanadePinv(int vecindad,int step,cv::Mat* img_t, cv::Mat* img_t1):LKanade(vecindad,step,img_t, img_t1) {

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;
	this->matrix= Mat::zeros(r,c,CV_32F); //Sumatorio Ix

}

void LKanadePinv::Calcula_UV(cv::Mat* img_t, cv::Mat* img_t1){

	this->img_t=img_t;
	this->img_t1=img_t1;


	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->Clean();

	this->Calcula_gradiente();

	//calculo sumatorios
	this->Calcula_sumatorios();

	#pragma omp parallel for schedule(static,1)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+this->step){

		float* _Iy2i=this->Iy2i.ptr<float>(i);
		float* _IxiIti=this->IxiIti.ptr<float>(i);
		float* _IyiIxi=this->IyiIxi.ptr<float>(i);
		float* _IyiIti=this->IyiIti.ptr<float>(i);
		float* _Ix2i=this->Ix2i.ptr<float>(i);
		float* _U=this->U.ptr<float>(i);
		float* _V=this->V.ptr<float>(i);
		float* _M=this->M.ptr<float>(i);

		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+this->step){

			float a=_Iy2i[j];
			float b=_IxiIti[j];
			float c=_IyiIxi[j];
			float d=_IyiIti[j];
			float e=_Ix2i[j];

			Mat C = (Mat_<float>(2,2) << e, c, c, a);
			Mat D = (Mat_<float>(2,1) << -b, -d);

			Mat matr;

			invert(C,matr, DECOMP_SVD);
			Mat UV= matr*D;
			float* _UV=UV.ptr<float>(0);
			_U[j]=_UV[0];
			_V[j]=_UV[1];

			_M[j]= sqrt(_U[j]*_U[j] + _V[j]*_V[j]);

		}
	}

}

LKanadePinv::~LKanadePinv() {
	// TODO Auto-generated destructor stub
}

