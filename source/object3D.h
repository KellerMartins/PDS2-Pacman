#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <raylib.h>
#include <string>
#include <list>
#include "utils.h"

class Object3D{
    private:
        bool _hasModel;
        std::list<Object3D*>::iterator _objectID;

    public:
        bool shouldRender;
        Vector3 position;
        Vector3 scale;
        Vector3 rotationAxis;
        float rotationAngle;
        Model model;

        bool HasModel();
        void Load3DModel(const std::string modelPath, Color color);
        void SetColor(Color color);
        void SetColor(unsigned char red, unsigned char green, unsigned char blue);
        
        Object3D(const std::string modelPath, Color color);
        Object3D();
        ~Object3D();
};

#endif