#include "Tanks/Tanks.h"
#include "Tanks/Objects3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;
using namespace objects3D;


Tanks::Tanks()
{
}


Tanks::~Tanks()
{
}


void Tanks::Init()
{
    renderCameraTarget = false;

    camera = new camera::Camera();

    Mesh* meshBox = new Mesh("box");
    meshBox->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[meshBox->GetMeshID()] = meshBox;

    Mesh* meshSphere = new Mesh("sphere");
    meshSphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[meshSphere->GetMeshID()] = meshSphere;

    Mesh* meshCube = objects3D::CreateCubeMesh();
    meshes[meshCube->GetMeshID()] = meshCube;

    Mesh* meshCylinder = objects3D::CreateCylinderMesh();
    meshes[meshCylinder->GetMeshID()] = meshCylinder;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    playerTank = initTank(glm::vec3(0));

    glm::vec3 aux = playerTank.position - glm::vec3(0, 0, 2);
    aux.y += 2;
    camera->Set(aux, glm::vec3(playerTank.position.x, playerTank.position.y + 0.5f, playerTank.position.z), glm::vec3(0, 1, 0));

    Shader* shader = new Shader("MyShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, "src", "Tanks", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, "src", "Tanks", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

}


void Tanks::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
    glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void Tanks::Update(float deltaTimeSeconds)
{
    timePassed += deltaTimeSeconds;
    if (playerTank.hp == 0) {
        return;
    }
    
    renderTank(playerTank);

    playerTank.cooldown -= deltaTimeSeconds;
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].cooldown -= deltaTimeSeconds;
    }

    renderProjectiles(deltaTimeSeconds);
    projectilesCollisionLogic();

    spawnEnemies(deltaTimeSeconds);
    enemiesActionLogic(deltaTimeSeconds);
    renderEnemies();
    
    tanksCollisionLogic();
    
}


void Tanks::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tanks::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tanks::MyRenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int hp)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();

    int colorLocation = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(colorLocation, 1, glm::value_ptr(color));

    int modelMatrixLocation = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int viewMatrixLocation = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    int projectionMatrixLocation = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int hpLocation = glGetUniformLocation(shader->program, "hp");
    glUniform1i(hpLocation, hp);


    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tanks::OnInputUpdate(float deltaTime, int mods)
{
    {
        float cameraSpeed = 2.0f;


        if (window->KeyHold(GLFW_KEY_W)) {
            playerTank.position += glm::normalize(playerTank.direction) * deltaTime * 3.f;
            camera->position += glm::normalize(playerTank.direction) * deltaTime * 3.f;
            configuration.position += glm::normalize(playerTank.direction) * deltaTime * 3.f;

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            glm::vec3 newDir = playerTank.direction;
            newDir.x = playerTank.direction.x * cos(-deltaTime * 2) - playerTank.direction.z * sin(-deltaTime * 2);
            newDir.z = playerTank.direction.x * sin(-deltaTime * 2) + playerTank.direction.z * cos(-deltaTime * 2);
            playerTank.direction = newDir;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            playerTank.position -= glm::normalize(playerTank.direction) * deltaTime * 3.f;
            camera->position -= glm::normalize(playerTank.direction) * deltaTime * 3.f;
            configuration.position -= glm::normalize(playerTank.direction) * deltaTime * 3.f;
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            glm::vec3 newDir = playerTank.direction;
            newDir.x = playerTank.direction.x * cos(deltaTime * 2) - playerTank.direction.z * sin(deltaTime * 2);
            newDir.z = playerTank.direction.x * sin(deltaTime * 2) + playerTank.direction.z * cos(deltaTime * 2);
            playerTank.direction = newDir;
        }
    }
}


void Tanks::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_T) {
        renderCameraTarget = !renderCameraTarget;
    }
}


void Tanks::OnKeyRelease(int key, int mods)
{
}


void Tanks::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float h = (float)deltaX / 1000.f;
    float v = (float)deltaY / 1000.f;

    if (!cameraLock) {
        playerTank.horizontalAngle -= h;
        playerTank.verticalAngle += v;

        if (playerTank.verticalAngle < -0.5f) {
            playerTank.verticalAngle = -0.5f;
        } else if (playerTank.verticalAngle > 0) {
            playerTank.verticalAngle = 0;
        }
    }

    camera->RotateThirdPerson_OY(-h);

    if ((camera->position.y > 0 || (camera->position.y < 0 && v > 0)) && (camera->position.y < 3 || (camera->position.y > 3 && v < 0))) {
        camera->RotateThirdPerson_OX(-v);
    }

}


void Tanks::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == 2) {
        cameraLock = true;
        configuration = camera->getConfiguration();
    }

    if (button == 1) {
        if (playerTank.cooldown <= 0) {
            glm::vec3 s = glm::vec3(sin(playerTank.horizontalAngle) * 10, sin(-playerTank.verticalAngle) * 10, cos(playerTank.horizontalAngle) * 10);
            glm::vec3 pos = glm::vec3(0, 0, 0);

            pos.y = pos.y * cos(playerTank.verticalAngle) - pos.z * sin(playerTank.verticalAngle);
            pos.z = pos.y * sin(playerTank.verticalAngle) + pos.z * cos(playerTank.verticalAngle);

            pos.x = pos.x * cos(-playerTank.horizontalAngle) - pos.z * sin(-playerTank.horizontalAngle);
            pos.z = pos.x * sin(-playerTank.horizontalAngle) + pos.z * cos(-playerTank.horizontalAngle);

            pos += playerTank.position + glm::vec3(0, 0.45f, 0);
            Projectile p = Projectile(pos, s, meshes["sphere"], 0.1f, false);

            projectiles.push_back(p);

            playerTank.cooldown = 3;
        }
    }
}


void Tanks::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == 2) {
        cameraLock = false;
        camera->loadConfiguration(configuration);
    }
}


void Tanks::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tanks::OnWindowResize(int width, int height)
{
}

Tank Tanks::initTank(glm::vec3 xzCenter) {
    Tank t = Tank(glm::vec3(0, 0, 1),
             xzCenter + glm::vec3(0, 0.25f, 0), meshes["cube"], glm::vec3(1, 0.5f, 0.6f),
             meshes["cube"], meshes["cylinder"], meshes["cube"]);

    return t;
}

void Tanks::renderTank(Tank t) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, t.position + glm::vec3(0, 0.1f, 0));
    modelMatrix = glm::rotate(modelMatrix, -atan(t.direction.z / t.direction.x), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, t.bodySize);
    MyRenderMesh(t.body, shaders["MyShader"], modelMatrix, FOREST, t.hp);
 
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, t.position + glm::vec3(0, 0.1f + 0.25f + 0.1f, 0));
    modelMatrix = glm::rotate(modelMatrix, t.horizontalAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, t.verticalAngle, glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0.4f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.8f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(1, 0, 0));
    MyRenderMesh(t.cannon, shaders["MyShader"], modelMatrix, GRAY, t.hp);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, t.position + glm::vec3(0, 0.1f + 0.25f + 0.1f, 0));
    modelMatrix = glm::rotate(modelMatrix, t.horizontalAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
    MyRenderMesh(t.turret, shaders["MyShader"], modelMatrix, CAMO, t.hp);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, t.position - glm::vec3(0, 0.2f, 0));
    modelMatrix = glm::rotate(modelMatrix, -atan(t.direction.z / t.direction.x), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0.2f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.1f, 0.1f));
    MyRenderMesh(t.tracks, shaders["MyShader"], modelMatrix, GRAY, t.hp);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, t.position - glm::vec3(0, 0.2f, 0));
    modelMatrix = glm::rotate(modelMatrix, -atan(t.direction.z / t.direction.x), glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -0.2f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.1f, 0.1f));
    MyRenderMesh(t.tracks, shaders["MyShader"], modelMatrix, GRAY, t.hp);
}

void Tanks::spawnEnemies(float deltaTime) {
    enemySpawnCooldown -= deltaTime;

    if (enemySpawnCooldown <= 0 && enemies.size() < maxEnemies) {
        float x = 0;
        float z = 0;

        while (true) {
            x = (float)rand() / (float)RAND_MAX * 40.0f - 20;
            z = (float)rand() / (float)RAND_MAX * 40.0f - 20;

            if (abs(x - playerTank.position.x) <= 2 || abs(z - playerTank.position.z) <= 2) {
                continue;
            }

            bool cont = false;
            for (int i = 0; i < enemies.size(); i++) {
                if (abs(x - enemies[i].position.x) <= 2 || abs(z - enemies[i].position.z) <= 2) {
                    cont = true;
                    break;
                }
            }
            if (cont) {
                continue;
            }
            break;
        }
        Tank t = initTank(glm::vec3(x, 2, z));
        enemies.push_back(t);

        enemySpawnCooldown = 10;
    }
}

void Tanks::renderProjectiles(float deltaTime) {
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].speed -= glm::vec3(0, 10, 0) * deltaTime;
        projectiles[i].position += projectiles[i].speed * deltaTime;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, projectiles[i].position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(projectiles[i].radius));
        MyRenderMesh(projectiles[i].mesh, shaders["MyShader"], modelMatrix, glm::vec3(1, 0, 0), -1);
    }
}

void Tanks::projectilesCollisionLogic() {
    for (int i = 0; i < projectiles.size(); i++) {
        if (projectiles[i].position.y <= 0) {
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }
        for (int j = 0; j < enemies.size(); j++) {
            if (!projectiles[i].enemy && projectiles[i].impactTank(enemies[j])) {
                projectiles.erase(projectiles.begin() + i);
                i--;
                enemies[j].hp--;
                break;
            }
            if (projectiles[i].enemy && projectiles[i].impactTank(playerTank)) {
                projectiles.erase(projectiles.begin() + i);
                i--;
                playerTank.hp--;
                break;
            }
        }
    }
}

void Tanks::enemiesActionLogic(float deltaTime) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].nextAction -= deltaTime;
        if (enemies[i].position.y > 0.25f) {
            enemies[i].position.y -= deltaTime * 1.5f;
            continue;
        }
        else {
            enemies[i].position.y = 0.25f;
        }
        if (glm::length(playerTank.position - enemies[i].position) <= 15) {
            enemies[i].action = -1;
        }
        if (enemies[i].nextAction <= 0 && enemies[i].action != -1) {
            enemies[i].action = rand() % 2;
            if (enemies[i].action == 0) {
                enemies[i].rotationDirection = rand() % 2 == 0 ? -1 : 1;
            }
            enemies[i].nextAction = (float)rand() / (float)RAND_MAX * 3;
        }
        if (enemies[i].action == 1) {
            enemies[i].moveForward(deltaTime);
        }
        else if (enemies[i].action == 0) {
            enemies[i].rotateTank(deltaTime);
        }
        else if (enemies[i].action == -1) {
            enemies[i].huntTank(deltaTime, playerTank);
        }
    }

    for (int i = 0; i < enemies.size(); i++) {
        if (glm::length(playerTank.position - enemies[i].position) <= 5) {
            if (enemies[i].alignWithEnemyAndTestFire(deltaTime, playerTank) && enemies[i].cooldown <= 0) {
                glm::vec3 s = glm::vec3(sin(enemies[i].horizontalAngle) * 10, sin(-enemies[i].verticalAngle) * 10, cos(enemies[i].horizontalAngle) * 10);
                glm::vec3 pos = glm::vec3(0, 0, 0);

                pos.y = pos.y * cos(enemies[i].verticalAngle) - pos.z * sin(enemies[i].verticalAngle);
                pos.z = pos.y * sin(enemies[i].verticalAngle) + pos.z * cos(enemies[i].verticalAngle);

                pos.x = pos.x * cos(-enemies[i].horizontalAngle) - pos.z * sin(-enemies[i].horizontalAngle);
                pos.z = pos.x * sin(-enemies[i].horizontalAngle) + pos.z * cos(-enemies[i].horizontalAngle);

                pos += enemies[i].position + glm::vec3(0, 0.45f, 0);
                Projectile p = Projectile(pos, s, meshes["sphere"], 0.1f, true);
                projectiles.push_back(p);

                enemies[i].cooldown = 3;
            }
        }
    }
}

void Tanks::renderEnemies() {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].hp == 0) {
            enemies.erase(enemies.begin() + i);
            i--;
            continue;
        }
        renderTank(enemies[i]);
    }
}

void Tanks::tanksCollisionLogic() {
    for (int i = 0; i < enemies.size(); i++) {
        if (playerTank.collisionTank(enemies[i])) {
            glm::vec2 p1 = glm::vec2(playerTank.position.x, playerTank.position.z);
            glm::vec2 p2 = glm::vec2(enemies[i].position.x, enemies[i].position.z);

            glm::vec2 r1 = glm::vec2(playerTank.bodySize.x / 2, playerTank.bodySize.z / 2);
            glm::vec2 r2 = glm::vec2(enemies[i].bodySize.x / 2, enemies[i].bodySize.z / 2);

            float p = glm::length(r1) + glm::length(r2) - glm::length(p1 - p2);
            glm::vec3 delta = p * glm::normalize(playerTank.position - enemies[i].position);
            playerTank.position += delta * 0.5f;
            enemies[i].position -= delta * 0.5f;

            camera->position += delta * 0.5f;
            configuration.position += delta * 0.5f;
        }
    }

    if (enemies.size() >= 2) {
        for (int i = 0; i < enemies.size() - 1; i++) {
            for (int j = i + 1; j < enemies.size(); j++) {
                if (enemies[i].collisionTank(enemies[j])) {
                    glm::vec2 p1 = glm::vec2(enemies[i].position.x, enemies[i].position.z);
                    glm::vec2 p2 = glm::vec2(enemies[j].position.x, enemies[j].position.z);

                    glm::vec2 r1 = glm::vec2(enemies[i].bodySize.x / 2, enemies[i].bodySize.z / 2);
                    glm::vec2 r2 = glm::vec2(enemies[j].bodySize.x / 2, enemies[j].bodySize.z / 2);

                    float p = glm::length(r1) + glm::length(r2) - glm::length(p1 - p2);
                    glm::vec3 delta = p * glm::normalize(enemies[i].position - enemies[j].position);
                    enemies[i].position += delta * 0.5f;
                    enemies[j].position -= delta * 0.5f;
                }
            }
        }
    }
}