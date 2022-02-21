#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/object2D.h"
#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/collisions.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 20;   // logic width
    logicSpace.height = 10;  // logic height

    player.translateX = 0;
    player.translateY = 0;
    player.angle = 0;
    scaleX = scaleY = 0;
    
    // initialize player coordinate
    player.x = logicSpace.width / 2;
    player.y = logicSpace.height / 2;

    player.body_radius = 0.7f;
    player.eye_radius = player.body_radius / 3;
    // radius of circle "hit box"
    player.img_r = player.body_radius + player.eye_radius / 2;

    // initialize map dimensions
    map.width = logicSpace.width * 1.2f;
    map.height = logicSpace.height * 2.f;

    InitializeObjects();
}


// Declares meshes used for rendering entities
void m1::Tema1::InitializeObjects()
{
    Mesh* map_rectangle = object2D::CreateRectangle("map", glm::vec3(0), map.height, map.width, grey, true);
    AddMeshToList(map_rectangle);

    Mesh* outline = object2D::CreateCircle("outline", player.body_radius, glm::vec3(player.x, player.y, 0), black, false);
    AddMeshToList(outline);

    Mesh* circle = object2D::CreateCircle("circle", player.body_radius, glm::vec3(player.x, player.y, 0), creme);
    AddMeshToList(circle);

    Mesh* obstacle = object2D::CreateRectangle("obstacle", glm::vec3(0), 1.f, 1.f, green, true);
    AddMeshToList(obstacle);

    Mesh* projectile = object2D::CreateSquare("bullet", glm::vec3(0), 0.3f, black, true);
    AddMeshToList(projectile);

    Mesh* enemy = object2D::CreateSquare("enemy", glm::vec3(0), 1.2f, red, true);
    AddMeshToList(enemy);

    Mesh* enemy_out = object2D::CreateSquare("enemy_out", glm::vec3(0), 1.2f, black, false);
    AddMeshToList(enemy_out);

    Mesh* healthBar = object2D::CreateRectangle("health", glm::vec3(0), 1.f, 5.f, red, true);
    AddMeshToList(healthBar);

    Mesh* healthBar_out = object2D::CreateRectangle("health_out", glm::vec3(0), 1.f, 5.f, red, false);
    AddMeshToList(healthBar_out);
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glDisable(GL_DEPTH_TEST);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, black, true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    DrawScene(visMatrix, deltaTimeSeconds);

    secSinceLastFired += deltaTimeSeconds;
    secSinceLastEnemyRender += deltaTimeSeconds;
}


void Tema1::FrameEnd()
{
}


// Draw game scene
void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    RenderMesh2D(meshes["map"], shaders["VertexColor"], visMatrix);

    DrawObstacles(visMatrix);

    if (health > 0) {
        DrawBullets(visMatrix, deltaTimeSeconds);

        DrawCharacter(visMatrix);

        DrawEnemies(visMatrix, deltaTimeSeconds);
    }
    else if (health == 0) {
        DrawGameOver();
    }
   
    DrawHealthBar(visMatrix);
}


// Draws player
void m1::Tema1::DrawCharacter(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix;

    // render body
    modelMatrix *= transform2D::Translate(player.translateX, player.translateY);
    modelMatrix *= transform2D::Translate(player.x, player.y);
    modelMatrix *= transform2D::Rotate(player.angle);
    modelMatrix *= transform2D::Translate(-player.x, -player.y);

    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["outline"], shaders["VertexColor"], modelMatrix);

    // render left hand
    modelMatrix *= transform2D::Translate(-player.body_radius * cos(70), -player.body_radius * sin(70));
    modelMatrix *= transform2D::Translate(player.x, player.y);
    modelMatrix *= transform2D::Scale(0.33f, 0.33f);
    modelMatrix *= transform2D::Translate(-player.x, -player.y);
    
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["outline"], shaders["VertexColor"], modelMatrix);
    
    // render right hand
    modelMatrix *= transform2D::Translate(player.body_radius*(cos(70) - cos(14.7)) * 3, 
                                          player.body_radius*(sin(70) - sin(14.7)) * 3);

    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["outline"], shaders["VertexColor"], modelMatrix);
}


// Draw obstacles 
void m1::Tema1::DrawObstacles(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix * transform2D::Translate(1.5, map.height - 5) * transform2D::Scale(4, 4);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);   
    obstacles.push_back(Obstacle(1.5, map.height - 5, 4, 4));

    modelMatrix = visMatrix * transform2D::Translate(2, 0.5) * transform2D::Scale(6, 1);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    obstacles.push_back(Obstacle(2, 0.5, 6, 1));

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(2, 0.5);
    modelMatrix *= transform2D::Scale(1, 6);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    obstacles.push_back(Obstacle(2, 0.5, 1, 6));

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(7, map.height - 4);
    modelMatrix *= transform2D::Scale(4, 2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    obstacles.push_back(Obstacle(7, map.height - 4, 4, 2));

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(map.width - 4, 1);
    modelMatrix *= transform2D::Scale(3, 3);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    obstacles.push_back(Obstacle(map.width - 4, 1, 3, 3));

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(map.width - 5, map.height - 6);
    modelMatrix *= transform2D::Scale(2, 5);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    obstacles.push_back(Obstacle(map.width - 5, map.height - 6, 2, 5));

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(map.width / 2, map.height / 2 - 3);
    modelMatrix *= transform2D::Scale(5, 5);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    obstacles.push_back(Obstacle(map.width / 2, map.height / 2 - 3, 5, 5));
}

// Draw and move enemies
void m1::Tema1::DrawEnemies(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    // if 3 seconds have passed declare a new enemy
    if (secSinceLastEnemyRender >= enemyRenderRate) {
        Enemy new_enemy;
        // radius of circle "hit box"
        new_enemy.img_r = new_enemy.length * sqrt(2) / 2 + new_enemy.length * sqrt(2) / 6;

        // choose enemy position and speed at random
        new_enemy.x = rand() % ((int)logicSpace.width - 1) + 1;
        new_enemy.y = rand() % ((int)logicSpace.height - 1) + 1;
        new_enemy.speed = rand() % 10 + 1;
        new_enemy.speed *= deltaTimeSeconds / 2;
        enemies.push_back(new_enemy);

        secSinceLastEnemyRender = 0;
    }

    for (int i = 0; i < enemies.size(); i++) {
        // old position
        float eX = enemies[i].x + enemies[i].position.x;
        float eY = enemies[i].y + enemies[i].position.y;

        // direction to reach player
        enemies[i].direction = glm::vec3(player.x + player.translateX - eX,
                                         player.y + player.translateY - eY, 0);

        // enemy shouldn't pass player
        float travelDistance = MIN(enemies[i].speed, glm::length(enemies[i].direction));
        enemies[i].position += glm::normalize(enemies[i].direction) * travelDistance;

        // new position
        eX = enemies[i].x + enemies[i].position.x;
        eY = enemies[i].y + enemies[i].position.y;

        // angle to rotate enemy to face player  
        enemies[i].angle = atan2(player.y + player.translateY - eY, player.x + player.translateX - eX);

        // enemy body
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(enemies[i].x, enemies[i].y);
        modelMatrix *= transform2D::Translate(enemies[i].position.x, enemies[i].position.y);
        modelMatrix *= transform2D::Rotate(enemies[i].angle);

        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemy_out"], shaders["VertexColor"], modelMatrix);

        // left eye
        modelMatrix *= transform2D::Translate(enemies[i].length / 2, enemies[i].length / 2);
        modelMatrix *= transform2D::Scale(0.33f, 0.33f);

        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemy_out"], shaders["VertexColor"], modelMatrix);

        // right eye
        modelMatrix *= transform2D::Translate(0, -enemies[i].length * 3);

        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["enemy_out"], shaders["VertexColor"], modelMatrix);

        // check if enemy collided with player or bullet
        if (collision::EnemyBullets(enemies[i], &bullets, &score) ||
            collision::PlayerEnemy(player, enemies[i], &health)) {
            enemies.erase(enemies.begin() + i);
        }
    }
}


// Draw and move bullets
void m1::Tema1::DrawBullets(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    // bullet speed
    float speed = 10 * deltaTimeSeconds;
    // distance they can cover
    float maxDistance = 5;

    for (int i = 0; i < bullets.size(); i++) {
        modelMatrix = visMatrix;
        bullets[i].position += bullets[i].direction * speed;

        modelMatrix *= transform2D::Translate(bullets[i].x, bullets[i].y);
        modelMatrix *= transform2D::Translate(bullets[i].position.x, bullets[i].position.y);
        modelMatrix *= transform2D::Rotate(bullets[i].angle);
       
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);

        float coveredDist = sqrt(bullets[i].position.x * bullets[i].position.x +
            bullets[i].position.y * bullets[i].position.y);

        if (coveredDist >= maxDistance) {
            bullets.erase(bullets.begin() + i);
            continue;
        }

        // checks if bullet collided with obstacle, map walls ot enemy
        if (collision::BulletObstacles(bullets[i], obstacles) ||
            collision::BulletMap(bullets[i], map) || 
            collision::BulletEnemies(bullets[i], &enemies, &score)) {
            bullets.erase(bullets.begin() + i);
            continue;
        }
    }
}


// Draw health bar
void m1::Tema1::DrawHealthBar(glm::mat3 visMatrix)
{
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(logicSpace.width + logicSpace.x - 5.5f, logicSpace.height + logicSpace.y - 1.f);

    RenderMesh2D(meshes["health_out"], shaders["VertexColor"], modelMatrix);

    if (health >= 0) {
        modelMatrix *= transform2D::Translate((10 - health) / 2.f, 0);
        modelMatrix *= transform2D::Scale(health / 10.f, 1);
        RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);
    }
}


// Write Game Over in console
void m1::Tema1::DrawGameOver()
{
        cout << R"(
                  _____                         ____                 
                 / ____|                       / __ \                
                | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ 
                | | |_ |/ _` | '_ ` _ \ / _ \ | |  | \ \ / / _ \ '__|
                | |__| | (_| | | | | | |  __/ | |__| |\ V /  __/ |   
                 \_____|\__,_|_| |_| |_|\___|  \____/  \_/ \___|_|   
                )" << endl;
        cout << "Score: " << score << endl;
        health--;
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{   
    float speed = 4.f * deltaTime;
    
    // move player up
    if (window->KeyHold(GLFW_KEY_W)) {
        if (collision::PlayerMoveUp(player, map, obstacles, speed)) {
            logicSpace.y += speed;
            player.translateY += speed;
        }
    }
    // move player down
    else if (window->KeyHold(GLFW_KEY_S)) {
        if (collision::PlayerMoveDown(player, map, obstacles, speed)) {
            logicSpace.y -= speed;
            player.translateY -= speed;
        }
    }

    // move player left
    if (window->KeyHold(GLFW_KEY_A)) {
        if (collision::PlayerMoveLeft(player, map, obstacles, speed)) {
            logicSpace.x -= speed;
            player.translateX -= speed;
        }
    }
    // move player right
    else if (window->KeyHold(GLFW_KEY_D)) {
        if (collision::PlayerMoveRight(player, map, obstacles, speed)) {
            logicSpace.x += speed;
            player.translateX += speed;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    
    // rotate player to follow mouse
    player.angle = atan2((mouseX - player.x * sx), (mouseY - player.y * sy));
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
   if (button == GLFW_MOUSE_BUTTON_RIGHT) {
       // if 0.3 seconds have passed fire a new bullet
       if (secSinceLastFired >= fireRate) {
           float sx = viewSpace.width / logicSpace.width;
           float sy = viewSpace.height / logicSpace.height;

           Bullet new_bullet;

           // radius of circle "hit box"
           new_bullet.img_r = new_bullet.len * sqrt(2) / 2;

           // direction to reach mouse position
           new_bullet.direction = glm::normalize(glm::vec3(mouseX - player.x * sx, 
                                                           player.y * sy - mouseY, 0));
           // bullet initial coordinates
           new_bullet.x = player.x + player.translateX;
           new_bullet.y = player.y + player.translateY;
           // roatate bullet to face target
           new_bullet.angle = player.angle;

           bullets.push_back(new_bullet);

           secSinceLastFired = 0;
       }
   }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
