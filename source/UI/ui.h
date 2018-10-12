#ifndef UI_H
#define UI_H

#include <vector>
#include "../utils.h"

class UI{
    private:
        static std::vector<UI*> _uiElements;

    protected:
        float _x;
        float _y;
        float _width;
        float _height;

    public:
        const float& x = _x;
        const float& y = _y;
        const float& width = _width;
        const float& height = _height;

        UI();
        virtual ~UI();

        void SetPosition(float X, float Y);
        void SetScale(float Width, float Height);

        virtual void OnDraw() {}

        static void DrawUI();
};

#endif