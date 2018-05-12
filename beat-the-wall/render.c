#include "render.h"

extern double wallWidth;

/**
 * Render paddle on the screen
 */
void renderPaddle(SDL_Renderer *renderer, int x, int y)
{
  SDL_Rect paddle;

  paddle = (SDL_Rect) {
    .x = x,
    .y = y,
    .w = PADDLE_WIDTH,
    .h = PADDLE_HEIGHT
  };
  SDL_RenderFillRect(renderer, &paddle);
}

/**
 * Render wall on the screen
 */
void renderWall(SDL_Renderer *renderer, int x, int y)
{
  SDL_Rect wall;

  wall = (SDL_Rect) {
    .x = x,
    .y = y,
    .w = wallWidth,
    .h = SCREEN_HEIGHT
  };
  SDL_RenderFillRect(renderer, &wall);
}

/**
 * Render ball on the screen
 */
void renderBall(SDL_Renderer *renderer, Ball *ball)
{
  SDL_Rect ballRect;

  ballRect = (SDL_Rect) {
    .x = ball->x,
    .y = ball->y,
    .w = BALL_SIZE,
    .h = BALL_SIZE
  };
  SDL_RenderFillRect(renderer, &ballRect);
}

/**
 * Render all gameplay components on the screen
 */
void renderGame(SDL_Renderer *renderer, GameData *gameData)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  renderPaddle(renderer, 0, gameData->player.y);
  renderWall(renderer, SCREEN_WIDTH, 0);
  renderBall(renderer, &gameData->ball);
  SDL_RenderPresent(renderer);
}
