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
	Mat Ix;
	Mat Iy;
	Mat Ix2;
	Mat Iy2;
	Mat It;
	Mat U0;
	Mat V0;
	Mat U;
	Mat V;
	Mat M;


public:
	HShunck(int vecindad,float landa,cv::Mat* img_t);
	void Calcula_gradiente();
	void Iterar(int iteraciones, float margen,cv::Mat* img_t);
	virtual ~HShunck();
};


#endif /* HSHUNCK_H_ */
