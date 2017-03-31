/*
 * FiltroParticulas.h
 *
 *  Created on: 31 mar. 2017
 *      Author: jsa
 */

#ifndef FILTROPARTICULAS_H_
#define FILTROPARTICULAS_H_

using namespace cv;
using namespace std;

class FiltroParticulas {

private:

	vector<std::array<float, 7>> particulas;
	vector<std::array<float, 7>> particulas_aux;
	RNG uni_x;
	RNG uni_y;
	RNG gauss_u;
	RNG gauss_v;
	RNG uni_h;
	RNG uni_w;

	float sigma_x;
	float sigma_y;
	float sigma_w;
	float sigma_h;
	float sigma_u;
	float sigma_v;



	void Reseed();

public:
	FiltroParticulas(int n_part,Mat& mascara);
	void CalcularPesos(Mat& mascara);
	void GenerarParticulas(Mat& mascara);
	void SeleccionarParticulas();
	void PerturbarParticulas();
	void PintarResultado(Mat& imagen);
	virtual ~FiltroParticulas();
};

#endif /* FILTROPARTICULAS_H_ */
