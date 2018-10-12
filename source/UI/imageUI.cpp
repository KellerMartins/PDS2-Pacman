#include "imageUI.h"

ImageUI::ImageUI(std::string imagePath, float X, float Y, float Width, float Height){
    SetPosition(X, Y);
    SetScale(Width, Height);

    _imageTex = LoadTexture(imagePath.c_str());
    SetTextureFilter(_imageTex, FILTER_TRILINEAR);
    GenTextureMipmaps(&_imageTex);
}

ImageUI::~ImageUI(){
    UnloadTexture(_imageTex);
}

void ImageUI::OnDraw(){
    float aspectRatio = (float)_imageTex.width/(float)_imageTex.height;
    float screenWidth = (float)RenderManager::GetWidth();
    float screenHeight = (float)RenderManager::GetHeight();
    float positionX = _x*screenWidth  - _width*screenHeight*aspectRatio/2;
    float positionY = _y*screenHeight - _height*screenHeight/2;

    DrawTexturePro(_imageTex, (Rectangle){0, 0, (float)_imageTex.width, (float)_imageTex.height}, 
                              (Rectangle){positionX, positionY, _width*screenHeight*aspectRatio, _height*screenHeight}, 
                              VECTOR2_ZERO, 0,WHITE);
}