#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>


/*
    A 2D Vector with x and y components
*/
typedef struct Vector2D
{
    float x;
    float y;
} Vector2D;


typedef struct Particle{
    SDL_Point pos;
    Vector2D vel;
    Vector2D acc;
    int mass;
    bool center;
} Particle;



/*
    Adds two vectors
*/
Vector2D add_vectors(Vector2D vec1, Vector2D vec2) 
{
    Vector2D newv = {vec1.x + vec2.x, vec1.y + vec2.y};
    return newv;
}

/*s
    Subtracts two vectors
*/
Vector2D sub_vectors(Vector2D vec1, Vector2D vec2) 
{
    Vector2D newv = {vec1.x - vec2.x, vec1.y - vec2.y};
    return newv;
}


/*
    Returns unit vector of the given vector
*/


Vector2D unit_vector(Vector2D vec)
{
    float factor = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    Vector2D newv = {vec.x / factor, vec.y / factor};
    return newv;
}

/*
    Returns the magnitude of the given vector
*/

float magnitude_of(Vector2D vec)
{
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

/*
    Returns the dot product of the two vectors
*/


float dot_vectors(Vector2D vec1, Vector2D vec2) 
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

/*
    Returns the angle between two vectors in radians
*/

float angle_between_rad(Vector2D vec1, Vector2D vec2)
{
    float mod1 = magnitude_of(vec1);
    float mod2 = magnitude_of(vec2);

    if (mod1 == 0 || mod2 == 0) return 0;

    return acosf(dot_vectors(vec1, vec2) / (magnitude_of(vec1) * magnitude_of(vec2)));
}

/*
    Returns the angle between two vectors in degrees
*/


float angle_between_deg(Vector2D vec1, Vector2D vec2)
{
    return angle_between_rad(vec1, vec2) * 180 / 3.141592653589;
}

float distance_between_sq(SDL_Point p1, SDL_Point p2){
    return  (pow(p1.x-p2.y, 2) + pow(p1.y- p2.y, 2));
}