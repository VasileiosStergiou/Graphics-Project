//GEORGOULAS DIMOSTHENIS 4039
//STERGIOU BASILEIOS 4300

#define STB_IMAGE_IMPLEMENTATION

#include <time.h> // Include for random
#include <vector> // Include for vector arrays
using namespace std;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "common/controls.hpp"

#include "common/stb_image.h"

// For Sphere
#define STACKS 25
#define SECTORS 25

// Struct for Cubes
struct Cube { // note typedef is not needed */
	GLuint id;
	GLuint vertexBufferID;
	GLuint colorBufferID;
	GLfloat vertices[108];
	GLfloat colors[144]; // specified the size
	vector<GLfloat> direction;
};

// Struct for Spheres
struct Sphere { // note typedef is not needed */
	GLuint id;
	GLuint vertexBufferID;
	GLuint colorBufferID;
	GLfloat vertices[3 * 2 * STACKS * SECTORS * 3];
	GLfloat colors[3 * 2 * STACKS * SECTORS * 4]; // specified the size
	vector<GLfloat> direction;
};

struct Cylinder { // note typedef is not needed */
	GLuint id;
	GLuint vertexBufferID;
	GLuint colorBufferID;
	GLfloat vertices[3 * 4 * SECTORS * 3];
	GLfloat colors[3 * 4 * SECTORS * 4]; // specified the size
	vector<GLfloat> direction;
};


vector<Cube> cubeBuffers; // Vector with cube buffers
vector<Sphere> sphereBuffers; // Vector with sphere buffers
vector<Cylinder> cylinderBuffers; //Vector with cylinder buffers

// Function that initialize cubes

// This function initiates the coordinates of the triangles
// that the outer cube has and copies them in an array to avoid problems
// with pointers

void makeCube(GLfloat inarray[])
{
	static GLfloat temparray[] = {
		//1-DOWN//
		0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		//2-UP//
		0.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,0.0f,
		//3-FRONT//
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,
		//4-BACK//
		0.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,
		//5-RIGHT//
		1.0f,0.0f,0.0f,
		1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,0.0f,
		1.0f,0.0f,0.0f,
		//6-LEFT//
		0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,0.0f
	};
	for (int i = 0; i < 108; i++)
	{
		inarray[i] = temparray[i];

	}
}

// Function that initialize spheres
void makeSphere(GLfloat shpere[]) {
	// we split each sector in 2 triangles
	// where each triangle consists of 3 vertices

	vector<GLfloat> v1 = { 0.0f,0.0f,0.0f }; 
	vector<GLfloat> v2 = { 0.0f,0.0f,0.0f };
	vector<GLfloat> v3 = { 0.0f,0.0f,0.0f };
	vector<GLfloat> v4 = { 0.0f,0.0f,0.0f };

	//the central sphere has r = 15
	float r = 0.5f;
	float y, x, z;

	float pi = 2 * acos(0.0); //we store the value of π

	// to calculate each x,y and z of the sphere
	// we need the φ and θ angles
	float step_f = 180.0f / STACKS;
	float step_theta = 360.0f / SECTORS;
	int n = 0;

	// we start from π/2 to -π/2 and we decrease the step for each stack

	for (float f = 90.0f; round(f) > -90.0f; f -= step_f)
	{;

	// we start from 0 to 2π and we increase the step for each sector
		for (float theta = 0.0f; round(theta) < 360.0f; theta += step_theta)
		{

			x = (r * cos(f * pi / 180)) * sin(theta * pi / 180);
			y = r * sin(f * pi / 180);
			z = (r * cos(f * pi / 180)) * cos(theta * pi / 180);
			v1 = { x,y,z };

			x = (r * cos((f - step_f) * pi / 180)) * sin(theta * pi / 180);
			y = r * sin((f - step_f) * pi / 180);
			z = (r * cos((f - step_f) * pi / 180)) * cos(theta * pi / 180);
			v2 = { x,y,z };

			x = (r * cos(f * pi / 180)) * sin((theta + step_theta) * pi / 180);
			y = r * sin(f * pi / 180);
			z = (r * cos(f * pi / 180)) * cos((theta + step_theta) * pi / 180);
			v3 = { x,y,z };

			x = (r * cos((f - step_f) * pi / 180)) * sin((theta + step_theta) * pi / 180);
			y = r * sin((f - step_f) * pi / 180);
			z = (r * cos((f - step_f) * pi / 180)) * cos((theta + step_theta) * pi / 180);
			v4 = { x,y,z };

			//	v1   v3
			//	|   /
			//	|  /
			//  | /
			//	v2

			shpere[0 + n] = v1.at(0);
			shpere[1 + n] = v1.at(1);
			shpere[2 + n] = v1.at(2);

			shpere[3 + n] = v2.at(0);
			shpere[4 + n] = v2.at(1);
			shpere[5 + n] = v2.at(2);

			shpere[6 + n] = v3.at(0);
			shpere[7 + n] = v3.at(1);
			shpere[8 + n] = v3.at(2);

			// v3
			// | \
			// |  \
			// v2--v4

			shpere[9 + n] = v3.at(0);
			shpere[10 + n] = v3.at(1);
			shpere[11 + n] = v3.at(2);

			shpere[12 + n] = v2.at(0);
			shpere[13 + n] = v2.at(1);
			shpere[14 + n] = v2.at(2);

			shpere[15 + n] = v4.at(0);
			shpere[16 + n] = v4.at(1);
			shpere[17 + n] = v4.at(2);
			n += 18;

		}
	}
}



// This function implements a cylinder

void makeCylinder(GLfloat cylinder[]) {

	// we split each sector in 2 triangles
	// where each triangle consists of 3 vertices

	vector<GLfloat> v1 = { 0.0f,0.0f,0.0f };
	vector<GLfloat> v2 = { 0.0f,0.0f,0.0f };
	vector<GLfloat> v3 = { 0.0f,0.0f,0.0f };
	vector<GLfloat> v4 = { 0.0f,0.0f,0.0f };
	float step_theta = 360.0f / SECTORS;
	float pi = 2 * acos(0.0);
	GLfloat H = 1.0f;

	//the cylinder has r = 15
	float r = 0.5f;

	int n = 0;
	
	// we start from 0 to 2π and we increase the step for each sector

	// we make the circle on the top side
	for (float theta = 0.0f; theta < 360.0f; theta += step_theta) {
		v1 = { 0.0f ,H , 0.0f };

		v2.at(0) = r * sin(theta * pi / 180);
		v2.at(1) = H ;
		v2.at(2) = r * cos(theta * pi / 180);

		v3.at(0) = r * sin((theta + step_theta) * pi / 180);
		v3.at(1) = H;
		v3.at(2) = r * cos((theta + step_theta) * pi / 180);

		cylinder[n] = v1.at(0);
		cylinder[n + 1] = v1.at(1);
		cylinder[n + 2] = v1.at(2);


		cylinder[n + 3] = v2.at(0);
		cylinder[n + 4] = v2.at(1);
		cylinder[n + 5] = v2.at(2);

		cylinder[n + 6] = v3.at(0);
		cylinder[n + 7] = v3.at(1);
		cylinder[n + 8] = v3.at(2);

		n += 9;
	}

	// we calcualte the coordinates of the triangles for each side of the cylinder

	for (float theta = 0.0f; theta < 360.0f; theta += step_theta) {
		v1.at(0) = r * sin(theta * pi / 180);
		v1.at(1) = H;
		v1.at(2) = r * cos(theta * pi / 180);

		v2.at(0) = r * sin((theta)*pi / 180);
		v2.at(1) = 0.0f;
		v2.at(2) = r * cos((theta)*pi / 180);

		v3.at(0) = r * sin((theta + step_theta) * pi / 180);
		v3.at(1) = H;
		v3.at(2) = r * cos((theta + step_theta) * pi / 180);


		v4.at(0) = r * sin((theta + step_theta) * pi / 180);
		v4.at(1) = 0.0f;
		v4.at(2) = r * cos((theta + step_theta) * pi / 180);

		cylinder[n] = v1.at(0);
		cylinder[n + 1] = v1.at(1);
		cylinder[n + 2] = v1.at(2);

		cylinder[n + 3] = v2.at(0);
		cylinder[n + 4] = v2.at(1);
		cylinder[n + 5] = v2.at(2);

		cylinder[n + 6] = v3.at(0);
		cylinder[n + 7] = v3.at(1);
		cylinder[n + 8] = v3.at(2);

		cylinder[n + 9] = v3.at(0);
		cylinder[n + 10] = v3.at(1);
		cylinder[n + 11] = v3.at(2);

		cylinder[n + 12] = v2.at(0);
		cylinder[n + 13] = v2.at(1);
		cylinder[n + 14] = v2.at(2);

		cylinder[n + 15] = v4.at(0);
		cylinder[n + 16] = v4.at(1);
		cylinder[n + 17] = v4.at(2);

		n += 18;
	}
	// we make the circle on the top side
	for (float theta = 0.0f; theta < 360.0f; theta += step_theta) {
		v1 = { 0.0f,0.0f,0.0f };

		v2.at(0) = r * sin(theta * pi / 180);
		v2.at(1) = 0.0f;
		v2.at(2) = r * cos(theta * pi / 180);

		v3.at(0) = r * sin((theta + step_theta) * pi / 180);
		v3.at(1) = 0.0f;
		v3.at(2) = r * cos((theta + step_theta) * pi / 180);

		cylinder[n] = v2.at(0);
		cylinder[n + 1] = v2.at(1);
		cylinder[n + 2] = v2.at(2);


		cylinder[n + 3] = v1.at(0);
		cylinder[n + 4] = v1.at(1);
		cylinder[n + 5] = v1.at(2);

		cylinder[n + 6] = v3.at(0);
		cylinder[n + 7] = v3.at(1);
		cylinder[n + 8] = v3.at(2);

		n += 9;
	}
}

//This function takes as an input the coordinates of the central sphere
//and calcualtes the new u,v coordinates so that they fit in the texture image
void makeTexture(GLfloat shpere[]) {
	vector<GLfloat> v1 = { 0.0f,0.0f };
	vector<GLfloat> v2 = { 0.0f,0.0f };
	vector<GLfloat> v3 = { 0.0f,0.0f };
	vector<GLfloat> v4 = { 0.0f,0.0f };
	float u, v;
	float step_f = 180.0f / STACKS;
	float step_theta = 360.0f / SECTORS;
	int n = 0;

	for (float f = 180.0f; round(f) > 0.0f; f -= step_f)
	{
		for (float theta = 0.0f; round(theta) < 360.0f; theta += step_theta)
		{

			u = (theta / 360.0f);
			v = 1 - (f / 180.0f);
			v1 = { u,v };

			u = (theta / 360.0f);
			v = 1 - ((f - step_f) / 180.0f);
			v2 = { u,v };

			u = ((theta + step_theta) / 360.0f);
			v = 1 - (f / 180.0f);
			v3 = { u,v };

			u = ((theta + step_theta) / 360.0f);
			v = 1 - ((f - step_f) / 180.0f);
			v4 = { u,v };

			shpere[0 + n] = v1.at(0);
			shpere[1 + n] = v1.at(1);

			shpere[2 + n] = v2.at(0);
			shpere[3 + n] = v2.at(1);

			shpere[4 + n] = v3.at(0);
			shpere[5 + n] = v3.at(1);

			shpere[6 + n] = v3.at(0);
			shpere[7 + n] = v3.at(1);

			shpere[8 + n] = v2.at(0);
			shpere[9 + n] = v2.at(1);

			shpere[10 + n] = v4.at(0);
			shpere[11 + n] = v4.at(1);
			n += 12;

		}
	}
}


// Function that initialize colors for cubes by a random number in range
// [0.1 , 0.9]

void makeColorsCube(GLfloat inarray[], GLfloat transparency)
{
	float color_r = rand() % 10;
	color_r = color_r / 10;
	float color_g = rand() % 10;
	color_g = color_g / 10;
	float color_b = rand() % 10;
	color_b = color_b / 10;
	for (int v = 0; v < 12 * 3; v++) {
		inarray[4 * v + 0] = color_r;
		inarray[4 * v + 1] = color_g;
		inarray[4 * v + 2] = color_b;
		inarray[4 * v + 3] = transparency;
	}
}


// Function that initialize colors for spheres 
void makeColorsSphere(GLfloat inarray[]) {
	float color_r = rand() % 10;
	color_r = color_r / 10;
	float color_g = rand() % 10;
	color_g = color_g / 10;
	float color_b = rand() % 10;
	color_b = color_b / 10;
	for (int v = 0; v < 3 * 2 * STACKS * SECTORS; v++) {
		inarray[4 * v + 0] = color_r;
		inarray[4 * v + 1] = color_g;
		inarray[4 * v + 2] = color_b;
		inarray[4 * v + 3] = 1.0f;	// Spheres have a solid color
	}
}


// Function that initialize colors for MAIN sphere 
void makeColorsMainSphere(GLfloat inarray[]) {
	float color_r = rand() % 10;
	color_r = color_r / 10;
	float color_g = rand() % 10;
	color_g = color_g / 10;
	float color_b = rand() % 10;
	color_b = color_b / 10;
	for (int v = 0; v < 3 * 2 * STACKS * SECTORS; v++) {
		inarray[4 * v + 0] = 1.0f;	// RED sphere
		inarray[4 * v + 1] = 0.0f;
		inarray[4 * v + 2] = 0.0f;
		inarray[4 * v + 3] = 1.0f;	// Main sphere is never transparent
	}
}



// this function apllies a random color on each vertex of the cylinder and
// a tranparency level for the colors

void makeColorsCylinder(GLfloat inarray[])
{
	float color_r = rand() % 10;
	color_r = color_r / 10;
	float color_g = rand() % 10;
	color_g = color_g / 10;
	float color_b = rand() % 10;
	color_b = color_b / 10;
	for (int v = 0; v < 3 * 4 * SECTORS; v++) {
		inarray[4 * v + 0] = color_r;
		inarray[4 * v + 1] = color_g;
		inarray[4 * v + 2] = color_b;
		inarray[4 * v + 3] = 1.0f;
	}
}

// Scale function for cubes
void scalematrix(GLfloat inarray[]) {
	//srand(time(NULL));
	float randomSize = rand() % 10 + 1;
	randomSize = randomSize / 100;
	printf("%f\n", randomSize);
	for (int i = 0; i < 108; i++)
	{
		inarray[i] = inarray[i] * randomSize + randomSize;
	}
}


// Scale function for spheres
void scalematrixSphere(GLfloat inarray[]) {
	float randomSize = rand() % 10 + 1;
	randomSize = randomSize / 100;
	printf("%f\n", randomSize);
	for (int i = 0; i < 3 * 2 * STACKS * SECTORS * 3; i++)
	{
		inarray[i] = inarray[i] * randomSize + randomSize;	// + randomSize to never collide with MAIN Cube
	}
}

void scalematrixMainSphere(GLfloat inarray[]) {
	for (int i = 0; i < 3 * 2 * STACKS * SECTORS * 3; i++)
	{
		inarray[i] = inarray[i] * 0.3f + 0.5f; // + 0.5 to be on the center of cube
	}
}


// this function resizes the cylinder by a random number in range
// [0.1 , 0.9]

void scaleCylinder(GLfloat inarray[]) {
	float randomSize = rand() % 10 + 1;
	randomSize = randomSize / 100;
	printf("%f\n", randomSize);
	for (int i = 0; i < 3 * 4 * SECTORS * 3; i++)
	{
		inarray[i] = inarray[i]  * randomSize + randomSize;
	}
}


// Initialize buffers, link objects with buffers
void InitObject(GLuint& VertexBuffer, GLuint& ColorBuffer, GLenum target, GLenum usage, const GLfloat* vertexdata, const GLfloat* colordata, int vertexsize, int colorsize)
{
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(target, VertexBuffer);
	glBufferData(target, vertexsize, vertexdata, usage);

	glGenBuffers(1, &ColorBuffer);
	glBindBuffer(target, ColorBuffer);
	glBufferData(target, colorsize, colordata, usage);
}

// Link updated objects with buffers
void updateObject(GLuint& VertexBuffer, GLuint& ColorBuffer, GLenum target, GLenum usage, const GLfloat* vertexdata, const GLfloat* colordata, int vertexsize, int colorsize)
{
	glBindBuffer(target, VertexBuffer);
	glBufferData(target, vertexsize, vertexdata, usage);

	glBindBuffer(target, ColorBuffer);
	glBufferData(target, colorsize, colordata, usage);
}

// Draw any given object
void InitDraws(GLuint& vertexbuffer, GLuint& colorbuffer, GLenum target, GLuint object_id, int loc)
{
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glUniform1i(loc, 0);	// Set loc to 0 to never draw texture and apply color
	glBindBuffer(target, vertexbuffer);
	glVertexAttribPointer(
		0,                  // Must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(target, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw depending on object id 
	if (object_id == 1) {
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	}
	else if (object_id == 2) {
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * STACKS * SECTORS);

	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, 3 * 4 * SECTORS * 3);
	}
}

// Check for collition with MAIN sphere
void collition_with_sphere(GLfloat *max_X, GLfloat *max_Y, GLfloat *max_Z, GLfloat *min_X, GLfloat *min_Y, GLfloat *min_Z, Sphere& sphereVertices) {

	// Get Min and Max x,y,z for Main sphere
	for (int i = 0; i < 3 * 2 * STACKS * SECTORS * 3; i += 3) {
		if (sphereVertices.vertices[i] > *max_X) {
			*max_X = sphereVertices.vertices[i];
		}

		if (sphereVertices.vertices[i] < *min_X) {
			*min_X = sphereVertices.vertices[i];
		}

		if (sphereVertices.vertices[i + 1] > *max_Y) {
			*max_Y = sphereVertices.vertices[i + 1];
		}

		if (sphereVertices.vertices[i + 1] < *min_Y) {
			*min_Y = sphereVertices.vertices[i + 1];
		}


		if (sphereVertices.vertices[i + 2] > *max_Z) {
			*max_Z = sphereVertices.vertices[i + 2];
		}

		if (sphereVertices.vertices[i + 2] < *min_Z) {
			*min_Z = sphereVertices.vertices[i + 2];
		}

	}

}

// MAIN sphere navigation
void navigateSphere(Sphere& currentSphere, int xyz, int posneg) {
	float speed = 0.001f * posneg; // posneg is for changing the direction of sphere 
	if (posneg == 1) {
		for (size_t i = 0; i < 3 * 2 * STACKS * SECTORS * 3; i += 3)
		{
			if ((currentSphere.vertices[i + xyz] + speed) >= 1.0f) {
				return;
			}
		}
	}

	else {
		for (size_t i = 0; i < 3 * 2 * STACKS * SECTORS * 3; i += 3)
		{
			if ((currentSphere.vertices[i + xyz] + speed) <= 0.0f) {
				return;
			}
		}
	}

	for (size_t i = 0; i < 3 * 2 * STACKS * SECTORS * 3; i += 3)
	{
		currentSphere.vertices[i + xyz] += speed;
	}

	updateObject(currentSphere.vertexBufferID, currentSphere.colorBufferID, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		currentSphere.vertices,
		currentSphere.colors,
		sizeof(currentSphere.vertices),
		sizeof(currentSphere.colors));
}

// Small cubes navigation
void navigateInside(GLfloat vertices[], vector<GLfloat> *direction, int id, Sphere& sphereVertices) {
	int array_size = 0;
	switch (id) {
		case 1:
			array_size = 108;
			break;
		case 2:
			array_size = 3 * 2 * STACKS * SECTORS * 3;
			break;
		case 3:
			array_size = 3 * 4 * SECTORS * 3;
			break;
	}
	GLfloat x = direction->at(0);
	GLfloat y = direction->at(1);
	GLfloat z = direction->at(2);

	GLfloat max_X = -1;
	GLfloat max_Y = -1;
	GLfloat max_Z = -1;

	GLfloat min_X = 2;
	GLfloat min_Y = 2;
	GLfloat min_Z = 2;

	collition_with_sphere(&max_X, &max_Y, &max_Z, &min_X, &min_Y, &min_Z, sphereVertices);

	for (int v = 0; v <= array_size - 3; v += 3) {

		// Check if we are out of Cube bounds
		if (vertices[v] > 1.0f
			|| vertices[v] < 0.0f) {
			x = -direction->at(0);
		}

		if (vertices[v + 1] > 1.0f
			|| vertices[v + 1] < 0.0f) {

			y = -direction->at(1);
		}

		if (vertices[v + 2] > 1.0f
			|| vertices[v + 2] < 0.0f) {

			z = -direction->at(2);
		}

		if ((vertices[v] >= min_X && vertices[v] <= max_X) &&
			(vertices[v + 1] >= min_Y && vertices[v + 1] <= max_Y) &&
			(vertices[v + 2] >= min_Z && vertices[v + 2] <= max_Z))
		{
			x = -direction->at(0);
			y = -direction->at(1);
			z = -direction->at(2);
		}
	}

	// Set direction (updated or not)
	direction->at(0) = x;
	direction->at(1) = y;
	direction->at(2) = z;
	for (int v = 0; v <= array_size - 3; v += 3) {
		vertices[v] += direction->at(0);
		vertices[v + 1] += direction->at(1);
		vertices[v + 2] += direction->at(2);
	}

};


// MAIN
int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(600, 600, u8"Συγκρουόμενα", NULL, NULL); // 600x600 window
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwPollEvents();
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// For transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable Texture
	glEnable(GL_TEXTURE_2D);

	// Initialise VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Initialise texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	// Load image using an external lib
	int width, height, nrChannels;
	unsigned char* data = stbi_load("texture.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	//Maybe is useless
	}
	else
	{
		printf("FAILED DATA IMPORT");
	}
	stbi_image_free(data); // Delete data after loading data on buffer

	// Set texture repeat if something goes wrong
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Maybe is useless 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//Maybe is useless 

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// We use it to choose between solid red color or texture for the sphere
	GLint loc = glGetUniformLocation(programID, "InSignal");

	srand(time(NULL)); // For random

	// MAIN Cube
	Cube newCube;
	newCube.id = 1;	// Id 1 for cubes
	newCube.direction = { 0.0f,0.0f,0.0f };	// Main cube is not moving
	makeCube(newCube.vertices);
	makeColorsCube(newCube.colors, 0.3f);
	cubeBuffers.push_back(newCube);

	InitObject(cubeBuffers.at(0).vertexBufferID,
		cubeBuffers.at(0).colorBufferID,
		GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		cubeBuffers.at(0).vertices,
		cubeBuffers.at(0).colors,
		sizeof(cubeBuffers.at(0).vertices), sizeof(cubeBuffers.at(0).colors));

	// MAIN Sphere
	Sphere newSphere;
	newSphere.id = 2;	// Id 2 for spheres
	newSphere.direction = { 0.0f,0.0f,0.0f };	// Main cube is not moving automatically

	makeSphere(newSphere.vertices);
	scalematrixMainSphere(newSphere.vertices);
	makeColorsMainSphere(newSphere.colors);
	sphereBuffers.push_back(newSphere);


	InitObject(sphereBuffers.at(0).vertexBufferID,
		sphereBuffers.at(0).colorBufferID,
		GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		sphereBuffers.at(0).vertices,
		sphereBuffers.at(0).colors,
		sizeof(sphereBuffers.at(0).vertices), sizeof(sphereBuffers.at(0).colors));

	// To communicate with fragment shader
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	
	// We create vertices for texture
	GLfloat texturesarray[3 * 2 * STACKS * SECTORS * 2];
	makeTexture(texturesarray);

	// Send textures vertices to buffer
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturesarray), texturesarray, GL_STATIC_DRAW);

	int sig = 0;	// Signal if space is pressed
	int sig2 = 0;	// Signal if T is pressed
	int btn = -1;	// If btn == -1 we apply texture else we disable texture and enable solid red color
	float global_speed = 1;
	do {
		// If Space is pressed we generate a random object
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE) && sig == 0) {	//we use sig==0 to get inside only once each time
			int randomObject = rand() % 3 + 1;	//generate object id
			//randomObject = 1;
			if (randomObject == 1) {	// for cubes
				sig = 1;
				Cube newCube;
				makeCube(newCube.vertices);
				makeColorsCube(newCube.colors, 1.0f);
				scalematrix(newCube.vertices);

				float randomX = rand() % 9 + 1;
				randomX = (randomX / 1000) * global_speed;

				float randomY = rand() % 9 + 1;
				randomY = (randomY / 1000) * global_speed;

				float randomZ = rand() % 9 + 1;
				randomZ = (randomZ / 1000) * global_speed;

				newCube.direction = { randomX, randomY, randomZ };
				newCube.id = 1;
				cubeBuffers.push_back(newCube);

				InitObject(cubeBuffers.at(cubeBuffers.size() - 1).vertexBufferID, cubeBuffers.at(cubeBuffers.size() - 1).colorBufferID, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
					cubeBuffers.at(cubeBuffers.size() - 1).vertices,
					cubeBuffers.at(cubeBuffers.size() - 1).colors,
					sizeof(cubeBuffers.at(cubeBuffers.size() - 1).vertices),
					sizeof(cubeBuffers.at(cubeBuffers.size() - 1).colors));
			}

			else if (randomObject == 2) {	// For spheres
				sig = 1;
				Sphere newSphere;
				makeSphere(newSphere.vertices);
				makeColorsSphere(newSphere.colors);
				scalematrixSphere(newSphere.vertices);

				float randomX = rand() % 9 + 1;
				randomX = (randomX / 1000) * global_speed;

				float randomY = rand() % 9 + 1;
				randomY = (randomY / 1000) * global_speed;

				float randomZ = rand() % 9 + 1;
				randomZ = (randomZ / 1000) * global_speed;

				newSphere.direction = { randomX, randomY, randomZ };
				newSphere.id = 2;
				sphereBuffers.push_back(newSphere);

				InitObject(sphereBuffers.at(sphereBuffers.size() - 1).vertexBufferID, sphereBuffers.at(sphereBuffers.size() - 1).colorBufferID, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
					sphereBuffers.at(sphereBuffers.size() - 1).vertices,
					sphereBuffers.at(sphereBuffers.size() - 1).colors,
					sizeof(sphereBuffers.at(sphereBuffers.size() - 1).vertices),
					sizeof(sphereBuffers.at(sphereBuffers.size() - 1).colors));
			}
			else {	// For cylinders
				sig = 1;
				Cylinder newCylinder;
				makeCylinder(newCylinder.vertices);
				makeColorsCylinder(newCylinder.colors);
				scaleCylinder(newCylinder.vertices);

				float randomX = rand() % 9 + 1;
				randomX = (randomX / 1000) * global_speed;

				float randomY = rand() % 9 + 1;
				randomY = (randomY / 1000) * global_speed;

				float randomZ = rand() % 9 + 1;
				randomZ = (randomZ / 1000) * global_speed;

				newCylinder.direction = { randomX, randomY, randomZ };
				newCylinder.id = 3;
				cylinderBuffers.push_back(newCylinder);

				InitObject(cylinderBuffers.at(cylinderBuffers.size() - 1).vertexBufferID, cylinderBuffers.at(cylinderBuffers.size() - 1).colorBufferID, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
					cylinderBuffers.at(cylinderBuffers.size() - 1).vertices,
					cylinderBuffers.at(cylinderBuffers.size() - 1).colors,
					sizeof(cylinderBuffers.at(cylinderBuffers.size() - 1).vertices),
					sizeof(cylinderBuffers.at(cylinderBuffers.size() - 1).colors));
			}
		}
		if (GLFW_RELEASE == glfwGetKey(window, GLFW_KEY_SPACE)) {
			sig = 0;	// We set sig back to 0 so we can press space again
		}

		// Increase or decrease speed for all objects (except Main cube and main sphere)
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_COMMA)) {
			global_speed *= 0.99f;
			for (int c = 1; c < cubeBuffers.size(); c++)
			{
				cubeBuffers.at(c).direction.at(0) *= 0.99f;
				cubeBuffers.at(c).direction.at(1) *= 0.99f;
				cubeBuffers.at(c).direction.at(2) *= 0.99f;
			}

			for (int c = 1; c < sphereBuffers.size(); c++)
			{
				sphereBuffers.at(c).direction.at(0) *= 0.99f;
				sphereBuffers.at(c).direction.at(1) *= 0.99f;
				sphereBuffers.at(c).direction.at(2) *= 0.99f;
			}

			for (int c = 0; c < cylinderBuffers.size(); c++)
			{
				cylinderBuffers.at(c).direction.at(0) *= 0.99f;
				cylinderBuffers.at(c).direction.at(1) *= 0.99f;
				cylinderBuffers.at(c).direction.at(2) *= 0.99f;
			}
		}
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_PERIOD)) {
			global_speed *= 1.01f;
			for (int c = 1; c < cubeBuffers.size(); c++)
			{
				cubeBuffers.at(c).direction.at(0) *= 1.01f;
				cubeBuffers.at(c).direction.at(1) *= 1.01f;
				cubeBuffers.at(c).direction.at(2) *= 1.01f;
			}

			for (int c = 1; c < sphereBuffers.size(); c++)
			{
				sphereBuffers.at(c).direction.at(0) *= 1.01f;
				sphereBuffers.at(c).direction.at(1) *= 1.01f;
				sphereBuffers.at(c).direction.at(2) *= 1.01f;
			}

			//navigate for Cylinders 
			for (int c = 0; c < cylinderBuffers.size(); c++)
			{
				cylinderBuffers.at(c).direction.at(0) *= 1.01f;
				cylinderBuffers.at(c).direction.at(1) *= 1.01f;
				cylinderBuffers.at(c).direction.at(2) *= 1.01f;
			}
		}

		// Navigate and bind updated objects to buffers
		for (int c = 1; c < cubeBuffers.size(); c++)
		{
			navigateInside(cubeBuffers.at(c).vertices,&cubeBuffers.at(c).direction, cubeBuffers.at(c).id, sphereBuffers.at(0));
			updateObject(cubeBuffers.at(c).vertexBufferID, cubeBuffers.at(c).colorBufferID, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
				cubeBuffers.at(c).vertices,
				cubeBuffers.at(c).colors,
				sizeof(cubeBuffers.at(c).vertices),
				sizeof(cubeBuffers.at(c).colors));
		}

		for (int c = 1; c < sphereBuffers.size(); c++)
		{
			navigateInside(sphereBuffers.at(c).vertices, &sphereBuffers.at(c).direction, sphereBuffers.at(c).id, sphereBuffers.at(0));
			updateObject(sphereBuffers.at(c).vertexBufferID, sphereBuffers.at(c).colorBufferID, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
				sphereBuffers.at(c).vertices,
				sphereBuffers.at(c).colors,
				sizeof(sphereBuffers.at(c).vertices),
				sizeof(sphereBuffers.at(c).colors));
		}

		for (int c = 0; c < cylinderBuffers.size(); c++)
		{
			navigateInside(cylinderBuffers.at(c).vertices, &cylinderBuffers.at(c).direction, cylinderBuffers.at(c).id, sphereBuffers.at(0));
			updateObject(cylinderBuffers.at(c).vertexBufferID, cylinderBuffers.at(c).colorBufferID, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
				cylinderBuffers.at(c).vertices,
				cylinderBuffers.at(c).colors,
				sizeof(cylinderBuffers.at(c).vertices),
				sizeof(cylinderBuffers.at(c).colors));
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();	// Camera control
		mat4 ProjectionMatrix = getProjectionMatrix();
		mat4 ViewMatrix = getViewMatrix();
		mat4 ModelMatrix = mat4(1.0);
		mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//Navigate Main Sphere in x,y,z axis
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			navigateSphere(sphereBuffers.at(0), 1, 1);
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			navigateSphere(sphereBuffers.at(0), 1, -1);
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			navigateSphere(sphereBuffers.at(0), 0, 1);
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			navigateSphere(sphereBuffers.at(0), 0, -1);
		}
		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
			navigateSphere(sphereBuffers.at(0), 2, 1);
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
			navigateSphere(sphereBuffers.at(0), 2, -1);
		}

		// Draw all objects 
		for (int c = cubeBuffers.size() - 1; c > 0; c--)
		{
			InitDraws(cubeBuffers.at(c).vertexBufferID, cubeBuffers.at(c).colorBufferID, GL_ARRAY_BUFFER, cubeBuffers.at(c).id, loc);
		}

		for (int c = sphereBuffers.size() - 1; c > 0; c--)
		{
			InitDraws(sphereBuffers.at(c).vertexBufferID, sphereBuffers.at(c).colorBufferID, GL_ARRAY_BUFFER, sphereBuffers.at(c).id, loc);
		}

		for (int c = cylinderBuffers.size() - 1; c >= 0; c--)
		{
			InitDraws(cylinderBuffers.at(c).vertexBufferID, cylinderBuffers.at(c).colorBufferID, GL_ARRAY_BUFFER, cylinderBuffers.at(c).id, loc);
		}

		// Activates texture and bind to buffer
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(TextureID, 0);
		
		// Send Sphere vetrices to shaders
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sphereBuffers.at(0).vertexBufferID);
		glVertexAttribPointer(
			0,                  // must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T) && sig2==0) {//we use sig2==0 to get inside only once each time
			sig2 = 1;	
			btn *= -1;	// Change to opposite statment		
		}
		if (GLFW_RELEASE == glfwGetKey(window, GLFW_KEY_T)) {
			sig2 = 0; // We set sig2 back to 0 so we can press T again
		}
		
		// Texture
		if (btn == 1) {
			glUniform1i(loc, 1);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
			glVertexAttribPointer(
				2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);	
		}

		// Solid color
		if (btn == -1) {
			glUniform1i(loc, 0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, sphereBuffers.at(0).colorBufferID);
			glVertexAttribPointer(
				1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				4,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);		
		}

		// Draw Main sphere
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * STACKS * SECTORS);

		// Draw Main cube as last object so we get a transparent cube
		InitDraws(cubeBuffers.at(0).vertexBufferID, cubeBuffers.at(0).colorBufferID, GL_ARRAY_BUFFER, cubeBuffers.at(0).id, loc);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteVertexArrays(1, &VertexArrayID);
	for (int c = 0; c < cubeBuffers.size(); c++)
	{
		glDeleteBuffers(1, &cubeBuffers.at(c).vertexBufferID);
		glDeleteBuffers(1, &cubeBuffers.at(c).colorBufferID);
	}
	for (int c = 0; c < sphereBuffers.size(); c++)
	{
		glDeleteBuffers(1, &sphereBuffers.at(c).vertexBufferID);
		glDeleteBuffers(1, &sphereBuffers.at(c).colorBufferID);
	}

	for (int c = 0; c < cylinderBuffers.size(); c++)
	{
		glDeleteBuffers(1, &cylinderBuffers.at(c).vertexBufferID);
		glDeleteBuffers(1, &cylinderBuffers.at(c).colorBufferID);
	}

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
