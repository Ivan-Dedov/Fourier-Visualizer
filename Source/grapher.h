#pragma once

#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <GLFW/glfw3.h>

using namespace std;

class Point {

public:
	float x;
	float y;
	float opacity;
};

float getMagnitude(Cplx z) { // returns magnitude of a complex number
	return sqrt(z.Real * z.Real + z.Imag * z.Imag);
}

void drawAxes() { //draws X and Y axes
	glColor3f(0.85f, 0.85f, 0.85f);
	glLineWidth(1.5);

	glBegin(GL_LINE_STRIP); // X axis
	glVertex2f(-1, 0);
	glVertex2f(1, 0);
	glEnd();

	glBegin(GL_LINE_STRIP); // Y axis
	glVertex2f(0, -1);
	glVertex2f(0, 1);
	glEnd();

	glBegin(GL_TRIANGLES); // X-axis arrow tip
	glVertex2d(0.95, 0.01);
	glVertex2d(0.95, -0.01);
	glVertex2d(1, 0);
	glEnd();

	glBegin(GL_TRIANGLES); // Y-axis arrow tip
	glVertex2d(-0.01, 0.95);
	glVertex2d(0.01, 0.95);
	glVertex2d(0, 1);
	glEnd();
	
	return;
}

void drawCircle(float x, float y, float radius) { // draws hollow circles
	int lineAmount = 50;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= lineAmount; i++) {
		glVertex2f(x + (radius * cos(2 * i * (float)M_PI / lineAmount)), y + (radius * sin(2 * i * (float)M_PI / lineAmount)));
	}
	glEnd();

	return;
}

void drawOriginal(int ShowAxes) {
	GLFWwindow* window;
	if (!glfwInit()) {
		cout << "[X] ERROR 401: CANNOT INITIALISE GLFW" << endl;
		exit(401);
	}

	window = glfwCreateWindow(900, 900, "FourierGraph", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cout << "[X] ERROR 402: CANNOT CREATE WINDOW" << endl;
		exit(402);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (ShowAxes == 1) drawAxes(); // draw axes if user wants

		ifstream Table;
		Table.open("Files/function.txt", ios::in);

		if (!Table.is_open()) {
			glfwTerminate();
			cout << "[X] ERROR 403: CANNOT OPEN FILE function.txt" << endl;
			exit(403);
		}

		float t, x, y;
		glColor3f(0.4f, 0.4f, 0.4f);
		glPointSize(1.75);
		glBegin(GL_POINTS);
		while (!Table.eof()) {
			Table >> t >> x >> y;
			glVertex2d(x, y);
		}
		glEnd();
		glfwSwapBuffers(window);

		Table.close();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Graph(int vectorCount, int pointCount, int ShowAxes) { // graphs the function
	
	GLFWwindow* window;
	if (!glfwInit()) {
		cout << "[X] ERROR 401: CANNOT INITIALISE GLFW" << endl;
		exit(401);
	}

	window = glfwCreateWindow(900, 900, "FourierGraph", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cout << "[X] ERROR 402: CANNOT CREATE WINDOW" << endl;
		exit(402);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	int k = -1;
	vector<Point> points(pointCount + 1);
	vector<Cplx> Coefficient(vectorCount);
	vector<int> vectorFrequency(vectorCount);
	float interval = (float)1 / pointCount;

	for (float t = 0; t < 1; t += interval) {
		k++;

		ifstream Coefficients;
		Coefficients.open("Files/coefficients.txt", ios::in);

		if (!Coefficients.is_open()) {
			glfwTerminate();
			cout << "[X] ERROR 404: CANNOT OPEN FILE coefficients.txt" << endl;
			exit(404);
		}

		Cplx point = { 0, 0 };
		for (int j = 0; j < vectorCount; j++) {
			Coefficients >> vectorFrequency[j] >> Coefficient[j].Real >> Coefficient[j].Imag;
			point = Add(point, Multiply(Coefficient[j], ToPowerTheta(2 * (float)M_PI * vectorFrequency[j] * t)));
		}
		Coefficients.close();

		points[k].x = point.Real;
		points[k].y = point.Imag;
	}

	deque<Point> outputPoints; // deque for plotting points
	float prev_x, prev_y;
	Cplx add = { 0, 0 };
	k = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		outputPoints.push_front({ points[k].x, points[k].y, 1 });

		if (k >= pointCount) k = 0;

		int dequeSize = outputPoints.size();
		for (int i = 1; i < dequeSize; i++) outputPoints.at(i).opacity -= interval;
		
		// visual clues - circles and lines
		prev_x = 0;
		prev_y = 0;
		add = { 0, 0 };
		for (int n = 0; n < vectorCount; n++) {
			// draw line
			glColor3f(0.7f, 0.7f, 0.7f);
			glLineWidth(1);
			add = Multiply(Coefficient[n], ToPowerTheta(2 * (float)M_PI * vectorFrequency[n] * k * interval));
			glBegin(GL_LINE_STRIP);
			glVertex2f(prev_x, prev_y);
			glVertex2f(prev_x + add.Real, prev_y + add.Imag);
			glEnd();
			
			// draw circle
			glColor4f(0.0f, 0.8f, 0.8f, 0.4f);
			glLineWidth(1);
			drawCircle(prev_x, prev_y, getMagnitude(add));
			
			prev_x += add.Real;
			prev_y += add.Imag;
		}
		
		if (ShowAxes == 1) drawAxes(); // draw axes if user wants

		// plotting points
		glPointSize(2);
		glBegin(GL_POINTS);
		for (int i = 0; i < dequeSize; i++) {
			glColor4f(1.0f, 1.0f, 0.0f, outputPoints.at(i).opacity);
			glVertex2d(outputPoints.at(i).x, outputPoints.at(i).y);
		}
		glEnd();
		glfwSwapBuffers(window);

		if (outputPoints.back().opacity <= 0.01) outputPoints.pop_back();

		k++;
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return;
}