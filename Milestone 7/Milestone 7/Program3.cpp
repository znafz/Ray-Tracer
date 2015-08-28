/*
	2015 Zachary Nafziger
*/

#include <iostream>
using namespace std;

#include "glm/glm.hpp"
#include "EasyBMP.h"
#include "Geometry.h"
#include "RayIntersection.h"
#include <math.h>
#include <time.h>


//these really just apply to the first function
int numSpheres = 6;
int numCubes = 6;

int main(){
	Sphere * sphere = new Sphere[numSpheres];
	Cube * cube = new Cube[numCubes];

	vec4 lightp(5,5,5,1);
	vec3 pointlightcolor(1,1,1);//scale of 0 to 1
	Color ambient_color(1,1,1);
	ambient_color.setIntensity(0.2f);

	mat4 SCALING(vec4(1.5f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat4 ROTATION(vec4(cos(3.14159265358979323846/4), 0.0f, -sin(3.14159265358979323846/4), 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(sin(3.14159265358979323846/4), 0.0f, cos(3.14159265358979323846/4), 0.0f),
                         vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat4 TRANSLATE(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(-3.0f, 0.0f, 0.0f, 1.0f));

	BMP output;
	
	sphere[0].setColor(255, 0, 0);
	sphere[1].setColor(0, 0, 255);
	sphere[2].setColor(0,255,0);
	sphere[3].setColor(255,255,0);
	sphere[4].setColor(255,255,255);

	cube[0].setColor(255, 0, 0);
	cube[1].setColor(0, 0, 255);
	cube[2].setColor(0,255,0);
	cube[3].setColor(255,255,0);
	cube[4].setColor(255,255,255);
	

	sphere[0].transform = TRANSLATE*ROTATION*SCALING;
	cube[0].transform = TRANSLATE*ROTATION*SCALING;
	TRANSLATE[3][0] = 3;
	sphere[1].transform = TRANSLATE*ROTATION*SCALING;
	cube[1].transform = TRANSLATE*ROTATION*SCALING;
	TRANSLATE[3][0] = 0;
	TRANSLATE[3][2] = 3;
	sphere[2].transform = TRANSLATE*ROTATION*SCALING;
	cube[2].transform = TRANSLATE*ROTATION*SCALING;
	TRANSLATE[3][2] =-3;
	sphere[3].transform = TRANSLATE*ROTATION*SCALING;
	cube[3].transform = TRANSLATE*ROTATION*SCALING;
	
	SCALING[0][0] = 2;
	SCALING[1][1] = 2;
	SCALING[2][2] = 2;
	sphere[4].transform = SCALING;
	cube[4].transform = SCALING;
	SCALING[0][0] = .5;
	SCALING[1][1] = .5;
	SCALING[2][2] = .5;
	TRANSLATE[3][2] = 0;
	TRANSLATE[3][1] = 4;
	sphere[5].transform = TRANSLATE*SCALING;
	cube[5].transform = TRANSLATE*SCALING;

	for(int j = 0; j < numSpheres; j++)
		for(int i = 0; i < sphere[j].numPoints; i++)
		{
			sphere[j].points[i].v = sphere[j].transform * sphere[j].points[i].v;
		}

	for(int j = 0; j < numCubes; j++)
		for(int i = 0; i < cube[j].numPoints; i++)
		{
			cube[j].points[i].v = cube[j].transform * cube[j].points[i].v;
		}

	for(int i = 0; i < numCubes; i++)
		cube[i].getNorms();

	
	vec4 EYEP(0,0,3,1);
	vec4 UVEC(0,1,0,0);
	float FOVY = 45;
	vec4 VDIR = vec4(0,0,0,1) - EYEP;
	VDIR = normalize(VDIR);
	float RESOX=1028, RESOY = 1028;
	output.SetSize(RESOX, RESOY);
	output.SetBitDepth(24);

	vec4 D, P, V, H, M;
	V = UVEC * tanf(FOVY * (3.14159/180));
	vec3 H3 = cross(vec3(V), vec3(VDIR));
	H.x = H3.x;
	H.y = H3.y;
	H.z = H3.z;
	H.w = 0;
	vec3 V3 = cross(vec3(VDIR),H3);
	V.x = V3.x;
	V.y = V3.y;
	V.z = V3.z;
	V.w = 0;

#pragma region spherescene1


	

		/*for(int i = 0; i <numCubes; i++)
			cube[i].getNorms();*/
	


	EYEP = vec4(-5,5,5,1);
	VDIR = vec4(0,0,0,1) - EYEP;
	VDIR = normalize(VDIR);
	V = UVEC * tanf(FOVY * (3.14159/180));
	H3 = cross(vec3(V), vec3(VDIR));
	H.x = H3.x;
	H.y = H3.y;
	H.z = H3.z;
	H.w = 0;
	V3 = cross(vec3(VDIR),H3);
	V.x = V3.x;
	V.y = V3.y;
	V.z = V3.z;
	V.w = 0;
	M = EYEP + VDIR;

	for(int x = 0; x < RESOX-1; x++)
		for(int y = 0; y < RESOY-1; y++)
		{
			D = M + H*(2.0f*x/(RESOX-1.0f) - 1.0f) + V*(2.0f*y/(RESOY-1.0f) - 1.0f);
			P = (D - EYEP); 
			P.w = 0;
			P = normalize(P);
			
			output(RESOX-1-x,RESOY-1-y)->Red = 0;
			output(RESOX-1-x,RESOY-1-y)->Green = 0;
			output(RESOX-1-x,RESOY-1-y)->Blue = 0;

			float intersectiont=1000;
			int geo = 0;

			for(int i = 0; i < numSpheres; i++)
			{
				float intersect = Test_RaySphereIntersect(EYEP, P, sphere[i].transform);
				if(intersect >=0 && intersect < intersectiont)
				{
					intersectiont = intersect;
					geo = i;
				}
			}

			

			if(intersectiont != 1000){
				//shadow feeler
				vec4 feelerOrigin(EYEP + P*intersectiont);
				vec4 feelerDir(lightp - feelerOrigin);
				feelerDir = normalize(feelerDir);
				float feelIntersect = 1000;
				int feelgeo = 0;
				float diffuseSpec = 1;
				for(int i = 0; i < numSpheres; i++)
				{
					float intersect = Test_RaySphereIntersect(feelerOrigin, feelerDir, sphere[i].transform);
					if(intersect >=0 && intersect < feelIntersect && i !=geo)
					{
						feelIntersect = intersect;
						feelgeo = i;
					}
				}
				if(feelIntersect != 1000) diffuseSpec = 0;
				//if path is unobstructed continue

				Color c = getColor(EYEP + P*intersectiont, sphere[geo]);
				vec4 L = (EYEP + P*intersectiont) - lightp;
				//vec3 N = vec3(get_normal(EYEP + P*intersectiont, sphere[geo]));
				vec3 N = vec3(feelerOrigin - sphere[geo].points[0].v);
				N = normalize(N);
				L = normalize(L);
				vec3 diffuse = pointlightcolor*dot(vec3(L), vec3(N));
				diffuse = diffuse*0.6f*diffuseSpec;
				if(diffuse.r < 0) diffuse.r = 0;
				if(diffuse.g < 0) diffuse.g = 0;
				if(diffuse.b < 0) diffuse.b = 0;

				//blinn phong
				vec3 V = vec3(-P);
				V = normalize(V);
				
				vec3 H = vec3(feelerDir) + V;
				H = normalize(H);
				float specAmount = dot(H, N);
				specAmount = pow(specAmount,10);
				//cout << specAmount << endl;
				vec3 spec = vec3(1,1,1)*specAmount*diffuseSpec*0.3f;
				//cout << spec.x << " " << spec.y << " " << spec.z << endl;
				if(spec.x < 0)spec.x = 0;
				if(spec.y < 0)spec.y = 0;
				if(spec.z < 0)spec.z = 0;


				//reflections

				if(sphere[geo].reflectivity > 0){
					//shoot a new ray from that point
					for(int r=0;r<4;r++)
					{
						float reflectIntersect = 1000;
						int reflectgeo = 0;
						vec3 rd = reflect(vec3(P), N);
						vec4 reflectDir(rd.x, rd.y, rd.z, 0);
						for(int i = 0; i < numSpheres; i++)
						{
							float intersect = Test_RaySphereIntersect(feelerOrigin, reflectDir, sphere[i].transform);
							if(intersect >=0 && intersect < reflectIntersect && i !=geo)
							{
								reflectIntersect = intersect;
								reflectgeo = i;
							}
						}
						if(reflectIntersect != 1000) {
							
							float weight_original = 1-sphere[geo].reflectivity;
							float weight_reflect = sphere[geo].reflectivity;
							c = c*weight_original + getColor(feelerOrigin, sphere[reflectgeo])*weight_reflect;
						} else break;
						geo = reflectgeo;
						P = reflectDir;
						feelerOrigin = feelerOrigin + reflectDir*reflectIntersect;
						N = vec3(feelerOrigin - sphere[reflectgeo].points[0].v);
					}
				}



				Color outcol((c.r*ambient_color.r + c.r*diffuse.x + c.r*spec.x),(c.g*ambient_color.g + c.g*diffuse.y + c.g*spec.y), (c.b*ambient_color.b + c.b*diffuse.z + c.b*spec.z));
				if(outcol.r > 255) outcol.r = 255;
				if(outcol.g > 255) outcol.g = 255;
				if(outcol.b > 255) outcol.b = 255;
				output(RESOX-1-x,RESOY-1-y)->Red = outcol.r;
				output(RESOX-1-x,RESOY-1-y)->Green = outcol.g;
				output(RESOX-1-x,RESOY-1-y)->Blue = outcol.b;
				
				
			}
		}

				
		output.WriteToFile("spherescene1.bmp");
		std::cout << "spherescene1 done"<<endl;
#pragma endregion spherescene1

		mat4 TRANSLATE2(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(0.0f, -3.0f, 0.0f, 1.0f));
		mat4 SCALE2(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(0.0f, 0.0f, 0.0f, 1.0f));
		numSpheres = 4;
		sphere = new Sphere[numSpheres];

		sphere[0].setColor(255, 0, 0);
	sphere[1].setColor(0, 0, 255);
	sphere[2].setColor(0,255,0);
	sphere[3].setColor(255,255,0);



	for(int i = 0; i < numSpheres; i++)
	{
		sphere[i].transform = TRANSLATE2*SCALE2;
		TRANSLATE2[3][1] += 2.0f;
		SCALE2[0][0] -= 0.2f;
		SCALE2[1][1] -= 0.2f;
		SCALE2[2][2] -= 0.2f;
		sphere[i].points[0].v = sphere[i].transform * sphere[i].points[0].v;
	}
	


#pragma region spherescene2


	

		/*for(int i = 0; i <numCubes; i++)
			cube[i].getNorms();*/
	


	EYEP = vec4(-5,5,5,1);
	VDIR = vec4(0,0,0,1) - EYEP;
	VDIR = normalize(VDIR);
	V = UVEC * tanf(FOVY * (3.14159/180));
	H3 = cross(vec3(V), vec3(VDIR));
	H.x = H3.x;
	H.y = H3.y;
	H.z = H3.z;
	H.w = 0;
	V3 = cross(vec3(VDIR),H3);
	V.x = V3.x;
	V.y = V3.y;
	V.z = V3.z;
	V.w = 0;
	M = EYEP + VDIR;

	for(int x = 0; x < RESOX-1; x++)
		for(int y = 0; y < RESOY-1; y++)
		{
			D = M + H*(2.0f*x/(RESOX-1.0f) - 1.0f) + V*(2.0f*y/(RESOY-1.0f) - 1.0f);
			P = (D - EYEP); 
			P.w = 0;
			P = normalize(P);
			
			output(RESOX-1-x,RESOY-1-y)->Red = 0;
			output(RESOX-1-x,RESOY-1-y)->Green = 0;
			output(RESOX-1-x,RESOY-1-y)->Blue = 0;

			float intersectiont=1000;
			int geo = 0;

			for(int i = 0; i < numSpheres; i++)
			{
				float intersect = Test_RaySphereIntersect(EYEP, P, sphere[i].transform);
				if(intersect >=0 && intersect < intersectiont)
				{
					intersectiont = intersect;
					geo = i;
				}
			}

			

			if(intersectiont != 1000){
				//shadow feeler
				vec4 feelerOrigin(EYEP + P*intersectiont);
				vec4 feelerDir(lightp - feelerOrigin);
				feelerDir = normalize(feelerDir);
				float feelIntersect = 1000;
				int feelgeo = 0;
				float diffuseSpec = 1;
				for(int i = 0; i < numSpheres; i++)
				{
					float intersect = Test_RaySphereIntersect(feelerOrigin, feelerDir, sphere[i].transform);
					if(intersect >=0 && intersect < feelIntersect && i !=geo)
					{
						feelIntersect = intersect;
						feelgeo = i;
					}
				}
				if(feelIntersect != 1000) diffuseSpec = 0;
				//if path is unobstructed continue
				Color c = getColor(EYEP + P*intersectiont, sphere[geo]);
				vec4 L = (EYEP + P*intersectiont) - lightp;
				//vec3 N = vec3(get_normal(EYEP + P*intersectiont, sphere[geo]));
				vec3 N = vec3(feelerOrigin - sphere[geo].points[0].v);
				N = normalize(N);
				L = normalize(L);
				vec3 diffuse = pointlightcolor*dot(vec3(L), vec3(N));
				diffuse = diffuse*0.6f*diffuseSpec;
				if(diffuse.r < 0) diffuse.r = 0;
				if(diffuse.g < 0) diffuse.g = 0;
				if(diffuse.b < 0) diffuse.b = 0;

				//blinn phong
				vec3 V = vec3(-P);
				V = normalize(V);
				
				vec3 H = vec3(feelerDir) + V;
				H = normalize(H);
				float specAmount = dot(H,N);
				specAmount = pow(specAmount,10);
				//cout << specAmount << endl;
				vec3 spec = vec3(1,1,1)*specAmount*diffuseSpec*0.3f;
				//cout << spec.x << " " << spec.y << " " << spec.z << endl;
				if(spec.x < 0)spec.x = 0;
				if(spec.y < 0)spec.y = 0;
				if(spec.z < 0)spec.z = 0;


				//reflections

				if(sphere[geo].reflectivity > 0){
					//shoot a new ray from that point
					for(int r=0;r<4;r++)
					{
						float reflectIntersect = 1000;
						int reflectgeo = 0;
						vec3 rd = reflect(vec3(P), N);
						vec4 reflectDir(rd.x, rd.y, rd.z, 0);
						for(int i = 0; i < numSpheres; i++)
						{
							float intersect = Test_RaySphereIntersect(feelerOrigin, reflectDir, sphere[i].transform);
							if(intersect >=0 && intersect < reflectIntersect && i !=geo)
							{
								reflectIntersect = intersect;
								reflectgeo = i;
							}
						}
						if(reflectIntersect != 1000) {
							
							float weight_original = 1-sphere[geo].reflectivity;
							float weight_reflect = sphere[geo].reflectivity;
							c = c*weight_original + getColor(feelerOrigin, sphere[reflectgeo])*weight_reflect;
						} else break;
						geo = reflectgeo;
						P = reflectDir;
						feelerOrigin = feelerOrigin + reflectDir*reflectIntersect;
						N = vec3(feelerOrigin - sphere[reflectgeo].points[0].v);
					}
				}



				Color outcol((c.r*ambient_color.r + c.r*diffuse.x + c.r*spec.x),(c.g*ambient_color.g + c.g*diffuse.y + c.g*spec.y), (c.b*ambient_color.b + c.b*diffuse.z + c.b*spec.z));
				if(outcol.r > 255) outcol.r = 255;
				if(outcol.g > 255) outcol.g = 255;
				if(outcol.b > 255) outcol.b = 255;
				output(RESOX-1-x,RESOY-1-y)->Red = outcol.r;
				output(RESOX-1-x,RESOY-1-y)->Green = outcol.g;
				output(RESOX-1-x,RESOY-1-y)->Blue = outcol.b;
				
				
			}
		}

				
		output.WriteToFile("spherescene2.bmp");
		cout << "spherescene2 done"<<endl;
#pragma endregion spherescene2


mat4 TRANSLATE3(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(0.0f, 0.0f, 0.0f, 1.0f));
		mat4 SCALE3(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(0.0f, 0.0f, 0.0f, 1.0f));
		numSpheres = 16;
		sphere = new Sphere[numSpheres];
		
		

		srand(time(NULL));
		//trying some randomization

	for(int i = 0; i < numSpheres; i++)
	{
		sphere[i].reflectivity = 0.95;
		sphere[i].setColor(rand()%255, rand()%255, rand()%255);
		sphere[i].transform = TRANSLATE3*SCALE3;
		TRANSLATE3[3][0] = 10*static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 5.0f;
		TRANSLATE3[3][1] = 10*static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 5.0f;
		TRANSLATE3[3][2] = 10*static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 5.0f;
		float scale = 0.5f+0.5f*static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		SCALE3[0][0] = scale;
		SCALE3[1][1] = scale;
		SCALE3[2][2] = scale;
		sphere[i].points[0].v = sphere[i].transform * sphere[i].points[0].v;
	}

	#pragma region spherescene3


	

		/*for(int i = 0; i <numCubes; i++)
			cube[i].getNorms();*/
	


	EYEP = vec4(5,2,8,1);
	VDIR = vec4(0,0,0,1) - EYEP;
	VDIR = normalize(VDIR);
	V = UVEC * tanf(FOVY * (3.14159/180));
	H3 = cross(vec3(V), vec3(VDIR));
	H.x = H3.x;
	H.y = H3.y;
	H.z = H3.z;
	H.w = 0;
	V3 = cross(vec3(VDIR),H3);
	V.x = V3.x;
	V.y = V3.y;
	V.z = V3.z;
	V.w = 0;
	M = EYEP + VDIR;

	for(int x = 0; x < RESOX-1; x++)
		for(int y = 0; y < RESOY-1; y++)
		{
			D = M + H*(2.0f*x/(RESOX-1.0f) - 1.0f) + V*(2.0f*y/(RESOY-1.0f) - 1.0f);
			P = (D - EYEP); 
			P.w = 0;
			P = normalize(P);
			
			output(RESOX-1-x,RESOY-1-y)->Red = 0;
			output(RESOX-1-x,RESOY-1-y)->Green = 0;
			output(RESOX-1-x,RESOY-1-y)->Blue = 0;

			float intersectiont=1000;
			int geo = 0;

			for(int i = 0; i < numSpheres; i++)
			{
				float intersect = Test_RaySphereIntersect(EYEP, P, sphere[i].transform);
				if(intersect >=0 && intersect < intersectiont)
				{
					intersectiont = intersect;
					geo = i;
				}
			}

			

			if(intersectiont != 1000){
				//shadow feeler
				vec4 feelerOrigin(EYEP + P*intersectiont);
				vec4 feelerDir(lightp - feelerOrigin);
				feelerDir = normalize(feelerDir);
				float feelIntersect = 1000;
				int feelgeo = 0;
				float diffuseSpec = 1;
				for(int i = 0; i < numSpheres; i++)
				{
					float intersect = Test_RaySphereIntersect(feelerOrigin, feelerDir, sphere[i].transform);
					if(intersect >=0 && intersect < feelIntersect && i !=geo)
					{
						feelIntersect = intersect;
						feelgeo = i;
					}
				}
				if(feelIntersect != 1000) diffuseSpec = 0;
				//if path is unobstructed continue
				Color c = getColor(EYEP + P*intersectiont, sphere[geo]);
				vec4 L = (EYEP + P*intersectiont) - lightp;
				//vec3 N = vec3(get_normal(EYEP + P*intersectiont, sphere[geo]));
				vec3 N = vec3(feelerOrigin - sphere[geo].points[0].v);
				N = normalize(N);
				L = normalize(L);
				vec3 diffuse = pointlightcolor*dot(vec3(L), vec3(N));
				diffuse = diffuse*0.6f*diffuseSpec;
				if(diffuse.r < 0) diffuse.r = 0;
				if(diffuse.g < 0) diffuse.g = 0;
				if(diffuse.b < 0) diffuse.b = 0;

				//blinn phong
				vec3 V = vec3(-P);
				V = normalize(V);
				
				vec3 H = vec3(feelerDir) + V;
				H = normalize(H);
				float specAmount = dot(H,N);
				specAmount = pow(specAmount,10);
				//cout << specAmount << endl;
				vec3 spec = vec3(1,1,1)*specAmount*diffuseSpec*0.3f;
				//cout << spec.x << " " << spec.y << " " << spec.z << endl;
				if(spec.x < 0)spec.x = 0;
				if(spec.y < 0)spec.y = 0;
				if(spec.z < 0)spec.z = 0;


				//reflections

				if(sphere[geo].reflectivity > 0){
					//shoot a new ray from that point
					for(int r=0;r<4;r++)
					{
						float reflectIntersect = 1000;
						int reflectgeo = 0;
						vec3 rd = reflect(vec3(P), N);
						vec4 reflectDir(rd.x, rd.y, rd.z, 0);
						for(int i = 0; i < numSpheres; i++)
						{
							float intersect = Test_RaySphereIntersect(feelerOrigin, reflectDir, sphere[i].transform);
							if(intersect >=0 && intersect < reflectIntersect && i !=geo)
							{
								reflectIntersect = intersect;
								reflectgeo = i;
							}
						}
						if(reflectIntersect != 1000) {
							
							float weight_original = 1-sphere[geo].reflectivity;
							float weight_reflect = sphere[geo].reflectivity;
							c = c*weight_original + getColor(feelerOrigin, sphere[reflectgeo])*weight_reflect;
						} else break;
						geo = reflectgeo;
						P = reflectDir;
						feelerOrigin = feelerOrigin + reflectDir*reflectIntersect;
						N = vec3(feelerOrigin - sphere[reflectgeo].points[0].v);
					}
				}



				Color outcol((c.r*ambient_color.r + c.r*diffuse.x + c.r*spec.x),(c.g*ambient_color.g + c.g*diffuse.y + c.g*spec.y), (c.b*ambient_color.b + c.b*diffuse.z + c.b*spec.z));
				if(outcol.r > 255) outcol.r = 255;
				if(outcol.g > 255) outcol.g = 255;
				if(outcol.b > 255) outcol.b = 255;
				output(RESOX-1-x,RESOY-1-y)->Red = outcol.r;
				output(RESOX-1-x,RESOY-1-y)->Green = outcol.g;
				output(RESOX-1-x,RESOY-1-y)->Blue = outcol.b;
				
				
			}
		}

				
		output.WriteToFile("spherescene3.bmp");
		cout << "spherescene3 done"<<endl;
#pragma endregion spherescene3




#pragma region cubescene1


	

		/*for(int i = 0; i <numCubes; i++)
			cube[i].getNorms();*/
	


	EYEP = vec4(-5,5,5,1);
	VDIR = vec4(0,0,0,1) - EYEP;
	VDIR = normalize(VDIR);
	V = UVEC * tanf(FOVY * (3.14159/180));
	H3 = cross(vec3(V), vec3(VDIR));
	H.x = H3.x;
	H.y = H3.y;
	H.z = H3.z;
	H.w = 0;
	V3 = cross(vec3(VDIR),H3);
	V.x = V3.x;
	V.y = V3.y;
	V.z = V3.z;
	V.w = 0;
	M = EYEP + VDIR;

	for(int x = 0; x < RESOX-1; x++)
		for(int y = 0; y < RESOY-1; y++)
		{
			D = M + H*(2.0f*x/(RESOX-1.0f) - 1.0f) + V*(2.0f*y/(RESOY-1.0f) - 1.0f);
			P = (D - EYEP); 
			P.w = 0;
			P = normalize(P);
			
			output(RESOX-1-x,RESOY-1-y)->Red = 0;
			output(RESOX-1-x,RESOY-1-y)->Green = 0;
			output(RESOX-1-x,RESOY-1-y)->Blue = 0;

			float intersectiont=1000;
			int geo = 0;

			for(int i = 0; i < numCubes; i++)
			{
				float intersect = Test_RayCubeIntersect(EYEP, P, cube[i].transform);
				if(intersect >=0 && intersect < intersectiont)
				{
					intersectiont = intersect;
					geo = i;
				}
			}

			

			if(intersectiont != 1000){
				//shadow feeler
				vec4 feelerOrigin(EYEP + P*intersectiont);
				vec4 feelerDir(lightp - feelerOrigin);
				feelerDir = normalize(feelerDir);
				float feelIntersect = 1000;
				int feelgeo = 0;
				float diffuseSpec = 1;
				for(int i = 0; i < numCubes; i++)
				{
					float intersect = Test_RayCubeIntersect(feelerOrigin, feelerDir, cube[i].transform);
					if(intersect >=0 && intersect < feelIntersect && i !=geo)
					{
						feelIntersect = intersect;
						feelgeo = i;
					}
				}
				if(feelIntersect != 1000) diffuseSpec = 0;
				//if path is unobstructed continue
				Color c = getColor(EYEP + P*intersectiont, cube[geo]);
				vec4 L = (EYEP + P*intersectiont) - lightp;
				vec3 N = vec3(get_normal(EYEP + P*intersectiont, cube[geo]));
				

				N = normalize(N);
				L = normalize(L);
				vec3 diffuse = pointlightcolor*dot(vec3(L), vec3(N));
				diffuse = diffuse*0.6f*diffuseSpec;
				if(diffuse.r < 0) diffuse.r = 0;
				if(diffuse.g < 0) diffuse.g = 0;
				if(diffuse.b < 0) diffuse.b = 0;

				//blinn phong
				vec3 V = vec3(-P);
				V = normalize(V);
				
				vec3 H = vec3(feelerDir) + V;
				H = normalize(H);
				float specAmount = dot(H, N);
				specAmount = pow(specAmount,10);
				//cout << specAmount << endl;
				vec3 spec = vec3(1,1,1)*specAmount*diffuseSpec*0.3f;
				//cout << spec.x << " " << spec.y << " " << spec.z << endl;
				if(spec.x < 0)spec.x = 0;
				if(spec.y < 0)spec.y = 0;
				if(spec.z < 0)spec.z = 0;


				//reflections

				if(cube[geo].reflectivity > 0){
					//shoot a new ray from that point
					for(int r=0;r<4;r++)
					{
						float reflectIntersect = 1000;
						int reflectgeo = 0;
						vec3 rd = reflect(vec3(P), N);
						vec4 reflectDir(rd.x, rd.y, rd.z, 0);
						for(int i = 0; i < numCubes; i++)
						{
							float intersect = Test_RayCubeIntersect(feelerOrigin, reflectDir, cube[i].transform);
							if(intersect >=0 && intersect < reflectIntersect && i !=geo)
							{
								reflectIntersect = intersect;
								reflectgeo = i;
							}
						}
						if(reflectIntersect != 1000) {
							
							float weight_original = 1-cube[geo].reflectivity;
							float weight_reflect = cube[geo].reflectivity;
							c = c*weight_original + getColor(feelerOrigin, cube[reflectgeo])*weight_reflect;
						} else break;
						geo = reflectgeo;
						P = reflectDir;
						feelerOrigin = feelerOrigin + reflectDir*reflectIntersect;
						N = vec3(get_normal(feelerOrigin, cube[reflectgeo]));
					}
				}



				Color outcol((c.r*ambient_color.r + c.r*diffuse.x + c.r*spec.x),(c.g*ambient_color.g + c.g*diffuse.y + c.g*spec.y), (c.b*ambient_color.b + c.b*diffuse.z + c.b*spec.z));
				if(outcol.r > 255) outcol.r = 255;
				if(outcol.g > 255) outcol.g = 255;
				if(outcol.b > 255) outcol.b = 255;
				output(RESOX-1-x,RESOY-1-y)->Red = outcol.r;
				output(RESOX-1-x,RESOY-1-y)->Green = outcol.g;
				output(RESOX-1-x,RESOY-1-y)->Blue = outcol.b;
				
				
			}
		}

				
		output.WriteToFile("cubescene1.bmp");
		cout << "cubescene1 done"<<endl;
#pragma endregion cubescene1

mat4 TRANSLATE4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(0.0f, -3.0f, 0.0f, 1.0f));
		mat4 SCALE4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
                         vec4(0.0f, 1.0f, 0.0f, 0.0f),
                         vec4(0.0f, 0.0f, 1.0f, 0.0f),
                         vec4(0.0f, 0.0f, 0.0f, 1.0f));
		numCubes = 4;
		cube = new Cube[numCubes];

		cube[0].setColor(255, 0, 0);
	cube[1].setColor(0, 0, 255);
	cube[2].setColor(0,255,0);
	cube[3].setColor(255,255,0);



	for(int i = 0; i < numCubes; i++)
	{
		cube[i].transform = TRANSLATE4*SCALE4;
		TRANSLATE4[3][1] += 2.0f;
		SCALE4[0][0] -= 0.2f;
		SCALE4[1][1] -= 0.2f;
		SCALE4[2][2] -= 0.2f;
	}

	for(int i = 0; i < numCubes; i++)
		for(int j = 0; j < cube[i].numPoints; j++)
		{
			cube[i].points[j].v =  cube[i].transform * cube[i].points[j].v;
		}
	for(int i = 0; i < numCubes; i++)
		cube[i].getNorms();

#pragma region cubescene2


	

		/*for(int i = 0; i <numCubes; i++)
			cube[i].getNorms();*/
	


	EYEP = vec4(-5,5,5,1);
	VDIR = vec4(0,0,0,1) - EYEP;
	VDIR = normalize(VDIR);
	V = UVEC * tanf(FOVY * (3.14159/180));
	H3 = cross(vec3(V), vec3(VDIR));
	H.x = H3.x;
	H.y = H3.y;
	H.z = H3.z;
	H.w = 0;
	V3 = cross(vec3(VDIR),H3);
	V.x = V3.x;
	V.y = V3.y;
	V.z = V3.z;
	V.w = 0;
	M = EYEP + VDIR;

	for(int x = 0; x < RESOX-1; x++)
		for(int y = 0; y < RESOY-1; y++)
		{
			D = M + H*(2.0f*x/(RESOX-1.0f) - 1.0f) + V*(2.0f*y/(RESOY-1.0f) - 1.0f);
			P = (D - EYEP); 
			P.w = 0;
			P = normalize(P);
			
			output(RESOX-1-x,RESOY-1-y)->Red = 0;
			output(RESOX-1-x,RESOY-1-y)->Green = 0;
			output(RESOX-1-x,RESOY-1-y)->Blue = 0;

			float intersectiont=1000;
			int geo = 0;

			for(int i = 0; i < numCubes; i++)
			{
				float intersect = Test_RayCubeIntersect(EYEP, P, cube[i].transform);
				if(intersect >=0 && intersect < intersectiont)
				{
					intersectiont = intersect;
					geo = i;
				}
			}

			

			if(intersectiont != 1000){
				//shadow feeler
				vec4 feelerOrigin(EYEP + P*intersectiont);
				vec4 feelerDir(lightp - feelerOrigin);
				feelerDir = normalize(feelerDir);
				float feelIntersect = 1000;
				int feelgeo = 0;
				float diffuseSpec = 1;
				for(int i = 0; i < numCubes; i++)
				{
					float intersect = Test_RayCubeIntersect(feelerOrigin, feelerDir, cube[i].transform);
					if(intersect >=0 && intersect < feelIntersect && i !=geo)
					{
						feelIntersect = intersect;
						feelgeo = i;
					}
				}
				if(feelIntersect != 1000) diffuseSpec = 0;
				//if path is unobstructed continue
				Color c = getColor(EYEP + P*intersectiont, cube[geo]);
				vec4 L = (EYEP + P*intersectiont) - lightp;
				vec3 N = vec3(get_normal(EYEP + P*intersectiont, cube[geo]));
				

				N = normalize(N);
				L = normalize(L);
				vec3 diffuse = pointlightcolor*dot(vec3(L), vec3(N));
				diffuse = diffuse*0.6f*diffuseSpec;
				if(diffuse.r < 0) diffuse.r = 0;
				if(diffuse.g < 0) diffuse.g = 0;
				if(diffuse.b < 0) diffuse.b = 0;

				//blinn phong
				vec3 V = vec3(-P);
				V = normalize(V);
				
				vec3 H = vec3(feelerDir) + V;
				H = normalize(H);
				float specAmount = dot(H, N);
				specAmount = pow(specAmount,10);
				//cout << specAmount << endl;
				vec3 spec = vec3(1,1,1)*specAmount*diffuseSpec*0.3f;
				//cout << spec.x << " " << spec.y << " " << spec.z << endl;
				if(spec.x < 0)spec.x = 0;
				if(spec.y < 0)spec.y = 0;
				if(spec.z < 0)spec.z = 0;


				//reflections

				if(cube[geo].reflectivity > 0){
					//shoot a new ray from that point
					for(int r=0;r<4;r++)
					{
						float reflectIntersect = 1000;
						int reflectgeo = 0;
						vec3 rd = reflect(vec3(P), N);
						vec4 reflectDir(rd.x, rd.y, rd.z, 0);
						for(int i = 0; i < numCubes; i++)
						{
							float intersect = Test_RayCubeIntersect(feelerOrigin, reflectDir, cube[i].transform);
							if(intersect >=0 && intersect < reflectIntersect && i !=geo)
							{
								reflectIntersect = intersect;
								reflectgeo = i;
							}
						}
						if(reflectIntersect != 1000) {
							
							float weight_original = 1-cube[geo].reflectivity;
							float weight_reflect = cube[geo].reflectivity;
							c = c*weight_original + getColor(feelerOrigin, cube[reflectgeo])*weight_reflect;
						} else break;
						geo = reflectgeo;
						P = reflectDir;
						feelerOrigin = feelerOrigin + reflectDir*reflectIntersect;
						N = vec3(get_normal(feelerOrigin, cube[reflectgeo]));
					}
				}



				Color outcol((c.r*ambient_color.r + c.r*diffuse.x + c.r*spec.x),(c.g*ambient_color.g + c.g*diffuse.y + c.g*spec.y), (c.b*ambient_color.b + c.b*diffuse.z + c.b*spec.z));
				if(outcol.r > 255) outcol.r = 255;
				if(outcol.g > 255) outcol.g = 255;
				if(outcol.b > 255) outcol.b = 255;
				output(RESOX-1-x,RESOY-1-y)->Red = outcol.r;
				output(RESOX-1-x,RESOY-1-y)->Green = outcol.g;
				output(RESOX-1-x,RESOY-1-y)->Blue = outcol.b;
				
				
			}
		}

				
		output.WriteToFile("cubescene2.bmp");
		cout << "cubescene2 done"<<endl;
#pragma endregion cubescene2
}