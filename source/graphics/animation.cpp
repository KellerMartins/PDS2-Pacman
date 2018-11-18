#include "animation.h"

#include <sstream>
#include "object3D.h"
#include "../utils.h"

Animation::Animation(std::string basePath, unsigned frameCount, bool loop){
    _basePath = basePath;
    _frameCount = frameCount;
    _timer = 0;
    _loop = loop;
}

void Animation::Preload(){
    for(unsigned i=0; i<_frameCount; i++){
        std::ostringstream stream;
        stream << _basePath << "_" << i << ".obj";
        Object3D::Cache3DModel(stream.str());
    }
}

void Animation::SetTimer(float time){
    if(!_loop && time > _frameCount-1){
        _timer = _frameCount-1;
    }else{
        _timer = fModulus(time,_frameCount);
    }
}

std::string Animation::GetCurrentFrame(){
    int frame = (int)_timer;
    std::ostringstream stream;
    stream << _basePath << "_" << frame << ".obj";
    return stream.str();
}

bool Animation::Finished(){
    if(_loop) return false;
    return ((unsigned)_timer == _frameCount-1);
}