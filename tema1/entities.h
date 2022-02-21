#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


struct Player{
	GLfloat x;
	GLfloat y;
	GLfloat body_radius;
	GLfloat eye_radius;
	GLfloat img_r;
	GLfloat translateX;
	GLfloat translateY;
	GLfloat angle;
};

struct Obstacle {
	Obstacle() : x(0), y(0), width(0), height(0) {}
	Obstacle(GLfloat x, GLfloat y, GLfloat width, GLfloat height) :
		x(x), y(y), width(width), height(height) {}
	GLfloat x;
	GLfloat y;
	GLfloat width;
	GLfloat height;
};

struct Bullet {
	float len = 0.3f;
	float img_r;
	float x;
	float y;
	float angle;
	glm::vec3 direction;
	glm::vec3 position = glm::vec3(0);
};

struct Enemy {
	float length = 1.2f;
	float img_r;
	float x;
	float y;
	float angle;
	float speed;
	glm::vec3 direction;
	glm::vec3 position = glm::vec3(0);
};

