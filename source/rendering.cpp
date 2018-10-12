#include "rendering.h"

namespace RenderManager{

    //Camera
    Camera camera = (Camera){{0,0,0}, {0,0,1}, {0,1,0}, 45.0f, CAMERA_PERSPECTIVE};

    //Internal globals
    //State
    bool _isInitialized = false;

    //Resolutions
    int _screenWidth = 0;
    int _screenHeight= 0;
    int _blurDownscale = 0;

    //Shaders and uniform locations
    Shader _blurPass;
    Shader _maskShader;
    Shader _colorShader;
    Shader _outlineFilter;
    Shader _screenShader;
    Shader _copyRTShader;
    int _blurPassPixelSizeLoc;
    int _copyRTPixelSizeLoc;

    //Render Textures and textures
    RenderTexture2D _maskRT;
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


    //Debug drawing
    #ifdef _DEBUG
    enum DebugPrimitiveType{Line, Cube, Sphere};
    class DebugDrawPrimitive{
        public:
            DebugPrimitiveType type;
            Vector3 position;
            Vector3 vectorParameter;
            float floatParameter;
            Color color;
    };

    std::list<DebugDrawPrimitive> _debugPrimitivesToRender;
    #endif


    void Init(int screenWidth, int screenHeight, std::string windowTitle){
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        InitWindow(screenWidth, screenHeight, windowTitle.c_str());
        SetTargetFPS(0);

        LoadShaders();
        SetResolution(screenWidth, screenHeight);
        glewInit();

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
        
            UnloadRenderTexture(_maskRT);
            UnloadRenderTexture(_baseRT);
            UnloadRenderTexture(_tempRT);
            UnloadRenderTexture(_blurRT);
            UnloadRenderTexture(_vBlurRT);

            CloseWindow();
        }
        _isInitialized = false;
    }

    void SetBloomDownscale(unsigned value){
        _blurDownscale = value;

        if(_isInitialized){
            UnloadRenderTexture(_blurRT);
            UnloadRenderTexture(_vBlurRT);
        }
        _blurRT = LoadRenderTexture(_screenWidth/(1+_blurDownscale), _screenHeight/(1+_blurDownscale));
        _vBlurRT = LoadRenderTexture(_screenWidth/(1+_blurDownscale), _screenHeight/(1+_blurDownscale));
    };

    void SetResolution(int width, int height){

        //Unload rts if they already been created
        if(_isInitialized){
            UnloadRenderTexture(_maskRT);
            UnloadRenderTexture(_baseRT);
            UnloadRenderTexture(_tempRT);
            UnloadRenderTexture(_blurRT);
            UnloadRenderTexture(_vBlurRT);
        }

        _screenWidth = width<800? 800 : width;
        _screenHeight = height<600? 600 : height;

        SetWindowSize(_screenWidth, _screenHeight);

        _maskRT = LoadRenderTexture(_screenWidth, _screenHeight);
        _baseRT = LoadRenderTexture(_screenWidth, _screenHeight);
        _tempRT = LoadRenderTexture(_screenWidth, _screenHeight);

        _blurRT = LoadRenderTexture(_screenWidth/(1+_blurDownscale), _screenHeight/(1+_blurDownscale));
        _vBlurRT = LoadRenderTexture(_screenWidth/(1+_blurDownscale), _screenHeight/(1+_blurDownscale));

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
        _copyRTPixelSizeLoc = GetShaderLocation(_copyRTShader, "PixelSize");

        int texIndex = 1;
        SetShaderValuei(_outlineFilter, GetShaderLocation(_outlineFilter, "texture1"), &texIndex, 1);
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

    void Render(){
        UpdateCamera(&camera);
        if(!_isInitialized) return;
        
        BeginDrawing();
            ClearBackground(BLACK);

            RenderBaseEffect();
            RenderBloom();
            RenderFinalComposition();

            UI::DrawUI();

            RenderDebugPrimitives();

            DrawFPS(0,0);

        EndDrawing();
    }

    void RenderBaseEffect(){
        //Render mask to _maskRT
        BeginTextureMode(_maskRT);    
            BeginMode3D(camera);  
                for(Object3D* m : _objectsToRender){
                    if(!m->HasModel() || !m->shouldRender) continue;

                    m->model.material.shader = _maskShader;
                    DrawModelEx(m->model, m->position, m->rotationAxis,m->rotationAngle,m->scale, WHITE);
                } 
            EndMode3D();
        EndTextureMode();

        //Render base color to _baseRT
        BeginTextureMode(_baseRT);    
            BeginMode3D(camera);  
                for(Object3D* m : _objectsToRender){
                    if(!m->HasModel() || !m->shouldRender) continue;
                    
                    m->model.material.shader = _colorShader;
                    DrawModelEx(m->model, m->position, m->rotationAxis,m->rotationAngle,m->scale, m->model.material.maps[MAP_DIFFUSE].color);
                    m->model.material.shader = GetShaderDefault();
                }  
            EndMode3D();
        EndTextureMode();

        //Render mask and base with outline shader to _tempRT        
        BeginTextureMode(_tempRT);
            BeginShaderMode(_outlineFilter);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, _baseRT.texture.id);

                DrawTexturePro(_maskRT.texture, (Rectangle){ 0, 0, (float)_screenWidth, (float)-_screenHeight }, 
                                                (Rectangle){ 0, 0, (float)_screenWidth, (float)_screenHeight }, (Vector2){ 0, 0 }, 0,  WHITE);

                glActiveTexture(0);
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
    }

    void RenderBloom(){
        //Render vertical pass
        BeginTextureMode(_vBlurRT);
            BeginShaderMode(_blurPass);

                Vector4 pixelSize4 = {0, (1.0f/_screenHeight)/2.0f, 0, 1.0f/_screenHeight};
                SetShaderValue(_blurPass, _blurPassPixelSizeLoc, (const float*) &pixelSize4, 4);

                DrawTexturePro(_baseRT.texture, (Rectangle){ 0, 0, (float)_screenWidth, (float)-_screenHeight }, 
                                                (Rectangle){ 0, 0, (float)_vBlurRT.texture.width, (float)_vBlurRT.texture.height }, (Vector2){ 0, 0 }, 0,  WHITE);

            EndShaderMode();
        EndTextureMode();

        //Render horizontal pass
        BeginTextureMode(_blurRT);
            BeginShaderMode(_blurPass);

                pixelSize4 = (Vector4){(1.0f/_screenWidth)/2.0f, 0, 1.0f/_screenWidth,0};
                SetShaderValue(_blurPass, _blurPassPixelSizeLoc, (const float*) &pixelSize4, 4);
                DrawTexturePro(_vBlurRT.texture, (Rectangle){ 0, 0, (float)_vBlurRT.texture.width, (float)-_vBlurRT.texture.height }, 
                                                (Rectangle){ 0, 0, (float)_blurRT.texture.width, (float)_blurRT.texture.height }, (Vector2){ 0, 0 }, 0,  WHITE);
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



    //Debug drawing functions
    void DrawDebugLine(Vector3 startPos, Vector3 endPos, Color color){
        #ifdef _DEBUG
        DebugDrawPrimitive p;
        p.type = DebugPrimitiveType::Line;
        p.position = startPos;
        p.vectorParameter = endPos;
        p.color = color;

        _debugPrimitivesToRender.insert(_debugPrimitivesToRender.end(), p);
        #endif
    }

    void DrawDebugCube(Vector3 position, Vector3 size, Color color){
        #ifdef _DEBUG
        DebugDrawPrimitive p;
        p.type = DebugPrimitiveType::Cube;
        p.position = position;
        p.vectorParameter = size;
        p.color = color;

        _debugPrimitivesToRender.insert(_debugPrimitivesToRender.end(), p);
        #endif
    }

    void DrawDebugSphere(Vector3 centerPos, float radius, Color color){
        #ifdef _DEBUG
        DebugDrawPrimitive p;
        p.type = DebugPrimitiveType::Sphere;
        p.position = centerPos;
        p.floatParameter = radius;
        p.color = color;

        _debugPrimitivesToRender.insert(_debugPrimitivesToRender.end(), p);
        #endif
    }

    void RenderDebugPrimitives(){
        #ifdef _DEBUG
        BeginMode3D(camera);  
            for(DebugDrawPrimitive &p : _debugPrimitivesToRender){
                switch(p.type){
                    case DebugPrimitiveType::Line:
                        DrawLine3D(p.position, p.vectorParameter, p.color);
                    break;
                    case DebugPrimitiveType::Cube:
                        DrawCubeWires(p.position, p.vectorParameter.x, p.vectorParameter.y, p.vectorParameter.z, p.color);
                    break;
                    case DebugPrimitiveType::Sphere:
                        DrawSphereWires(p.position, p.floatParameter, 4, 8, p.color);
                    break;
                }
            }  
        EndMode3D();

        _debugPrimitivesToRender.clear();
        #endif
    }
}