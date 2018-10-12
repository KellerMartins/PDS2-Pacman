#ifndef IMAGEUI_H
#define IMAGEUI_H

#include <raylib.h>
#include <string>
#include "UI.h"
#include "../rendering.h"

class ImageUI : public UI{
    private:
        Texture2D _imageTex;
        
    public:
        ImageUI(std::string imagePath, float X, float Y, float Width, float Height);
        ~ImageUI();

        virtual void OnDraw() override;
};

#endif