
#include "gfx2.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int radius;
    float x_speed;
    float y_speed;
    int speed_change;
    float x_pos;
    float y_pos;
    int food;
    int high_score;
    int green;
} Amoeba;

typedef struct {
	int radius;
	int x_location;
	int y_location;

} Food;


void start_animation(int, int);
void display_main_menu();
int update_direction(char, char, int);


int main(){
    int direction = 0;  // 0: stationary, 1: up, 2: down, 3: left, 4: right
    char key, c, start;
    int height = 700, width = 800;
    int eaten = 0, shown = 0;
    int mode = 0;
    int radius = 10;

    Amoeba particle;
    particle.radius = 10;
    particle.x_speed = .5;
    particle.y_speed = .5;
    particle.x_pos = width/2;
    particle.y_pos = height/2;
    particle.speed_change = 0;
    particle.food = 0;
    particle.high_score = 0;
    particle.green = 0;
    
    srand(time(0));

    Food food;
    food.radius = 10;
    food.x_location = rand()%width+10;
    food.y_location = rand()%height+10;




    gfx_open(width,height,"microbe.io");

    while(1){
	

	 if(mode ==  0){
		display_main_menu();
		if(gfx_event_waiting()){
			start= gfx_wait();
			if(start == ' '){
				mode = 1;
				direction = rand()%4;

				
				
			
			}
		}
	 
	 }
	 if (mode ==1){
	
	
        if(gfx_event_waiting()){
            key = gfx_wait();
        }

	direction = update_direction(key, c, direction);

        
        gfx_clear();
	gfx_circle(particle.x_pos,particle.y_pos,particle.radius);
	if(particle.green >= 255){
		gfx_text(particle.x_pos, particle.y_pos, "MAX GREEN!");
	}
	gfx_color(0,particle.green,0);
	gfx_fill_circle(particle.x_pos,particle.y_pos,particle.radius);
	gfx_color(255,255,255);
	gfx_circle(food.x_location,food.y_location,food.radius);
        if(direction == 1){
            particle.y_pos -= particle.y_speed;
        }
        else if(direction == 2){
            particle.y_pos += particle.y_speed;
        }
        else if(direction == 3){
            particle.x_pos -= particle.x_speed;
        }
        else if(direction == 4){
            particle.x_pos += particle.x_speed;
        }
        gfx_flush();

        usleep(10000);
        if(gfx_event_waiting()){
            key = gfx_wait();
            c = key;
        }

	if(abs(particle.x_pos - food.x_location) < (particle.radius + food.radius) && abs(particle.y_pos - food.y_location) < (particle.radius + food.radius)){
            // The amoeba has collided with the food
            eaten++;
            particle.food++;
            food.x_location = rand()%width+10;
            food.y_location = rand()%height+10;
	    particle.radius += particle.radius*0.16;
	    particle.x_speed += particle.x_speed*0.2;
	    particle.y_speed += particle.y_speed*0.2;
	    particle.green += 20;

	    if(particle.green > 255){
	    	particle.green = 255;
	    }

	    
	    
	
	}

	if (particle.x_pos - particle.radius <= 0 || particle.x_pos + particle.radius >= width ||
    particle.y_pos - particle.radius <= 0 || particle.y_pos + particle.radius >= height) {
    mode = 2; // set mode to a value that indicates the game has ended
}


    }

	if(mode ==2){

		char score[5], high[5];

		if(shown == 0){
		gfx_color(0,255,0);
		for(int i = 0; i<6; i++){
			gfx_fill_rectangle(0,0,width, height);
		}
		gfx_color(255,255,255);
		gfx_clear();
		shown = 1;
		}
		if(particle.food <= particle.high_score){
			
			gfx_clear();
			gfx_text(350, 150, "GAME OVER");

			gfx_text(350, 200, "You scored");
			sprintf(score, "%d", particle.food);
			gfx_text( 450, 200, score);
			gfx_text(350, 230, "High Score");
			sprintf(high, "%d", particle.high_score);
			gfx_text(450, 230, high);

			gfx_text(330, 350, "Press SPACEBAR to play again");


		}

		if(particle.food > particle.high_score){
			gfx_clear();
			gfx_text(350, 150, "NEW HIGH SCORE");

			gfx_text(350, 200, "You scored");
                        sprintf(score, "%d", particle.food);
                        gfx_text( 450, 200, score);
			gfx_text(330, 350, "Press SPACEBAR to play again!");

			particle.high_score = particle.food;
		}		

		if(gfx_event_waiting()){
			start = gfx_wait();
			if(start == ' '){
				particle.x_pos = width/2;
    				particle.y_pos = height/2;
				particle.food = 0;
				particle.green = 0;
				particle.radius = 10;
				particle.x_speed = .5;
				particle.y_speed = .5;
			
				mode =1;

			}
		}
	}
    }} 


void start_animation(int width, int height) {
    int radius = 10;
    while (radius < width/2 || radius < height/2) {
        gfx_clear();
        gfx_color(0, 255, 0);
        gfx_circle(width/2, height/2, radius);
        gfx_fill_circle(width/2, height/2, radius);
        usleep(100000);
        radius += radius*0.3;
    }
    usleep(500000);
    gfx_clear();
}

void display_main_menu(){
                gfx_text(350, 150, "MICROBE.I0");
                gfx_text(290, 250, "Use W, A, S, D to control your Amoeba");
                gfx_text(200, 200, "Help your Amoeba eat as much as possible and attain MAX GREEN Status");



                gfx_text(330, 350, "Press SPACEBAR to begin!");


}

int update_direction(char key, char c, int direction){
        if(key != 's' && key != 'w' && key != 'a' && key != 'd' && key != 258 && key != 259 && key != 260 && key != 261){
            key = c;
        }

        if(key == 's' || key == 258){
            direction = 2;
        }
        if(key == 'w' || key == 259){
            direction = 1;
        }
        if(key == 'd' || key == 260) {
            direction = 4;
        }
        if(key == 'a' || key == 261) {
            direction = 3;
        }

	return direction;
}
