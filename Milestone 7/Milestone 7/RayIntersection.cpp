/*
	2015 Zachary Nafziger
*/
#include <iostream>
#include "RayIntersection.h"

using namespace glm;

float triangle_area(vec3 a, vec3 b, vec3 c){
	//using Heron's formula
	float side1 = length(a-b);
	float side2 = length(b-c);
	float side3 = length(c-a);
	float p = (side1 + side2 + side3)/2;
	return sqrt(p*(p-side1)*(p-side2)*(p-side3));
}

double Test_RaySphereIntersect(const vec4& P0, const vec4& V0, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.

	//define the origin and radius
	vec3 ORIGIN(0.0f, 0.0f, 0.0f);
	float r = 1;
	

	mat4 Cstar(T);
	Cstar[3][0] =0;
	Cstar[3][1] =0;
	Cstar[3][2] =0;
	vec4 P1(P0);
	P1.w = 1;
	vec3 P(inverse(T)*P1);
	
	vec3 V(inverse(Cstar)*V0);
	
	float a = dot(V, V);
	float b = 2.0f*dot(V, (P-ORIGIN));
	float c = dot((P-ORIGIN),(P-ORIGIN)) - r*r;

	float disc = b*b - 4*a*c;

	
	if(disc < 0.0f){
		return -1;
		
		}
	else{
		float t;
		float distSqrt = sqrt(disc);
        float q;

        if (b < 0.0)
            q = (-b - distSqrt)/2;
        else
            q = (-b + distSqrt)/2;

        float t0 = q/a;
        float t1 = c/q;

        if (t0 > t1)
        {
            double temp = t0;
            t0 = t1;
            t1 = temp;
        }

        if (t1 < 0.0)
            return -1;

        if (t0 < 0.0)
            t = t1;
        else
            t = t0;

		
		return t;
	}
	
	return -1;
}

double Test_RayPolyIntersect(const vec4& P0, const vec4& V0, const vec4& p1, const vec4& p2, const vec4& p3, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.

	//apply transformations
	vec3 a(p1);
	vec3 b(p2);
	vec3 c(p3);
	vec3 normal = cross((b-a),(c-a));;
	mat4 Cstar(T);
	Cstar[3][0] =0;
	Cstar[3][1] =0;
	Cstar[3][2] =0;
	vec4 P1(P0);
	P1.w = 1;
	vec3 P(inverse(T)*P1);
	vec3 V(inverse(Cstar)*V0);
	normal = normalize(normal);

	if(dot(normal, V) == 0) return -1;
	float t = dot(normal, (a-P))/dot(normal, V);
	//std::cout << t;
	if(t < 0) return -1;
	vec3 R = P + t*V;
	
	float S = triangle_area(a, b, c);
	float S1 = triangle_area(R, b, c)/S;
	float S2 = triangle_area(R, c, a)/S;
	float S3 = triangle_area(R, a, b)/S;
	float test = S1+S2+S3;
	
	if(S1 >= 0 && S1 <= 1 && S2 >= 0 && S2 <= 1 && S3 >= 0 && S3 <= 1 && S1+S2+S3-1.0f <0.001 && S1+S2+S3-1.0f >-0.001) return t;
	

	return -1;
}

double Test_RayCubeIntersect(const vec4& P0, const vec4& V0, const mat4& T) {
	// TODO fill this in.
	// See the documentation of this function in stubs.h.
	vec4 p1(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 p2(-0.5f, -0.5f, -0.5f, 1.0f);
	mat4 Cstar(T);
	Cstar[3][0] =0;
	Cstar[3][1] =0;
	Cstar[3][2] =0;
	vec3 a(p1);
	vec3 b(p2);
	vec4 P1(P0);
	P1.w = 1;
	vec3 P(inverse(T)*P1);
	vec3 V(inverse(Cstar)*V0);
	//check perpendicular cases
	//x
	if(V.y == 0 && V.z == 0)
		if(b.y <= P.y && P.y <= a.y && b.z <= P.z && P.z <= a.z)
		{
			//calculate the t's
			float t1, t2;
			/*if(V.x > 0)
			{*/
				t1 = (b.x - P.x)/V.x;
				t2 = (a.x - P.x)/V.x;
			/*} else{
				t1 = P.x - b.x;
				t2 = P.x - a.x;
			}*/
			if(t1 < 0 && t2 < 0) return -1;
			else if (t1 < t2 && t1 >= 0) return t1;
			else return t2;

		} else return -1;

	//y
	if(V.x == 0 && V.z == 0)
		if(b.x <= P.x && P.x <= a.x && b.z <= P.z && P.z <= a.z)
		{
			//calculate the t's
			float t1, t2;
			/*if(V.y > 0)
			{*/
				t1 = (b.y - P.y)/V.y;
				t2 = (a.y - P.y)/V.y;
			/*} else{
				t1 = (P.y - b.y)/V.y;
				t2 = (P.y - a.y)/V.y;
			}*/
			if(t1 < 0 && t2 < 0) return -1;
			else if (t1 < t2 && t1 >= 0) return t1;
			else return t2;

		} else return -1;

	//z
	if(V.x == 0 && V.y == 0)
		if(b.x <= P.x && P.x <= a.x && b.y <= P.y && P.y <= a.y)
		{
			
			//calculate the t's
			float t1, t2;
			/*if(V.z > 0)
			{*/
				t1 = (b.z - P.z)/V.z;
				t2 = (a.z - P.z)/V.z;
			/*} else{
				
				t1 = (P.z - b.z)/V.z;
				t2 = (P.z - a.z)/V.z;
			}*/
			if(t1 < 0 && t2 < 0) return -1;
			else if (t1 < t2 && t1 >= 0) return t1;
			else return t2;

		} else return -1;



	//after that, do the non-perpendicular test
	//test x axes
	//x
	float t1 = (a.x-P.x)/V.x;
	float t2 = (b.x-P.x)/V.x;
	float near, far;
	if(t1 > t2){
		near = t2;
		far = t1;
	} else {
		near = t1;
		far = t2;
	}
	
	//y
	t1 = (a.y-P.y)/V.y;
	t2 = (b.y-P.y)/V.y;
	if(t1 > t2){
		if(t2 > near)near = t2;
		if(t1<far)far = t1;
	} else {
		if(t1>near) near = t1;
		if(t2<far) far = t2;
	}

	//z
	t1 = (a.z-P.z)/V.z;
	t2 = (b.z-P.z)/V.z;
	if(t1 > t2){
		if(t2 > near)near = t2;
		if(t1<far)far = t1;
	} else {
		if(t1>near) near = t1;
		if(t2<far) far = t2;
	}

	if(far < near) return -1;
	else{
		//std::cout<<near<<std::endl;
		return near;
	}

	return -1;
}


float ShadowFeeler(const vec4& intersection_point, const vec4& light_source, Geometry * objects, int numObjects)
{
	//test for intersection between point and light source
	vec4 dir = light_source - intersection_point;
	for(int i = 0; i < numObjects; i++)
	{
		float t;
		if(objects[i].type == "cube"){
			t = Test_RayCubeIntersect(intersection_point, dir, objects[i].transform);
		} else if(objects[i].type == "sphere"){
			t = Test_RaySphereIntersect(intersection_point, dir, objects[i].transform);
		} else if(objects[i].type == "triangle"){
			t = Test_RayPolyIntersect(intersection_point, dir, objects[i].points[0].v, objects[i].points[1].v, objects[i].points[2].v, objects[i].transform);
		}
		if(t != -1) return t;
	}
	return -1;
}