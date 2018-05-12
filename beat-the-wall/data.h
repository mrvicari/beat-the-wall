// Standard library headers
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// SDL headers
#include <SDL2/SDL.h>

// Macro definitions
#define M_PI 3.14159265358979323846

#define MAX_HIT_DISTANCE PADDLE_HEIGHT/2 + BALL_SIZE/2
#define PADDLE_MAXANGLE 85*M_PI/180

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Enum definitions

// Direction of paddle or ball
typedef enum {
  RIGHT = 1, // down
  LEFT = -1 // up
} Direction;

// Paddle characteristics
enum {
  PADDLE_WIDTH = SCREEN_WIDTH/100,
  PADDLE_HEIGHT = PADDLE_WIDTH*8,
  PADDLE_SPEED = 800
};

// Wall characteristics (+variable width)
enum {
  WALL_HEIGHT = SCREEN_HEIGHT
};

// Ball characteristics
enum {
  BALL_SIZE = SCREEN_WIDTH/90,
  BALL_SPEED = 900
};

// Structure definitions

// Visible video components
typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} GameVideo;

// Paddle variables
typedef struct {
  float y; // bottom left corner coordinate
  float dy; // direction in y
} Paddle;

// Ball variables
typedef struct {
  float x, y; // bottom left corner coordinates
  float dx, dy; // direction in x and y
} Ball;

// Gameplay components
typedef struct {
  bool finished; // is game over?

  Paddle player; // paddle
  Paddle wall; // wall/opponent

  Ball ball; // ball

  Uint32 last_update_ms; // time since SDL last updated
} GameData;

// Game components
typedef struct {
  GameVideo gameVideo; // Visible video
  GameData gameData; // Gameplay components
} Game;
