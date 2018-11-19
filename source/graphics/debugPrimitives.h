#ifndef DEBUGPRIMITIVES_H
#define DEBUGPRIMITIVES_H

#include <raylib.h>

namespace RenderManager{
    
    void DrawDebugLine(Vector3 startPos, Vector3 endPos, Color color);
    void DrawDebugCube(Vector3 position, Vector3 size, Color color);
    void DrawDebugSphere(Vector3 centerPos, float radius, Color color);   
    void RenderDebugPrimitives(Camera &camera);
}

#endif