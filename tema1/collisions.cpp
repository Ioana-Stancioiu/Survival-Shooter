#include "collisions.h"
#include <iostream>

bool collision::PlayerMoveLeft(Player player, Tema1::Map map, vector<Obstacle> obstacles, float speed)
{
    float pX = player.x + player.translateX - speed;
    float pY = player.y + player.translateY;

    if (pX - player.img_r <= 0) {
        return false;
    }

    for (Obstacle obstacle : obstacles) {
        if (IsCollidedCircleRect(pX, pY, player.img_r,
                                 obstacle.x, obstacle.y, obstacle.width, obstacle.height)) {
            return false;
        }
    }

	return true;
}

bool collision::PlayerMoveRight(Player player, Tema1::Map map, vector<Obstacle> obstacles, float speed)
{
    float pX = player.x + player.translateX + speed;
    float pY = player.y + player.translateY;

    if (pX + player.img_r >= map.width) {
        return false;
    }

    for (Obstacle obstacle : obstacles) {
        if (IsCollidedCircleRect(pX, pY, player.img_r,
                                 obstacle.x, obstacle.y, obstacle.width, obstacle.height)) {
            return false;
        }
    }

    return true;
}

bool collision::PlayerMoveUp(Player player, Tema1::Map map, vector<Obstacle> obstacles, float speed)
{
    float pX = player.x + player.translateX;
    float pY = player.y + player.translateY + speed;

    if (pY + player.img_r >= map.height) {
        return false;
    }

    for (Obstacle obstacle : obstacles) {
        if (IsCollidedCircleRect(pX, pY, player.img_r,
                                 obstacle.x, obstacle.y, obstacle.width, obstacle.height)) {
            return false;
        }
    }

    return true;
}

bool collision::PlayerMoveDown(Player player, Tema1::Map map, vector<Obstacle> obstacles, float speed)
{
    float pX = player.x + player.translateX;
    float pY = player.y + player.translateY - speed;

    if (pY - player.img_r <= 0) {
        return false;
    }

    for (Obstacle obstacle : obstacles) {
        if (IsCollidedCircleRect(pX, pY, player.img_r,
                                 obstacle.x, obstacle.y, obstacle.width, obstacle.height)) {
            return false;
        }
    }

    return true;
}

bool collision::BulletObstacles(Bullet bullet, vector<Obstacle> obstacles)
{
    float bulletX = bullet.x + bullet.position.x;
    float bulletY = bullet.y + bullet.position.y;

    for (Obstacle obstacle : obstacles) {
        if (IsCollidedCircleRect(bulletX, bulletY, bullet.img_r,
            obstacle.x, obstacle.y, obstacle.width, obstacle.height)) {
            return true;
        }
    }
    return false;
}

bool collision::BulletMap(Bullet bullet, Tema1::Map map)
{
    float bulletX = bullet.x + bullet.position.x;
    float bulletY = bullet.y + bullet.position.y;

    if (bulletX + bullet.img_r >= map.width ||
        bulletX - bullet.img_r <= 0 ||
        bulletY + bullet.img_r >= map.height ||
        bulletY + bullet.img_r <= 0)
        return true;
    return false;
}

bool collision::BulletEnemies(Bullet bullet, vector<Enemy> *enemies, int* score)
{
    float bulletX = bullet.x + bullet.position.x;
    float bulletY = bullet.y + bullet.position.y;
    float enemyX, enemyY;


    for (int i = 0; i < enemies->size(); i++) {
        enemyX = (*enemies)[i].x + (*enemies)[i].position.x;
        enemyY = (*enemies)[i].y + (*enemies)[i].position.y;

        if (IsCollidedCircleCircle(bulletX, bulletY, bullet.img_r,
            enemyX, enemyY, (*enemies)[i].img_r)) {
            (*score)++;
            cout << "Enemy killed! " << "Score : " << *score << endl;
            enemies->erase(enemies->begin() + i);
            return true;
        }
    }
    return false;
}

bool collision::EnemyBullets(Enemy enemy, vector<Bullet>* bullets, int* score)
{
    float enemyX = enemy.x + enemy.position.x;
    float enemyY = enemy.y + enemy.position.y;
    float bulletX, bulletY;


    for (int i = 0; i < bullets->size(); i++) {
        bulletX = (*bullets)[i].x + (*bullets)[i].position.x;
        bulletY = (*bullets)[i].y + (*bullets)[i].position.y;

        if (IsCollidedCircleCircle(enemyX, enemyY, enemy.img_r,
            bulletX, bulletY, (*bullets)[i].img_r)) {
            (*score)++;
            cout << "Enemy killed! " << "Score : " << *score << endl;
            bullets->erase(bullets->begin() + i);
            return true;
        }
    }
    return false;
}


bool collision::PlayerEnemy(Player player, Enemy enemy, float* health)
{
    float playerX = player.x + player.translateX;
    float playerY = player.y + player.translateY;
    float enemyX = enemy.x + enemy.position.x;
    float enemyY = enemy.y + enemy.position.y;

    if (IsCollidedCircleCircle(playerX, playerY, player.img_r,
        enemyX, enemyY, enemy.img_r)) {
        (*health)--;
        return true;
    }
    return false;
}

bool collision::IsCollidedRectRect(float rect1_x, float rect1_y, float rect1_w, float rect1_h, 
                           float rect2_x, float rect2_y, float rect2_w, float rect2_h)
{
    if (rect1_x < rect2_x + rect2_w &&
        rect1_x + rect1_w > rect2_x &&
        rect1_y < rect2_y + rect2_h &&
        rect1_h + rect1_y > rect2_y)
        return true;
    return false;
}

bool collision::IsCollidedCircleCircle(float cX1, float cY1, float r1, float cX2, float cY2, float r2)
{
    float distx = (cX1 + r1) - (cX2 + r2);
    float disty = (cY1 + r1) - (cY2 + r2);
    float distance = sqrt(distx * distx + disty * disty);

    if (distance < r1 + r2)
        return true;
    return false;
}

bool collision::IsCollidedCircleRect(float cX1, float cY1, float r1, float rectX, float rectY, float rectW, float rectH)
{
    glm::vec2 circle_center = glm::vec2(cX1, cY1);
    glm::vec2 aabb_half_extends(rectW / 2.f, rectH / 2.f);
    glm::vec2 aabb_center(rectX + aabb_half_extends.x, rectY + aabb_half_extends.y);

    glm::vec2 difference = circle_center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extends, aabb_half_extends);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - circle_center;
    return glm::length(difference) < r1;
}
