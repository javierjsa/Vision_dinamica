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
	int constante;

public:
	LKanade(int a,cv::Mat* img_t, cv::Mat* img_t1);
	virtual ~LKanade();
};

#endif /* LKANADE_H_ */
