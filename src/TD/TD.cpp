#include "TD/TD.h"


#include <vector>
#include <iostream>

#include "TD/transform2D.h"
#include "TD/object2D.h"

using namespace std;
using namespace m1;
using namespace shapes;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


TD::TD()
{
    modelMatrix = glm::mat3(1);
    totalTime = 0;
    lastStarTime = 0;
    starInterval = 5.0f;
    lastEnemyTime = 0;
    enemyInterval = 1.0f;
    points = 4;
    lives = 3;
    inHand = false;
    mouseXRatio = 1;
    mouseYRatio = 1;
    height = 720;
    waveNumber = 0;
}


TD::~TD()
{
}


void TD::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    createAllMeshes();
    createGrid();
    createShop();
    
}


void TD::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void TD::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    int resX = resolution.x;
    int resY = resolution.y;

    totalTime += deltaTimeSeconds;
    lastStarTime += deltaTimeSeconds;
    lastEnemyTime += deltaTimeSeconds;

    if (lives == 0) {
        return;
    }
    drawLifeCounter();


    for (int i = 0; i < shop.size(); i++) {
        shop[i].decreaseCooldown(deltaTimeSeconds);
    }

    drawTowerInHand();
    
    starPointsSpawnLogic();
    drawPointsStars(deltaTimeSeconds);   
    
    projectilesSpawnLogic(deltaTimeSeconds);
    updateAndDrawProjectiles(deltaTimeSeconds);
    
    enemiesSpawnLogic();
    drawEnemies(deltaTimeSeconds);
    enemiesDeathAnimation(deltaTimeSeconds);

    drawExplosions(deltaTimeSeconds);

    drawTowers();
    towersDeathAnimation(deltaTimeSeconds);
    
    drawPlayfield();
    drawShop();

}


void TD::FrameEnd()
{
}


void TD::OnInputUpdate(float deltaTime, int mods)
{
}


void TD::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_0) {
        printf("points: %d\n", points);
    }
    if (key == GLFW_KEY_I) {
        for (int i = 0; i < towersVector.size(); i++) {
            Tower t = towersVector[i];
            printf("%d: %d %d\n", i, t.getRow(), t.getColumn());
        }
        printf("\n");
    }
    if (key == GLFW_KEY_T) {
        printf("%.2f\n", totalTime);
    }
    if (key == GLFW_KEY_P) {
        for (int i = 0; i < shop.size(); i++) {
            printf("%i %.2f\n", i, shop[i].getCooldown());
        }
        printf("\n");
    }
}


void TD::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TD::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (inHand) {
        towerInHand.setCenterX(mouseX * mouseXRatio);
        towerInHand.setCenterY((height - mouseY) * mouseYRatio);
    }
}


void TD::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2) {
        // star pickup
        auto it = starPointsVector.begin();
        while (it != starPointsVector.end()) {
            int i = distance(starPointsVector.begin(), it);
            if (starPointsVector[i].checkHitbox(mouseX * mouseXRatio, (height - mouseY) * mouseYRatio)) {
                it = starPointsVector.erase(it);
                points++;
                if (points >= 99) {
                    points = 99;
                }
            }
            else {
                it++;
            }
        }

        // pick up tower from shop
        for (int i = 0; i < shop.size(); i++) {
            Tile t = shop[i];
            if (t.getRectangle().checkHitbox(mouseX * mouseXRatio, (height - mouseY) * mouseYRatio) && t.getTower().getCost() <= points && t.getCooldown() <= 0) {
                inHand = true;
                towerInHand = t.getTower();
                shopIndex = i;
            }
        }

    } else if (button == GLFW_MOUSE_BUTTON_3) {
        auto it = towersVector.begin();
        while (it != towersVector.end()) {
            int i = distance(towersVector.begin(), it);

            Tower* t = &towersVector[i];
            if (grid[t->getRow()][t->getColumn()].getRectangle().checkHitbox(mouseX * mouseXRatio, (height - mouseY) * mouseYRatio)) {
                grid[t->getRow()][t->getColumn()].setHasTower(false);
                towersToDie.push_back(*t);
                towersVector.erase(it);

                break;
            }
            it++;
        }
    }
}


void TD::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2) {
        if (inHand) {
            bool breaker = true;
            for (int i = 0; i < grid.size() && breaker; i++) {
                for (int j = 0; j < grid[i].size() && breaker; j++) {
                    Tile t = grid[i][j];
                    if (t.getRectangle().checkHitbox(mouseX * mouseXRatio, (height - mouseY) * mouseYRatio)) {
                        if (towerInHand.getType() == 1 && !t.doesHaveGrass()) {
                            grid[i][j].setHasGrass(true);

                            points -= towerInHand.getCost();
                            inHand = false;
                            breaker = false;
                            shop[shopIndex].resetCooldown();
                        }
                        else if (t.doesHaveGrass() && towerInHand.getType() != 1 && !t.doesHaveTower()) {
                            if (towerInHand.getType() != 4) {
                                towerInHand.setCenterX(t.getRectangle().getCenterX());
                                towerInHand.setCenterY(t.getRectangle().getCenterY());
                                towerInHand.setRow(t.getRow());
                                towerInHand.setColumn(t.getColumn());

                                towersVector.push_back(towerInHand);
                                grid[i][j].setHasTower(true);
                                
                            } else {
                                Rectangle r = t.getRectangle();
                                Star bigStar = Star(2, r.getCenterX(), r.getCenterY(), 178, meshes["star"], YELLOW);
                                Star smallStar = Star(2, r.getCenterX(), r.getCenterY(), 122.75f, meshes["star"], ORANGE);

                                explosionsVector.push_back(Explosion(bigStar, smallStar, 0.5f));
                            }

                            points -= towerInHand.getCost();
                            inHand = false;
                            breaker = false;
                            shop[shopIndex].resetCooldown();
                        }
                    }
                }
            }
            if (breaker) {
                inHand = false;
            }
        }
    }
}


void TD::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void TD::OnWindowResize(int width, int height)
{
    mouseXRatio = 1280.0f / width;
    mouseYRatio = 720.0f / height;
    this->height = height;
}




void TD::createAllMeshes() {
    Mesh* square = object2D::CreateSquareMesh("square");
    AddMesh(square);

    Mesh* star = object2D::CreateStarMesh("star");
    AddMesh(star);

    Mesh* hexagon = object2D::CreateHexagonMesh("hexagon");
    AddMesh(hexagon);

    Mesh* tower = object2D::CreateTowerMesh("tower");
    AddMesh(tower);

    Mesh* emptySquare = object2D::CreateEmptySquareMesh("emptySquare");
    AddMesh(emptySquare);

    std::vector<Mesh*> digits = object2D::CreateDigitsMeshes();
    for (int i = 0; i < digits.size(); i++) {
        AddMesh(digits[i]);
    }

    std::vector<Mesh*> hearts = object2D::CreateHeartMeshes();
    for (int i = 0; i < hearts.size(); i++) {
        AddMesh(hearts[i]);
    }

    Mesh* claymore = object2D::CreateClaymoreMesh();
    AddMesh(claymore);

    Mesh* water = object2D::CreateWaterMesh();
    AddMesh(water);

    Mesh* tree = object2D::CreateTreeMesh();
    AddMesh(tree);

    Mesh* bomb = object2D::CreateBombMesh();
    AddMesh(bomb);
}

void TD::createGrid() {
    basePlate = Rectangle(80, 480, 20 + 80 / 2, 20 + 480 / 2, meshes["square"], RED);
    grid.resize(4);

    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 0 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], GREEN), 0, 0, true, 0));
    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 1 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], OFF_GREEN), 0, 1, true, 0));
    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 2 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], GREEN), 0, 2, true, 0));
    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 3 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], OFF_GREEN), 0, 3, false, 0));
    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 4 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], GREEN), 0, 4, false, 0));
    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 5 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], OFF_GREEN), 0, 5, false, 0));
    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 6 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], GREEN), 0, 6, false, 0));
    grid[0].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 7 + 120 / 2, 20 + 120 * 0 + 120 / 2, meshes["square"], OFF_GREEN), 0, 7, false, 0));

    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 0 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], OFF_GREEN), 1, 0, true, 0));
    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 1 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], GREEN), 1, 1, true, 0));
    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 2 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], OFF_GREEN), 1, 2, true, 0));
    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 3 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], GREEN), 1, 3, false, 0));
    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 4 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], OFF_GREEN), 1, 4, false, 0));
    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 5 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], GREEN), 1, 5, false, 0));
    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 6 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], OFF_GREEN), 1, 6, false, 0));
    grid[1].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 7 + 120 / 2, 20 + 120 * 1 + 120 / 2, meshes["square"], GREEN), 1, 7, false, 0));

    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 0 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], GREEN), 2, 0, true, 0));
    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 1 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], OFF_GREEN), 2, 1, true, 0));
    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 2 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], GREEN), 2, 2, true, 0));
    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 3 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], OFF_GREEN), 2, 3, false, 0));
    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 4 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], GREEN), 2, 4, false, 0));
    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 5 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], OFF_GREEN), 2, 5, false, 0));
    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 6 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], GREEN), 2, 6, false, 0));
    grid[2].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 7 + 120 / 2, 20 + 120 * 2 + 120 / 2, meshes["square"], OFF_GREEN), 2, 7, false, 0));

    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 0 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], OFF_GREEN), 3, 0, true, 0));
    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 1 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], GREEN), 3, 1, true, 0));
    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 2 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], OFF_GREEN), 3, 2, true, 0));
    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 3 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], GREEN), 3, 3, false, 0));
    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 4 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], OFF_GREEN), 3, 4, false, 0));
    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 5 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], GREEN), 3, 5, false, 0));
    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 6 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], OFF_GREEN), 3, 6, false, 0));
    grid[3].push_back(Tile(Rectangle(120, 120, 20 + 80 + 120 * 7 + 120 / 2, 20 + 120 * 3 + 120 / 2, meshes["square"], GREEN), 3, 7, false, 0));
}

void TD::createShop() {
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 1 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 5));
    shop[0].addTower(Tower(20 + 120 * 1 + 120 / 2 + 10, 720 - 20 - 120 / 2, 120, 1, meshes["tower"], ORANGE, 0));
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 2 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 5));
    shop[1].addTower(Tower(20 + 120 * 2 + 120 / 2 + 10, 720 - 20 - 120 / 2, 120, 2, meshes["tower"], BLUE, 0));
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 3 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 5));
    shop[2].addTower(Tower(20 + 120 * 3 + 120 / 2 + 10, 720 - 20 - 120 / 2, 120, 2, meshes["tower"], CYAN, 0));
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 4 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 5));
    shop[3].addTower(Tower(20 + 120 * 4 + 120 / 2 + 10, 720 - 20 - 120 / 2, 120, 3, meshes["tower"], PURPLE, 0));
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 5 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 2));
    shop[4].addTower(Tower(20 + 120 * 5 + 100 / 2 + 10, 720 - 20 - 120 / 2, 80, 1, meshes["water"], glm::vec3(0, 0, 0), 1));
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 6 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 5));
    shop[5].addTower(Tower(20 + 120 * 6 + 100 / 2 + 10, 720 - 20 - 120 / 2, 60, 2, meshes["tree"], glm::vec3(0, 0, 0), 2));
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 7 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 15));
    shop[6].addTower(Tower(20 + 120 * 7 + 100 / 2 + 10, 720 - 20 - 120 / 2, 60, 2, meshes["claymore"], glm::vec3(0, 0, 0), 3));
    shop.push_back(Tile(Rectangle(120, 120, 20 + 120 * 8 + 120 / 2, 720 - 20 - 120 / 2, meshes["emptySquare"], PINK), -1, -1, true, 40));
    shop[7].addTower(Tower(20 + 120 * 8 + 100 / 2 + 10, 720 - 20 - 120 / 2, 50, 8, meshes["bomb"], glm::vec3(0, 0, 0), 4));
}

void TD::drawLifeCounter() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1280 - 20 - 120 / 2, 720 - 20 - 120 / 2);
    modelMatrix *= transform2D::Scale(60, 60);
    switch (lives) {
        case 3: {
            RenderMesh2D(meshes["fullHeart"], modelMatrix, RED);
            break;
        }
        case 2: {
            RenderMesh2D(meshes["2LivesHeart"], modelMatrix, RED);
            break;
        }
        case 1: {
            RenderMesh2D(meshes["1LifeHeart"], modelMatrix, RED);
            break;
        }
    }
    modelMatrix *= transform2D::Scale(2, 2);
    RenderMesh2D(meshes["emptySquare"], modelMatrix, RED);
}

void TD::enemiesSpawnLogic() {
    if (!waveManagement.empty() && totalTime < waveManagement[0].first) {
        if (lastEnemyTime > enemyInterval) {
            lastEnemyTime = 0;
            int tickets = waveManagement[0].second;
            // spawning till out of tickets
            while (tickets > 0) {  
                int cost = 0;

                int row = rand() % 4;
                int degrees = rand() % 60;

                int shield = 0;
                if (waveNumber >= 4) {
                    shield = rand() % 2;
                }
                if (shield) {
                    cost += 2;
                    if (cost > tickets) {
                        continue;
                    }
                }

                int colorOuter = rand() % 4;
                switch (colorOuter) {
                case 0: {
                    cost += 1;
                    break;
                }
                case 1: {
                    cost += 2;
                    break;
                }
                case 2: {
                    cost += 2;
                    break;
                }
                case 3: {
                    cost += 3;
                    break;
                }
                }

                if (cost > tickets) {
                    continue;
                }

                int colorInner = colorOuter;
                while (colorInner == colorOuter) {
                    colorInner = rand() % 4;
                }

                Hexagon outer = Hexagon(40, 1280 - 20, 20 + row * 120 + 120 / 2, RADIANS(degrees), meshes["hexagon"], ENEMY_COLORS[colorOuter]);
                Hexagon inner = Hexagon(20, 1280 - 20, 20 + row * 120 + 120 / 2, RADIANS(degrees), meshes["hexagon"], ENEMY_COLORS[colorInner]);

                Enemy e = Enemy(outer, inner, row, 3, 50);
                if (shield) {
                    e.giveShield();
                }
                enemiesVector.push_back(e);

                waveManagement[0].second -= cost;
                break;

            }
        }
    }
    else if (!waveManagement.empty()) {
        printf("Congratulations! You made it to wave %d!\n", waveNumber);
        waveManagement.erase(waveManagement.begin());
        waveNumber++;
        if (waveManagement.size() == 1) {
            waveManagement.push_back(make_pair(waveManagement[0].first + 30, 20 + (waveNumber - 5) * 3));
        }
    }
}

void TD::drawTowerInHand() {
    if (inHand) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(towerInHand.getCenterX(), towerInHand.getCenterY());
        modelMatrix *= transform2D::Scale(towerInHand.getSize(), towerInHand.getSize());
        if (towerInHand.getType()) {
            RenderMesh2D(towerInHand.getMesh(), shaders["VertexColor"], modelMatrix);
        }
        else {
            RenderMesh2D(towerInHand.getMesh(), modelMatrix, towerInHand.getColor());
        }
    }
}

void TD::starPointsSpawnLogic() {
    if (lastStarTime > starInterval) {
        lastStarTime = 0;
        starInterval = (rand() % 21 + 50) / 10.0f;
        int noOfStars = rand() % 5;
        float starX = rand() % 1240 + 20;
        float starY = rand() % 480;
        Star newStar = Star(0, starX, starY, 35, meshes["star"], YELLOW);
        starPointsVector.push_back(newStar);
        if (noOfStars == 4) {
            starX = rand() % 1240 + 20;
            starY = rand() % 480;
            newStar = Star(0, starX, starY, 35, meshes["star"], YELLOW);
            starPointsVector.push_back(newStar);
        }
    }
}

void TD::drawPointsStars(float deltaTimeSeconds) {
    auto it = starPointsVector.begin();
    while (it != starPointsVector.end()) {
        int i = distance(starPointsVector.begin(), it);
        starPointsVector[i].updateTimeAlive(deltaTimeSeconds);
        if (starPointsVector[i].getTimeAlive() < 0) {
            it = starPointsVector.erase(it);
        }
        else {
            modelMatrix = glm::mat3(1);
            Star s = starPointsVector[i];
            modelMatrix *= transform2D::Translate(s.getCenterX(), s.getCenterY());
            modelMatrix *= transform2D::Scale(s.getSize(), s.getSize());
            RenderMesh2D(s.getMesh(), modelMatrix, s.getColor());

            it++;
        }
    }
}

void TD::projectilesSpawnLogic(float deltaTimeSeconds) {
    for (int i = 0; i < towersVector.size(); i++) {
        Tower* t = &towersVector[i];
        if (t->getType() == 0) {
            t->updateCooldown(deltaTimeSeconds);

            for (int j = 0; j < enemiesVector.size(); j++) {
                Enemy e = enemiesVector[j];
                if (t->getRow() == e.getRow() && (t->getColor() == e.getOuterHexagon().getColor() || e.hasShield()) && t->canShoot() && t->getCenterX() < e.getOuterHexagon().getCenterX()) {


                    Star s = Star(1, t->getCenterX() + 20, t->getCenterY(), 25, meshes["star"], t->getColor());
                    s.setRow(t->getRow());
                    starProjectilesVector.push_back(s);

                    t->setCooldown(2);
                }
            }
        }
        else if (t->getType() == 2) {
            t->updateCooldown(deltaTimeSeconds);

            if (t->canShoot()) {
                Star s = Star(0, t->getCenterX(), t->getCenterY() - 10, 35, meshes["star"], YELLOW);
                starPointsVector.push_back(s);
                t->setCooldown(10);
            }
        }
    }
}

void TD::updateAndDrawProjectiles(float deltaTimeSeconds) {
    auto it = starProjectilesVector.begin();
    while (it != starProjectilesVector.end()) {
        int i = distance(starProjectilesVector.begin(), it);
        Star* s = &starProjectilesVector[i];

        bool breaker = true;
        for (int j = 0; j < enemiesVector.size() && breaker; j++) {
            Enemy* e = &enemiesVector[j];
            float distance = e->getOuterHexagon().getCenterX() - s->getCenterX();
            if (s->getRow() == e->getRow() && s->getCenterX() < e->getOuterHexagon().getCenterX() && distance <= e->getOuterHexagon().getSize() + s->getSize()) {
                if (s->getColor() == e->getOuterHexagon().getColor() || e->hasShield()) {
                    e->loseHp();
                    if (e->getHp() == 0) {
                        auto iter = enemiesVector.begin();
                        Enemy temp = enemiesVector[j];
                        enemiesToDie.push_back(temp);
                        enemiesVector.erase(iter + j);
                    }
                }
                breaker = false;
            }
        }

        if (breaker) {
            float distance = 100 * deltaTimeSeconds;

            s->updateRightCenterX(distance);
            s->updateSpin(deltaTimeSeconds);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(s->getCenterX(), s->getCenterY());
            modelMatrix *= transform2D::Scale(s->getSize(), s->getSize());
            modelMatrix *= transform2D::Rotate(s->getSpin());
            RenderMesh2D(s->getMesh(), modelMatrix, s->getColor());

            it++;
        }
        else {
            it = starProjectilesVector.erase(it);
        }
    }
}

void TD::enemiesDeathAnimation(float deltaTimeSeconds) {
    auto it = enemiesToDie.begin();
    while (it != enemiesToDie.end()) {
        int i = distance(enemiesToDie.begin(), it);

        Enemy* e = &enemiesToDie[i];
        e->shrink(deltaTimeSeconds * 200);

        if (e->getOuterHexagon().getSize() <= 0) {
            it = enemiesToDie.erase(it);
        }
        else {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(e->getInnerHexagon().getCenterX(), e->getInnerHexagon().getCenterY());
            modelMatrix *= transform2D::Scale(e->getInnerHexagon().getSize(), e->getInnerHexagon().getSize());
            modelMatrix *= transform2D::Rotate(e->getInnerHexagon().getTilt());
            RenderMesh2D(e->getInnerHexagon().getMesh(), modelMatrix, e->getInnerHexagon().getColor());
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(e->getOuterHexagon().getCenterX(), e->getOuterHexagon().getCenterY());
            modelMatrix *= transform2D::Scale(e->getOuterHexagon().getSize(), e->getOuterHexagon().getSize());
            modelMatrix *= transform2D::Rotate(e->getOuterHexagon().getTilt());
            RenderMesh2D(e->getOuterHexagon().getMesh(), modelMatrix, e->getOuterHexagon().getColor());

            it++;
        }
    }
}

void TD::drawEnemies(float deltaTimeSeconds) {
    auto it = enemiesVector.begin();
    while (it != enemiesVector.end()) {
        int i = distance(enemiesVector.begin(), it);

        enemiesVector[i].move(enemiesVector[i].getSpeed() * deltaTimeSeconds);
        Enemy e = enemiesVector[i];

        bool breaker = true;
        for (int j = 0; j < explosionsVector.size() && breaker; j++) {
            Explosion expl = explosionsVector[j];
            float distanceX = abs(e.getOuterHexagon().getCenterX() - expl.getBigStar().getCenterX());
            float distanceY = abs(e.getOuterHexagon().getCenterY() - expl.getBigStar().getCenterY());
            if (distanceX < expl.getBigStar().getSize() && distanceY < expl.getBigStar().getSize()) {
                it = enemiesVector.erase(it);
                breaker = false;
            }
        }
        if (!breaker) {
            continue;
        }

        if (e.getOuterHexagon().getCenterX() - e.getOuterHexagon().getSize() < basePlate.getCenterX()) {
            lives--;
            if (lives == 0) {
                printf("Game Over!\n");
            }
            enemiesToDie.push_back(e);
            it = enemiesVector.erase(it);
        }
        else {
            if (e.hasShield()) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(e.getOuterHexagon().getCenterX() - e.getOuterHexagon().getSize(), e.getOuterHexagon().getCenterY());
                modelMatrix *= transform2D::Scale(15, e.getOuterHexagon().getSize() * 2);
                RenderMesh2D(meshes["square"], modelMatrix, GRAY);
            }
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(e.getInnerHexagon().getCenterX(), e.getInnerHexagon().getCenterY());
            modelMatrix *= transform2D::Scale(e.getInnerHexagon().getSize(), e.getInnerHexagon().getSize());
            modelMatrix *= transform2D::Rotate(e.getInnerHexagon().getTilt());
            RenderMesh2D(e.getInnerHexagon().getMesh(), modelMatrix, e.getInnerHexagon().getColor());
            modelMatrix *= transform2D::Scale(e.getOuterHexagon().getSize() / e.getInnerHexagon().getSize(), e.getOuterHexagon().getSize() / e.getInnerHexagon().getSize());
            RenderMesh2D(e.getOuterHexagon().getMesh(), modelMatrix, e.getOuterHexagon().getColor());

            it++;
        }
    }
}

void TD::drawExplosions(float deltaTimeSeconds) {
    // explosion render
    auto it7 = explosionsVector.begin();
    while (it7 != explosionsVector.end()) {
        int i = distance(explosionsVector.begin(), it7);

        Explosion* e = &explosionsVector[i];
        e->updateTimeLeft(deltaTimeSeconds);
        if (e->getTimeLeft() <= 0) {
            it7 = explosionsVector.erase(it7);
        }
        else {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(e->getSmallStar().getCenterX(), e->getSmallStar().getCenterY());
            modelMatrix *= transform2D::Scale(e->getSmallStar().getSize(), e->getSmallStar().getSize());
            RenderMesh2D(e->getSmallStar().getMesh(), modelMatrix, e->getSmallStar().getColor());
            modelMatrix *= transform2D::Scale(e->getBigStar().getSize() / e->getSmallStar().getSize(), e->getBigStar().getSize() / e->getSmallStar().getSize());
            modelMatrix *= transform2D::Rotate(RADIANS(36));
            RenderMesh2D(e->getBigStar().getMesh(), modelMatrix, e->getBigStar().getColor());

            it7++;
        }
    }
}

void TD::drawTowers() {
    // tower render
    auto it3 = towersVector.begin();
    while (it3 != towersVector.end()) {
        int i = distance(towersVector.begin(), it3);
        Tower t = towersVector[i];

        bool breaker = true;
        for (int j = 0; j < enemiesVector.size() && breaker; j++) {
            Enemy e = enemiesVector[j];
            float distance = e.getOuterHexagon().getCenterX() - t.getCenterX();

            if (t.getRow() == e.getRow() && t.getCenterX() < e.getOuterHexagon().getCenterX() && distance <= e.getOuterHexagon().getSize() / 2 + t.getSize() / 2) {
                breaker = false;
                grid[t.getRow()][t.getColumn()].setHasTower(false);

                if (t.getType() == 3) {
                    enemiesVector.erase(enemiesVector.begin() + j);

                    Star bigStar = Star(2, t.getCenterX() + t.getSize() / 2, t.getCenterY(), 58, meshes["star"], YELLOW);
                    Star smallStar = Star(2, t.getCenterX() + t.getSize() / 2, t.getCenterY(), 40, meshes["star"], ORANGE);

                    explosionsVector.push_back(Explosion(bigStar, smallStar, 0.5f));
                }
            }
        }

        if (breaker) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(t.getCenterX(), t.getCenterY());
            modelMatrix *= transform2D::Scale(t.getSize(), t.getSize());
            if (t.getType()) {
                RenderMesh2D(t.getMesh(), shaders["VertexColor"], modelMatrix);
            }
            else {
                RenderMesh2D(t.getMesh(), modelMatrix, t.getColor());
            }
            it3++;
        }
        else {
            if (t.getType() != 3) {
                towersToDie.push_back(t);
            }
            it3 = towersVector.erase(it3);
        }

    }
}

void TD::towersDeathAnimation(float deltaTimeSeconds) {
    // tower death animation
    auto it6 = towersToDie.begin();
    while (it6 != towersToDie.end()) {
        int i = distance(towersToDie.begin(), it6);

        Tower* t = &towersToDie[i];
        t->decreaseSize(deltaTimeSeconds * 200);

        if (t->getSize() <= 0) {
            it6 = towersToDie.erase(it6);
        }
        else {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(t->getCenterX(), t->getCenterY());
            modelMatrix *= transform2D::Scale(t->getSize(), t->getSize());
            if (t->getType()) {
                RenderMesh2D(t->getMesh(), shaders["VertexColor"], modelMatrix);
            }
            else {
                RenderMesh2D(t->getMesh(), modelMatrix, t->getColor());
            }
            it6++;
        }
    }
}

void TD::drawPlayfield() {
    // base plate render
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(basePlate.getCenterX(), basePlate.getCenterY());
    modelMatrix *= transform2D::Scale(basePlate.getWidth(), basePlate.getHeight());
    RenderMesh2D(basePlate.getMesh(), modelMatrix, basePlate.getColor());

    // tiles render
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(grid[i][j].getRectangle().getCenterX(), grid[i][j].getRectangle().getCenterY());
            modelMatrix *= transform2D::Scale(grid[i][j].getRectangle().getWidth(), grid[i][j].getRectangle().getHeight());
            if (grid[i][j].doesHaveGrass()) {
                RenderMesh2D(grid[i][j].getRectangle().getMesh(), modelMatrix, grid[i][j].getRectangle().getColor());
            }
            else {
                RenderMesh2D(meshes["emptySquare"], modelMatrix, grid[i][j].getRectangle().getColor());
            }


        }
    }
}

void TD::drawShop() {
    // star counter
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20 + 120 / 2, 720 - 20 - 120 / 2);
    modelMatrix *= transform2D::Scale(60, 60);
    RenderMesh2D(meshes["star"], modelMatrix, YELLOW);

    if (points <= 9) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(20 + 120 / 2, 720 - 20 - 120 - 40 / 2);
        modelMatrix *= transform2D::Scale(15, 15);
        RenderMesh2D(meshes[to_string(points)], modelMatrix, WHITE);
        RenderMesh2D(meshes["8"], modelMatrix, DARK_GRAY);
    }
    else {
        int tens = points / 10;
        int digits = points % 10;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(20 + 120 / 2 - 12, 720 - 20 - 120 - 40 / 2);
        modelMatrix *= transform2D::Scale(15, 15);
        RenderMesh2D(meshes[to_string(tens)], modelMatrix, WHITE);
        RenderMesh2D(meshes["8"], modelMatrix, DARK_GRAY);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(20 + 120 / 2 + 12, 720 - 20 - 120 - 40 / 2);
        modelMatrix *= transform2D::Scale(15, 15);
        RenderMesh2D(meshes[to_string(digits)], modelMatrix, WHITE);
        RenderMesh2D(meshes["8"], modelMatrix, DARK_GRAY);
    }


    // shop render
    for (int i = 0; i < shop.size(); i++) {
        Rectangle r = shop[i].getRectangle();
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(r.getCenterX(), r.getCenterY());
        modelMatrix *= transform2D::Scale(r.getWidth(), r.getHeight());
        RenderMesh2D(r.getMesh(), modelMatrix, r.getColor());

        Tower t = shop[i].getTower();
        if (shop[i].getCooldown() <= 0) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(t.getCenterX(), t.getCenterY());
            modelMatrix *= transform2D::Scale(t.getSize(), t.getSize());
            if (t.getType()) {
                RenderMesh2D(t.getMesh(), shaders["VertexColor"], modelMatrix);
            } else {
                RenderMesh2D(t.getMesh(), modelMatrix, t.getColor());
            }
        }

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(r.getCenterX(), r.getCenterY() - r.getHeight() / 2 - 40.0f / 2);
        modelMatrix *= transform2D::Scale(15, 15);
        RenderMesh2D(meshes[to_string(t.getCost())], modelMatrix, WHITE);
        RenderMesh2D(meshes["8"], modelMatrix, DARK_GRAY);
    }
}