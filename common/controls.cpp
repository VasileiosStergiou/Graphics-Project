//GEORGOULAS DIMOSTHENIS 4039
//STERGIOU BASILEIOS 4300

// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

mat4 ViewMatrix;
mat4 ProjectionMatrix;

mat4 getViewMatrix(){
	return ViewMatrix;
}
mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

// Initial horizontal angle
float horizontalAngle = 0.0f;
// Initial vertical angle
float verticalAngle = 0.0f;

float speed = 30.0f; // 30 units / second
float mouseSpeed = 0.005f;

float r = 2.0f;

// Initial position : on +Z
vec3 position = vec3 (
	cos(verticalAngle * (3.14f / 180.0f)) * sin(horizontalAngle * (3.14f / 360.0f)) * r + 0.5f,
	sin(verticalAngle * (3.14f / 180.0f)) * r + 0.5f,
	cos(verticalAngle * (3.14f / 180.0f)) * cos(horizontalAngle * (3.14f / 360.0f)) * r + 0.5f
);


void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	vec3 direction(
		cos(verticalAngle * (3.14f / 180.0f)) * sin(horizontalAngle * (3.14f / 360.0f)) * r + 0.5f,
		sin(verticalAngle * (3.14f / 180.0f)) * r + 0.5f,
		cos(verticalAngle * (3.14f / 180.0f)) * cos(horizontalAngle * (3.14f / 360.0f)) * r + 0.5f
	);
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_X)) {
		r += speed * deltaTime / 10;
		position = direction;
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_E)) {
		r -= speed * deltaTime /10;
		position = direction;
	}

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position = direction;
		verticalAngle += 3.14f * speed* deltaTime;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		position = direction;
		verticalAngle -= 3.14f * speed* deltaTime;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		position = direction;
		//printf("horizontal angle = %f\n", horizontalAngle);
		horizontalAngle += 3.14f * speed* deltaTime;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		position = direction;
		//printf("horizontal angle = %f\n", horizontalAngle);
		horizontalAngle -= 3.14f * speed* deltaTime;
	}

	// Projection matrix : 45° Field of View, 1:1 ratio, display range : 1 unit <-> 100 units
	ProjectionMatrix = perspective(radians(45.0f), 1.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = lookAt(
								position,           // Camera is here
								vec3(0.5f, 0.5f, 0.5f), // and looks here : at the same position, plus "direction"
								vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}