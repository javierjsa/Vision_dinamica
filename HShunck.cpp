/*
 * HShunck.cpp
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#include "HShunck.h"

HShunck::HShunck(int vecindad,float landa,cv::Mat* img_t, cv::Mat* img_t1):Calculaflujos(img_t,img_t1)  {

	this->vecindad=vecindad;
	this->landa=landa;

	this->Uant=0.1;
	this->Vant=0.1;

	this->Uact=0.0;
	this->Vact=0.0;

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->Ixi= Mat::zeros(r,c,CV_32FC1); //Sumatorio Ix
	this->Iyi= Mat::zeros(r,c,CV_32FC1); // Sumatorio Iy
	this->Ix2i= Mat::zeros(r,c,CV_32FC1); // Sumatorio Ix al cuadrado
	this->Iy2i= Mat::zeros(r,c,CV_32FC1); // Sumatorio Iy al cuadrado
	this->Iti= Mat::zeros(r,c,CV_32FC1);  // Sumatorio It
	//this->IxiIti= Mat::zeros(r,c,CV_32FC1); //Sumatorio Ixi* Iti
	//this->IyiIti= Mat::zeros(r,c,CV_32FC1); //Sumatorio  Iyi* Iti
	//this->Ix2iIy2i= Mat::zeros(r,c,CV_32FC1); // Sumatior Ix cuadrado * Iy cuadrado
	//this->IyiIxi = Mat::zeros(r,c,CV_32FC1);
	this->U= Mat::zeros(r,c,CV_32FC1); // componente U
	this->V= Mat::zeros(r,c,CV_32FC1); // componente V
	this->M= Mat::zeros(r,c,CV_32FC1); // Modulos

}


void HShunck::Iterar(int iteraciones, float margen){

	int it_aux=0;
	int margen_aux=100;

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	//calculo de U,V
	#pragma omp parallel for simd collapse (2)
	for (int i=this->vecindad;i<=(r-this->vecindad);i++){
		for (int j=this->vecindad;j<=(c-this->vecindad);j++){
			while (it_aux<iteraciones && margen_aux>margen){



			}
		}
	}

}

//probablemente no hace falta
/*
void HShunck::Calcula_sumatorios(){

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

}*/
HShunck::~HShunck() {
	// TODO Auto-generated destructor stub
}

