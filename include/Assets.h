#pragma once
#include <string>

class Assets
{
public:
	// Asset path util
	static std::string GetAssetPath(std::string_view fileName);

	// Texture Management
	static void LoadGameTextures();
	static void UnloadGameTextures();
	static void UpdateWindowIcon(int tileSize);

	// Sprite util
	static Rectangle GetSpriteRect(int col, int row, float tileSize);

	// Acesss snake atlas
	static Texture2D GetSnakeAtlas();

private:
	static Texture2D m_snakeAtlas;
};
