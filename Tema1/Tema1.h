#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/Character.h";

#include <iostream>
#include <vector>

using namespace std;

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

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        bool ColisionBullets(pair<pair<float, float>, pair<float, float>>, pair<pair<float, float>, pair<float, float>>);
        bool ColisionCharacter(glm::vec2 center, pair<pair<float, float>, pair<float, float>> two);
        void DrawScene(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        float length, width;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        float character_tx, character_ty, angularstep;
        bool shoot;
        vector<pair<float, float>> bullets_pos;
        vector<float> range;
        vector<float> bullets_angularstep;
        int count_bullets, count_enemies;
        float cooldown;
        float spawn_enemy;
        vector<pair<float, float>> enemies_pos;
        vector<float> enemies_speed;
        float health;
        vector<pair<pair<float, float>, pair<float, float>>> obstacles;
        int count_obstacles;
        int score;
    };
}   // namespace m1
