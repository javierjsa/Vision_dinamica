/*
 * LKanade.cpp
 *
 *  Created on: 7 mar. 2017
 *      Author: jsa
 */

#include "LKanade.h"
#include <math.h>

#define D_TYPE CV_32F

LKanade::LKanade(int vecindad,int step,cv::Mat* img_t, cv::Mat* img_t1):Calculaflujos(img_t,img_t1) {

	this->vecindad=vecindad;
	this->step=step;
	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->Ixi= Mat::zeros(r,c,D_TYPE); //Sumatorio Ix
	this->Iyi= Mat::zeros(r,c,D_TYPE); // Sumatorio Iy
	this->Ix2i= Mat::zeros(r,c,D_TYPE); // Sumatorio Ix al cuadrado
	this->Iy2i= Mat::zeros(r,c,D_TYPE); // Sumatorio Iy al cuadrado
	this->Iti= Mat::zeros(r,c,D_TYPE);  // Sumatorio It
	this->IxiIti= Mat::zeros(r,c,D_TYPE); //Sumatorio Ixi* Iti
	this->IyiIti= Mat::zeros(r,c,D_TYPE); //Sumatorio  Iyi* Iti
	this->Ix2iIy2i= Mat::zeros(r,c,D_TYPE); // Sumatior Ix cuadrado * Iy cuadrado
	this->IyiIxi = Mat::zeros(r,c,D_TYPE);
	this->U= Mat::zeros(r,c,D_TYPE); // componente U
	this->V= Mat::zeros(r,c,D_TYPE); // componente V
	this->M= Mat::zeros(r,c,D_TYPE); // Modulos


}

void LKanade::Calcula_UV(cv::Mat* img_t, cv::Mat* img_t1){

	this->img_t=img_t;
	this->img_t1=img_t1;

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->Calcula_gradiente();

	//calculo sumatorios
	this->Calcula_sumatorios();

	//calculo de U,V

	#pragma omp parallel for schedule(static,1)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+this->step){

		float* _a=(this->Iy2i).ptr<float>(i);
		float* _b=(this->IxiIti).ptr<float>(i);
		float* _c=this->IyiIxi.ptr<float>(i);
		float* _d=this->IyiIti.ptr<float>(i);
		float* _e=this->Ix2i.ptr<float>(i);
		float* _U=this->U.ptr<float>(i);
		float* _V=this->V.ptr<float>(i);
		float* _M=this->M.ptr<float>(i);

		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+this->step){

			float a=_a[j];
			float b=_b[j];
			float c=_c[j];
			float d=_d[j];
			float e=_e[j];

			float num=(-a*b+c*d)/(e*a-c*c);
			float den=(c*d-e*d)/(e*a-c*c);

			if (isnan(num)){
				_U[j]=0;
			} else {
				_U[j]=(-a*b+c*d)/(e*a-c*c);
			}

			if (isnan(den)){
				_V[j]=0;
			} else {
				_V[j]=(c*d-e*d)/(e*a-c*c);
			}

			_M[j]= sqrt(_U[j]*_U[j] + _V[j]*_V[j]);

		}
	}
}

void LKanade::Calcula_sumatorios(){

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	//calculo sumatorios
    #pragma omp parallel for schedule(static,1)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+this->step){

		float* _Ixi = this->Ixi.ptr<float>(i);
		float* _Iyi = this->Iyi.ptr<float>(i);
		float* _Ix2i = this->Ix2i.ptr<float>(i);
		float* _Iy2i = this->Iy2i.ptr<float>(i);
		float* _Iti  = this->Iti.ptr<float>(i);
		float* _IxiIti = this->IxiIti.ptr<float>(i);
		float* _IyiIti = this->IyiIti.ptr<float>(i);
		float* _IyiIxi = this->IyiIxi.ptr<float>(i);

		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+this->step){
			for (int swi=(i-this->vecindad);swi<=(i+this->vecindad);swi++){

				float* _Ix=this->Ix.ptr<float>(swi);
				float* _Iy=this->Iy.ptr<float>(swi);
				float* _It=this->It.ptr<float>(swi);

				for (int swj=(j-this->vecindad);swj<=(j+this->vecindad);swj++){

					_Ixi[j]+=_Ix[swj];
					_Iyi[j]+=_Iy[swj];
					_Ix2i[j]+=_Ix[swj]*_Ix[swj];
					_Iy2i[j]+=_Iy[swj]*_Iy[swj];
					_Iti[j]+=_It[swj];
					_IxiIti[j]+=_Ix[swj]*_It[swj];
					_IyiIti[j]+=_Iy[swj]*_It[swj];
					_IyiIxi[j]+=_Iy[swj]*_Ix[swj];
				}
			}
		}
	}

}

void LKanade::pintaVector(cv::Mat* img_a){
	int c=(img_a)->cols;
	int r=(img_a)->rows;

	double minh, maxh;
	Scalar medsc=mean(this->M);
	float med=log(medsc.val[0]);
	cv::minMaxLoc(this->M, &minh, &maxh);
	//float mlog =log(maxh);

	//Mat cM,cMU8;
	//this->M.convertTo(cMU8, CV_8UC1);
	//applyColorMap(cMU8, cM, COLORMAP_JET);


	//float mean= (float)(max+min)/2;

	#pragma omp parallel for schedule(static,1)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+this->step){ //filas, o sea,y

		float* _U=this->U.ptr<float>(i);
		float* _V=this->V.ptr<float>(i);
		float* _M=this->M.ptr<float>(i);
		//float* _cM=cM.ptr<float>(i);

		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+this->step){ //columnas, o sea, x

			CvPoint p = cvPoint(j,i);
			float modulo = _M[j]; //at(fila,columna)


			if (modulo>0){
				float x2=_U[j];
				float y2=_V[j];
				float max = (abs(x2) > abs(y2)) ? abs(x2) : abs(y2);
				float ratio = (this->step)/max;
				int hue=(int)((modulo-minh)*255)/(maxh-minh);
				//CvPoint p2 = cvPoint(p.x + x2, p.y +y2);
				//float ang= atan((p.y+y2)/(p.x+x2))*180 / CV_PI;
				//CvPoint dir = cv::Point(p.x+(5* cos(ang)), p.y+(5 * sin(ang))); // calculate direction
				CvPoint dir = cv::Point(p.x+x2*abs(med), p.y+y2*abs(med)); // calculate direction
				//if (isnan(ang)){
				//	continue;
				//}
			    //cv::arrowedLine(*img_a, p,dir, , 1,  CV_AA, 0, 0.5);

				cv::arrowedLine(*img_a, p,dir, CV_RGB(hue,0,0), 1,  CV_AA, 0, 0.5);
			}
		}
	}

}


LKanade::~LKanade() {
	// TODO Auto-generated destructor stub
}
