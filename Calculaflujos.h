/*
 * Calculaflujos.h
 *
 *  Created on: 6 mar. 2017
 *      Author: jsa
 */
#ifndef INCLUDES
#define INCLUDES

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>


#endif

using namespace cv;
using namespace std;

#ifndef CALCULAFLUJOS_H_
#define CALCULAFLUJOS_H_



class Calculaflujos {

protected:

	Mat* img_t; //Imagen t
	Mat* img_t1; // Imagen t+1
	Mat  Ixt; // Derivada X t
	Mat  Ixt1; // Derivada X t+1
	Mat  Iyt; // Derivada Y t
	Mat  Iyt1; // Derivada Y t+1
	Mat  Ix;   // Derivada X
	Mat  Iy;   // Derivada Y
	Mat  I2t;  // Suavizado t
	Mat  I2t1; // Suavizado t+1
	Mat  It; // Suavizado

	virtual void Calcula_gradiente();

public:

	Calculaflujos(cv::Mat* img_t, cv::Mat* img_t1);


	virtual ~Calculaflujos();

};

#endif
