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
	this->IyiIxi = Mat::zeros(r,c,CV_32F);
	this->U= Mat::zeros(r,c,CV_32F); // componente U
	this->V= Mat::zeros(r,c,CV_32F); // componente V
	this->M= Mat::zeros(r,c,CV_32F); // Modulos

}

void LKanade::Calcula_UV(){

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	//calculo sumatorios
	this->Calcula_sumatorios();

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

void LKanade::Calcula_sumatorios(){

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

}

void LKanade::pintaVector(cv::Mat* img_a){
	int c=(img_a)->cols;
	int r=(img_a)->rows;
	#pragma omp parallel for simd collapse (2)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+5){ //filas, o sea,y
		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+5){ //columnas, o sea, x

			CvPoint p = cvPoint(j,i);
			float modulo = sqrt(this->U.at<float>(i,j) + this->V.at<float>(i,j)); //at(fila,columna)
			float x2=U.at<float>(i,j);
			float y2=V.at<float>(i,j);
			if (modulo>0){

				CvPoint p2 = cvPoint(p.x + x2, p.y +y2);
				float ang= atan((p.y+y2)/(p.x+x2))*180 / CV_PI;
				//CvPoint dir = cv::Point(p.x+(5* cos(ang)), p.y+(5 * sin(ang))); // calculate direction
				CvPoint dir = cv::Point(p.x+(x2*20), p.y+(y2*20)); // calculate direction
				if (isnan(ang)){
					continue;
				}
				//cout<<"------------------------------\n";
				//cout<<"p.x:"<<p.x<<" p.y:"<<p.y<<"\n";
				//cout<<"ang:"<<ang<<" dir_x:"<<dir.x<<" dir_y:"<<dir.y<<"\n";

				//cv::line( *img_a, p, dir, CV_RGB(0,255,0), 1, CV_AA, 0 );
			    cv::arrowedLine(*img_a, p,dir, CV_RGB(0,255,0), 1,  CV_AA, 0, 0.5); // draw arrow!
			}
		}
	}

}


LKanade::~LKanade() {
	// TODO Auto-generated destructor stub
}
