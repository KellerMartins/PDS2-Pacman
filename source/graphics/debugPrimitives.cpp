#include "debugPrimitives.h"

#include <list>

namespace RenderManager{

    #ifndef NDEBUG
    class DebugPrimitive{
        public:
            virtual void Draw() = 0;
            virtual ~DebugPrimitive(){}
    };

    class DebugLine : public DebugPrimitive{
        private:
            Vector3 _start;
            Vector3 _end;
            Color _color;
        public:
            DebugLine(Vector3 start, Vector3 end, Color color) : _start(start), 
                                                                _end(end), 
                                                                _color(color) {}
            virtual void Draw(){
                DrawLine3D(_start, _end, _color);
            }
    };

    class DebugSphere: public DebugPrimitive{
        private:
            Vector3 _center;
            float _radius;
            Color _color;
        public:
            DebugSphere(Vector3 center, float radius, Color color) : _center(center), 
                                                                    _radius(radius), 
                                                                    _color(color) {}
            virtual void Draw(){
                DrawSphereWires(_center, _radius, 4, 8, _color);
            }
    };

    class DebugCube: public DebugPrimitive{
        private:
            Vector3 _position;
            Vector3 _size;
            Color _color;
        public:
            DebugCube(Vector3 position, Vector3 size, Color color) : _position(position), 
                                                                    _size(size), 
                                                                    _color(color) {}
            virtual void Draw(){
                DrawCubeWires(_position, _size.x, _size.y, _size.z, _color);
            }
    };

    std::list<DebugPrimitive*> _debugPrimitivesToRender;
    #endif



    void DrawDebugLine(Vector3 startPos, Vector3 endPos, Color color){
        #ifndef NDEBUG
        _debugPrimitivesToRender.push_back(new DebugLine(startPos, endPos, color));
        #endif
    }

    void DrawDebugCube(Vector3 position, Vector3 size, Color color){
        #ifndef NDEBUG
        _debugPrimitivesToRender.push_back(new DebugCube(position, size, color));
        #endif
    }

    void DrawDebugSphere(Vector3 centerPos, float radius, Color color){
        #ifndef NDEBUG
        _debugPrimitivesToRender.push_back(new DebugSphere(centerPos, radius, color));
        #endif
    }

    void RenderDebugPrimitives(Camera &camera){
        #ifndef NDEBUG
        BeginMode3D(camera);  
            for(DebugPrimitive* p : _debugPrimitivesToRender){
                p->Draw();
                delete p;
            }  
        EndMode3D();

        _debugPrimitivesToRender.clear();
        #endif
    }
}