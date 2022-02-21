#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "lab_m1/tema1/entities.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
         };

        struct Map
        {
            float width;
            float height;
        };

     public:
        Tema1();
        ~Tema1();

        void Init() override;
        void DrawGameOver();

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void InitializeObjects();
        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawCharacter(glm::mat3 visMatrix);
        void DrawObstacles(glm::mat3 visMatrix);
        void DrawEnemies(glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawBullets(glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawHealthBar(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

     protected:
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        Map map;
        Player player;
        std::vector<Obstacle> obstacles;
        std::vector<Bullet> bullets;
        std::vector<Enemy> enemies;

        // player's score and health
        int score = 0;
        float health = 10;

        float scaleX, scaleY;

        // fire rate for bullets
        float fireRate = 0.5f;
        float secSinceLastFired = 0;

        // time between enemey render
        float enemyRenderRate = 3.f;
        float secSinceLastEnemyRender = 0;
      
        // colors
        glm::vec3 creme = glm::vec3(0.957, 0.643, 0.376);
        glm::vec3 black = glm::vec3(0);
        glm::vec3 grey = glm::vec3(0.412);
        glm::vec3 green = glm::vec3(0, 1, 0);
        glm::vec3 red = glm::vec3(1, 0, 0);
    };
}
