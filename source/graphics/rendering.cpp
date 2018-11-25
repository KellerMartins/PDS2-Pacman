#include "rendering.h"

#include "glad/glad.h"
#include "../utils.h"
#include "ui.h"

namespace RenderManager{

    //Camera
    Camera camera = (Camera){{0,0,0}, {0,0,1}, {0,1,0}, 45.0f, CAMERA_PERSPECTIVE};

    //Internal globals
    Vector3 _cameraOffset = {0,0,0};

    //State
    bool _isInitialized = false;

    //Resolutions
    int _screenWidth = 0;
    int _screenHeight= 0;

    //Blur settings
    int _blurDownscale = 1;
    int _blurQuality = 0;

    //Shaders and uniform locations
    Shader _blurPass;
    Shader _maskShader;
    Shader _colorShader;
    Shader _outlineFilter;
    Shader _screenShader;
    Shader _copyRTShader;
    int _blurPassPixelSizeLoc;
    int _blurPassBlurQualityLoc;
    int _copyRTPixelSizeLoc;

    //Render Textures and textures
    RenderTexture2D _baseRT;
    RenderTexture2D _tempRT;
    RenderTexture2D _blurRT;
    RenderTexture2D _vBlurRT;
    Texture2D _blackTexture;
    Texture2D _maskTexture;

    //List of renderable objects
    std::list<Object3D*> _objectsToRender;


    //Internal functions headers
    void LoadShaders();
    void SetShaderUniforms();
    void RenderBaseEffect();
    void RenderBloom();
    void RenderFinalComposition();
    void RenderDebugPrimitives();


    void Init(int screenWidth, int screenHeight, bool fullscreen, std::string windowTitle){
        if(fullscreen)
            ToggleFullscreen();

        #ifdef NDEBUG
        SetTraceLog(0);
        ShowLogo();
        #endif
        
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        InitWindow(screenWidth, screenHeight, windowTitle.c_str());

        #ifdef NDEBUG
        SetTargetFPS(60);
        #else
        SetTargetFPS(0);
        #endif
        
        LoadShaders();
        SetResolution(screenWidth, screenHeight);

        Image blackImg = GenImageColor(2,2,BLACK);
        _blackTexture = LoadTextureFromImage(blackImg);
        UnloadImage(blackImg);

        _maskTexture = LoadTexture(MASK_TEXTURE_PATH);

        _isInitialized = true;
    }

    void Quit(){
        if(_isInitialized){
            UnloadShader(_outlineFilter);
            UnloadShader(_maskShader);
            UnloadShader(_colorShader);
            UnloadShader(_blurPass);
            UnloadShader(_screenShader);
            UnloadShader(_copyRTShader);

            UnloadTexture(_blackTexture);
        
            UnloadRenderTexture(_baseRT);
            UnloadRenderTexture(_tempRT);
            UnloadRenderTexture(_blurRT);
            UnloadRenderTexture(_vBlurRT);

            CloseWindow();
        }
        _isInitialized = false;
    }

    void SetBloomQuality(BlurQuality quality){
        switch(quality){
            case Low:
                _blurDownscale = 3;
                _blurQuality = 0;
            break;

            case Medium:
                _blurDownscale = 2;
                _blurQuality = 1;
            break;

            case High:
                _blurDownscale = 1;
                _blurQuality = 2;
            break;
        }
        

        if(_isInitialized){
            UnloadRenderTexture(_blurRT);
            UnloadRenderTexture(_vBlurRT);
        }
        _blurRT = LoadRenderTexture(_screenWidth/(_blurDownscale), _screenHeight/(_blurDownscale));
        _vBlurRT = LoadRenderTexture(_screenWidth/(_blurDownscale), _screenHeight/(_blurDownscale));
    };

    void SetResolution(int width, int height){

        //Unload rts if they already been created
        if(_isInitialized){
            UnloadRenderTexture(_baseRT);
            UnloadRenderTexture(_tempRT);
            UnloadRenderTexture(_blurRT);
            UnloadRenderTexture(_vBlurRT);
        }

        _screenWidth = width<MIN_WIDTH? MIN_WIDTH : width;
        _screenHeight = height<MIN_HEIGHT? MIN_HEIGHT : height;

        SetWindowSize(_screenWidth, _screenHeight);

        _baseRT = LoadRenderTexture(_screenWidth, _screenHeight);
        _tempRT = LoadRenderTexture(_screenWidth, _screenHeight);

        _blurRT = LoadRenderTexture(_screenWidth/(_blurDownscale), _screenHeight/(_blurDownscale));
        _vBlurRT = LoadRenderTexture(_screenWidth/(_blurDownscale), _screenHeight/(_blurDownscale));

        SetShaderUniforms();
    }

    int GetWidth(){
        return _screenWidth;
    }

    int GetHeight(){
        return _screenHeight;
    }

    void LoadShaders(){
        _outlineFilter = LoadShader("assets/shaders/fxBase.vs","assets/shaders/outlineFilter.fs");
        _blurPass = LoadShader("assets/shaders/fxBase.vs","assets/shaders/blurPass.fs");
        _screenShader = LoadShader("assets/shaders/fxBase.vs","assets/shaders/screen.fs");
        _copyRTShader = LoadShader("assets/shaders/fxBase.vs","assets/shaders/copyRT.fs");

        _maskShader = LoadShader("assets/shaders/modelVert.vs","assets/shaders/maskFrag.fs");
        _colorShader = LoadShader("assets/shaders/modelVert.vs","assets/shaders/colorFrag.fs");
    }

    void ReloadShaders(){

        //Unload old shaders
        if(_isInitialized){
            UnloadShader(_outlineFilter);
            UnloadShader(_maskShader);
            UnloadShader(_colorShader);
            UnloadShader(_blurPass);
            UnloadShader(_screenShader);
            UnloadShader(_copyRTShader);
        }

        LoadShaders();
        SetShaderUniforms();
    }

    void SetShaderUniforms(){

        _blurPassPixelSizeLoc = GetShaderLocation(_blurPass, "xyHalfPixelSize_zwPixelSize");
        _blurPassBlurQualityLoc = GetShaderLocation(_blurPass, "blurQuality");
        _copyRTPixelSizeLoc = GetShaderLocation(_copyRTShader, "PixelSize");

        int texIndex = 1;
        SetShaderValuei(_blurPass, GetShaderLocation(_blurPass, "texture1"), &texIndex, 1);
        SetShaderValuei(_screenShader, GetShaderLocation(_screenShader, "texture1"), &texIndex, 1);

        float val = _screenWidth;
        SetShaderValue(_outlineFilter, GetShaderLocation(_outlineFilter, "width"), &val, 1);
        SetShaderValue(_screenShader, GetShaderLocation(_screenShader, "width"), &val, 1);
        val = _screenHeight;
        SetShaderValue(_outlineFilter, GetShaderLocation(_outlineFilter, "height"), &val, 1);
        SetShaderValue(_screenShader, GetShaderLocation(_screenShader, "height"), &val, 1);

        val = 0.25;
        SetShaderValue(_screenShader, GetShaderLocation(_screenShader, "vignettePower"), &val, 1);
    }

    Texture2D GetMaskTexture(){
        return _maskTexture;
    }

    std::list<Object3D*>::iterator RegisterObjectToRender(Object3D *obj){
        return _objectsToRender.insert(_objectsToRender.begin(), obj);
    }

    void RemoveObjectFromRenderer(std::list<Object3D*>::iterator objID){
        _objectsToRender.erase(objID);
    }

    void SetCameraOffset(Vector3 offset){
        _cameraOffset = offset;
    }

    void CameraFollow(Vector3 position, float speed){
        Vector3 newPos =  (Vector3){position.x + _cameraOffset.x, 
                                    position.y + _cameraOffset.y,
                                    position.z + _cameraOffset.z};
        camera.position = Lerp(camera.position, newPos, GetFrameTime() * speed);
        camera.target = Lerp(camera.target, position, GetFrameTime() * speed);
    }

    void CameraFollow(Vector3 position){
        CameraFollow(position, CAMERA_DEFAULT_SPEED);
    }

    void Render(){
        UpdateCamera(&camera);
        if(!_isInitialized) return;
        
        BeginDrawing();
            ClearBackground(BLACK);

            glDisable(GL_BLEND);
            RenderBaseEffect();
            RenderBloom();
            RenderFinalComposition();

            glEnable(GL_BLEND);
            UI::Render();

            RenderDebugPrimitives(camera);

            #ifndef NDEBUG
            DrawFPS(0,0);
            #endif

        EndDrawing();
    }

    void RenderBaseEffect(){
        //Render mask to _baseRT
        BeginTextureMode(_baseRT);    
            BeginMode3D(camera);  
                for(Object3D* m : _objectsToRender){
                    if(!m->HasModel() || !m->shouldRender) continue;

                    m->model.material.shader = _maskShader;
                    DrawModelEx(m->model, m->position, m->rotationAxis,m->rotationAngle,m->scale, m->model.material.maps[MAP_DIFFUSE].color);
                    m->model.material.shader = GetShaderDefault();
                } 
            EndMode3D();
        EndTextureMode();

        //Apply outline shader in _baseRT and copy to _tempRT        
        BeginTextureMode(_tempRT);
            BeginShaderMode(_outlineFilter);
                DrawTexturePro(_baseRT.texture, (Rectangle){ 0, 0, (float)_screenWidth, (float)-_screenHeight }, 
                                                (Rectangle){ 0, 0, (float)_screenWidth, (float)_screenHeight }, (Vector2){ 0, 0 }, 0,  WHITE);
            EndShaderMode();
        EndTextureMode();

        //Apply bicubic filter and copy from _tempRT to _baseRT
        BeginTextureMode(_baseRT);
            BeginShaderMode(_copyRTShader);

            Vector2 pixelSize = {1.0f/_screenWidth, 1.0f/_screenHeight};
            SetShaderValue(_copyRTShader, _copyRTPixelSizeLoc, (const float*) &pixelSize, 2);

            DrawTexturePro(_tempRT.texture, (Rectangle){ 0, 0, (float)_screenWidth, (float)-_screenHeight }, 
                                            (Rectangle){ 0, 0, (float)_screenWidth, (float)_screenHeight }, (Vector2){ 0, 0 }, 0,  WHITE);

            EndShaderMode();
        EndTextureMode();

        //Apply bicubic filter, downsample and copy _tempRT to _blurRT
        BeginTextureMode(_blurRT);
            BeginShaderMode(_copyRTShader);
            DrawTexturePro(_tempRT.texture, (Rectangle){ 0, 0, (float)_screenWidth, (float)-_screenHeight }, 
                                            (Rectangle){ 0, 0, (float)_blurRT.texture.width, 
                                                               (float)_blurRT.texture.height }, 
                                            (Vector2){ 0, 0 }, 0,  WHITE);
            EndShaderMode();
        EndTextureMode();
    }

    void RenderBloom(){
        //Render vertical pass
        BeginTextureMode(_vBlurRT);
            BeginShaderMode(_blurPass);

                Vector4 pixelSize4 = {0, (1.0f/_blurRT.texture.height)/2.0f, 0, 1.0f/_blurRT.texture.height};
                SetShaderValue(_blurPass, _blurPassPixelSizeLoc, (const float*) &pixelSize4, 4);
                SetShaderValuei(_blurPass, _blurPassBlurQualityLoc, (const int*) &_blurQuality, 1);

                Rectangle src = (Rectangle){ 0, 0, (float)_blurRT.texture.width, (float)-_blurRT.texture.height };
                Rectangle dest = (Rectangle){ 0, 0, (float)_vBlurRT.texture.width, (float)_vBlurRT.texture.height };
                DrawTexturePro(_blurRT.texture, src, dest, (Vector2){ 0, 0 }, 0,  WHITE);

            EndShaderMode();
        EndTextureMode();

        //Render horizontal pass
        BeginTextureMode(_blurRT);
            BeginShaderMode(_blurPass);

                pixelSize4 = (Vector4){(1.0f/_vBlurRT.texture.width)/2.0f, 0, 1.0f/_vBlurRT.texture.width,0};
                SetShaderValue(_blurPass, _blurPassPixelSizeLoc, (const float*) &pixelSize4, 4);
                SetShaderValuei(_blurPass, _blurPassBlurQualityLoc, (const int*) &_blurQuality, 1);

                src = (Rectangle){ 0, 0, (float)_vBlurRT.texture.width, (float)-_vBlurRT.texture.height };
                dest = (Rectangle){ 0, 0, (float)_blurRT.texture.width, (float)_blurRT.texture.height };
                DrawTexturePro(_vBlurRT.texture, src, dest, (Vector2){ 0, 0 }, 0,  WHITE);
            EndShaderMode();
        EndTextureMode();
    }


    void RenderFinalComposition(){
        BeginShaderMode(_screenShader);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, _blurRT.texture.id);
            DrawTexturePro(_baseRT.texture, (Rectangle){ 0, 0, (float)_screenWidth, (float)-_screenHeight }, 
                                            (Rectangle){ 0, 0, (float)_screenWidth, (float)_screenHeight }, (Vector2){ 0, 0 }, 0,  WHITE);
        EndShaderMode();
    }
}