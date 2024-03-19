#pragma once

#include "components/simple_scene.h"
#include "TD/Shapes.h"
#include "TD/Extras.h"


namespace m1
{
    class TD : public gfxc::SimpleScene
    {
    public:
        TD();
        ~TD();

        void Init() override;
        void AddMesh(Mesh* mesh) {
            if (mesh->GetMeshID()) {
                meshes[mesh->GetMeshID()] = mesh;
            }
        };

        const glm::vec3 RED = glm::vec3(1, 0, 0);
        const glm::vec3 GREEN = glm::vec3(0, 0.8f, 0);
        const glm::vec3 OFF_GREEN = glm::vec3(0, 0.6f, 0.2f);
        const glm::vec3 PINK = glm::vec3(1, 0.2f, 0.6f);
        const glm::vec3 ORANGE = glm::vec3(1, 0.4f, 0);
        const glm::vec3 BLUE = glm::vec3(0, 0, 1);
        const glm::vec3 CYAN = glm::vec3(0, 1, 1);
        const glm::vec3 PURPLE = glm::vec3(0.4f, 0, 0.4f);
        const glm::vec3 YELLOW = glm::vec3(1, 1, 0);
        const glm::vec3 WHITE = glm::vec3(1, 1, 1);
        const glm::vec3 DARK_GRAY = glm::vec3(0.1f, 0.1f, 0.1f);
        const glm::vec3 GRAY = glm::vec3(0.5f, 0.5f, 0.5f);

        const std::vector<glm::vec3> ENEMY_COLORS = {
            ORANGE,
            BLUE,
            CYAN,
            PURPLE
        };

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void createAllMeshes();
        void createGrid();
        void createShop();

        void drawLifeCounter();
        void drawTowerInHand();
        void drawPointsStars(float deltaTimeSeconds);
        void drawEnemies(float deltaTimeSeconds);
        void drawExplosions(float deltaTimeSeconds);
        void drawTowers();
        void drawPlayfield();
        void drawShop();


        void enemiesSpawnLogic();
        void starPointsSpawnLogic();
        void projectilesSpawnLogic(float deltaTimeSeconds);

        void updateAndDrawProjectiles(float deltaTimeSeconds);

        void enemiesDeathAnimation(float deltaTimeSeconds);
        void towersDeathAnimation(float deltaTimeSeconds);

    protected:
        std::unordered_map<std::string, Mesh*> meshes;

        glm::mat3 modelMatrix;

        shapes::Rectangle basePlate;
        std::vector<std::vector<Tile>> grid;
        std::vector<Tile> shop;

        float totalTime;
        float lastStarTime;
        float lastEnemyTime;
        float starInterval;
        float enemyInterval;
        int points;
        int lives;
        int waveNumber;

        int shopIndex;
        bool inHand;
        shapes::Tower towerInHand;

        std::vector<Star> starPointsVector;
        std::vector<Tower> towersVector;
        std::vector<Enemy> enemiesVector;
        std::vector<Star> starProjectilesVector;
        std::vector<Explosion> explosionsVector;

        std::vector<Enemy> enemiesToDie;
        std::vector<Tower> towersToDie;

        std::vector<std::pair<float, int>> waveManagement = {
            std::make_pair(10.0f, 0),
            std::make_pair(25.0f, 2),
            std::make_pair(40.0f, 3),
            std::make_pair(65.0f, 5),
            std::make_pair(85.0f, 8),
            std::make_pair(105.0f, 12),
            std::make_pair(135.0f, 20),

        };

        float mouseXRatio;
        float mouseYRatio;
        int height;
    };
    
}
