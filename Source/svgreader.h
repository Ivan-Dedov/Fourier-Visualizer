#pragma once

#include <iostream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

int ReadSVG(float interval) { // creates pre-table [k  x  y]

	ifstream inputPath; // input SVG path code (<path d="[THIS PART]" />)
	ofstream values; // pre-function file (almost finished)
	inputPath.open("Files/svg_code.txt", ios::in);
	values.open("Files/values.txt", ios::out);

	if (!inputPath.is_open()) {
		cout << "[X] ERROR 301: CANNOT OPEN FILE svg_code.txt" << endl;
		exit(301);
	}

	string cmd; // current command
	int k = 0; // index counter
	float current_x = 0, current_y = 0, start_x = 0, start_y = 0; // x, y coordinates of points
	float prev_x1 = 0, prev_y1 = 0, prev_x2 = 0, prev_y2 = 0; // save previous values
	float t, x, y, x1, y1, x2, y2, dx, dy, dx1, dy1, dx2, dy2;

	while (!inputPath.eof()) {
		inputPath >> cmd; // reading a command
		t = interval;

		// UPPERCASE COMMANDS

		if (cmd == "M") { // command M
			inputPath >> x >> y;
			current_x = x;
			start_x = x;
			current_y = y;
			start_y = y;
			values << endl << k << " " << x << " " << y;
			k++;
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		if (cmd == "L") { // command L
			inputPath >> x >> y;
			while (t <= 1) {
				values << endl << k << " " << (1 - t) * current_x + t * x << " " << (1 - t) * current_y + t * y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		if (cmd == "H") { // command H
			inputPath >> x;
			while (t < 1) {
				values << endl << k << " " << (1 - t) * current_x + t * x << " " << current_y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_x2 = current_x;
		}

		if (cmd == "V") { // command V
			inputPath >> y;
			while (t < 1) {
				values << endl << k << " " << current_x << " " << (1 - t) * current_y + t * y;
				t += interval;
				k++;
			}
			prev_y1 = current_y;
			prev_y2 = current_y;
		}

		if (cmd == "Q") { // command Q
			inputPath >> x1 >> y1 >> x >> y;
			while (t <= 1) {
				values << endl << k << " " << x1 + (1 - t) * (1 - t) * (current_x - x1) + t * t * (x - x1) << " " << y1 + (1 - t) * (1 - t) * (current_y - y1) + t * t * (y - y1);
				t += interval;
				k++;
			}
			prev_x1 = x1;
			prev_y1 = y1;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		if (cmd == "T") { // command T
			inputPath >> x >> y;
			x1 = 2 * current_x - prev_x1;
			y1 = 2 * current_y - prev_y1;
			while (t <= 1) {
				values << endl << k << " " << x1 + (1 - t) * (1 - t) * (current_x - x1) + t * t * (x - x1) << " " << y1 + (1 - t) * (1 - t) * (current_y - y1) + t * t * (y - y1);
				t += interval;
				k++;
			}
			prev_x1 = x1;
			prev_y1 = y1;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}
			
		if (cmd == "C") { // command C
			inputPath >> x1 >> y1 >> x2 >> y2 >> x >> y;
			while (t <= 1) {
				values << endl << k << " " << pow(1 - t, 3) * current_x + 3 * (1 - t) * (1 - t) * t * x1 + 3 * t * t * (1 - t) * x2 + t * t * t * x << " " << pow(1 - t, 3) * current_y + 3 * (1 - t) * (1 - t) * t * y1 + 3 * t * t * (1 - t) * y2 + t * t * t * y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = x2;
			prev_y2 = y2;
		}

		if (cmd == "S") { // command S
			inputPath >> x2 >> y2 >> x >> y;
			x1 = 2 * current_x - prev_x2;
			y1 = 2 * current_y - prev_y2;
			while (t <= 1) {
				values << endl << k << " " << pow(1 - t, 3) * current_x + 3 * (1 - t) * (1 - t) * t * x1 + 3 * t * t * (1 - t) * x2 + t * t * t * x << " " << pow(1 - t, 3) * current_y + 3 * (1 - t) * (1 - t) * t * y1 + 3 * t * t * (1 - t) * y2 + t * t * t * y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = x2;
			prev_y2 = y2;
		}

		// LOWERCASE COMMANDS

		if (cmd == "m") { // command m
			inputPath >> dx >> dy;
			start_x = current_x + dx;
			start_y = current_y + dy;
			x = current_x + dx;
			y = current_y + dy;
			values << endl << k << " " << start_x + dx << " " << start_y + dy;
			k++;
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		if (cmd == "l") { // command l
			inputPath >> dx >> dy;
			x = current_x + dx;
			y = current_y + dy;
			while (t <= 1) {
				values << endl << k << " " << (1 - t) * current_x + t * x << " " << (1 - t) * current_y + t * y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		if (cmd == "h") { // command h
			inputPath >> dx;
			x = current_x + dx;
			while (t < 1) {
				values << endl << k << " " << (1 - t) * current_x + t * x << " " << current_y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_x2 = current_x;
		}

		if (cmd == "v") { // command v
			inputPath >> dy;
			y = current_y + dy;
			while (t < 1) {
				values << endl << k << " " << current_x << " " << (1 - t) * current_y + t * y;
				t += interval;
				k++;
			}
			prev_y1 = current_y;
			prev_y2 = current_y;
		}

		if (cmd == "q") { // command q
			inputPath >> dx1 >> dy1 >> dx >> dy;
			x = current_x + dx;
			y = current_y + dy;
			x1 = current_x + dx1;
			y1 = current_y + dy1;
			while (t <= 1) {
				values << endl << k << " " << x1 + (1 - t) * (1 - t) * (current_x - x1) + t * t * (x - x1) << " " << y1 + (1 - t) * (1 - t) * (current_y - y1) + t * t * (y - y1);
				t += interval;
				k++;
			}
			prev_x1 = x1;
			prev_y1 = y1;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		if (cmd == "t") { // command t
			inputPath >> dx >> dy;
			x = current_x + dx;
			y = current_y + dy;
			x1 = 2 * current_x - prev_x1;
			y1 = 2 * current_y - prev_y1;
			while (t <= 1) {
				values << endl << k << " " << x1 + (1 - t) * (1 - t) * (current_x - x1) + t * t * (x - x1) << " " << y1 + (1 - t) * (1 - t) * (current_y - y1) + t * t * (y - y1);
				t += interval;
				k++;
			}
			prev_x1 = x1;
			prev_y1 = y1;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		if (cmd == "c") { // command c
			inputPath >> dx1 >> dy1 >> dx2 >> dy2 >> dx >> dy;
			x = current_x + dx;
			y = current_y + dy;
			x1 = current_x + dx1;
			y1 = current_y + dy1;
			x2 = current_x + dx2;
			y2 = current_y + dy2;
			while (t <= 1) {
				values << endl << k << " " << pow(1 - t, 3) * current_x + 3 * (1 - t) * (1 - t) * t * x1 + 3 * t * t * (1 - t) * x2 + t * t * t * x << " " << pow(1 - t, 3) * current_y + 3 * (1 - t) * (1 - t) * t * y1 + 3 * t * t * (1 - t) * y2 + t * t * t * y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = x2;
			prev_y2 = y2;
		}

		if (cmd == "s") { // command s
			inputPath >> dx2 >> dy2 >> dx >> dy;
			x1 = 2 * current_x - prev_x2;
			y1 = 2 * current_y - prev_y2;
			x = current_x + dx;
			y = current_y + dy;
			x2 = current_x + dx2;
			y2 = current_y + dy2;
			while (t <= 1) {
				values << endl << k << " " << pow(1 - t, 3) * current_x + 3 * (1 - t) * (1 - t) * t * x1 + 3 * t * t * (1 - t) * x2 + t * t * t * x << " " << pow(1 - t, 3) * current_y + 3 * (1 - t) * (1 - t) * t * y1 + 3 * t * t * (1 - t) * y2 + t * t * t * y;
				t += interval;
				k++;
			}
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = x2;
			prev_y2 = y2;
		}

		if ((cmd == "Z") || (cmd == "z")) { // command Z or z (equivalent)
			while (t <= 1) {
				values << endl << k << " " << (1 - t) * current_x + t * start_x << " " << (1 - t) * current_y + t * start_y;
				t += interval;
				k++;
			}
			current_x = start_x;
			current_y = start_y;
			prev_x1 = current_x;
			prev_y1 = current_y;
			prev_x2 = current_x;
			prev_y2 = current_y;
		}

		current_x = x;
		current_y = y;
	}

	inputPath.close();
	values.close();
	remove("Files/svg_code.txt"); // deletes svg_code.txt as not useful anymore

	return (k - 1);
}

int fTable(int pointCount, float svgHorizRes, float svgVertRes, float zoom, float xOffset, float yOffset) { // creates the value table [t Real Imag]
	ifstream Values; // almost ready file
	ofstream Eqn; // stores the table
	Values.open("Files/values.txt", ios::in);
	Eqn.open("Files/function.txt", ios::out);
	int k;
	float x, y;

	if ((!Values.is_open()) || (!Eqn.is_open())) {
		cout << "[X] ERROR 302: CANNOT OPEN FILE values.txt" << endl;
		exit(302);
	}

	while (!Values.eof()) {
		Values >> k >> x >> y;
		Eqn << (float)k / pointCount << " " << (x + xOffset - 0.5 * svgHorizRes) * zoom / svgHorizRes << " " << (-y + yOffset + 0.5 * svgVertRes) * zoom / svgVertRes << endl; // moves the SVG into negative coordinates
	}

	Values.close();
	Eqn.close();
	remove("Files/values.txt"); // deletes "values.txt" as not useful anymore

	return 0;
}