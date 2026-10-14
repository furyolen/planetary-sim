#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include "vectors.h"

typedef struct particle{
    SDL_Point pos;
    Vector2D vel;
    Vector2D acc;
    int mass;
}particle;

typedef struct Univ{
    SDL_Window *window;
    SDL_Renderer *renderer;
}Univ;


bool SDLinitialize(Univ *Univ);
void cleanup(Univ *univ, bool exitStatus);
particle initialize(particle *p1);
int point_movement(particle *p1, particle *p2);
float distance_sq(SDL_Point p1, SDL_Point p2);



particle p1; 
particle p2;




int main(){
    Univ univ = {
        .renderer = NULL,
        .window = NULL
    };

    if(SDLinitialize(&univ)){
        cleanup(&univ, EXIT_FAILURE);
        printf("ts did NOT work \n :(\n");
    };

    initialize(&p1);
    initialize(&p2);

    p1.pos.x = 50;
    p1.pos.y = 50;

    p2.pos.x = 850;
    p2.pos.y = 850;


    
    printf("post of p1: %d, %d \n", p1.pos.x, p1.pos.y);
    printf("post of p2: %d, %d \n", p2.pos.x, p2.pos.y);
    
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
                case SDL_QUIT:
                    cleanup(&univ, EXIT_SUCCESS);
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode){
                        case SDL_SCANCODE_ESCAPE:
                            cleanup(&univ, EXIT_SUCCESS);
                            break;
                        default:
                            break;
                    }
                    break;

                
                default:
                    break;
            }
    }
    

    while(true){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                    case SDL_QUIT:
                        cleanup(&univ, EXIT_SUCCESS);
                        break;

                    case SDL_KEYDOWN:
                        switch(event.key.keysym.scancode){
                            case SDL_SCANCODE_ESCAPE:
                                cleanup(&univ, EXIT_SUCCESS);
                                break;
                            default:
                                break;
                        }
                        break;

                    
                    default:
                        break;
                }
        }

        printf("post of p1: %d, %d \n", p1.pos.x, p1.pos.y);
        printf("post of p2: %d, %d \n", p2.pos.x, p2.pos.y);

        point_movement(&p1, &p2);
        point_movement(&p2, &p1);
        SDL_SetRenderDrawColor(univ.renderer, 0, 0, 0, 225);
        SDL_RenderClear(univ.renderer);
        SDL_SetRenderDrawColor(univ.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_RenderDrawPoint(univ.renderer,p1.pos.x, p1.pos.y);
        SDL_RenderDrawPoint(univ.renderer,p2.pos.x, p2.pos.y);
        SDL_RenderPresent(univ.renderer);

        SDL_Delay(16);  
    }


}


particle initialize(particle *p1){
    // p1->pos.x = rand() % (201) + 400;
    // p1->pos.y = rand() % (201) + 400;


    p1->vel.x = rand() % (4) ;
    p1->vel.y = rand() % (4) ;
    
    p1->mass = 60;
}

void cleanup(struct Univ *univ, bool exit_status){

    SDL_DestroyRenderer(univ->renderer);
    SDL_DestroyWindow(univ->window);
    SDL_Quit();
    
    if(exit_status){
        fprintf(stderr, "error occured: %s \n", SDL_GetError());
    }

    exit(exit_status);

}

bool SDLinitialize(struct Univ *univ){
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        fprintf(stderr, "SDL not initialised! %s\n", SDL_GetError());
        return true;
    }

    
    univ->window = SDL_CreateWindow("nigesh", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, 0);

    if(!univ->window){
        fprintf(stderr, "error creating window: %s\n ", SDL_GetError());
        return true;
    }

    univ->renderer = SDL_CreateRenderer(univ->window, -1, 0);

    if(!univ->renderer){
        fprintf(stderr, "error creating renderer: %s\n ", SDL_GetError());
        return true;

    }

    
    
    return false;   

}

float distance_sq(SDL_Point p1, SDL_Point p2){
    return pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2);
}


int point_movement(particle *p1, particle *p2){
    float max_speed = 70; 

    int dirn_x = p2->pos.x - p1->pos.x;
    int dirn_y = p2->pos.y - p1->pos.y;
    float magnitude_p1 = sqrt(pow(dirn_x, 2) + pow(dirn_y, 2));



    
    float p1_unit_x = dirn_x / magnitude_p1;
    float p1_unit_y = dirn_y / magnitude_p1;

    //desired vector
    float des_v_x = p1_unit_x * max_speed;
    float des_v_y = p1_unit_y * max_speed;


    //resultant forces
    int f_steer_x = des_v_x - p1->vel.x;
    int f_steer_y = des_v_y - p1->vel.y;

    int accx = f_steer_x / p1->mass;
    int accy = f_steer_y / p1->mass;

    p1->pos.x = p1->pos.x + p1->vel.x;
    p1->pos.y = p1->pos.y + p1->vel.y;
    
    if(p1->pos.x > 900 || p1->pos.x < 0){
        p1->pos.x = p1->pos.x - 2 * p1->vel.x;
        p1->vel.x *= -1; 
        
        
    }else if(p1->pos.y > 900 || p1->pos.y < 0){
        p1->pos.y = p1->pos.y - 2 * p1->vel.y;
        p1->vel.y *= -1;
    }


    p1->vel.x = p1->vel.x + accx;
    p1->vel.y = p1->vel.y + accy;
}
