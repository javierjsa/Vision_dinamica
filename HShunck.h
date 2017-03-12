/*
 * HShunck.h
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#ifndef HSHUNCK_H_
#define HSHUNCK_H_

#include "Calculaflujos.h"

class HShunck: public Calculaflujos {

private:
	int vecindad;
	float landa;

	float Uant;
	float Vant;

	float Uact;
	float Vact;

	Mat Ixi;
	Mat Iyi;
	Mat Ix2i;
	Mat Iy2i;
	Mat Iti;
	Mat IxiIti;
	Mat IyiIti;
	Mat Ix2iIy2i;
	Mat IyiIxi;
	Mat U;
	Mat V;
	Mat M;

public:
	HShunck(int vecindad,float landa,cv::Mat* img_t, cv::Mat* img_t1);
	//void Calcula_sumatorios();
	void Iterar(int iteraciones, float margen);
	virtual ~HShunck();
};


#endif /* HSHUNCK_H_ */
