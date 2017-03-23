/*
 * LKanadePinv.h
 *
 *  Created on: 12 mar. 2017
 *      Author: jsa
 */

#ifndef LKANADEPINV_H_
#define LKANADEPINV_H_

#include "LKanade.h"

class LKanadePinv: public LKanade {

protected:

	Mat matrix;
	Mat UV;


public:
	LKanadePinv(int vecindad,int step,cv::Mat* img_t, cv::Mat* img_t1);
	void Calcula_UV(cv::Mat* img_t, cv::Mat* img_t1);
	virtual ~LKanadePinv();
};

#endif /* LKANADEPINV_H_ */
