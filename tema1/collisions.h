#pragma once

#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "lab_m1/tema1/entities.h"
#include "lab_m1/tema1/tema1.h"

using namespace m1;
using namespace std;

namespace collision {
	// checks if player can move to the left
	bool PlayerMoveLeft(Player player, Tema1::Map map, vector<Obstacle> obstacles, float speed);

	// checks if player can move to the right
	bool PlayerMoveRight(Player player, Tema1::Map map, vector <Obstacle> obstacles, float speed);

	// checks if player can move up
	bool PlayerMoveUp(Player player, Tema1::Map map, vector <Obstacle> obstacles, float speed);

	// checks if player can move down
	bool PlayerMoveDown(Player player, Tema1::Map map, vector <Obstacle> obstacles, float speed);

	// checks for collision between bullet and obstacles
	bool BulletObstacles(Bullet bullet, vector<Obstacle> obstacles);

	// checks for collision between bullet and map walls
	bool BulletMap(Bullet bullet, Tema1::Map map);

	// checks for collision between bullet and enemies
	bool BulletEnemies(Bullet bullet, vector<Enemy> *enemies, int* score);

	// checks for collision between enemy and bullets
	bool EnemyBullets(Enemy enemy, vector<Bullet> *bullets, int* score);

	// checks for collision between player and enemies
	bool PlayerEnemy(Player player, Enemy enemy, float* health);

	// rectangle rectangle collison
	bool IsCollidedRectRect(float rect1_x, float rect1_y, float rect1_w, float rect1_h,
		                   float rect2_x, float rect2_y, float rect2_w, float rect2_h);

	// circle circle collision
	bool IsCollidedCircleCircle(float cX1, float cY1, float r1, float cX2, float cY2, float r2);

	// circle rectangle collision
	bool IsCollidedCircleRect(float cX1, float cY1, float r1,
		                      float rectX, float rectY, float rectW, float rectH);
}