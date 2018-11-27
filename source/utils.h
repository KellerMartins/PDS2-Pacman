#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>
#include <cmath>
#include <iostream>
#include <fstream>

#define VECTOR4_ZERO (Vector4){0.0,0.0,0.0,0.0}
#define VECTOR3_ZERO (Vector3){0.0,0.0,0.0}
#define VECTOR2_ZERO (Vector2){0.0,0.0}

double Clamp(double x, double lower, double upper);
float Lerp(float v0, float v1, float t);
Vector3 Lerp(Vector3 v0, Vector3 v1, float t);
Color Lerp(Color v0, Color v1, float t);

float Distance(Vector3 a, Vector3 b);

int Step(float edge, float x );
float Smoothstep(float edge0, float edge1, float x);

//Modulus functions, returning only positive values
int Modulus(int a, int b);
float fModulus(float a, float b);

#endif
