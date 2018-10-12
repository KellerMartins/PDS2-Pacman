#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H
#include <vector>
#include <string>
#include <iostream>

// Macro for generating event headers. Remember to call this inside
// the GameEvents class and the function generator inside gameEvents.cpp
// for every new event created
#define GENERATE_EVENT_HEADERS( eventName ) \
        virtual void On ## eventName () {};   \
        static void Trigger ## eventName ();

class GameEvents{
    private:
        static std::vector<GameEvents*> _events;
    public:
        GameEvents();
        virtual ~GameEvents();
        GENERATE_EVENT_HEADERS(Update)
        GENERATE_EVENT_HEADERS(Test)
};

#endif