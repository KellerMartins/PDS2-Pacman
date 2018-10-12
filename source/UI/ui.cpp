#include "ui.h"

std::vector<UI*> UI::_uiElements;

UI::UI(){
    _uiElements.push_back(this);
}

UI::~UI(){
    for(unsigned i=0; i<_uiElements.size(); i++){
        if(_uiElements[i] == this){
            _uiElements[i] = _uiElements[_uiElements.size()-1];
            _uiElements.pop_back();
            break;
        }
    }
}

void UI::SetPosition(float X, float Y){
    _x = X;
    _y = Y;
}

void UI::SetScale(float Width, float Height){
    _width = Clamp(Width, 0, INFINITY);
    _height = Clamp(Height, 0, INFINITY);
}

void UI::DrawUI(){
    for(UI* e : _uiElements){
        e->OnDraw();
    }
}