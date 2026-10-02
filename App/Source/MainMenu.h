#pragma once

#include "GameState.h"

#include <Layer.h>
#include <raylib.h>

class MainMenu : public Core::Layer
{
public:
    MainMenu()
    {
        m_PixelFont = LoadFont("Assets/Fonts/font.ttf");
    }
    ~MainMenu()
    {
        UnloadFont(m_PixelFont);
    }

    void OnUpdate(float ts) override
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            TransitionTo<GameState>();
        }
    }

    void OnRender() override
    {
        const char* text = "Welcome to my game! ... press enter to play";
        float size = 32.0f;
        float spacing = 1.0f;

        Vector2 dim = MeasureTextEx(m_PixelFont, text, size, spacing);
        Vector2 pos = { (GetScreenWidth() - dim.x) / 4.0f,
                        (GetScreenHeight() - dim.y) / 2.0f };

        DrawTextEx(m_PixelFont, text, pos, size, spacing, WHITE);
    }
private:
    Font m_PixelFont;
};