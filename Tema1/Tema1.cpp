#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/objects2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    srand(time(NULL));
    count_bullets = 0;
    count_enemies = 0;
    angularstep = 0;
    shoot = false;
    cooldown = 0;
    spawn_enemy = 0;
    health = 5;
    score = 0;
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.width = 32;   // logic width
    logicSpace.height = 16;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;
    width = 0.6f;

    // declarare obstacole
    pair<float, float> x = make_pair(23, length * 1.3f);
    pair<float, float> y = make_pair(3.5f, width * 1.1f);
    obstacles.push_back(make_pair(x, y));
    x = make_pair(21, length * 3.4f);
    y = make_pair(10.5f, width * 3.3f);
    obstacles.push_back(make_pair(x, y));
    x = make_pair(14, length * 0.7f);
    y = make_pair(12.5, width * 1.83f);
    obstacles.push_back(make_pair(x, y));
    x = make_pair(5, length * 4.25f);
    y = make_pair(12.5f, width * 1.1f);
    obstacles.push_back(make_pair(x, y));
    x = make_pair(11, length * 0.9f);
    y = make_pair(3, width * 0.85f);
    obstacles.push_back(make_pair(x, y));
    x = make_pair(7, length * 1.2f);
    y = make_pair(5, width * 1.1f);
    obstacles.push_back(make_pair(x, y));
    x = make_pair(10, length * 1.2f);
    y = make_pair(7.5, width * 3.1f);
    obstacles.push_back(make_pair(x, y));
    x = make_pair(1, length);
    y = make_pair(4, width);
    obstacles.push_back(make_pair(x, y));
    count_obstacles = 8;

    // sa nu se spawneze caracterul intr-un obstacol
    bool colide = true;
    while (colide != false) {
        character_tx = rand() % 32;
        character_ty = rand() % 16;
        logicSpace.x = character_tx - 3.5;
        logicSpace.y = character_ty - 3.5;
        for (int i = 0; i < count_obstacles; i++)
            if (ColisionCharacter(glm::vec2(character_tx + length / 2, character_ty + length / 2), obstacles[i])) {
                colide = true;
                break;
            }
            else 
                colide = false;
    }

    Mesh* map = objects2D::CreateRectangle("map", glm::vec3(0), logicSpace.width, logicSpace.height, glm::vec3(0.4f), true);
    AddMeshToList(map);

    Mesh* character = objects2D::CreateCircle("character", glm::vec3(0.5f, 0.5f, 0), length / 2, glm::vec3(0, 0, 1), true);
    AddMeshToList(character);

    Mesh* eye = objects2D::CreateCircle("eye", glm::vec3(length / 8, length / 8, 0), length / 8, glm::vec3(1, 1, 0), true);
    AddMeshToList(eye);

    Mesh* bullet = objects2D::CreateSquare("bullet", corner, length / 4, glm::vec3(0.5f, 0, 0.5f), true);
    AddMeshToList(bullet);

    Mesh* obstacle = objects2D::CreateRectangle("obstacle", corner, length, width, glm::vec3(0, 1, 0), true);
    AddMeshToList(obstacle);

    Mesh* enemy = objects2D::CreateSquare("enemy", corner, length / 2, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemy);

    Mesh* current_health = objects2D::CreateRectangle("health", corner, 5.0f / 3, width, glm::vec3(1, 0, 0), false);
    AddMeshToList(current_health);
}


// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / 8;
    sy = viewSpace.height / 8;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / 8;
    sy = viewSpace.height / 8;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * 8) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * 8) / 2;

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
    cooldown -= deltaTimeSeconds;
    spawn_enemy -= deltaTimeSeconds;

    if (health == 0) {
        DrawScene(visMatrix);
        return;
    }
    // misca gloantele
    vector<int> bullets_to_delete;
    for (int i = 0; i < count_bullets; i++) {
        bool colision = false;
        if (bullets_pos[i].first > 0 && bullets_pos[i].first < 32 - length / 4 && bullets_pos[i].second > 0 
                        && bullets_pos[i].second < 16 - length / 5 && range[i] > 0) {
            bullets_pos[i].first -= 4 * deltaTimeSeconds * sin(bullets_angularstep[i]);
            bullets_pos[i].second += 4 * deltaTimeSeconds * cos(bullets_angularstep[i]);
            range[i] = range[i] - abs(4 * deltaTimeSeconds * sin(bullets_angularstep[i])) - abs(4 * deltaTimeSeconds * cos(bullets_angularstep[i]));
        }
        else {
            bullets_to_delete.push_back(i);
            colision = true;
        }

        if (colision)
            continue;

        pair<float, float> x = make_pair(bullets_pos[i].first, length / 4);
        pair<float, float> y = make_pair(bullets_pos[i].second, length / 4);
        pair<pair<float, float>, pair<float, float>> bullet = make_pair(x, y);
        for (int j = 0; j < count_obstacles; j++) {
            if (ColisionBullets(bullet, obstacles[j])) {
                bullets_to_delete.push_back(i);
                colision = true;
                break;
            }
        }
        if (colision)
            continue;

        for(int j = 0; j < count_enemies; j++)
            if (ColisionBullets(bullet, make_pair(make_pair(enemies_pos[j].first, length / 2), make_pair(enemies_pos[j].second, length / 2)))) {
                bullets_to_delete.push_back(i);
                enemies_pos.erase(enemies_pos.begin() + j);
                enemies_speed.erase(enemies_speed.begin() + j);
                count_enemies--;
                score++;
                cout << "Score: " << score << "\n";
                colision = true;
                break;
            }
    }
    for (int i : bullets_to_delete) {
        bullets_pos.erase(bullets_pos.begin() + i);
        bullets_angularstep.erase(bullets_angularstep.begin() + i);
        range.erase(range.begin() + i);
        count_bullets--;
    }
    //verifica daca s-a tras
    if (shoot && cooldown < 0)
    {
        pair<float, float> aux = make_pair(character_tx + length * 3 / 8 - (length / 2 * sin(angularstep)), character_ty + length * 3 / 8 + (length / 2 * cos(angularstep)));
        bullets_pos.push_back(aux);
        bullets_angularstep.push_back(angularstep);
        range.push_back(4);
        count_bullets++;
        shoot = false;
        cooldown = 0.5f;
    }
    else if (shoot)
        shoot = false;
    // misca inamicii spre caracter
    vector<int> enemies_to_delete;
    for (int i = 0; i < count_enemies; i++) {
        float angularstep_enemy = atan2(enemies_pos[i].first - character_tx, enemies_pos[i].second - character_ty);
        enemies_pos[i].first -= enemies_speed[i] * deltaTimeSeconds * sin(angularstep_enemy);
        enemies_pos[i].second -= enemies_speed[i] * deltaTimeSeconds * cos(angularstep_enemy);

        if (ColisionCharacter(glm::vec2(character_tx + length / 2, character_ty + length / 2), 
            make_pair(make_pair(enemies_pos[i].first, length / 2), make_pair(enemies_pos[i].second, length / 2)))) {
            enemies_to_delete.push_back(i);
            health -= 1;
        }
    }
    for (int i : enemies_to_delete) {
        enemies_pos.erase(enemies_pos.begin() + i);
        enemies_speed.erase(enemies_speed.begin() + i);
        count_enemies--;
    }

    // creeaza inamici 
    while (spawn_enemy < 0) {
        float x, y;
        x = rand() % 32;
        y = rand() % 16;
        
        if (!ColisionCharacter(glm::vec2(character_tx + length * 2, character_ty + length * 2),
            make_pair(make_pair(x, length / 2), make_pair(y, length / 2)))) {
            enemies_pos.push_back(make_pair(rand() % 32, rand() % 16));
            enemies_speed.push_back(rand() % 4 + 1);
            spawn_enemy = 3;
            count_enemies++;
        }
    }

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix);
}



bool Tema1::ColisionBullets(pair<pair<float, float>, pair<float, float>> one, pair<pair<float, float>, pair<float, float>> two)
{
    pair<float, float>x1 = one.first;
    pair<float, float>y1 = one.second;
    pair<float, float>x2 = two.first;
    pair<float, float>y2 = two.second;

    bool collisionX = x1.first + x1.second >= x2.first && x2.first + x2.second >= x1.first;

    bool collisionY = y1.first + y1.second >= y2.first && y2.first + y2.second >= y1.first;
    return collisionX && collisionY;
}

bool Tema1::ColisionCharacter(glm::vec2 center, pair<pair<float, float>, pair<float, float>> two) {
    pair<float, float> xTwo = two.first;
    pair<float, float> yTwo = two.second;
    glm::vec2 aabb_half_extents(xTwo.second / 2.0f, yTwo.second / 2.0f);
    glm::vec2 aabb_center(
        xTwo.first + aabb_half_extents.x,
        yTwo.first + aabb_half_extents.y
    );
    
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    
    glm::vec2 closest = aabb_center + clamped;
    
    difference = closest - center;
    return glm::length(difference) < length / 2;
}

void Tema1::DrawScene(glm::mat3 visMatrix)
{
    // afisarea barii de viata
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(character_tx + 3.8f, character_ty + 2.4f);
    RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);
    Mesh* missing_health = objects2D::CreateRectangle("missing", glm::vec3(0.001, 0.001, 0), health / 3, width, glm::vec3(1, 0, 0), true);
    RenderMesh2D(missing_health, shaders["VertexColor"], modelMatrix);
    
    // afisarea ochilor
    modelMatrix = visMatrix * transform2D::Translate(character_tx + length / 8 , character_ty + length * 7 / 8);
    modelMatrix *= transform2D::Translate(length * 3 / 8, -length * 3 / 8) * transform2D::Rotate(angularstep) *
        transform2D::Translate(-length * 3 / 8, length * 3 / 8);
    RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(character_tx + length * 5 / 8, character_ty + length * 7 / 8);
    modelMatrix *= transform2D::Translate(-length / 8, -length * 3 / 8) * transform2D::Rotate(angularstep) *
        transform2D::Translate(length / 8, length * 3 / 8);
    RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

    // afisarea caracterului
    modelMatrix = visMatrix * transform2D::Translate(character_tx, character_ty);
    modelMatrix *= transform2D::Translate(length / 2, length / 2) * transform2D::Rotate(angularstep)
        * transform2D::Translate(-length / 2, -length / 2);
    RenderMesh2D(meshes["character"], shaders["VertexColor"], modelMatrix);

    // afisarea gloantelor
    for (int i = 0; i < count_bullets; i++)
    {
        modelMatrix = visMatrix * transform2D::Translate(bullets_pos[i].first, bullets_pos[i].second) * transform2D::Translate(length / 8, length / 8) 
            * transform2D::Rotate(bullets_angularstep[i]) * transform2D::Translate(-length / 8, -length / 8);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }

    //afisarea inamicilor
    for (int i = 0; i < count_enemies; i++)
    {
        modelMatrix = visMatrix * transform2D::Translate(enemies_pos[i].first, enemies_pos[i].second);
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    //Afisarea pe harta a obstacolelor
    for (int i = 0; i < count_obstacles; i++) {
        pair<float, float> x = obstacles[i].first;
        pair<float, float> y = obstacles[i].second;
        modelMatrix = visMatrix * transform2D::Translate(x.first, y.first) * transform2D::Scale(x.second / length, y.second / width);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    }

    // draw map
    modelMatrix = visMatrix;
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
}



void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    bool colide = false;
    if (health == 0)
        return;

    if (window->KeyHold(GLFW_KEY_A)) {
        for (int i = 0; i < count_obstacles; i++) 
            if (ColisionCharacter(glm::vec2(character_tx - deltaTime + length / 2, character_ty + length / 2), obstacles[i]))
                colide = true;

        if (character_tx > 0 && !colide) {
            character_tx -= deltaTime;
            logicSpace.x -= deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        for (int i = 0; i < count_obstacles; i++) 
            if (ColisionCharacter(glm::vec2(character_tx + deltaTime + length / 2, character_ty + length / 2), obstacles[i]))
                colide = true;

        if (character_tx < 31 && !colide) {
            logicSpace.x += deltaTime;
            character_tx += deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        for (int i = 0; i < count_obstacles; i++) 
            if (ColisionCharacter(glm::vec2(character_tx + length / 2, character_ty + deltaTime + length / 2), obstacles[i]))
                colide = true;

        if (character_ty < 15 && !colide) {
            logicSpace.y += deltaTime;
            character_ty += deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        for (int i = 0; i < count_obstacles; i++) 
            if (ColisionCharacter(glm::vec2(character_tx + length / 2, character_ty - deltaTime + length / 2), obstacles[i]))
                colide = true;

        if (character_ty > 0 && !colide) {
            character_ty -= deltaTime;
            logicSpace.y -= deltaTime;
        }
    }
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (health == 0)
        return;

    glm::vec3 coord = glm::vec3(character_tx + length / 2 - logicSpace.x, character_ty + length / 2 - logicSpace.y, 1);
    coord = coord * VisualizationTransf2D(logicSpace, viewSpace);
   
    if (coord[0] - mouseX == 0)
        angularstep = 0;
    else
        angularstep = atan2(coord[0] - mouseX, coord[1] - mouseY);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        shoot = true;
}