#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef MOVEMENT_H
#define MOVEMENT_H





Particle pointMovement(SDL_Point *render_buff, Particle *particles, SDL_Point *centers, int size){
    //for each center
    
    for(int i = 0; i<size; i++){

        //center selection strat 

            float c1, c2, c3;
            SDL_Point center = centers[0];
            c1 = distance_between_sq(centers[0], particles[i].pos);
            c3 = distance_between_sq(centers[2], particles[i].pos);
            c2 = distance_between_sq(centers[1], particles[i].pos);
            float smallest = c1;
            smallest = c2 < c1 ? (c2 < c3  ? c2 : c3) : (c1 < c3 ? c1 : c3);
            
            if(smallest == c1){
                center = centers[0];
                printf("first center\n");
            } else if(smallest == c2){
                center = centers[1];
                printf("second center\n");
            } else{
                center = centers[2];
            };


    
        float max_speed = 100;
        if(!particles[i].center){
            for(int j = 0; j<3; j++){

                //find vector towards closest center
                int dirn_x = center.x - particles[i].pos.x;
                int dirn_y = center.y - particles[i].pos.y;
                
                
                //find the vector towards each center
                // int dirn_x = centers[j].x - particles[i].pos.x;
                // int dirn_y = centers[j].y - particles[i].pos.y;
                float magnitude_p1 = sqrt(pow(dirn_x, 2) + pow(dirn_y, 2));
    
    
    
                
                float p1_unit_x = dirn_x / magnitude_p1;
                float p1_unit_y = dirn_y / magnitude_p1;
    
                //chahat wala vector 
                float des_v_x = p1_unit_x * max_speed;
                float des_v_y = p1_unit_y * max_speed;
    
    
                //resultant forces
                int f_steer_x = des_v_x - particles[i].vel.x;
                int f_steer_y = des_v_y - particles[i].vel.y;
    
                int accx = f_steer_x / particles[i].mass;
                int accy = f_steer_y / particles[i].mass;
    
                particles[i].pos.x = particles[i].pos.x + particles[i].vel.x;
                particles[i].pos.y = particles[i].pos.y + particles[i].vel.y;
    
                if(particles[i].pos.x > 900 || particles[i].pos.x < 0){
                    particles[i].pos.x = particles[i].pos.x - 2 * particles[i].vel.x;
                    particles[i].vel.x *= -1; 
                    
                }else if(particles[i].pos.y > 900 || particles[i].pos.y < 0){
                    particles[i].pos.y = particles[i].pos.y - 2 * particles[i].vel.y;
                    particles[i].vel.y *= -1;
                }
                
                particles[i].vel.x = particles[i].vel.x + accx;
                particles[i].vel.y = particles[i].vel.y + accy;
    
                render_buff[i] = particles[i].pos;
                
            }




        }

    }

};

#endif