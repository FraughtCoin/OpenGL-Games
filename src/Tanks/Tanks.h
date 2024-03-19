#pragma once

#include "components/simple_scene.h"
#include "Tanks/Camera.h"
#include "Tanks/Tank.h"
#include "Tanks/Projectile.h"


namespace m1
{
    class Tanks : public gfxc::SimpleScene
    {
    public:
        Tanks();
        ~Tanks();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void MyRenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int health);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


        Tank initTank(glm::vec3 xzCenter);
        void renderTank(Tank t);

        void spawnEnemies(float deltaTime);
        void enemiesActionLogic(float deltaTime);
        void renderEnemies();

        void renderProjectiles(float deltaTime);
        void projectilesCollisionLogic();
        
        void tanksCollisionLogic();

        const glm::vec3 CAMO = glm::vec3(0.4f, 0.4f, 0.2f);
        const glm::vec3 FOREST = glm::vec3(0, 0.2f, 0);
        const glm::vec3 GRAY = glm::vec3(0.3f, 0.3f, 0.3f);

    protected:
        camera::Camera* camera;
        camera::Camera configuration;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        Tank playerTank;

        std::vector<Projectile> projectiles;
        std::vector<Tank> enemies;
        
        bool cameraLock = false;
        float timePassed = 0;
        float enemySpawnCooldown = 5;
        int maxEnemies = 5;

    };

        
}
