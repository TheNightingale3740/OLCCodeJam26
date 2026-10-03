#pragma once

#include "Layer.h"

#include <raylib.h>
#include <raymath.h>

#include <filesystem>

#include <imgui.h>

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
    }

    void OnUIRender()
    {
    }

    void Update(float ts)
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
                // Player Grounded
                isGrounded = true;
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

        DrawGrid(10, 1.0f);

        EndShaderMode();
        EndMode3D();
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