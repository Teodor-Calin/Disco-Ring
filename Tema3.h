#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


#define FLOOR_TRANSITION_TIME  3
#define DANCER_TRANSITION_TIME 5

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
     public:
        Tema3();
        ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderFloorMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        void RenderWallMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void RenderCeilingMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void RenderDancerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void RenderConeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        void RenderGlobeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1);

        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        float materialKe;

        glm::vec3 floor_light_pos[9];
        glm::vec3 floor_light_colors[9];

        glm::vec3 dancer_pos[5];
        glm::vec3 dancer_next_pos[5];
        float dancer_transition_times[5];
        float dancer_speed[5];

        float floor_timer = FLOOR_TRANSITION_TIME;
        glm::vec3 floor_colors[8][8];
        glm::vec3 next_floor_colors[8][8];

        float spotlights_angle = 10;
        glm::vec3 spotlights_pos[4];
        glm::vec3 spotlights_col[4];

        Texture2D* random_texture;

        glm::vec3 globe_pos = glm::vec3(4, 6, 4);

        int light_mode = 0;

    };
}   // namespace m1
