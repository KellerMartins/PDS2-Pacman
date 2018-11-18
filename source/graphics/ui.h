#ifndef UI_H
#define UI_H

#include <vector>
#include <map>
#include <raylib.h>
#include "../utils.h"
#include "rendering.h"

namespace UI{
 
    void DrawImage(std::string imagePath, float X, float Y, float Width, float Height);
    void DrawText(std::string Text, float X, float Y, float Size, float Spacing, Color ColorTint);
    void DrawTextCentered(std::string Text, float X, float Y, float Size, float Spacing, Color ColorTint);

    void SetFont(std::string fontPath);
    void Render();
}
#endif