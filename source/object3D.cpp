#include "object3D.h"
Object3D::Object3D(){
    _hasModel = false;
    position = VECTOR3_ZERO;
    scale = (Vector3){1,1,1};
    rotationAxis = (Vector3){0,1,0};
    rotationAngle = 0;
}

Object3D::Object3D(const std::string modelPath, const std::string textureMaskPath, Color color) : Object3D(){
    Load3DModel(modelPath, textureMaskPath, color);
}

Object3D::~Object3D(){
    if(_hasModel)
        UnloadModel(model);
}

void Object3D::Load3DModel(const std::string modelPath, const std::string textureMaskPath, Color color){
    if(_hasModel){
        UnloadTexture(model.material.maps[MAP_DIFFUSE].texture);
        UnloadModel(model);    
    }
    _hasModel = true;
    
    model = LoadModel(modelPath.c_str());
    model.material.maps[MAP_DIFFUSE].texture = LoadTexture(textureMaskPath.c_str());
    model.material.maps[MAP_DIFFUSE].color = color;
}

void Object3D::SetColor(Color color){
    model.material.maps[MAP_DIFFUSE].color = color;
}

void Object3D::SetColor(unsigned char red, unsigned char green, unsigned char blue){
    model.material.maps[MAP_DIFFUSE].color = (Color){red, green, blue, (unsigned char)255};
}