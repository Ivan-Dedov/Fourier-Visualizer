#include <iostream>
#include <math.h>
#include <fstream>
#include "complex.h" // complex functions
#include "svgreader.h" // reads .SVG as .TXT
#include "grapher.h" // create value table + graphing

using namespace std;

int main()
{
	// Input - drawing
	int Approximate = 2;
	while ((Approximate != 1) && (Approximate != 0)) {
		cout << "[>] What do you want to draw? (1 for VECTOR APPROXIMATION, 0 for ORIGINAL): ";
		cin >> Approximate;
	}

	int vectorCount = 0;
	int outputPointCount = -1;

	if (Approximate == 1) {
		// Input - number of vectors
		do {
			cout << "[>] Number of vectors: ";
			cin >> vectorCount;
		} while (vectorCount <= 0);

		// Input - number of points
		do {
			cout << "[>] Number of points to be plotted (MUST BE greather than number of vectors), 0 for DEFAULT (1250): ";
			cin >> outputPointCount;
			if (outputPointCount == 0) outputPointCount = 1250;
		} while (outputPointCount < vectorCount);
	}
	else {
		vectorCount = 1;
		outputPointCount = 2;
	}

	// Input - zoom
	float zoom = 0;
	do {
		cout << "[>] Zoom (0..1 to ZOOM OUT, 1 for NO ZOOM, > 1 to ZOOM IN): ";
		cin >> zoom;
	} while (zoom <= 0);

	// Input - X and Y offset
	float xOffset, yOffset;
	cout << "[>] Offset on X-axis: ";
	cin >> xOffset;
	cout << "[>] Offset on Y-axis: ";
	cin >> yOffset;

	// Input - ShowAxes
	int ShowAxes = 2;
	while ((ShowAxes != 1) && (ShowAxes != 0)) {
		cout << "[>] Want to show axes? (1 for YES, 0 for NO): ";
		cin >> ShowAxes;
	}

	cout << "===== EXECUTION STARTED =====" << endl;

	// Reading SVG code
	cout << "[#] Reading SVG..." << endl;
	char copier = 'T'; //initialize as non-zero
	ifstream SVG_in;
	ofstream SVG_out;
	SVG_in.open("Files/input.txt", ios::in);
	SVG_out.open("Files/svg_code.txt", ios::out);

	if ((!SVG_in.is_open()) || (!SVG_out.is_open())) {
		cout << "[X] ERROR 101: CANNOT OPEN FILE input.txt" << endl;
		exit(101);
	}

	float HorizRes, VertRes;
	SVG_in >> HorizRes >> VertRes;
	while (copier != '\0') {
		copier = '\0';
		SVG_in.get(copier);
		if (copier == ',') SVG_out << " ";
			else {
				if ((copier == 'M') || (copier == 'm') || (copier == 'L') || (copier == 'l') || (copier == 'H') || (copier == 'h') || (copier == 'V') || (copier == 'v') || (copier == 'Q') || (copier == 'q') || (copier == 'T') || (copier == 't') || (copier == 'C') || (copier == 'c') || (copier == 'S') || (copier == 's') || (copier == 'Z') || (copier == 'z')) SVG_out << copier << " ";
				else SVG_out << copier;
			}
	}
	SVG_in.close();
	SVG_out.close();
	int pointCount = ReadSVG(0.005); //ReadSVG() returns the number of points with calculated coordinates
	cout << "[!] Finished reading SVG." << endl;

	// Building table of the function
	cout << "[#] Building value table of given input..." << endl;
	fTable(pointCount, HorizRes, VertRes, zoom, xOffset, yOffset);
	cout << "[!] Finished building table." << endl;

	if (Approximate) {

		// Calculating coefficients
		cout << "[#] Calculating coefficients..." << endl;
		ofstream Coefficients;
		Coefficients.open("Files/coefficients.txt", ios::out);
		if (!Coefficients.is_open()) {
			cout << "[X] ERROR 102: CANNOT OPEN FILE coefficients.txt" << endl;
			exit(102);
		}

		Cplx integrRes; //result of integration
		int vectorsCalculated = 1; //current vector out of vectorCount

		integrRes = Integrate(pointCount, 0);
		Coefficients << 0 << " " << integrRes.Real << " " << integrRes.Imag << endl;
		for (int i = 1; vectorsCalculated < vectorCount; i++) {

			integrRes = Integrate(pointCount, i);
			Coefficients << i << " " << integrRes.Real << " " << integrRes.Imag << endl;
			vectorsCalculated++;

			if (vectorsCalculated < vectorCount) {
				integrRes = Integrate(pointCount, -i);
				Coefficients << -i << " " << integrRes.Real << " " << integrRes.Imag << endl;
				vectorsCalculated++;
			}

		}
		Coefficients.close();
		cout << "[!] Finished calculating coefficients." << endl;

		// Graphing
		cout << "[#] Graphing..." << endl;
		Graph(vectorCount, outputPointCount, ShowAxes);
	}
	else {
		cout << "[#] Graphing..." << endl;
		drawOriginal(ShowAxes);
	}

	cout << "[!] Finished graphing." << endl;

	return 0;
}