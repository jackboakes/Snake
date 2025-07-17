#ifndef ASSET_H
#define ASSET_H

// Asset path util
const char* GetAssetPath(const char* fileName);

// Texture Management
void LoadGameTextures();
void UnloadGameTextures();
void UpdateWindowIcon(int tileSize);

// Sprite util
Rectangle GetSpriteRect(int col, int row, float tileSize);

// Acesss snake atlas
Texture2D GetSnakeAtlas();

#endif