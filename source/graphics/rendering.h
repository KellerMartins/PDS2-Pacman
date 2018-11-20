#ifndef RENDERING_H
#define RENDERING_H

#include <raylib.h>
#include <list>
#include <string>
#include "object3D.h"
#include "debugPrimitives.h"


#define MASK_TEXTURE_PATH "assets/models/textureMask.png"
#define DEFAULT_MODEL_PATH "assets/models/Cube.obj"

#define CAMERA_SPEED 1

namespace RenderManager{

        extern Camera camera;

        //Internal object render register functions
        //Automatically called by the constructor and destructors of the Object3D class
        //Do not call manually, use the Object3D interface
        std::list<Object3D*>::iterator RegisterObjectToRender(Object3D *obj);
        void RemoveObjectFromRenderer(std::list<Object3D*>::iterator objID);

        void Init(int screenWidth, int screenHeight, std::string windowTitle);
        void Quit();

        int GetWidth();
        int GetHeight();
        void SetResolution(int width, int height);
        void SetBloomDownscale(unsigned value);
        void ReloadShaders();
        void Render();

        void SetCameraOffset(Vector3 offset);
        void CameraFollow(Vector3 position);

        Texture2D GetMaskTexture();
};

#endif
