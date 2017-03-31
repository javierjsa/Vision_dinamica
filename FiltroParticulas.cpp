/*
 * FiltroParticulas.cpp
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
#include <vector>
#include <array>
#include <ctime>
#endif


#ifndef FILTROPARTICULAS_H_
#include "FiltroParticulas.h"
#endif

#define _D
#define _DBLOCKO
#define _DBLOCKC

#include "FiltroParticulas.h"

using namespace cv;
using namespace std;

FiltroParticulas::FiltroParticulas(int n_part,Mat& mascara) {

	this->particulas=std::vector<std::array<float,7>>(n_part,{0,0,0,0,0,0,0});
	this->uni_x=  RNG(time(NULL));
	this->uni_y=  RNG(time(NULL));
	this->gauss_u=RNG(time(NULL));
	this->gauss_v=RNG(time(NULL));
	this->uni_h=  RNG(time(NULL));
	this->uni_w=  RNG(time(NULL));

	int r= mascara.rows;
	int c= mascara.cols;

	this->sigma_x=c/6;
	this->sigma_y=r/6;
	this->sigma_w=c/12;
	this->sigma_h=r/12;
	this->sigma_u=c/100;
	this->sigma_v=r/100;

}


void FiltroParticulas::Reseed(){

	this->uni_x.state=time(NULL);
	this->uni_y.state=time(NULL);
	this->gauss_u.state=time(NULL);
	this->gauss_v.state=time(NULL);
	this->uni_h.state=time(NULL);
	this->uni_w.state=time(NULL);


}

void FiltroParticulas::GenerarParticulas(Mat& mascara){

	this->Reseed();
	int r= mascara.rows;
	int c= mascara.cols;
	//#pragma omp parallel for schedule(static,1)
	#pragma omp parallel
	#pragma omp single
	for (auto & it: this->particulas) {
		int w,h,x,y;
		w =this->uni_w.uniform(1,c/2);
		h =this->uni_h.uniform(1,r/2);
		x =this->uni_x.uniform(0,c-w);
		y =this->uni_y.uniform(0,r-h);
		it[0]=x;
		it[1]=y;
		it[2]=w;
		it[3]=h;
		_D cout<<it[0]<<","<<it[1]<<","<<it[2]<<","<<it[3]<<"\n";

	}

	#pragma omp parallel
	#pragma omp single
	for (auto & it: this->particulas) {
		int x=(int)it[0];
		int y=(int)it[1];
		int w=(int)it[2];
		int h=(int)it[3];
		//void rectangle(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
		rectangle(mascara,Rect(x,y,w,h),cvScalar(128,128,128),1,8);
	}

	_D namedWindow( "mascara", CV_WINDOW_AUTOSIZE );
	_D imshow( "mascara", mascara );
	_D waitKey(0);

}

void FiltroParticulas::CalcularPesos(Mat& mascara){

	int tpixels= countNonZero(mascara);
	float tweight=0;
	int r= mascara.rows;
	int c= mascara.cols;

	//calculo de pesos
	#pragma omp parallel
	#pragma omp single
	for (auto & it: this->particulas) {
		float ratio =it[2]*it[3];
		Mat roi= mascara(Rect((int)it[0], (int)it[1], (int)it[2], (int)it[3]));
		float pixels =countNonZero(roi);
		float w = pixels/ratio;
		_D cout<<"pixels:"<<pixels<< " ratio:"<<ratio<<" peso:"<<w<<"\n";
		it[6]=w;
		tweight+=w;

	}
	_D cout<<"tweight"<<tweight<<"\n";
	//normalización
	#pragma omp parallel
	#pragma omp single
	for (auto & it: this->particulas) {
		it[6]=it[6]/tweight;
		_D cout<<"\n"<<it[6]<<",";
	}
	_D cout<<"\n";

}


void FiltroParticulas::SeleccionarParticulas(){
	_D cout<<"\n Ruleta\n";
	//ruleta de pesos acumulados, previamente normalizados
	float acum=0;
	int index=0;
	float ruleta[this->particulas.size()];
	for (auto & it: this->particulas) {
		acum+=it[6];
		ruleta[index]=acum;
		_D cout<<ruleta[index]<<" -- ";
		index++;
	}
	_D cout<<"\n Ruleta\n";

	for (index=0;index<this->particulas.size();index++){
		float a = (float)(rand() / ((double) RAND_MAX));
		int rindex=0;
		while (ruleta[rindex]<a)
			rindex++;
		this->particulas_aux.push_back(this->particulas[rindex]);
	}

	_DBLOCKO

	for (auto & it: particulas_aux) {
		_D cout<<it[0]<<","<<it[1]<<","<<it[2]<<","<<it[3]<<", "<<it[6]<<"\n";
		}

	_DBLOCKC


}


void FiltroParticulas::PerturbarParticulas(){

	this->particulas.clear();



}



FiltroParticulas::~FiltroParticulas() {
	// TODO Auto-generated destructor stub
}

