#include "gameEvents.h"

std::vector<GameEvents*> GameEvents::_events;

//Macro for generating event trigger functions
#define GENERATE_EVENT_FUNCTIONS( eventName )\
void GameEvents::Trigger ## eventName () {   \
    for(GameEvents* e : _events){            \
        e->On ## eventName ();               \
    }                                        \
}

//Macro calls for every game event
GENERATE_EVENT_FUNCTIONS(MenuUpdate)
GENERATE_EVENT_FUNCTIONS(Update)
GENERATE_EVENT_FUNCTIONS(Restart)

GameEvents::GameEvents(){
    _events.push_back(this);
}

GameEvents::~GameEvents(){
    for(unsigned i=0; i<_events.size(); i++){
        //Remove "this" from the vector, doesn't maintain order
        if(_events[i] == this){
            _events[i] = _events[_events.size()-1];
            _events.pop_back();
            break;
        }
    }
}