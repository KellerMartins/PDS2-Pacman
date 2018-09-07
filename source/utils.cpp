#include "utils.h"

double Clamp(double x, double upper, double lower){
    return x>upper? upper : (x<lower? lower : x);
}

char ClampColor(double x){
    return x>255? (char)255 : (x<0? 0 : x);
}

double Logistic(double x, double x0, double max, double steepness){
    return max/(1+exp(-steepness*(x-x0)));
}

double Gaussian(double x, double x0, double max, double steepness){
    return max*exp(-( (x-x0)*(x-x0) )/(2*steepness*steepness));
}

float Lerp(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}

Color Lerp(Color v0, Color v1, float t) {
    return (Color){(unsigned char)((1 - t) * v0.r + t * v1.r),
                   (unsigned char)((1 - t) * v0.g + t * v1.g),
                   (unsigned char)((1 - t) * v0.b + t * v1.b),
                   (unsigned char)((1 - t) * v0.a + t * v1.a)};
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

//Modulus function, returning only positive values
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