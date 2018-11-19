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