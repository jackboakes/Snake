#include "raylib.h"
#include "Assets.h"


// Constructs the full path to the asset file by combining the .exe directory
// with a path to the asset folder
// utilises GetApplicationDirectory() from raylib and traverses up two folder levels to root
// works for both running from cmd line with build\debug or release\snake.exe
// and double clicking the .exe directly
const char* GetAssetPath(const char* fileName)
{
    return TextFormat("%s../../assets/%s", GetApplicationDirectory(), fileName);
}