#ifndef RENDERING_H
#define RENDERING_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <raylib.h>
#include <list>
#include <array>
#include "utils.h"
#include "object3D.h"

class RenderManager{
    private:
        int _screenWidth, _screenHeight;
        std::list<Object3D*> _objectsToRender;
        Texture2D _blackTexture;
        int _blurDownscale;

        void SetShaderUniforms();
        void RenderBaseEffect();
        void RenderBloom();
        void RenderFinalComposition();

    public:

        const int &screenWidth = _screenWidth;
        const int &screenHeight = _screenHeight;

        Camera camera;

        Shader blurPass;
        Shader maskShader;
        Shader colorShader;
        Shader outlineFilter;
        Shader screenShader;
        Shader copyRTShader;

        int blurPassPixelSizeLoc;
        int copyRTPixelSizeLoc;

        RenderTexture2D maskRT;
        RenderTexture2D baseRT;
        RenderTexture2D tempRT;
        RenderTexture2D blurRT;
        RenderTexture2D vBlurRT;

        std::list<Object3D*>::iterator RegisterObjectToRender(Object3D *obj);
        void RemoveObjectFromRenderer(std::list<Object3D*>::iterator objID);

        void SetResolution(int width, int height);
        void SetBloomDownscale(unsigned value);
        void ReloadShaders();
        void Render();


        RenderManager();
        ~RenderManager();
};


#endif
