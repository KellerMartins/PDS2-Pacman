#include "object3D.h"
#include "rendering.h"
#include "../utils.h"

std::map<std::string, Model> Object3D::_modelsCache;

Object3D::Object3D(){
    _hasModel = false;
    _color = RAYWHITE;
    shouldRender = false;
    position = VECTOR3_ZERO;
    scale = (Vector3){1,1,1};
    rotationAxis = (Vector3){0,1,0};
    rotationAngle = 0;
}

Object3D::Object3D(const std::string modelPath, Color color) : Object3D(){
    Load3DModel(modelPath, color);
}

Object3D::~Object3D(){
    if(_hasModel){
        RenderManager::RemoveObjectFromRenderer(_objectID);
    }
}

bool Object3D::HasModel(){
    return _hasModel;
}

void Object3D::Load3DModel(const std::string modelPath){
    if(!_hasModel){
        _objectID = RenderManager::RegisterObjectToRender((Object3D*)this);
    }
    _hasModel = true;
    shouldRender = true;

    //Insert the loaded model inside the cache when loading it for the first time
    auto modelIt = _modelsCache.find(modelPath);
    if(modelIt == _modelsCache.end()){
        Cache3DModel(modelPath);
    }
    
    _modelsCache[modelPath].material.maps[MAP_DIFFUSE].color = _color;
    model = _modelsCache[modelPath];
    
}

void Object3D::Load3DModel(const std::string modelPath, Color color){
    Load3DModel(modelPath);
    SetColor(color);
}

void Object3D::Cache3DModel(const std::string modelPath){
    _modelsCache[modelPath] = LoadModel(modelPath.c_str());
    _modelsCache[modelPath].material.maps[MAP_DIFFUSE].texture = RenderManager::GetMaskTexture();
}

void Object3D::SetColor(Color color){
    _color = color;
    model.material.maps[MAP_DIFFUSE].color = color;
}

void Object3D::SetColor(unsigned char red, unsigned char green, unsigned char blue){
    SetColor((Color){red, green, blue, (unsigned char)255});
}