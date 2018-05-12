#include "setup.h"

extern long startTime;
extern int hits;
extern double wallWidth;

/**
 * Setup window, otherwise print error
 */
void initialiseWindow (GameVideo *gameVideo)
{
  if (SDL_Init(SDL_INIT_VIDEO) == 0)
  {
    if (((gameVideo->window = SDL_CreateWindow("Beat The Wall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS)) == NULL)
      || ((gameVideo->renderer = SDL_CreateRenderer(gameVideo->window, -1, SDL_RENDERER_ACCELERATED)) == NULL))
    {
      fprintf(stderr, "Could not set SDL video mode: %s\n", SDL_GetError());
      SDL_Quit();
    }
    SDL_ShowCursor(SDL_DISABLE);
  }
}

/**
 * Set the ball off in a random direction
 */
void toggleBall (Ball *ball)
{
  float angle = rand()/(float)RAND_MAX * PADDLE_MAXANGLE*2 - PADDLE_MAXANGLE;
  ball->dy = sinf(angle);
  if (ball->dx < 0)
    ball->dx = cosf(angle);
  else
    ball->dx = -cosf(angle);
}

/**
 * Re-position ball in starting place
 */
void resetBall (Ball *ball)
{
  ball->x = (SCREEN_WIDTH - SCREEN_WIDTH/10);
  ball->y = (SCREEN_HEIGHT/2 - BALL_SIZE/2);
  toggleBall(ball);
}

/**
 * Prepare variables to begin game
 */
void initialisePlay(GameData *gameData)
{
  gameData->player.y = (SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2);
  gameData->finished = false;
  gameData->ball.dx = 0;
  gameData->ball.dy = 0;
  resetBall(&gameData->ball);

  wallWidth = -SCREEN_WIDTH/100;
  startTime = time(NULL);
  hits = 0;
}

/**
 * Close the game
 */
void endGame(Game *game)
{
  SDL_DestroyRenderer(game->gameVideo.renderer);
  SDL_DestroyWindow(game->gameVideo.window);
  SDL_Quit();
}

/**
 * Restart game
 */
void resetGame(GameData *gameData)
{
  gameData->last_update_ms = SDL_GetTicks();
  initialisePlay(gameData);
}
