/*
 * HShunck.cpp
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#include "HShunck.h"


#define D_TYPE CV_32F

HShunck::HShunck(int vecindad,int step,float landa,cv::Mat* img_t,cv::Mat* img_t1){

	this->img_t=img_t;
	this->img_t1=img_t1;

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->vecindad=vecindad;
	this->step=step;
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
	this->U0= Mat::zeros(r,c,CV_32F); // componente U
	this->V0= Mat::zeros(r,c,CV_32F); // componente V
	this->M= Mat::zeros(r,c,CV_32F); // Modulos

}

void HShunck::Clean(){

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	this->Umean=Mat::zeros(r,c,CV_32F);
	this->Vmean=Mat::zeros(r,c,CV_32F);

	this->Uact=Mat::zeros(r,c,CV_32F);
	this->Vact=Mat::zeros(r,c,CV_32F);

	this->Uact=Scalar(0.0001);
	this->Vact=Scalar(0.0001);

	this->Umean=Mat::zeros(r,c,CV_32F);
	this->Vmean=Mat::zeros(r,c,CV_32F);



}

void HShunck::Calcula_gradiente(cv::Mat* img_t,cv::Mat* img_t1){

		Mat I2t,I2t1,Ixt,Iyt,Ixt1,Iyt1;

		this->img_t=img_t;
		this->img_t1=img_t1;

		int kernel_size = 41;



		GaussianBlur(*(this->img_t),I2t,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
		GaussianBlur(*img_t1,I2t1,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);

		this->It=I2t1-I2t;

		Mat kernelx = (Mat_<float>(1,3)<<-0.5, 0, 0.5);
		Mat kernely = (Mat_<float>(3,1)<<-0.5, 0, 0.5);

		filter2D(I2t, this->Ix, -1, kernelx);
		filter2D(I2t, this->Iy, -1, kernely);

		this->Ix2=this->Ix.mul(this->Ix);
		this->Iy2=this->Iy.mul(this->Iy);
		/*
		int kernel_size = 41;

		GaussianBlur(*(this->img_t),I2t,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
		GaussianBlur(*(this->img_t1),I2t1,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);

		this->It=(I2t1)-(I2t);

		Mat kernelx = (Mat_<float>(1,3)<<-0.5, 0, 0.5);
		Mat kernely = (Mat_<float>(3,1)<<-0.5, 0, 0.5);

		filter2D(I2t, Ixt, -1, kernelx);
		filter2D(I2t, Iyt, -1, kernely);

		filter2D(I2t1, Ixt1, -1, kernelx);
		filter2D(I2t1, Iyt1, -1, kernely);

		/*Sobel(*this->img_t, this->Ixt, D_TYPE, 1, 0, 3);
		Sobel(*this->img_t, this->Iyt, D_TYPE, 0, 1, 3);

		Sobel(this->I2t1, this->Ixt1, D_TYPE, 1, 0, 3);
		Sobel(this->I2t1, this->Iyt1, D_TYPE, 0, 1, 3);

		this->Ix=(Ixt+Ixt1)/2;
		this->Iy=(Iyt+Iyt1)/2;
		*/


}


void HShunck::Calcula_UV(int iteraciones,float margen,Mat* img_t,cv::Mat* img_t1){

	int it=0;
	this->Clean();
	this->Calcula_gradiente(img_t,img_t1);
	while (it<=iteraciones){
		bool conv=this->Aux_UV(margen,img_t,img_t1);
		if (conv)
			break;
		it++;
	}

}


bool HShunck::Aux_UV(float margen,cv::Mat* img_t,cv::Mat* img_t1){


	this->Uant=this->Uact.clone();
	this->Vant=this->Vact.clone();

	int c=(this->img_t)->cols;
	int r=(this->img_t)->rows;

	//calculo de Umean,Vmean
	#pragma omp parallel for schedule(static,1)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+this->step){
		float* _Umean=(this->Umean).ptr<float>(i);
		float* _Vmean=(this->Vmean).ptr<float>(i);

		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+this->step){

			for (int swi=(i-this->vecindad);swi<=(i+this->vecindad);swi++){

				float* _Uant=(this->Uant).ptr<float>(swi);
				float* _Vant=(this->Vant).ptr<float>(swi);

				for (int swj=(j-this->vecindad);swj<=(j+this->vecindad);swj++){

					_Umean[j]+=_Uant[swj];
					_Vmean[j]+=_Vant[swj];
				}
			}
			_Umean[j]=_Umean[j]/(this->vecindad*this->vecindad);
			_Vmean[j]=_Vmean[j]/(this->vecindad*this->vecindad);
		}
	}

	//calculo de Uact,Vact
	#pragma omp parallel for schedule(static,1)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+this->step){

		float* _Uact= (this->Uact).ptr<float>(i);
		float* _Vact= (this->Vact).ptr<float>(i);
		float* _Ix= (this->Ix).ptr<float>(i);
		float* _Iy= (this->Iy).ptr<float>(i);
		float* _Ix2= (this->Ix2).ptr<float>(i);
		float* _Iy2= (this->Iy2).ptr<float>(i);
		float* _It= (this->It).ptr<float>(i);
		float* _Umean=(this->Umean).ptr<float>(i);
		float* _Vmean=(this->Vmean).ptr<float>(i);
		float* _M=(this->M).ptr<float>(i);

		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+this->step){

			float num = (_Ix[j]*_Umean[j] + _Iy[j]*_Vmean[j]+_It[j]);
			float den = this->landa*this->landa+_Ix2[j]+_Iy2[j];

			//if (abs(den)<0.0001)
			//	cerr<<"error\n";

			_Uact[j]=_Umean[j] - _Ix[j]*(num/den);
			_Vact[j]=_Vmean[j] - _Iy[j]*(num/den);
			_M[j]=sqrt(_Uact[j]*_Uact[j]+_Vact[j]*_Vact[j]);



		}
	}
	//Calcular convergencia
	if (margen !=-1){
		float margenU=sum(this->Uact - this->Uant)[0];
		float margenV=sum(this->Vant - this->Vant)[0];
		if (margenU<=margen and margenV<=margen)
			return true;
	}
	return false;
}

void HShunck::pintaVector(cv::Mat* img_a){
	int c=(img_a)->cols;
	int r=(img_a)->rows;

	double minh, maxh;
	Scalar medsc=mean(this->M);
	float med=log(medsc.val[0])*log(this->step);
	cv::minMaxLoc(this->M, &minh, &maxh);
	//float mlog =log(maxh);

	//Mat cM,cMU8;
	//this->M.convertTo(cMU8, CV_8UC1);
	//applyColorMap(cMU8, cM, COLORMAP_JET);


	//float mean= (float)(max+min)/2;

	#pragma omp parallel for schedule(static,1)
	for (int i=this->vecindad;i<=(r-this->vecindad);i=i+this->step){ //filas, o sea,y

		float* _Uact=this->Uact.ptr<float>(i);
		float* _Vact=this->Vact.ptr<float>(i);
		float* _M=this->M.ptr<float>(i);
		//float* _cM=cM.ptr<float>(i);

		for (int j=this->vecindad;j<=(c-this->vecindad);j=j+this->step){ //columnas, o sea, x

			CvPoint p = cvPoint(j,i);
			float modulo = _M[j]; //at(fila,columna)


			if (modulo>1){
				float x2=_Uact[j];
				float y2=_Vact[j];
				float max = (abs(x2) > abs(y2)) ? abs(x2) : abs(y2);
				float ratio = (this->step)/max;
				int hue=(int)((modulo-minh)*255)/(maxh-minh);
				//CvPoint p2 = cvPoint(p.x + x2, p.y +y2);
				float ang= atan((p.y+y2)/(p.x+x2))*180 / CV_PI;
				int hue2=(int)((ang)*255)/(360);
				//CvPoint dir = cv::Point(p.x+(5* cos(ang)), p.y+(5 * sin(ang))); // calculate direction
				Point dir;
				if ((this->step)>4){
					//dir = Point(p.x+x2, p.y+y2); // calculate direction
					//dir = Point(p.x+x2*abs(med), p.y+y2*abs(med)); // calculate direction
					dir = Point(p.x+x2*ratio, p.y+y2*ratio); // calculate direction
					cv::arrowedLine(*img_a, p,dir, CV_RGB(0,hue,0), 1,  CV_AA, 0, 0.5);
				}else if (hue>5) {
					dir = Point(p.x+x2/log(maxh), p.y+y2/log(maxh));
					circle(*img_a,p, this->step, CV_RGB(0,hue,0), 1, 8, 0);
				}

				//if (isnan(ang)){
				//	continue;
				//}
			    //cv::arrowedLine(*img_a, p,dir, , 1,  CV_AA, 0, 0.5);


			}
		}
	}

}





HShunck::~HShunck() {
	// TODO Auto-generated destructor stub
}

