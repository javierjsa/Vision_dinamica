/*
 * HShunck.h
 *
 *  Created on: 7 mar. 2017
 *      Author: jsa
 */

#ifndef HSHUNCK_H_
#define HSHUNCK_H_

#include "Calculaflujos.h"

class HShunck: public Calculaflujos {
public:
	HShunck(int a,cv::Mat* img_t, cv::Mat* img_t1);
	virtual ~HShunck();
};

#endif /* HSHUNCK_H_ */
