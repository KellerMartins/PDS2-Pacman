#ifndef RENDERING_H
#define RENDERING_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <raylib.h>
#include <list>
#include <string>
#include "utils.h"
#include "object3D.h"

namespace RenderManager{

        extern Camera camera;

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

        //Debug drawing functions
        void DrawDebugLine(Vector3 startPos, Vector3 endPos, Color color);
        void DrawDebugCube(Vector3 position, Vector3 size, Color color);
        void DrawDebugSphere(Vector3 centerPos, float radius, Color color);                                             
};

#endif
