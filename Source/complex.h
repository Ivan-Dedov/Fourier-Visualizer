#pragma once

#include <iostream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class Cplx { // class of complex numbers

public:
	float Real; // real part
	float Imag; // imaginary part
};

Cplx Add(Cplx z1, Cplx z2) { // sum of 2 complex numbers
	Cplx out = { 0, 0 };
	out.Real = z1.Real + z2.Real;
	out.Imag = z1.Imag + z2.Imag;
	return out;
};

Cplx Subtract(Cplx z1, Cplx z2) { // difference of 2 complex numbers
	Cplx out = { 0, 0 };
	out.Real = z1.Real - z2.Real;
	out.Imag = z1.Imag - z2.Imag;
	return out;
};

Cplx Multiply(Cplx z1, Cplx z2) { // product of 2 complex numbers
	Cplx out = { 0, 0 };
	out.Real = z1.Real * z2.Real - z1.Imag * z2.Imag;
	out.Imag = z1.Real * z2.Imag + z1.Imag * z2.Real;
	return out;
}

Cplx Divide(Cplx z1, Cplx z2) { // quotient (division) of 2 complex numbers
	Cplx out = { 0, 0 };
	if ((z2.Real == 0) && (z2.Imag == 0)) return { 0, 0 };
	else {
		out.Real = (z1.Real * z2.Real + z1.Imag * z2.Imag) / (z2.Real * z2.Real + z2.Imag * z2.Imag);
		out.Imag = (z2.Real * z1.Imag - z1.Real * z2.Imag) / (z2.Real * z2.Real + z2.Imag * z2.Imag);
		return out;
	}
};

Cplx ToPowerTheta(float angle) { // e^(i*angle)
	Cplx out = { 0, 0 };
	out.Real = cos(angle);
	out.Imag = sin(angle);
	return out;
}

Cplx Integrate(int k_max, int n) { // integrates f(t)*e^(-2*pi*i*n*t)dt over 0..duration
	Cplx out = { 0, 0 };
	
	ifstream Table;
	Table.open("Files/function.txt", ios::in);

	if (!Table.is_open()) {
		cout << "[X] ERROR 201: CANNOT OPEN FILE function.txt" << endl;
		exit(201);
	}

	// Integrating
	Cplx value = { 0, 0 }, dt;
	float t, next_t;

	Table >> t;
	for (int i = 0; i < k_max; i++) {
		next_t = 0;
		Table >> value.Real >> value.Imag >> next_t;

		dt = { next_t - t, 0 };
		out = Add(out, Multiply(Multiply(value, ToPowerTheta(n * (-2) * (float)M_PI * t)), dt));
		t = next_t;
	}
	Table.close();
	
	return out; 
}