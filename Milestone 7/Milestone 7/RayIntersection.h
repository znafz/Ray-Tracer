/**
  This testing framework has been developed/overhauled over time, primarily by:
  Chris Czyzewicz
  Ben Sunshine-Hill
  Cory Boatright 
  
  While the three were PhD students at UPenn (through 2013).  This particular version has some
  modifications by Cory since joining the faculty of Grove City College.
  
  Last revised 4/15/2015
*/

#ifndef RAYINTERSECTION_H
#define RAYINTERSECTION_H

#include "glm\glm.hpp"
#include "Geometry.h"

using namespace glm;



// Tries to find the intersection of a ray and a sphere.
// P0 is the position from which the ray emanates; V0 is the
// direction of the ray.
// matrix is the transformation matrix of the sphere.
double Test_RaySphereIntersect(const vec4& P0, const vec4& V0, const mat4& T);

// Tries to find the intersection of a ray and a triangle.
// This is just like the above function, but it intersects the ray with a
// triangle instead. The parameters p1, p2, and p3 specify the three
// points of the triangle, in object space.
double Test_RayPolyIntersect(const vec4& P0, const vec4& V0, const vec4& p1, const vec4& p2, const vec4& p3, const mat4& T);

// This is just like Test_RaySphereIntersect, but with a unit cube instead of a
// sphere. A unit cube extends from -0.5 to 0.5 in all axes.
double Test_RayCubeIntersect(const vec4& P0, const vec4& V0, const mat4& T);

//shadow feeler function
float ShadowFeeler(const vec4& intersection_point, const vec4& light_source, Geometry * objects, int numObjects);
#endif