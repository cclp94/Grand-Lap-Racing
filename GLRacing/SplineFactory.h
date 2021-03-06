#pragma once

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\ext.hpp>
#include <vector>

using namespace std;

/*
	Spline Factory generates Catmull Spline curves from given vertices
*/

class SplineFactory {

	static void subdivide(vector<GLfloat> &spline, float u0, float u1, float angleThreshold, 
	glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, bool curv) {
	float umid = (u0 + u1) / 2.0;		// Get middle point
	glm::vec3 pInitial = drawSpline(u0, p0, p1, p2, p3);	// Get Spline point in u0
	glm::vec3 pFinal = drawSpline(u1, p0, p1, p2, p3);		// Get Spline point in u1
	glm::vec3 pMid = drawSpline(umid, p0, p1, p2, p3);		// Get Spline point in uMid
	if(curv){
		if (u0 == 0 && u1 == 1) {			// Force first recursion
			subdivide(spline, u0, umid, angleThreshold, p0, p1, p2, p3, curv);
			subdivide(spline, umid, u1, angleThreshold, p0, p1, p2, p3, curv);
		}
		else {

			

			glm::vec3 totalDistance = glm::normalize(pFinal - pInitial);	// Vector from p1 - p0
			glm::vec3 midDistance = glm::normalize(pMid - pInitial);		// Vector from p0 to pMiddle

			float angle = glm::acos(glm::dot(midDistance, totalDistance));	// Angle to get curvature

			if (angle > angleThreshold) {									// If angle is not 'curvy' enough
				subdivide(spline, u0, umid, angleThreshold, p0, p1, p2, p3, curv);		// Recurse
				subdivide(spline, umid, u1, angleThreshold, p0, p1, p2, p3, curv);		// Recurse
			}
			else {
				spline.push_back(pInitial.x);		// Put point in vector
				spline.push_back(pInitial.y);		// Put point in vector
				spline.push_back(pInitial.z);		// Put point in vector
			}
		}
	}
	else {
		glm::vec3 totalDistance = glm::normalize(pFinal - pInitial);	// Vector from p1 - p0
		glm::vec3 midDistance = glm::normalize(pMid - pInitial);		// Vector from p0 to pMiddle

		float distance = glm::distance(pInitial, pFinal);	// Angle to get curvature

		if (distance > angleThreshold) {									// If angle is not 'curvy' enough
			subdivide(spline, u0, umid, angleThreshold, p0, p1, p2, p3, curv);		// Recurse
			subdivide(spline, umid, u1, angleThreshold, p0, p1, p2, p3, curv);		// Recurse

		}
		else {
			spline.push_back(pInitial.x);		// Put point in vector
			spline.push_back(pInitial.y);		// Put point in vector
			spline.push_back(pInitial.z);		// Put point in vector
												
		}
	}
}

	static glm::vec3 drawSpline(float u, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2,
		glm::vec3 p3) {

		GLfloat u2 = u*u;
		GLfloat u3 = u2*u;
		GLfloat fu1 = 2.0*u3 - 3.0*u2 + 1.0;
		GLfloat fu2 = -2.0*u3 + 3.0*u2;
		GLfloat fu3 = u3 - 2.0*u2 + u;
		GLfloat fu4 = u3 - u2;

		glm::vec3 p;

		p += fu1 * p0;

		p += fu2 * p1;

		p += fu3 * p2;

		p += fu4 * p3;

		return p;
	}
public:
	static vector<GLfloat> buildSpline(vector<GLfloat> vertices) {
		vector<GLfloat> spline;
		for (int i = 1; i <= (vertices.size() / 3) - 3; i++)
		{
			// Get Control points and tangents using Catmull splines
			glm::vec3 p0 = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
			glm::vec3 p1 = glm::vec3(vertices[i * 3 + 3], vertices[i * 3 + 4], vertices[i * 3 + 5]);
			// 0.5 value to tangent 
			glm::vec3 p2 = 0.5f * (p1 - glm::vec3(vertices[i * 3 - 3], vertices[i * 3 - 2], vertices[i * 3 - 1]));
			glm::vec3 p3 = 0.5f * (glm::vec3(vertices[i * 3 + 6], vertices[i * 3 + 7], vertices[i * 3 + 8]) - p0);
			// Get spline
			subdivide(spline, 0, 1, 0.02, p0, p1, p2, p3, false);
		}
		return spline;
	}

	/*
		Extrudes a given spline curve to generate Mesh
	*/

	static vector<GLfloat> extrudeSpline(vector<GLfloat> vertices, int scale) {
		vector<GLfloat> spline;
		for (int i = 0; i < (vertices.size() / 3) - 1; i++)
		{
			glm::vec3 normal = glm::vec3();

			glm::vec3 path = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
			glm::vec3 path2 = glm::vec3(vertices[i * 3 + 3], vertices[i * 3 + 4], vertices[i * 3 + 5]);


			// Note: Because we need to look ahead 1 array index, we make
			// sure that we do not exceed limits of path[i] array.
			if (i < vertices.size() - 1)
			{
				normal = glm::cross(path2 - path, glm::vec3(0.0, 1.0, 0.0));
				normal = glm::normalize(normal);
			}

			// Store left extrusion (calculated normal + point position
			glm::vec3 p = glm::vec3(glm::translate(glm::mat4(), normal / scale) *
				glm::vec4(path, 1.0));
			spline.push_back(p.x); spline.push_back(p.y); spline.push_back(p.z);
			// Store right extrusion (flipped calculated normal + point position)
			p = glm::vec3(glm::translate(glm::mat4(), -normal / scale) *
				glm::vec4(path, 1.0));
			spline.push_back(p.x); spline.push_back(p.y); spline.push_back(p.z);
		}
		return spline;
	}
	
};

