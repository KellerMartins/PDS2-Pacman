#include "ui.h"

enum ElementType{UIText, UITextCentered, UIImage};
struct UIElement{
    ElementType type;
    float x;
    float y;
    float width;
    float height;
    std::string string;

    Color colorTint;

    UIElement(std::string ImagePath, float X, float Y, float Width, float Height){
        string = ImagePath;
        type = ElementType::UIImage;
        x = X;
        y = Y;
        width = Width;
        height = Height;
    }
    UIElement(std::string Text, float X, float Y, float Size, float Spacing, Color ColorTint, bool Centered){
        colorTint = ColorTint;
        string = Text;
        type = Centered? ElementType::UITextCentered : ElementType::UIText;
        x = X;
        y = Y;
        width = Spacing;
        height = Size;
    }
};

namespace UI{

    Font _font;
    bool _loadedFont = false;

    std::vector<UIElement> _elements;
    std::map<std::string, Texture2D> _images;

    Texture2D& GetImage(std::string path){
        auto imageIt = _images.find(path);
        if(imageIt == _images.end()){
            Texture2D& imageTex = _images[path];

            imageTex = LoadTexture(path.c_str());
            SetTextureFilter(imageTex, FILTER_TRILINEAR);
            GenTextureMipmaps(&imageTex);
        }

        return _images[path];
    }


    void SetFont(std::string fontPath){
        _loadedFont = true;
        _font = LoadFont(fontPath.c_str());
        SetTextureFilter(_font.texture, FILTER_TRILINEAR);
        GenTextureMipmaps(&_font.texture);
    }

    void DrawImage(std::string imagePath, float X, float Y, float Width, float Height){
        _elements.push_back(UIElement(imagePath, X, Y, Width, Height));
    }

    void DrawText(std::string Text, float X, float Y, float Size, float Spacing, Color ColorTint){
        _elements.push_back(UIElement(Text, X, Y, Size, Spacing, ColorTint, false));
    }

    void DrawTextCentered(std::string Text, float X, float Y, float Size, float Spacing, Color ColorTint){
        _elements.push_back(UIElement(Text, X, Y, Size, Spacing, ColorTint, true));
    }

    void Render(){
        float screenWidth = (float)RenderManager::GetWidth();
        float screenHeight = (float)RenderManager::GetHeight();

        if(!_loadedFont){
            _font = GetFontDefault();
        }

        for(UIElement& e : _elements){
            if(e.type == ElementType::UIImage){
                
                Texture2D& imageTex = GetImage(e.string);

                float aspectRatio = (float)imageTex.width/(float)imageTex.height;
                
                float positionX = e.x*screenWidth  - e.width*screenHeight*aspectRatio/2;
                float positionY = e.y*screenHeight - e.height*screenHeight/2;

                DrawTexturePro(imageTex, (Rectangle){0, 0, (float)imageTex.width, (float)imageTex.height}, 
                                         (Rectangle){positionX, positionY, e.width*screenHeight*aspectRatio, e.height*screenHeight}, 
                                         VECTOR2_ZERO, 0,WHITE);
            }
            else if(e.type == ElementType::UIText || e.type == ElementType::UITextCentered){
                
                Vector2 position;
                position.x = e.x*screenWidth;
                position.y = e.y*screenHeight;
                float fontSize = e.height*screenHeight/100;
                float spacing = e.width*screenHeight/100;

                if(e.type == ElementType::UITextCentered){
                    Vector2 textSize = MeasureTextEx(_font, e.string.c_str(), fontSize, spacing);
                    position.x -= textSize.x/2;
                    position.y -= textSize.y/2;

                }

                DrawTextEx(_font, e.string.c_str(), position, fontSize, spacing, e.colorTint);
            }
        }

        _elements.clear();
    }

}