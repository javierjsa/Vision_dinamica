/*
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

	this->sigma_x=c/32;
	this->sigma_y=r/32;
	this->sigma_w=c/64;
	this->sigma_h=r/64;
	this->sigma_u=0.5;
	this->sigma_v=0.5;

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
		//_D cout<<it[0]<<","<<it[1]<<","<<it[2]<<","<<it[3]<<"\n";

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

	//_D namedWindow( "mascara", CV_WINDOW_AUTOSIZE );
	//_D imshow( "mascara", mascara );
	//_D waitKey(1);

}

void FiltroParticulas::CalcularPesos(Mat& mascara){
	_D cerr<<"\n\nCalcular pesos\n\n";
	//int tpixels= countNonZero(mascara);
	float tweight=0;
	int r= mascara.rows;
	int c= mascara.cols;
	cout<<r<<" "<<c<<"\n";
	//calculo de pesos
	//#pragma omp parallel
	//#pragma omp single
	for (auto & it: this->particulas) {

		if (it[0]<0)
			it[0]=0;
		if (it[0]>=c)
			it[0]=c-1;
		if (it[1]<0)
			it[1]=0;
		if (it[1]>=r)
			it[1]=r-2;
		if (it[0]+it[2]>=c)
			it[2]=c-(it[0]+1);
		if (it[2]<=0)
			it[2]=1;
		if (it[1]+it[3]>=r)
			it[3]=r-(it[1]+1);
		if (it[3]<=0)
			it[3]=1;

		float ratio =it[2]*it[3];
		_D cerr<<it[0]<<","<<it[1]<<","<<it[2]<<","<<it[3]<<"\n";
		Mat roi= mascara(Rect((int)it[0], (int)it[1], (int)it[2], (int)it[3]));
		float pixels =countNonZero(roi);
		float w = pixels/ratio;
		if (isnan(w))
			w=0;
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
	_D namedWindow( "mascara", CV_WINDOW_AUTOSIZE );
	_D imshow( "mascara", mascara );
	_D waitKey(1);
}


void FiltroParticulas::SeleccionarParticulas(){
	_D cout<<"\n\n Ruleta\n\n";
	//ruleta de pesos acumulados, previamente normalizados
	this->particulas_aux.clear();

	float acum=0;
	int index=0;
	float ruleta[this->particulas.size()];
	for (auto & it: this->particulas) {
		acum+=it[6];
		ruleta[index]=acum;
		_D cout<<ruleta[index]<<" -- ";
		index++;
	}

	index=0;
	while(index<this->particulas.size()){
		float a = (float)(rand() / ((double) RAND_MAX));
		int rindex=0;
		while (ruleta[rindex]<a)
			rindex++;
		if (this->particulas[rindex][6]!=0){
			this->particulas_aux.push_back(this->particulas[rindex]);
			index++;
		}
	}

	/*_DBLOCKO

	for (auto & it: particulas_aux) {
		_D cout<<it[0]<<","<<it[1]<<","<<it[2]<<","<<it[3]<<", "<<it[6]<<"\n";
		}

	_DBLOCKC*/


}


void FiltroParticulas::PerturbarParticulas(){
	_D cout<<"\n\nPerturbar particulas\n\n";
	this->particulas.clear();
	this->Reseed();

	for (auto & it: this->particulas_aux) {

		_D cout<<it[0]<<","<<it[1]<<","<<it[2]<<","<<it[3]<<", "<<it[4]<<", "<<it[5]<<", "<<it[6]<<"\n";

		it[0]=it[0]+(float)this->gauss_u.gaussian(this->sigma_x);//X
		it[1]=it[1]+(float)this->gauss_u.gaussian(this->sigma_y);//Y
		it[2]=it[2]+(float)this->gauss_u.gaussian(this->sigma_w);//W
		it[3]=it[3]+(float)this->gauss_u.gaussian(this->sigma_h);//H
		it[4]=it[4]+(float)this->gauss_u.gaussian(this->sigma_u);//U
		it[5]=it[5]+(float)this->gauss_u.gaussian(this->sigma_v);//V

		//Suma o resta componentes de velocidad
		it[0]+=it[4];
		it[1]+=it[5];

		//_D cout<<it[0]<<","<<it[1]<<","<<it[2]<<","<<it[3]<<", "<<it[4]<<", "<<it[5]<<", "<<it[6]<<"\n";
	}

	this->particulas=this->particulas_aux;

}

void FiltroParticulas::PintarResultado(Mat& imagen){
	cerr<<"Pintar resultado\n";
	//int r= imagen.rows;
	//int c= imagen.cols;

	for (auto & it: this->particulas_aux) {

		/*
		if (it[0]<0)
			it[0]=0;
		if (it[0]>=c)
			it[0]=c-2;
		if (it[1]<0)
			it[1]=0;
		if (it[1]>=r)
			it[1]=r-2;


		if (it[2]+it[0]>=c)
			it[2]=c-(it[0]);
		if (it[2]<=0)
			it[2]=2;
		if (it[3]+it[1]>=c)
			it[3]=r-(it[1]);
		if (it[3]<=0)
			it[3]=2;
		*/
		if (it[6]>0)
			rectangle(imagen,Rect((int)it[0],(int)it[1],(int)it[2],(int)it[3]),cvScalar(0,255,0),1,8);


	}

	namedWindow("imagen", CV_WINDOW_AUTOSIZE );
	imshow( "imagen", imagen );
	waitKey(1);


}

FiltroParticulas::~FiltroParticulas() {
	// TODO Auto-generated destructor stub
}
