/*
 * HShunck.h
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#ifndef HSHUNCK_H_
#define HSHUNCK_H_

#include "Calculaflujos.h"

class HShunck {

private:
	int vecindad;
	float landa;

	Mat Uant;
	Mat Vant;

	Mat Uact;
	Mat Vact;

	Mat Umean;
	Mat Vmean;

	Mat* img_t;
	Mat* img_t1;
	Mat Ix;
	Mat Iy;
	Mat Ix2;
	Mat Iy2;
	Mat It;
	Mat U0;
	Mat V0;
	Mat M;

	int step;

	void Clean();
	void Calcula_gradiente(cv::Mat* img_t, cv::Mat* img_t1);
	bool Aux_UV(float margen,cv::Mat* img_t,cv::Mat* img_t1);

public:
	HShunck(int vecindad,int step,float landa,cv::Mat* img_t,cv::Mat* img_t1);
	void Calcula_UV(int iteraciones,float margen,cv::Mat* img_t,cv::Mat* img_t1);
	void pintaVector(cv::Mat* img_a);
	virtual ~HShunck();
};


#endif /* HSHUNCK_H_ */
