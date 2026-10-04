#pragma once

#include "Layer.h"

#include <imgui.h>
#include <raylib.h>
#include <raymath.h>

#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

static bool s_PlayerWon = false;
static bool s_PlayerDied = false;

struct Interactable
{
    Mesh Geometry;
    BoundingBox AABB;
    std::function<void()> Interact;
};

class WinScreen : public Core::Layer
{
public:
    WinScreen() {}
    ~WinScreen() {}

    void OnRender() override
    {
        const char* text = "YOU WON!!";
        int fontSize = 50;

        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1.0f);
        DrawText(text, GetScreenWidth() / 2 - textSize.x, GetScreenHeight() / 2 - textSize.y, fontSize, WHITE);
    }
};

class Player
{
public:
    Player()
    {}

    ~Player()
    {}

    void Init(Model *model)
    {
        m_MainCamera.position = { 5.0f, 5.0f, 5.0f };
        m_MainCamera.up = { 0.0f, 1.0f, 0.0f };
        m_MainCamera.target = { 0.0f, 0.0f, 0.0f };
        m_MainCamera.fovy = 70.0f;
        m_MainCamera.projection = CAMERA_PERSPECTIVE;

        m_CollisionGeometry = model;

        Mesh bridge1 = model->meshes[0];
        Mesh bridge2 = model->meshes[1];

        std::unique_ptr<Interactable> exitDoor = std::make_unique<Interactable>();
        exitDoor->Geometry = model->meshes[4];
        exitDoor->AABB = GetMeshBoundingBox(exitDoor->Geometry);
        exitDoor->Interact = [&]() {
            // TODO: Show win screen
            m_SomethingHappened = false;
            s_PlayerWon = true;
        };

        std::unique_ptr<Interactable> button1 = std::make_unique<Interactable>();
        button1->Geometry = model->meshes[3];
        button1->AABB = GetMeshBoundingBox(button1->Geometry);
        button1->Interact = [&]() {
            // TODO: Interact logic for button1
            m_MainCamera.position = { -71.5f, 5.0f, 29.5f };
            m_SomethingHappened = true;
        };

        std::unique_ptr<Interactable> button2 = std::make_unique<Interactable>();
        button2->Geometry = model->meshes[5];
        button2->AABB = GetMeshBoundingBox(button2->Geometry);
        button2->Interact = [&]() {
            // TODO: Interact logic for button1
            //m_SomethingHappened = true;
        };

        m_Interactables.push_back(std::move(exitDoor));
        m_Interactables.push_back(std::move(button1));
        m_Interactables.push_back(std::move(button2));
    }

    void OnUIRender()
    {
        ImGui::Begin("Debug Panel");
        ImGui::Text("Player Alive?: %s", (m_IsAlive ? "True" : "False"));
        ImGui::Text("Show Prompt?: %s", (m_ShowPrompt ? "True" : "False"));
        ImGui::Text("Player Y velocity: %.3f", m_VelocityY);
        ImGui::Text("Something happened? %s", (m_SomethingHappened ? "True" : "False"));
        
        float x = m_MainCamera.position.x;
        float y = m_MainCamera.position.y;
        float z = m_MainCamera.position.z;
        
        ImGui::Text("Player Position: %.3f %.3f %.3f", x, y, z);
        ImGui::End();
    }

    void Update(float ts)
    {
        m_ShowPrompt = false;

        if (m_VelocityY < -15.0f) // Hacky way to kill the player
        {
            m_IsAlive = false;
            s_PlayerDied = true;
        }

        for (auto& interactable : m_Interactables)
        {
            Ray r;
            r.position = m_MainCamera.position;
            r.direction = Vector3Normalize(m_MainCamera.target - m_MainCamera.position);
            
            RayCollision payload = GetRayCollisionBox(r, interactable->AABB);
            if (payload.hit && payload.distance < 5.0f)
            {
                m_ShowPrompt = true;
                
                if (IsKeyPressed(KEY_F))
                {
                    interactable->Interact();
                    break;
                }
            }
        }

        PlayerMovement(ts);
    }

    void OnRender()
    {
        if (m_ShowPrompt)
        {
            const char* text = "Press \"F\" to interact";
            int fontSize = 20;

            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1.0f);
            DrawText(text, GetScreenWidth() / 4 - textSize.x, GetScreenHeight() / 2 - textSize.y, fontSize, WHITE);
        }
    }

    void PlayerMovement(float ts)
    {
        Vector3 lastPos = m_MainCamera.position;
        Vector3 lastTarget = m_MainCamera.target;
        
        UpdateCamera(&m_MainCamera, CAMERA_FIRST_PERSON);
        UpdateVertical(ts);
        
        Vector3 newPos = m_MainCamera.position;
        Vector3 displacement = newPos - lastPos;

        if (Vector3Length(displacement) >= 0.0001f)
        {
            Ray r;
            r.position = lastPos;
            r.direction = Vector3Normalize(displacement);

            for (int i = 0; i < m_CollisionGeometry->meshCount; i++)
            {
                RayCollision payload = GetRayCollisionMesh(r, m_CollisionGeometry->meshes[i], MatrixIdentity());
                if (payload.hit && payload.distance < 1.0f) // Player Radius
                {
                    // Collission Detected!
                    m_MainCamera.position = m_MainCamera.position - displacement;
                    m_MainCamera.target = lastTarget;

                    break;
                }
            }
        }
    }

    void UpdateVertical(float ts)
    {
        Ray r;
        r.position = m_MainCamera.position;
        r.direction = Vector3Normalize(Vector3(0.0f, -1.0f, 0.0f)); // Trust issues

        bool isGrounded = false;

        for (int i = 0; i < m_CollisionGeometry->meshCount; i++)
        {
            RayCollision payload = GetRayCollisionMesh(r, m_CollisionGeometry->meshes[i], MatrixIdentity());
            if (payload.hit && payload.distance <= 5.0f) // Player Height
            {
                // TODO: Snap palyer to the floor to avoid bugs
                isGrounded = true; // Player Grounded
            }
        }

        if (isGrounded)
        {
            m_VelocityY = 0.0f;
            return;
        }

        // Player is falling apply gravity
        m_VelocityY += m_Gravity * ts;
        m_MainCamera.position.y += m_VelocityY * ts;
    }

    Camera3D GetMainCamera() const { return m_MainCamera; }

private:
    Camera3D m_MainCamera;
    Model* m_CollisionGeometry;

    float m_VelocityY = 0.0f;
    float m_Gravity = -9.8f;

    std::vector<std::unique_ptr<Interactable>> m_Interactables;

    bool m_IsAlive = true;
    bool m_ShowPrompt = false;
    bool m_SomethingHappened = false;
};

class GameState : public Core::Layer
{
public:
    GameState()
    {
        DisableCursor();

        LoadResources();

        m_Player.Init(&m_LevelGeometry);
    }

    ~GameState()
    {
        UnloadResources();
    }

    void OnUpdate(float ts) override
    {
        m_Player.Update(ts);

        if (s_PlayerWon)
        {
            TransitionTo<WinScreen>();
        }
        else if (s_PlayerDied)
        {
            CloseWindow(); // I DONT CARE!!! 💢 .... ITS A DEATH SCREEN THATS WHAT MATTERS!!!
        }
    }

    void OnUIRender() override
    {
        m_Player.OnUIRender();
    }

    void OnRender() override
    {
        BeginMode3D(m_Player.GetMainCamera());
        BeginShaderMode(m_LambertDiffuse);

        DrawModel(m_LevelGeometry, Vector3Zero(), 1, WHITE);

        EndShaderMode();
        EndMode3D();

        m_Player.OnRender();
    }
private:
    void LoadResources()
    {
        m_LambertDiffuse = LoadShader("Assets/Shaders/LambertianDiffuse.vs", "Assets/Shaders/LambertianDiffuse.fs");

        m_LevelGeometry = LoadMesh("Assets/Models/Level1.glb");
    }

    void UnloadResources()
    {
        UnloadShader(m_LambertDiffuse);

        UnloadModel(m_LevelGeometry);
    }

    Model LoadMesh(std::filesystem::path path) const
    {
        Model model = LoadModel(path.string().c_str()); // Because .... reasons -_-

        for (int i = 0; i < model.materialCount; i++)
        {
            model.materials[i].shader = m_LambertDiffuse;
        }

        return model;
    }
private:
    Player m_Player;
    
    Model m_LevelGeometry;
    Shader m_LambertDiffuse;
};