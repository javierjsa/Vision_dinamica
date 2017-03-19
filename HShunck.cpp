/*
 * HShunck.cpp
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#include "HShunck.h"

HShunck::HShunck(int vecindad,float landa,cv::Mat* img_t){

	this->img_t=img_t;

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->vecindad=vecindad;
	this->landa=landa;

	this->Uant=Mat::zeros(r,c,CV_32F);
	this->Vant=Mat::zeros(r,c,CV_32F);

	this->Uact=Mat::zeros(r,c,CV_32F);
	this->Vact=Mat::zeros(r,c,CV_32F);

	this->Umean=Mat::zeros(r,c,CV_32F);
	this->Vmean=Mat::zeros(r,c,CV_32F);


	this->Ix= Mat::zeros(r,c,CV_32F); //Sumatorio Ix
	this->Iy= Mat::zeros(r,c,CV_32F); // Sumatorio Iy
	this->Ix2= Mat::zeros(r,c,CV_32F); // Sumatorio Ix al cuadrado
	this->Iy2= Mat::zeros(r,c,CV_32F); // Sumatorio Iy al cuadrado
	this->It= Mat::zeros(r,c,CV_32F);  // Sumatorio It
	this->U= Mat::zeros(r,c,CV_32F); // componente U
	this->V= Mat::zeros(r,c,CV_32F); // componente V
	this->M= Mat::zeros(r,c,CV_32F); // Modulos

}

void HShunck::Calcula_gradiente(){



		int kernel_size = 21;

		GaussianBlur(*this->img_t,this->It,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);

		Sobel(*this->img_t, this->Ix, CV_32F, 1, 0, 3);
		Sobel(*this->img_t, this->Iy, CV_32F, 0, 1, 3);

		this->Ix2=this->Ix.mul(this->Ix);
		this->Iy2=this->Iy.mul(this->Iy);

}



void HShunck::Iterar(int iteraciones, float margen,cv::Mat* img_t){

	int it_aux=0;
	int margen_aux=100;

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	//calculo de Umean,Vmean
	#pragma omp parallel for simd collapse (2)
	for (int i=this->vecindad;i<=(r-this->vecindad);i++){
		for (int j=this->vecindad;j<=(c-this->vecindad);j++){
			float u_acum;
			float v_acum;
			for (int swi=(i-this->vecindad);swi<=(i+this->vecindad);swi++){
				for (int swj=(j-this->vecindad);swj<=(j+this->vecindad);swj++){

					u_acum+=this->Uant.at<float>(swi,swj);
					v_acum+=this->Vant.at<float>(swi,swj);
				}
			}
			this->Uact.at<float>(i,j)=u_acum/(this->vecindad*this->vecindad);
			this->Vact.at<float>(i,j)=v_acum/(this->vecindad*this->vecindad);
		}
	}

	//calculo de Uact,Vact
	#pragma omp parallel for simd collapse (2)
	for (int i=this->vecindad;i<=(r-this->vecindad);i++){
		for (int j=this->vecindad;j<=(c-this->vecindad);j++){

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

