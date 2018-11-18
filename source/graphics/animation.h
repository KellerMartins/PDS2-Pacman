#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>

class Animation{
    private:
        std::string _basePath;
        unsigned _frameCount;
        float _timer;
        bool _loop;
    public:
        Animation(std::string basePath, unsigned frameCount, bool loop);
        void Preload();
        void SetTimer(float time);
        std::string GetCurrentFrame();
        bool Finished();
};

#endif