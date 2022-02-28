#include "lab_m1/Tema3/Tema3.h"
#include "lab_m1/Tema3/Transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


Mesh* create_cone(
    const std::string& name,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0), color)
    };

    for (int i = 0; i < 100; i++) {
        vertices.push_back(VertexFormat(glm::vec3(cos(i * M_PI / 50), -1, sin(i * M_PI / 50)), color));
    }

    Mesh* cone = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 0; i < 101; i++) {
        indices.push_back(i);
    }
    indices.push_back(1);

    cone->SetDrawMode(GL_TRIANGLE_FAN);

    cone->InitFromData(vertices, indices);
    return cone;
}


void Tema3::Init()
{
    // MESHES
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* cone = create_cone("cone", glm::vec3(1, 0, 0));
        AddMeshToList(cone);
    }

    // SHADERS
    {
        Shader *shader = new Shader("WallShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "WallVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "WallFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("FloorShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FloorVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FloorFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("DancerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "DancerVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "DancerFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ConeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "ConeVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "ConeFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("GlobeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "GlobeVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "GlobeFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("CeilingShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "CeilingVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "CeilingFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        random_texture = CreateRandomTexture(16, 16);
    }

    // Light & material properties
    {
        lightPosition = glm::vec3(100, 100, 1);
        materialShininess = 100;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    // set first floor colors
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            floor_colors[i][j] = glm::vec3(((float)(rand() % 100)) / 100, ((float)(rand() % 100)) / 100, ((float)(rand() % 100)) / 100);
            next_floor_colors[i][j] = glm::vec3(((float)(rand() % 100)) / 100, ((float)(rand() % 100)) / 100, ((float)(rand() % 100)) / 100);
        }
    }

    // set first dancer positions
    for (int i = 0; i < 5; i++) {
        dancer_pos[i] = glm::vec3(rand() % 7 + 1, 1.5, rand() % 7 + 1);
        dancer_next_pos[i] = glm::vec3(rand() % 7 + 1, 1.5, rand() % 7 + 1);
        dancer_transition_times[i] = (float) (rand() % 4 + 2);
        dancer_speed[i] = distance(dancer_pos[i], dancer_next_pos[i]) / dancer_transition_times[i];
    }

    // set spotlights positions and colors
    spotlights_pos[0] = glm::vec3(2, 7, 2);
    spotlights_pos[1] = glm::vec3(2, 7, 6);
    spotlights_pos[2] = glm::vec3(6, 7, 2);
    spotlights_pos[3] = glm::vec3(6, 7, 6);
    spotlights_col[0] = glm::vec3(1, 0, 0);
    spotlights_col[1] = glm::vec3(0, 1, 0);
    spotlights_col[2] = glm::vec3(0, 0, 1);
    spotlights_col[3] = glm::vec3(0.7, 0.3, 0.5);
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    // Render ground
    {
        if (floor_timer > 0) {
            
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    floor_colors[i][j] += (next_floor_colors[i][j] - floor_colors[i][j]) * (deltaTimeSeconds / FLOOR_TRANSITION_TIME);
                }
            }
            floor_timer -= deltaTimeSeconds;
        }
        else {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    next_floor_colors[i][j] = glm::vec3(((float)(rand() % 100)) / 100, ((float)(rand() % 100)) / 100, ((float)(rand() % 100)) / 100);
                }
            }
            floor_timer = FLOOR_TRANSITION_TIME;
        }

        materialShininess = 50;
        materialKd = 1;
        materialKs = 1;
        materialKe = 1.3f;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5, 1, j + 0.5));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
                RenderFloorMesh(meshes["plane"], shaders["FloorShader"], modelMatrix, floor_colors[i][j]);
            }
        }

    }

    // Render walls
    {
        materialShininess = 30;
        materialKd = 0.7;
        materialKs = 0.5;
        materialKe = 0;

        for (int i = 0; i < 8; i++) {


            // pereti stanga
            floor_light_pos[0] = glm::vec3(i - 0.5, 0.5, 0.5);
            floor_light_pos[1] = glm::vec3(i + 0.5, 0.5, 0.5);
            floor_light_pos[2] = glm::vec3(i + 1.5, 0.5, 0.5);
            if (i == 0) {
                floor_light_colors[0] = glm::vec3(0);
            }
            else {
                floor_light_colors[0] = floor_colors[i - 1][0];
            }
            floor_light_colors[1] = floor_colors[i][0];

            if (i == 7){
                floor_light_colors[2] = glm::vec3(0);
            }
            else {
                floor_light_colors[2] = floor_colors[i + 1][0];
            }
            
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5, 4, 0));
            modelMatrix *= Transform3D::RotateOX(M_PI / 2);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f, 0.02f, 0.12f));
            RenderWallMesh(meshes["plane"], shaders["WallShader"], modelMatrix);


            // pereti dreapta
            floor_light_pos[0] = glm::vec3(i - 0.5, 0.5, 7.5);
            floor_light_pos[1] = glm::vec3(i + 0.5, 0.5, 7.5);
            floor_light_pos[2] = glm::vec3(i + 1.5, 0.5, 7.5);
            
            if (i == 0) {
                floor_light_colors[0] = glm::vec3(0);
            }
            else {
                floor_light_colors[0] = floor_colors[i - 1][7];
            }

            floor_light_colors[1] = floor_colors[i][7];

            if (i == 7) {
                floor_light_colors[2] = glm::vec3(0);
            }
            else {
                floor_light_colors[2] = floor_colors[i + 1][7];
            }

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i + 0.5, 4, 8));
            modelMatrix *= Transform3D::RotateOX(-M_PI / 2);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f, 0.02f, 0.12f));
            RenderWallMesh(meshes["plane"], shaders["WallShader"], modelMatrix);


            // pereti spate
            floor_light_pos[0] = glm::vec3(7.5, 0.5, i - 0.5);
            floor_light_pos[1] = glm::vec3(7.5, 0.5, i + 0.5);
            floor_light_pos[2] = glm::vec3(7.5, 0.5, i + 1.5);

            if (i == 0) {
                floor_light_colors[0] = glm::vec3(0);
            }
            else {
                floor_light_colors[0] = floor_colors[7][i - 1];
            }
          
            floor_light_colors[1] = floor_colors[7][i];

            if (i == 7) {
                floor_light_colors[2] = glm::vec3(0);
            }
            else {
                floor_light_colors[2] = floor_colors[7][i + 1];
            }

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(8, 4, i + 0.5));
            modelMatrix *= Transform3D::RotateOZ(M_PI / 2);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.12f, 0.02f, 0.02f));
            RenderWallMesh(meshes["plane"], shaders["WallShader"], modelMatrix);
        }

    }

    // Render ceiling
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(4, 7, 4));
        modelMatrix = glm::scale(modelMatrix, glm::vec3((8.0 / 50.0), 0.02f, (8.0 / 50.0)));
        RenderCeilingMesh(meshes["plane"], shaders["CeilingShader"], modelMatrix);
    }

    // Render dancers
    {
        materialShininess = 30;
        materialKd = 0.6;
        materialKs = 0.5;
        materialKe = 0;

        for (int i = 0; i < 5; i++) {

            if (dancer_transition_times[i] > 0) {
                
                dancer_pos[i] += normalize(dancer_next_pos[i] - dancer_pos[i]) * dancer_speed[i] * deltaTimeSeconds;

                dancer_transition_times[i] -= deltaTimeSeconds;
            }
            else
            {
                dancer_next_pos[i] = glm::vec3(rand() % 7 + 1, 1.5, rand() % 7 + 1);
                dancer_transition_times[i] = (float) (rand() % 6 + 3);
                dancer_speed[i] = distance(dancer_pos[i], dancer_next_pos[i]) / dancer_transition_times[i];
            }

            int x, y;
            x = (int) (dancer_pos[i][0]);
            y = (int) (dancer_pos[i][2]);

            floor_light_pos[0] = glm::vec3(x + 0.5f, 1, y + 0.5f);
            floor_light_colors[0] = floor_colors[x][y];

            floor_light_pos[1] = glm::vec3(x + 0.5, 1, y + 1.5);
            floor_light_colors[1] = floor_colors[x][y + 1];

            floor_light_pos[2] = glm::vec3(x + 0.5, 1, y - 0.5);
            floor_light_colors[2] = floor_colors[x][y - 1];

            floor_light_pos[3] = glm::vec3(x + 1.5, 1, y + 0.5);
            floor_light_colors[3] = floor_colors[x + 1][y];

            floor_light_pos[4] = glm::vec3(x + 1.5, 1, y + 1.5);
            floor_light_colors[4] = floor_colors[x + 1][y + 1];

            floor_light_pos[5] = glm::vec3(x + 1.5, 1, y - 0.5);
            floor_light_colors[5] = floor_colors[x + 1][y - 1];

            floor_light_pos[6] = glm::vec3(x - 0.5, 1, y + 0.5);
            floor_light_colors[6] = floor_colors[x - 1][y];

            floor_light_pos[7] = glm::vec3(x - 0.5, 1, y + 1.5);
            floor_light_colors[7] = floor_colors[x - 1][y + 1];

            floor_light_pos[8] = glm::vec3(x - 0.5, 1, y - 0.5);
            floor_light_colors[8] = floor_colors[x - 1][y - 1];
            

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, dancer_pos[i]);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 1, 0.5));
            RenderDancerMesh(meshes["box"], shaders["DancerShader"], modelMatrix);
        }
 
    }

    // Render globe
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, globe_pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5));
        RenderGlobeMesh(meshes["sphere"], shaders["GlobeShader"], modelMatrix, random_texture);
    }


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // cone
    for (int i = 0; i < 4; i++) {

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spotlights_pos[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 6, 1));
        RenderConeMesh(meshes["cone"], shaders["ConeShader"], modelMatrix, spotlights_col[i]);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

}


void Tema3::FrameEnd()
{
    DrawCoordinateSystem();
}



void Tema3::RenderFloorMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties

    glUniform1i(glGetUniformLocation(shader->program, "light_mode"), light_mode);

    GLuint loc_spotlights_pos = glGetUniformLocation(shader->program, "spotlights_pos");
    glUniform3fv(loc_spotlights_pos, 4, glm::value_ptr(spotlights_pos[0]));

    GLuint loc_spotlights_col = glGetUniformLocation(shader->program, "spotlights_col");
    glUniform3fv(loc_spotlights_col, 4, glm::value_ptr(spotlights_col[0]));

    glUniform1f(glGetUniformLocation(shader->program, "spotlights_angle"), spotlights_angle);

    glUniform3fv(glGetUniformLocation(shader->program, "globe_pos"), 1, glm::value_ptr(globe_pos));

    int timeLocation = glGetUniformLocation(shader->program, "CurrentTime");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, random_texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 1);



    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    // TODO(student): Set material property uniforms (shininess, kd, ks, object color)
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
    glUniform1f(glGetUniformLocation(shader->program, "material_ke"), materialKe);
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderWallMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties

    glUniform1i(glGetUniformLocation(shader->program, "light_mode"), light_mode);

    GLuint loc1 = glGetUniformLocation(shader->program, "floor_light_pos");
    glUniform3fv(loc1, 3, glm::value_ptr(floor_light_pos[0]));

    GLuint loc2 = glGetUniformLocation(shader->program, "floor_light_colors");
    glUniform3fv(loc2, 3, glm::value_ptr(floor_light_colors[0]));

    glUniform3fv(glGetUniformLocation(shader->program, "globe_pos"), 1, glm::value_ptr(globe_pos));

    int timeLocation = glGetUniformLocation(shader->program, "CurrentTime");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, random_texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 1);


    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();

    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
    glUniform1f(glGetUniformLocation(shader->program, "material_ke"), materialKe);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderCeilingMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties

    glUniform1i(glGetUniformLocation(shader->program, "light_mode"), light_mode);

    glUniform3fv(glGetUniformLocation(shader->program, "globe_pos"), 1, glm::value_ptr(globe_pos));

    int timeLocation = glGetUniformLocation(shader->program, "CurrentTime");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, random_texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 1);


    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();

    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
    glUniform1f(glGetUniformLocation(shader->program, "material_ke"), materialKe);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderDancerMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties

    glUniform1i(glGetUniformLocation(shader->program, "light_mode"), light_mode);

    GLuint loc_spotlights_pos = glGetUniformLocation(shader->program, "spotlights_pos");
    glUniform3fv(loc_spotlights_pos, 4, glm::value_ptr(spotlights_pos[0]));

    GLuint loc_spotlights_col = glGetUniformLocation(shader->program, "spotlights_col");
    glUniform3fv(loc_spotlights_col, 4, glm::value_ptr(spotlights_col[0]));

    glUniform1f(glGetUniformLocation(shader->program, "spotlights_angle"), spotlights_angle);

    GLuint loc1 = glGetUniformLocation(shader->program, "floor_light_pos");
    glUniform3fv(loc1, 9, glm::value_ptr(floor_light_pos[0]));

    GLuint loc2 = glGetUniformLocation(shader->program, "floor_light_colors");
    glUniform3fv(loc2, 9, glm::value_ptr(floor_light_colors[0]));

    glUniform3fv(glGetUniformLocation(shader->program, "globe_pos"), 1, glm::value_ptr(globe_pos));

    int timeLocation = glGetUniformLocation(shader->program, "CurrentTime");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, random_texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 1);


    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    // TODO(student): Set material property uniforms (shininess, kd, ks, object color)
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
    glUniform1f(glGetUniformLocation(shader->program, "material_ke"), materialKe);

     // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderConeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties

    glUniform1i(glGetUniformLocation(shader->program, "light_mode"), light_mode);

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    // TODO(student): Set material property uniforms (shininess, kd, ks, object color)
    glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderGlobeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glUniform1i(glGetUniformLocation(shader->program, "light_mode"), light_mode);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    int timeLocation = glGetUniformLocation(shader->program, "CurrentTime");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];


    for (int i = 0; i < size; ++i)
    {
        data[i] = rand() % 255;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event

    if (key == GLFW_KEY_V)
    {
        light_mode = (light_mode + 1) % 4;
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
