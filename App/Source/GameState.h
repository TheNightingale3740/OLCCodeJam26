#pragma once

#include "Layer.h"

#include <raylib.h>
#include <raymath.h>

#include <filesystem>

#include <imgui.h>

#include <print>

class Player
{
public:
    Player()
    {}

    ~Player()
    {}

    void Init(Mesh mesh)
    {
        m_MainCamera.position = { 5.0f, 5.0f, 5.0f };
        m_MainCamera.up = { 0.0f, 1.0f, 0.0f };
        m_MainCamera.target = { 0.0f, 0.0f, 0.0f };
        m_MainCamera.fovy = 45.0f;
        m_MainCamera.projection = CAMERA_PERSPECTIVE;

        m_LevelMesh = mesh;
    }

    void OnUIRender()
    {
    }

    void Update(float ts)
    {
        Vector3 lastPos = m_MainCamera.position;
        Vector3 lastTarget = m_MainCamera.target;
        
        UpdateCamera(&m_MainCamera, CAMERA_FIRST_PERSON);
        
        Vector3 newPos = m_MainCamera.position;
        Vector3 displacement = newPos - lastPos;

        if (displacement != Vector3Zero())
        {
            Ray r;
            r.position = lastPos;
            r.direction = Vector3Normalize(displacement);

            RayCollision payload = GetRayCollisionMesh(r, m_LevelMesh, MatrixIdentity());
            if (payload.distance < 1.0f) // Player Radius
            {
                // Collission Detected!
                m_MainCamera.position = m_MainCamera.position - displacement; // Revert!
                m_MainCamera.target = lastTarget;
            }
        }
    }

    Camera3D GetMainCamera() const { return m_MainCamera; }

private:
    Camera3D m_MainCamera;
    Mesh m_LevelMesh;
};

class GameState : public Core::Layer
{
public:
    GameState()
    {
        DisableCursor();

        LoadResources();

        m_Player.Init(m_LevelGeometry.meshes[0]); // There is only one mesh in the level as of right now
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
        Model model = LoadModel(path.c_str());

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