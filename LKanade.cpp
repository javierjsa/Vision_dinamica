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

	this->Ixi= Mat::zeros(r,c,CV_32FC1); //Sumatorio Ix
	this->Iyi= Mat::zeros(r,c,CV_32FC1); // Sumatorio Iy
	this->Ix2i= Mat::zeros(r,c,CV_32FC1); // Sumatorio Ix al cuadrado
	this->Iy2i= Mat::zeros(r,c,CV_32FC1); // Sumatorio Iy al cuadrado
	this->Iti= Mat::zeros(r,c,CV_32FC1);  // Sumatorio It
	this->IxiIti= Mat::zeros(r,c,CV_32FC1); //Sumatorio Ixi* Iti
	this->IyiIti= Mat::zeros(r,c,CV_32FC1); //Sumatorio  Iyi* Iti
	this->Ix2iIy2i= Mat::zeros(r,c,CV_32FC1); // Sumatior Ix cuadrado * Iy cuadrado
	this->IyiIxi = Mat::zeros(r,c,CV_32FC1);
	this->U= Mat::zeros(r,c,CV_32FC1); // componente U
	this->V= Mat::zeros(r,c,CV_32FC1); // componente V
	this->M= Mat::zeros(r,c,CV_32FC1); // Modulos

}

void LKanade::Calcula_UV(){

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	//calculo sumatorios
	#pragma omp parallel for simd collapse (2)
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
					this->IyiIxi.at<float>(i,j)+=(this->Iy.at<float>(swi,swj))*(this->Ix.at<float>(swi,swj));
				}
			}
		}
	}

	//calculo de U,V
	#pragma omp parallel for simd collapse (2)
	for (int i=this->vecindad;i<=(r-this->vecindad);i++){
		for (int j=this->vecindad;j<=(c-this->vecindad);j++){

			float a=this->Iy2i.at<float>(i,j);
			float b=this->IxiIti.at<float>(i,j);
			float c=this->IyiIxi.at<float>(i,j);
			float d=this->IyiIti.at<float>(i,j);
			float e=this->Ix2i.at<float>(i,j);

			float num=(-a*b+c*d)/(e*a-c*c);
			float den=(c*d-e*d)/(e*a-c*c);

			if (isnan(num)){
				this->U.at<float>(i,j)=0;
			} else {
				this->U.at<float>(i,j)=(-a*b+c*d)/(e*a-c*c);
			}

			if (isnan(den)){
				this->V.at<float>(i,j)=0;
			} else {
				this->V.at<float>(i,j)=(c*d-e*d)/(e*a-c*c);
			}
			//cerr<<"( "<<this->U.at<float>(i,j)<<" , "<< this->V.at<float>(i,j)<<" )\n";
		}
	}


	/*namedWindow( "V", WINDOW_AUTOSIZE );
	imshow( "V", this->V );
	namedWindow( "U", WINDOW_AUTOSIZE );
	imshow( "U", this->U );
	waitKey(0);
	destroyAllWindows();*/

}

cv::Mat* LKanade::get_U(){

	return &(this->U);
}
cv::Mat* LKanade::get_V(){

	return &(this->V);
}

void LKanade::pintaVector(cv::Mat* img_a){
	int c=(img_a)->cols;
	int r=(img_a)->rows;
	#pragma omp parallel for simd collapse (2)
	for (int i=3;i<=(r-3);i++){
		for (int j=3;j<=(c-3);j++){
			CvPoint p = cvPoint(j, i);
			float modulo = sqrt(this->U.at<float>(i,j) + this->V.at<float>(i,j));
			if (modulo>0){
				int x2,y2;
				if(this->U.at<float>(i,j)<0 and this->U.at<float>(i,j)<-5)
					x2=-5;
				if(this->U.at<float>(i,j)>0 and this->U.at<float>(i,j)>5)
					x2=5;
				if(this->V.at<float>(i,j)<-0 and this->V.at<float>(i,j)<-5)
					y2=-5;
				if(this->V.at<float>(i,j)>0 and this->V.at<float>(i,j)>-5)
				    y2=5;

				CvPoint p2 = cvPoint(p.x + x2, p.y + y2);
				cv::line( *img_a, p, p2, CV_RGB(0,255,0), 1, CV_AA, 0 );

			}
		}
	}

}


LKanade::~LKanade() {
	// TODO Auto-generated destructor stub
}
