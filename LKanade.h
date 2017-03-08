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


public:
	LKanade(int vecindad,cv::Mat* img_t, cv::Mat* img_t1);

	void Calcula_UV();

	virtual ~LKanade();
};

#endif /* LKANADE_H_ */
