#ifndef POINTGEN_H
#define POINTGEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <stdbool.h>


int pointGen(SDL_Point *render_buff, Particle *particles, int size){
    for(int n=0; n<size;n++){
            if(particles[n].center){}
            render_buff[n].x = rand() % 900 + 10;
            render_buff[n].y = rand() % (900) + 10;     
        // if(n%10 == 0) printf("x: %d, y: %d \n", render_buff[n].x, render_buff[n].y);
            particles[n].pos = render_buff[n];
            particles[n].vel.x = rand() % 75 - 50;
            particles[n].vel.y = rand() % 75 - 50;
            particles[n].center = false;
            particles[n].mass = 50;
    }
}

#endif