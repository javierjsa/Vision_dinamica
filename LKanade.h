/*
 * LKanade.h
 *
 *  Created on: 7 mar. 2017
 *      Author: jsa
 */

#ifndef LKANADE_H_
#define LKANADE_H_

#include "Calculaflujos.h"

class LKanade: public Calculaflujos {

private:
	int vecindad;
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
	LKanade(int vecindad,cv::Mat* img_t, cv::Mat* img_t1);

	void Calcula_UV();

	cv::Mat* get_U();
	cv::Mat* get_V();

	void pintaVector(cv::Mat* img_a);

	virtual ~LKanade();
};

#endif /* LKANADE_H_ */
