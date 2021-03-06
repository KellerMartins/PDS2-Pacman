#include "utils.h"

double Clamp(double x, double lower, double upper){
    return x>upper? upper : (x<lower? lower : x);
}

float Lerp(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}

Vector3 Lerp(Vector3 v0, Vector3 v1, float t) {
    return (Vector3){((1 - t) * v0.x + t * v1.x),
                     ((1 - t) * v0.y + t * v1.y),
                     ((1 - t) * v0.z + t * v1.z)};
}

Color Lerp(Color v0, Color v1, float t) {
    if(t<=0) return v0;
    if(t>1) return v1;
    return (Color){(unsigned char)((1 - t) * v0.r + t * v1.r),
                   (unsigned char)((1 - t) * v0.g + t * v1.g),
                   (unsigned char)((1 - t) * v0.b + t * v1.b),
                   (unsigned char)((1 - t) * v0.a + t * v1.a)};
}

float Distance(Vector3 a, Vector3 b){
    return sqrt( (b.x-a.x)*(b.x-a.x) + 
                 (b.y-a.y)*(b.y-a.y) + 
                 (b.z-a.z)*(b.z-a.z));
}

int Step(float edge, float x ) 
{
   return x<edge? 0:1;
} 

float Smoothstep(float edge0, float edge1, float x)
{
    // Scale, bias and saturate x to 0..1 range
    x = Clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}

//Modulus functions, returning only positive values
int Modulus(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

float fModulus(float a, float b)
{
    float r = fmod(a,b);
    return r < 0 ? r + b : r;
}