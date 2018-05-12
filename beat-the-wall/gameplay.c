#include "gameplay.h"

extern long startTime;
extern long endTime;
extern int hits;
extern char score[10];
extern double wallWidth;

/**
 * Check for the ball colliding with the paddle
 *
 *@return true if there is collision, false otherwise
 */
bool paddleBallCollision(Ball *ball, Paddle *paddle)
{
  // Variables to determine collision
  float ballY0, ballY1;          // ball's bottom and top left y coordinate
  float paddleY0, paddleY1;      // paddle's bottom and top left y coordinate

  // Assign values to the Variables
  ballY0 = ball->y;
  ballY1 = ball->y + BALL_SIZE;
  paddleY0 = paddle->y;
  paddleY1 = paddle->y + PADDLE_HEIGHT;
  // If either of the ball's y values is between the paddle's y values, then there is a collision
  if ((paddleY0 < ballY0 && ballY0 < paddleY1) || (paddleY0 < ballY1 && ballY1 < paddleY1))
  {
    int random = (rand()%10);
    // Give the ball a random direction while keeping direction vector = 1
    ball->dy = sin(random);
    ball->dx = fabs(cos(random));

    // Keep count of number of hits
    hits ++;

    return true;
  }
  // If there is no collision
  else

    return false;
}

/**
 * Calculate direction of ball after collisions
 */
void ballCollision(GameData *gameData, Uint32 delta_ms)
{
  Ball *ball;
  float nextX, nextY;      // Values of x and y after collision

  ball = &gameData->ball;
  nextX = (ball->x + ball->dx*delta_ms*BALL_SPEED/1000);
  nextY = (ball->y + ball->dy*delta_ms*BALL_SPEED/1000);
  // If the ball hits the screen limit, change the ball's direction
  if (nextY > (SCREEN_HEIGHT - BALL_SIZE) || nextY < 0)
    ball->dy = -ball->dy;
  // If the ball hits the wall, change the balls direction
  if (nextX > (SCREEN_WIDTH + wallWidth - BALL_SIZE))
    ball->dx = -ball->dx;
  // If next x is past the paddle
  if (nextX < PADDLE_WIDTH)
  {
    // And there was not a collision between the two
    if (!paddleBallCollision(ball, &gameData->player))
      // Then the game is over
      gameData->finished = true;
  }
}

/**
 * Play movements and check for collisions
 */
void play(GameData *gameData, Uint32 now_ms)
{
  Uint32 delta_ms = (now_ms - gameData->last_update_ms);

  ballCollision(gameData, delta_ms);
  playMovements(gameData, delta_ms);
  gameData->last_update_ms = now_ms;
}

/**
 * Loop gameplay functions
 */
void gameMainWhile(Game *game)
{
  // Refresh rate data
  enum {
    FPS = 60,
    SEC = 1000,
    MAX_WAIT_MS = SEC/FPS
  };

  SDL_Event event;
  Uint32 last_ms, delta_ms;

  game->gameData.last_update_ms = SDL_GetTicks();
  // Main while loop
  while (1)
  {
    last_ms = SDL_GetTicks();
    // Check for key presses
    while (SDL_PollEvent(&event))
    {
      // Either quit the game
      if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        return;
      // Or play movements
      handleKey(&game->gameData, &event);
    }
    // Play movements and check for collisions
    play(&game->gameData, SDL_GetTicks());
    // Render all elements
    renderGame(game->gameVideo.renderer, &game->gameData);
    // Allow the CPU a break
    delta_ms = (SDL_GetTicks() - last_ms);
    if (delta_ms < MAX_WAIT_MS)
      SDL_Delay(MAX_WAIT_MS - delta_ms);
    // Constantly make the wall bigger, until it reaches half the screen
    if (-wallWidth < SCREEN_WIDTH/2)
      wallWidth -= 0.1;
    // Once game is over
    if (game->gameData.finished)
    {
      // Get time take and calculate score
      endTime = time(NULL);
      sprintf(score, "%s %ld", "Score: ", (hits * (endTime - startTime)));
      // Inform the user
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                               "Game Over",
                               score,
                               game->gameVideo.window);
      // Restart the game
      resetGame(&game->gameData);
    }
  }
}
