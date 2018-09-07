#include "rendering.h"

RenderManager::RenderManager(){
    _screenWidth = _screenHeight = 0;
    _blurDownscale = 0;
    glewInit();
    ReloadShaders();

    Image blackImg = GenImageColor(2,2,BLACK);
    _blackTexture = LoadTextureFromImage(blackImg);
    UnloadImage(blackImg);
}

RenderManager::~RenderManager(){
    UnloadShader(outlineFilter);
    UnloadShader(maskShader);
    UnloadShader(colorShader);
    UnloadShader(blurPass);
    UnloadShader(screenShader);
    UnloadShader(copyRTShader);

    UnloadTexture(_blackTexture);

    if(_screenWidth!=0 || _screenHeight!=0){
        UnloadRenderTexture(maskRT);
        UnloadRenderTexture(baseRT);
        UnloadRenderTexture(tempRT);
        UnloadRenderTexture(blurRT);
        UnloadRenderTexture(vBlurRT);
    }
}

void RenderManager::SetBloomDownscale(unsigned value){
    _blurDownscale = value;

    if(_screenWidth!=0 || _screenHeight!=0){
        UnloadRenderTexture(blurRT);
        UnloadRenderTexture(vBlurRT);
    }
    blurRT = LoadRenderTexture(_screenWidth/(1+_blurDownscale), _screenHeight/(1+_blurDownscale));
    vBlurRT = LoadRenderTexture(_screenWidth/(1+_blurDownscale), _screenHeight/(1+_blurDownscale));
};

void RenderManager::SetResolution(int width, int height){

    //Unload rts if they already been created (when changing resolution)
    if(_screenWidth!=0 || _screenHeight!=0){
        UnloadRenderTexture(maskRT);
        UnloadRenderTexture(baseRT);
        UnloadRenderTexture(tempRT);
        UnloadRenderTexture(blurRT);
        UnloadRenderTexture(vBlurRT);
    }

    _screenWidth = width<800? 800 : width;
    _screenHeight = height<600? 600 : height;

    maskRT = LoadRenderTexture(width, height);
    baseRT = LoadRenderTexture(width, height);
    tempRT = LoadRenderTexture(width, height);

    blurRT = LoadRenderTexture(width/(1+_blurDownscale), height/(1+_blurDownscale));
    vBlurRT = LoadRenderTexture(width/(1+_blurDownscale), height/(1+_blurDownscale));

    SetShaderUniforms();
}

void RenderManager::ReloadShaders(){
    outlineFilter = LoadShader("assets/shaders/fxBase.vs","assets/shaders/outlineFilter.fs");
    blurPass = LoadShader("assets/shaders/fxBase.vs","assets/shaders/blurPass.fs");
    screenShader = LoadShader("assets/shaders/fxBase.vs","assets/shaders/screen.fs");
    copyRTShader = LoadShader("assets/shaders/fxBase.vs","assets/shaders/copyRT.fs");

    maskShader = LoadShader("assets/shaders/modelVert.vs","assets/shaders/maskFrag.fs");
    colorShader = LoadShader("assets/shaders/modelVert.vs","assets/shaders/colorFrag.fs");

    SetShaderUniforms();
}

void RenderManager::SetShaderUniforms(){

    blurPassPixelSizeLoc = GetShaderLocation(blurPass, "xyHalfPixelSize_zwPixelSize");
    copyRTPixelSizeLoc = GetShaderLocation(copyRTShader, "PixelSize");

    int texIndex = 1;
    SetShaderValuei(outlineFilter, GetShaderLocation(outlineFilter, "texture1"), &texIndex, 1);
    SetShaderValuei(blurPass, GetShaderLocation(blurPass, "texture1"), &texIndex, 1);
    SetShaderValuei(screenShader, GetShaderLocation(screenShader, "texture1"), &texIndex, 1);

    float val = _screenWidth;
    SetShaderValue(outlineFilter, GetShaderLocation(outlineFilter, "width"), &val, 1);
    SetShaderValue(screenShader, GetShaderLocation(screenShader, "width"), &val, 1);
    val = _screenHeight;
    SetShaderValue(outlineFilter, GetShaderLocation(outlineFilter, "height"), &val, 1);
    SetShaderValue(screenShader, GetShaderLocation(screenShader, "height"), &val, 1);

    val = 0.25;
    SetShaderValue(screenShader, GetShaderLocation(screenShader, "vignettePower"), &val, 1);
}

std::list<Object3D*>::iterator RenderManager::RegisterObjectToRender(Object3D *obj){
    return _objectsToRender.insert(_objectsToRender.begin(), obj);
}

void RenderManager::RemoveObjectFromRenderer(std::list<Object3D*>::iterator objID){
    _objectsToRender.erase(objID);
}

void RenderManager::Render(){
    if(_screenWidth==0 || _screenHeight==0) return;

    BeginDrawing();
        ClearBackground(BLACK);

        RenderBaseEffect();
        RenderBloom();
        RenderFinalComposition();

        DrawFPS(0,0);

    EndDrawing();
}

void RenderManager::RenderBaseEffect(){
    //Render mask to maskRT
    BeginTextureMode(maskRT);    
        BeginMode3D(camera);  
            for(Object3D* m : _objectsToRender){
                m->model.material.shader = maskShader;
                DrawModelEx(m->model, m->position, m->rotationAxis,m->rotationAngle,m->scale, WHITE);
            } 
            DrawGrid(1000,10);
        EndMode3D();
    EndTextureMode();

    //Render base color to baseRT
    BeginTextureMode(baseRT);    
        BeginMode3D(camera);  
            for(Object3D* m : _objectsToRender){
                m->model.material.shader = colorShader;
                DrawModelEx(m->model, m->position, m->rotationAxis,m->rotationAngle,m->scale, m->model.material.maps[MAP_DIFFUSE].color);
            }  
            DrawGrid(1000,10);
        EndMode3D();
    EndTextureMode();

    //Render mask and base with outline shader to tempRT        
    BeginTextureMode(tempRT);
        BeginShaderMode(outlineFilter);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, baseRT.texture.id);

            DrawTexturePro(maskRT.texture, (Rectangle){ 0, 0, (float)screenWidth, (float)-screenHeight }, 
                                            (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0,  WHITE);

            glActiveTexture(0);
        EndShaderMode();
    EndTextureMode();

    //Apply bicubic filter and copy from tempRT to baseRT
    BeginTextureMode(baseRT);
        BeginShaderMode(copyRTShader);

        Vector2 pixelSize = {1.0f/screenWidth, 1.0f/screenHeight};
        SetShaderValue(copyRTShader, copyRTPixelSizeLoc, (const float*) &pixelSize, 2);

        DrawTexturePro(tempRT.texture, (Rectangle){ 0, 0, (float)screenWidth, (float)-screenHeight }, 
                                        (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0,  WHITE);

        EndShaderMode();
    EndTextureMode();
}

void RenderManager::RenderBloom(){
    //Render vertical pass
    BeginTextureMode(vBlurRT);
        BeginShaderMode(blurPass);

            Vector4 pixelSize4 = {0, (1.0f/screenHeight)/2.0f, 0, 1.0f/screenHeight};
            SetShaderValue(blurPass, blurPassPixelSizeLoc, (const float*) &pixelSize4, 4);

            DrawTexturePro(baseRT.texture, (Rectangle){ 0, 0, (float)screenWidth, (float)-screenHeight }, 
                                            (Rectangle){ 0, 0, (float)vBlurRT.texture.width, (float)vBlurRT.texture.height }, (Vector2){ 0, 0 }, 0,  WHITE);

        EndShaderMode();
    EndTextureMode();

    //Render horizontal pass
    BeginTextureMode(blurRT);
        BeginShaderMode(blurPass);

            pixelSize4 = (Vector4){(1.0f/screenWidth)/2.0f, 0, 1.0f/screenWidth,0};
            SetShaderValue(blurPass, blurPassPixelSizeLoc, (const float*) &pixelSize4, 4);
            DrawTexturePro(vBlurRT.texture, (Rectangle){ 0, 0, (float)vBlurRT.texture.width, (float)-vBlurRT.texture.height }, 
                                            (Rectangle){ 0, 0, (float)blurRT.texture.width, (float)blurRT.texture.height }, (Vector2){ 0, 0 }, 0,  WHITE);
        EndShaderMode();
    EndTextureMode();
}


void RenderManager::RenderFinalComposition(){
    BeginShaderMode(screenShader);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, blurRT.texture.id);
        DrawTexturePro(baseRT.texture, (Rectangle){ 0, 0, (float)screenWidth, (float)-screenHeight }, 
                                        (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight }, (Vector2){ 0, 0 }, 0,  WHITE);
    EndShaderMode();
}