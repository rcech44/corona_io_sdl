#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "sdl.h"

typedef struct {
  double x, y;
  double radius;
  int rotationDir;
  double rotationSpeed;
  double rotation;
  int dir;
  int speedX;
  int speedY;
  int alive;
} Ball;

bool ball_collides(Ball *b1, Ball *b2) {

  if (sqrt((abs(b1->x - b2->x))*(abs(b1->x - b2->x)) + (abs(b1->y - b2->y))*(abs(b1->y - b2->y))) > (b1->radius + b2->radius))
  {
      // printf("ne");
      // printf("\nB1 %f\t%f\n", b1->x, b1->y);
      // printf("B2 %f\t%f", b2->x, b2->y);
      printf("NE\n");
      return true;
  }
  else
  {
      //printf("\nano   ");
      // printf("\nB1 %f\n", b1->radius);
      // printf("B2 %f", b2->radius);
      // printf("kolize: %f\n", (sqrt((abs(b1->x - b2->x))^2 + (abs(b1->y - b2->y))^2)));
      // printf("disdim: %f\n", (b1->radius + b2->radius));
      printf("kolize\n");
      return false;
  }
}

void ball_respawn(Ball *ball, int width, int height) 
{
    // nevim proc nefunguje ??
    
    // ball->radius = 50 * (double) rand() / RAND_MAX + 10;
    // ball->x = randomNum(ball->radius, width - ball->radius);
    // ball->y = randomNum(ball->radius, height - ball->radius);
    // ball->alive = 1;
    
}

int randomNum(int min, int max)
{
  return rand() % (max - min + 1) + min;
}

int main(int argc, char **argv) {
  int width = 1024;
  int height = 768;

  srand(time(NULL));

  if(argc >= 2) {
    if(sscanf(argv[1], "%dx%d", &width, &height) != 2) {
      fprintf(stderr, "Usage: %s WxH\n", argv[0]);
      exit(1);
    }
  }

  // initialize SDL and create window (for details see sdl.h/sdl.c)
  SDL_Context *ctx = sdl_context_new("game", width, height);

  SDL_Texture *image = IMG_LoadTexture(ctx->renderer, "resources/ball.svg");
  if (image == NULL) {
    fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Texture *kekw = IMG_LoadTexture(ctx->renderer, "resources/kekw.png");
  if (kekw == NULL) {
    fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
    return 1;
  }

  const int N = 10;

  Ball ball[N];
  
  time_t t;
  srand((unsigned) time(&t));

  for (int i = 0; i < N; i++)
  {
      ball[i].radius = 50 * (double) rand() / RAND_MAX + 10;
      ball[i].x = randomNum(ball[i].radius, width - ball[i].radius);
      ball[i].y = randomNum(ball[i].radius, height - ball[i].radius);
      ball[i].dir = 0;
      ball[i].speedX = 400;
      ball[i].speedY = 200;
      ball[i].alive = 1;
      ball[i].rotation = 0;
      ball[i].rotationSpeed = 2 * (double) rand() / RAND_MAX;
      ball[i].rotationDir = 0;
  }

  int gameOver = 0;

  bool quit = false;
  Uint64 prev_counter = SDL_GetPerformanceCounter();
  SDL_Point Mouse;
  Mouse.x = ball[0].x;
  Mouse.y = ball[0].y;
  while (!quit) {
    Uint64 now = SDL_GetPerformanceCounter();
    // how much time elapsed since the last frame in seconds?
    // use this to compute object moves, not the number of frames...!
    double dt = (double) (now - prev_counter) / SDL_GetPerformanceFrequency();
    prev_counter = now;

    // handle all events from keyboard, mouse, quit and resize events, ...
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
        break;
      }
      else if (e.type == SDL_KEYDOWN) {
        // key is pressed DOWN
        switch (e.key.keysym.sym) {
          case SDLK_ESCAPE:
          case SDLK_q:
            quit = true;
            break;
          case SDLK_DOWN:
          case SDLK_UP:
          case SDLK_SPACE:
              break;
        }
      } else if(e.type == SDL_MOUSEMOTION) {
        // mouse move
        Mouse.x = e.motion.x;
        Mouse.y = e.motion.y;
        //printf("mouse x=%d y=%d\n", Mouse.x, Mouse.y);
      } else if(e.type == SDL_MOUSEBUTTONUP) {
        // e.button.button: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT
        //printf("mouse x=%d y=%d button=%d\n", e.button.x, e.button.y, e.button.button);
      }
    }
                  // ball_respawn
    //SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ctx->renderer);

    SDL_Rect rect[N];
    
    ball[0].x = ball[0].x - ball[0].speedX * dt * cos(atan2(ball[0].y - Mouse.y, ball[0].x - Mouse.x));
    ball[0].y = ball[0].y - ball[0].speedY * dt * sin(atan2(ball[0].y - Mouse.y, ball[0].x - Mouse.x));
    //printf("\n%d %d", ball[0].x, ball[0].y);


    for (int i = 0; i < N; i++)
    {
        if (gameOver == 0)
        {
          if (ball[i].alive == 1)
          {
            if (ball[i].y - ball[i].radius < 0)
            {
                ball[i].speedY = abs(ball[i].speedY);
            }
            else if (ball[i].y + ball[i].radius >= height)
            {
                ball[i].speedY = -abs(ball[i].speedY);
            }

            if (ball[i].x - ball[i].radius < 0)
            {
                ball[i].speedX = abs(ball[i].speedY);
            }
            else if (ball[i].x + ball[i].radius >= width)
            {
                ball[i].speedX = -abs(ball[i].speedY);
            }
        
            
            
            if (i == 0)
            {
                ball[0].x = ball[0].x - ball[0].speedX * dt * cos(atan2(ball[0].y - Mouse.y, ball[0].x - Mouse.x));
                ball[0].y = ball[0].y - ball[0].speedY * dt * sin(atan2(ball[0].y - Mouse.y, ball[0].x - Mouse.x));
            }
            else
            {
                ball[i].x+= ball[i].speedX * dt;
                ball[i].y+= ball[i].speedY * dt;
            }
            

            rect[i].w = rect[i].h = 2 * ball[i].radius;
            rect[i].x = ball[i].x - ball[i].radius;
            rect[i].y = ball[i].y - ball[i].radius;

            for (int y = i + 1; y < N; y++)
            {
                // printf("\nB1 %d\t%d\n", ball[0].x, ball[0].y);
                // printf("B2 %d\t%d", ball[y].x, ball[y].y);
                if (ball[y].alive == 1)
                {
                  if (ball_collides(&ball[i], &ball[y]) == false)
                  {
                    if (ball[i].radius > ball[y].radius)
                    {
                        ball[y].alive = 0;
                        ball_respawn(&ball[y], width, height);
                        ball[y].radius = 50 * (double) rand() / RAND_MAX + 10;
                        ball[y].x = randomNum(ball[y].radius, width - ball[y].radius);
                        ball[y].y = randomNum(ball[y].radius, height - ball[y].radius);
                        ball[y].alive = 1;
                        ball[y].rotationDir = 0;

                        ball[i].rotationDir = 1;
                        ball[i].radius += 5;
                    }
                    else
                    {
                        ball[i].alive = 0;
                        ball_respawn(&ball[i], width, height);
                        ball[i].radius = 50 * (double) rand() / RAND_MAX + 10;
                        ball[i].x = randomNum(ball[i].radius, width - ball[i].radius);
                        ball[i].y = randomNum(ball[i].radius, height - ball[i].radius);
                        ball[i].alive = 1;
                        ball[i].rotationDir = 0;

                        ball[y].rotationDir = 1;
                        ball[y].radius += 5;
                    }
                  }
                }

              if (ball[i].radius > (width / 2)) 
              {
                  gameOver = 1;
              }
          }
        }
        }
        
    }

    // for (int i = 0; i < N; i++)
    // {
    //     if (ball[i].alive == 0)
    //     {
    //         ball_respawn(&ball[i], width, height);
    //     }
    // }
    
    if (gameOver == 0)
    {
      for (int i = 0; i < N; i++)
      {
          if (ball[i].alive == 1)
          {
              if (i == 0)
              {
                  // SDL_RenderCopy(ctx->renderer, kekw, NULL, &rect[i]);
                  if (ball[i].rotationDir == 1)
                  {
                      ball[i].rotation -= ball[i].rotationSpeed;
                  }
                  else
                  {
                    ball[i].rotation += ball[i].rotationSpeed;
                  }
                  
                  SDL_RenderCopyEx(ctx->renderer, kekw, NULL, &rect[i], ball[i].rotation, NULL, SDL_FLIP_NONE);
              }
              else
              {
                  if (ball[i].rotationDir == 1)
                  {
                      ball[i].rotation -= ball[i].rotationSpeed;
                  }
                  else
                  {
                    ball[i].rotation += ball[i].rotationSpeed;
                  }

                  SDL_RenderCopyEx(ctx->renderer, image, NULL, &rect[i], ball[i].rotation, NULL, SDL_FLIP_NONE);
              }
              
              
          }
          
      }
    }
    else
    {
        for (int y = 0; y < N; y++)
        {
            ball[y].alive = 0;
            ball_respawn(&ball[y], width, height);
            ball[y].radius = 50 * (double) rand() / RAND_MAX + 10;
            ball[y].x = randomNum(ball[y].radius, width - ball[y].radius);
            ball[y].y = randomNum(ball[y].radius, height - ball[y].radius);
            ball[y].alive = 1;
            ball[y].rotationDir = 0;
        }
        gameOver = 0;
    }
    
    
    // SDL_RenderCopy(ctx->renderer, image, NULL, &rect[0]);

    // update the screen
    SDL_RenderPresent(ctx->renderer);
  }

  // cleanup all resources
  SDL_DestroyTexture(image);
  sdl_context_delete(ctx);
  return 0;
}
