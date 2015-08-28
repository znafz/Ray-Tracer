/*
	Zachary Nafziger, 2015
	This is the geometry header file.

*/
#pragma once
#include <string>
#include <iostream>
#include "glm\glm.hpp"
using namespace glm;
using namespace std;

struct Color{
	Color();
	Color(float red, float green, float blue){r=red;g=green;b=blue;}
	void setIntensity(float i){r*=i;g*=i;b*=i;}
	void set(float x, float y, float z);
	float r,g,b;
};

Color operator*(Color c, float f);

Color operator+(Color c, Color c2);

struct Point
{
	vec4 v;
	vec4 normal;
	Color color;
	void out(){
		cout << v.x << " " << v.y << " " << v.z <<endl;
	}
};




struct Geometry{
	Geometry();
	float reflectivity; //i'm using a scale of 0 to 1
	string type;
	mat4 transform;
	Point * points;
	int numPoints;
	void out(){
		for(int i = 0; i < numPoints; i++)
		{
			cout << "Point " << i << ": ";
			points[i].out();
		}
	}
	void setColor(float r, float g, float b){
		for(int i = 0; i < numPoints; i++)
		{
			points[i].color = Color(r,g,b);
		}
	}
};

struct Cube: Geometry{
	Cube();
	void getNorms();
	void out();
};


struct Triangle: Geometry{
	Triangle();
	vec4 get_normal();
	Color color;
	
};

struct Sphere: Geometry{
	Sphere();
	Color color;
	float radius;
};

Point createPoint(float a, float b, float c, Color color);



Color getColor(vec4 intersection, Cube cube);

Color getColor(vec4 intersection, Sphere sphere);

vec4 get_normal(vec4 intersection, Cube cube);
