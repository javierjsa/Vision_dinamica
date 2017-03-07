/*
 * LKanade.cpp
 *
 *  Created on: 7 mar. 2017
 *      Author: jsa
 */

#include "LKanade.h"

LKanade::LKanade(int a,cv::Mat* img_t, cv::Mat* img_t1):Calculaflujos(img_t,img_t1) {

	this->constante=a;
	// TODO Auto-generated constructor stub

}

LKanade::~LKanade() {
	// TODO Auto-generated destructor stub
}

