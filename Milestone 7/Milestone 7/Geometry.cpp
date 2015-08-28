/*
	Zachary Nafziger, 2015
	This is the geometry functionality file.

*/
#include "Geometry.h"

Sphere::Sphere(){
	numPoints = 1; //just the origin
	points = new Point[numPoints];
	points[0] = createPoint(0,0,0,Color(255,0,0));
	radius = 1.0f;
	type = "sphere";
	color.r = 255;
	color.g = 0;
	color.b = 0;
}

Triangle::Triangle()
{
	numPoints = 3;
	points = new Point[numPoints];
	points[0] = createPoint(0, 1, 0, Color(0,0,255));
	points[1] = createPoint(1, -1, 0, Color(0,0,255));
	points[2] = createPoint(-1, -1, 0, Color(0,0,255));
	type = "triangle";
	color = Color(0,0,255);
}

vec4 Triangle::get_normal()
{
	vec3 norm = cross(vec3(points[0].v-points[1].v), vec3(points[2].v-points[1].v));
	norm = normalize(norm);
	return vec4(-norm.x, -norm.y, -norm.z, 0.0f);
}

Color operator+(Color c, Color c2)
{
	return Color(c.r + c2.r, c.g + c2.g, c.b + c2.b);
}


Color operator*(Color c, float f)
{
	c.r *=f;
	c.g*=f;
	c.b*=f;
	return c;
}

Geometry::Geometry(){
	reflectivity = 0.7;
	transform = mat4(
			vec4(1.0f, 0.0f, 0.0f, 0.0f), 
			vec4(0.0f, 1.0f, 0.0f, 0.0f),
			vec4(0.0f, 0.0f, 1.0f, 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f)
			);//initialize to the identity matrix
}

Point createPoint(float a, float b, float c, Color color){
	vec4 v(a,b,c,1.0f);
	vec4 norm(0,0,0,0);
	Point p = {v, norm, color};
	return p;
}

void Cube::out()
{
	//only need to output the front and back face nodes, since the rest of the nodes overlap
	for(int i = 0; i < 8; i++)
	{
		cout << "Point " << i << ": ";
		points[i].out();
	}
}

Cube::Cube(){
	numPoints = 24; // 4 for each face
	points = new Point[numPoints];

	//now define the faces

	//front face
	points[0] = createPoint(0.5f, 0.5f, -0.5f, Color(255,255,255));//top right
	points[1] = createPoint(0.5f, -0.5f, -0.5f, Color(255,255,255));//bottom right
	points[2] = createPoint(-0.5f, -0.5f, -0.5f, Color(255,255,255));//bottom left
	points[3] = createPoint(-0.5f, 0.5f, -0.5f, Color(255,255,255));//top left
							

	//back face
	points[4] = createPoint(0.5f, 0.5f, 0.5f, Color(255,0,0));//top left
	points[5] = createPoint(0.5f, -0.5f, 0.5f, Color(255,0,0));//bottom left
	points[6] = createPoint(-0.5f, -0.5f, 0.5f, Color(255,0,0));//bottom right
	points[7] = createPoint(-0.5f, 0.5f, 0.5f, Color(255,0,0));//top right
							
	//top face
	points[8] = createPoint(0.5f, 0.5f, 0.5f, Color(255,255,0));//top right
	points[9] = createPoint(0.5f, 0.5f, -0.5f, Color(255,255,0));//bottom right
	points[10] = createPoint(-0.5f, 0.5f, -0.5f, Color(255,255,0));//bottom left
	points[11] = createPoint(-0.5f, 0.5f, 0.5f, Color(255,255,0));//top left
	

	//bottom face
	points[12] = createPoint(0.5f, -0.5f, 0.5f, Color(0,255,0));//top left
	points[13] = createPoint(0.5f, -0.5f, -0.5f, Color(0,255,0));//bottom left
	points[14] = createPoint(-0.5f, -0.5f, -0.5f, Color(0,255,0));//bottom right
	points[15] = createPoint(-0.5f, -0.5f, 0.5f, Color(0,255,0));//top right

	//right face
	points[16] = createPoint(0.5f, 0.5f, 0.5f, Color(0,0, 255));//top right
	points[17] = createPoint(0.5f, -0.5f, 0.5f, Color(0,0, 255));//bottom right
	points[18] = createPoint(0.5f, 0.5f, -0.5f, Color(0,0, 255));//bottom left
	points[19] = createPoint(0.5f, -0.5f, -0.5f, Color(0,0, 255));//top left

	//left face
	points[20] = createPoint(-0.5f, 0.5f, 0.5f, Color(0,0, 0));//top right
	points[21] = createPoint(-0.5f, -0.5f, 0.5f, Color(0,0, 0));//bottom right
	points[22] = createPoint(-0.5f, 0.5f, -0.5f, Color(0,0, 0));//bottom left
	points[23] = createPoint(-0.5f, -0.5f, -0.5f, Color(0,0, 0));//top left

	type = "cube";
}

Color::Color(){
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
}

vec4 planeNorm(vec4 p1, vec4 p2, vec4 p3){
	vec3 norm;
	vec3 a(p1);
	vec3 b(p2);
	vec3 c(p3);
	norm = cross((a - b),(a - c));
		
	norm = normalize(norm);
	return vec4(norm.x, norm.y, norm.z, 0);
}
void Cube::getNorms(){
	
	vec4 norm;
		//check front
		norm = planeNorm(points[0].v, points[1].v, points[2].v);
		
		
		
		for (int i = 0; i < 4; i++)
		{
			points[i].normal = norm;
		}
		//cout << "norm" << norm.x << " " << norm.y << " " << norm.z << endl;
		
		//check back

		norm = planeNorm(points[4].v, points[5].v, points[6].v);
		norm.x = norm.x*-1;
		norm.y = norm.y*-1;
		norm.z = norm.z*-1;
		for (int i = 0; i < 4; i++)
		{
			points[i+4].normal = norm;
		}
		
		//cout << "norm" << norm.x << " " << norm.y << " " << norm.z << endl;
		
		//check top
		norm = planeNorm(points[8].v, points[9].v, points[10].v);
		for (int i = 0; i < 4; i++)
		{
			points[i+8].normal = norm;
		}
		
		//cout << "norm" << norm.x << " " << norm.y << " " << norm.z << endl;

		//check bottom
		norm = planeNorm(points[12].v, points[13].v, points[14].v);
		norm.x = norm.x*-1;
		norm.y = norm.y*-1;
		norm.z = norm.z*-1;
		for (int i = 0; i < 4; i++)
		{
			points[i+12].normal = norm;
		}
		
		//cout << "norm" << norm.x << " " << norm.y << " " << norm.z << endl;
		 
		//check right
		norm = planeNorm(points[16].v, points[17].v, points[18].v);
		
		for (int i = 0; i < 4; i++)
		{
			points[i+16].normal = norm;
		}
		
		//cout << "norm" << norm.x << " " << norm.y << " " << norm.z << endl;
		//check left
		norm = planeNorm(points[20].v, points[21].v, points[22].v);
		norm.x = norm.x*-1;
		norm.y = norm.y*-1;
		norm.z = norm.z*-1;
		for (int i = 0; i < 4; i++)
		{
			points[i+20].normal = norm;
		}
		//cout << "norm" << norm.x << " " << norm.y << " " << norm.z << endl;
}
void Color::set(float x, float y, float z){
	r = x;
	g = y;
	b = z;
}


Color getColor(vec4 intersection, Cube cube){
	
	Color col;
	for(int i = 0; i <cube.numPoints; i+=4)
	{
		//check each face's plane
		if(abs(dot((  intersection - (cube.points[i].v)), cube.points[i].normal)) <= 0.001){
			col = cube.points[i].color;
			//return col;
		}
	}
	//cout << "COLOR: " << col.r << " " << col.g << " " << col.b << endl;
	return col;
}

Color getColor(vec4 intersection, Sphere sphere){
	
	Color col;
	col = sphere.points[0].color;
	return col;
}

vec4 get_normal(vec4 intersection, Cube cube){
	for(int i = 0; i <cube.numPoints; i+=4)
	{
		//check each face's plane
		if(abs(dot((  intersection - (cube.points[i].v)), cube.points[i].normal)) <= 0.01){
			vec4 blah = cube.points[i].normal * -1.0f;
			//cout << blah.x << " " << blah.y << " " << blah.z << endl;
			return cube.points[i].normal * -1.0f;
		}
	}
	return vec4(-1,-1,-1,-1);
}