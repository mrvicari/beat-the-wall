#include "movement.h"

/**
 * Check for key presses and act accordingly
 */
void handleKey(GameData *gameData, SDL_Event *event)
{
  if (event->type == SDL_KEYDOWN)
  {
    switch (event->key.keysym.sym)
    {
      case SDLK_UP:
        gameData->player.dy = -1;
        return;
      case SDLK_DOWN:
        gameData->player.dy = 1;
        return;
    }
  }
  gameData->player.dy = 0;
}

/**
 * Clamp x to top or bottom of the screen
 *
 *@return 0 if beyond the top, max if below the bottom or x if inbetween
 */
float edgeLimit(float x, float max)
{
  if (x < 0)
    return 0;
  if (x > max)
    return max;
  else
    return x;
}

/**
 * Move paddle according to its speed and screen boundaries
 */
void movePaddle(Paddle *paddle, Uint32 delta_ms)
{
  paddle->y += delta_ms * PADDLE_SPEED/1000 * paddle->dy;
  paddle->y = edgeLimit(paddle->y, (SCREEN_HEIGHT - PADDLE_HEIGHT));
}

/**
 * Move ball according to its speed and screen boundaries
 */
void moveBall(Ball *ball, Uint32 delta_ms)
{
  ball->x += ball->dx * delta_ms * BALL_SPEED/1000;
  ball->y += ball->dy * delta_ms * BALL_SPEED/1000;
  ball->x = edgeLimit(ball->x, SCREEN_WIDTH);
  ball->y = edgeLimit(ball->y, (SCREEN_HEIGHT - BALL_SIZE));
}

/**
 * Move all elements simultaneously
 */
void playMovements(GameData *gameData, Uint32 delta_ms)
{
  movePaddle(&gameData->player, delta_ms);
  moveBall(&gameData->ball, delta_ms);
}
