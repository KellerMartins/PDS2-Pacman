#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <raylib.h>
#include <string>
#include "utils.h"

class Object3D{
    private:
        bool _hasModel;

    public:
        Vector3 position;
        Vector3 scale;
        Vector3 rotationAxis;
        float rotationAngle;
        Model model;

        void Load3DModel(const std::string modelPath, const std::string textureMaskPath, Color color);
        void SetColor(Color color);
        void SetColor(unsigned char red, unsigned char green, unsigned char blue);
        
        Object3D(const std::string modelPath, const std::string textureMaskPath, Color color);
        Object3D();
        ~Object3D();
};

#endif