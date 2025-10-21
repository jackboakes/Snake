#include "raylib.h"
#include "Assets.h"
#include <filesystem>


Texture2D Assets::m_snakeAtlas = { 0 };

// Constructs the full path to the asset file by combining the .exe directory
// with a path to the asset folder.
// Utilises GetApplicationDirectory() from raylib and traverses up two folder levels to root/exe.
// Works for both running from cmd line with build\debug or release\snake.exe,
// and double clicking the .exe directly.
std::string Assets::GetAssetPath(std::string_view fileName)
{
    std::filesystem::path appDirectory { GetApplicationDirectory() };
    std::filesystem::path assetPath { appDirectory / ".." / ".." / "assets" / fileName };

    return assetPath.string();
}

void Assets::LoadGameTextures()
{
    m_snakeAtlas = LoadTexture(GetAssetPath("snake_atlas.png").c_str());

    if (m_snakeAtlas.id > 0)
    {
        TraceLog(LOG_INFO, "Snake atlas loaded successfully");
    }
    else
    {
        TraceLog(LOG_WARNING, "Failed to load snake atlas");
    }
}

void Assets::UnloadGameTextures()
{
    if (m_snakeAtlas.id > 0)
    {
        UnloadTexture(m_snakeAtlas);
    }
}

// Sets the icon for the window using image from our atlas
void Assets::UpdateWindowIcon(int tileSize)
{
    Image fullSnakeAtlas { LoadImageFromTexture(m_snakeAtlas) };
    if (!fullSnakeAtlas.data)
    {
        return;
    }
    Rectangle iconSpriteRect { GetSpriteRect(2, 0, static_cast<float>(tileSize)) }; // South facing head
    Image windowIcon { ImageFromImage(fullSnakeAtlas, iconSpriteRect) };
    UnloadImage(fullSnakeAtlas);

    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
}

// Helper function to get a 32x32 sprite from the atlas by index
Rectangle Assets::GetSpriteRect(int col, int row, float tileSize)
{
    Rectangle spriteRect {
            (static_cast<float>(col) * tileSize),
            (static_cast<float>(row) * tileSize),
            tileSize,
            tileSize
    };

    return  spriteRect;
}

Texture2D Assets::GetSnakeAtlas()
{
    if (m_snakeAtlas.id > 0)
    {
        return m_snakeAtlas;
    }
    // If we get here it will return a zerod texture
    Texture2D debugTexture { 0 };
    TraceLog(LOG_WARNING, "Failed to get snake atlas");
    return debugTexture;
}