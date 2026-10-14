#include "vectors.h"
#include "movement.h"
#include "pointgen.h"
#include <stdbool.h>



#define WinTitle "Sim ong frfr"
#define width 900
#define height 900
#define start_val 900
#define max_val 1000
#define G  0.0674

typedef struct Univ{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Univ;



int size = 900;
SDL_mutex *datalock = NULL;



bool SDLinitialize(struct Univ *univ);
void cleanup(Univ *univ, bool exitStatus, SDL_Point *render_buff, Particle *particles);


int main(){
    Univ univ = {
        .window = NULL,
        .renderer = NULL,
    };

    datalock = SDL_CreateMutex();

    SDL_Point *render_buff = (SDL_Point *)malloc(sizeof(SDL_Point)*size);
    Particle *particles = (Particle *)malloc(sizeof(Particle)*size);

    pointGen(render_buff, particles, size);

    //picking centers
    SDL_Point centers[3]; 
    

    for(int i = 0; i < 3; i++) {
        int valid = 0; 

        while (!valid) {
            int r = rand() % 801 + 100; 

            int inter = 0;

            for (int j = 0; j < i; j++) {
                if (distance_between_sq(render_buff[r], centers[j]) < 75*75) {
                    inter = 1;
                    break; 
                }
            }

            if (!inter) {
                centers[i].x = render_buff[r].x;
                centers[i].y = render_buff[r].y; 
                particles[r].center = true;
                valid = 1; 
            }
        }
    }


    if(SDLinitialize(&univ)){
        cleanup(&univ, EXIT_FAILURE, render_buff, particles);
        printf("ts did NOT work \n");
    };

    

    printf("it worked :D \n");

    while(true){

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    cleanup(&univ, EXIT_SUCCESS, render_buff, particles);
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode){
                        SDL_Point *temp;
                        Particle *temp_particle;
                        case SDL_SCANCODE_ESCAPE:
                            cleanup(&univ, EXIT_SUCCESS, render_buff, particles);
                            break;
                        case SDL_SCANCODE_D:

                            //locking the thread
                            if(SDL_LockMutex(datalock)==0){
                                if(size>20){
                                    int i = size - 10;
                                    temp = (SDL_Point *)realloc(render_buff, sizeof(SDL_Point)*i);
                                    temp_particle = (Particle *)realloc(particles, sizeof(Particle)* i);
                                    if(temp != NULL && temp_particle != NULL){
                                        render_buff = temp;
                                        size = i;

                                        for(int j = 0; j<size; j++){
                                            temp_particle[j].pos = temp[j];
                                        }

                                        particles = temp_particle;

                                        break;
                                    }else{
                                        fprintf(stderr, "an error occured deleting particles");
                                        break;
                                    }
                                    break;
                                }else{
                                    break;
                                }

                                SDL_UnlockMutex(datalock);

                            }
                        case SDL_SCANCODE_A:

                            if(SDL_LockMutex(datalock) == 0){
                                int j = size + 50;
                                int count = 0;
                                temp= (SDL_Point *)realloc(render_buff, sizeof(SDL_Point)*j);
                                temp_particle = (Particle *)realloc(particles, sizeof(Particle)* j);
                                printf("realloc is executing \n");

                                if(temp != NULL){
                                    for(int n = size; n<j;n++){
                                        temp[n].x = rand() % 900 + 10;
                                        temp[n].y = rand() % (900) + 10;
                                        count++;
                                    }
                                    render_buff = temp;
                                    size = j;


                                    //initialize new particles
                                    for(int i = 0; i<size; i++){
                                        temp_particle[i].pos = temp[i];
                                        temp_particle[i].vel.x = rand() % 75 - 50;
                                        temp_particle[i].vel.y = rand() % 75 - 50;
                                        temp_particle[i].acc.x = 0;
                                        temp_particle[i].acc.y = 0;
                                        temp_particle[i].mass = 50;
                                    }

                                    particles = temp_particle;
                                }else{
                                    printf("an error occured adding particles: %s", SDL_GetError());
                                }
                                SDL_UnlockMutex(datalock);
                                break;
                            }
                            break;
                        default:
                            break;
                    }
                    break;

                
                default:
                    break;
            }
        }

        if (SDL_LockMutex(datalock) ==0){
            pointMovement(render_buff, particles, centers, size);
            SDL_UnlockMutex(datalock);
        }else{
            printf("could not move points: %s\n", SDL_GetError());
        }



        
        SDL_SetRenderDrawColor(univ.renderer, 0, 0, 0, 225);
        SDL_RenderClear(univ.renderer);
        SDL_SetRenderDrawColor(univ.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawPoints(univ.renderer, render_buff, size);
        SDL_RenderDrawPoints(univ.renderer, centers, 3); //hardcoded deal w it later 
        SDL_RenderPresent(univ.renderer);

        SDL_Delay(50);  
    }

    cleanup(&univ, EXIT_SUCCESS, render_buff, particles);
}


void cleanup(struct Univ *univ, bool exit_status, SDL_Point *render_buff, Particle *particles){

    SDL_DestroyRenderer(univ->renderer);
    SDL_DestroyWindow(univ->window);
    SDL_DestroyMutex(datalock);
    SDL_Quit();
    free(render_buff);
    free(particles);
    
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

    //the output to SDL_CreateWindow is a pointer to the SDL window that is created
    //so we assign it to the window instance of the struct we're passing to the function
    univ->window = SDL_CreateWindow(WinTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

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



