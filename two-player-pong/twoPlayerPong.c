// Standard library headers
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

// SDL headers
#include <SDL2/SDL.h>

// Macro definitions
#define M_PI 3.14159265358979323846

#define MAX_HIT_DISTANCE PADDLE_HEIGHT/2 + BALL_SIZE/2
#define PADDLE_MAXANGLE 85*M_PI/180
#define MAX_SCORE 7

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Enum definitions

typedef enum {
  RIGHT = 1,
  LEFT = -1
} Direction;

enum {
  PADDLE_WIDTH = SCREEN_WIDTH/100,
  PADDLE_HEIGHT = PADDLE_WIDTH*8,
  PADDLE_SPEED = 800
};

enum {
  BALL_SIZE = SCREEN_WIDTH/90,
  BALL_SPEED = 900
};

// Structure definitions

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} GameVideo;

typedef struct {
  float y;
  float dy;
} Paddle;

typedef struct {
  float x, y;
  float dx, dy;
} Ball;

typedef struct {
  int player1;
  int player2;
} Score;

typedef struct {
  Paddle player1;
  Paddle player2;

  Ball ball;

  Uint32 last_update_ms;

  Score score;
} GameData;

typedef struct {
  GameVideo gameVideo;
  GameData gameData;
} Game;

// Function prototypes

void initialiseWindow (GameVideo *gameVideo);
void toggle_ball (Ball *ball);
void resetBall (Ball *ball);
void initialisePlay(GameData *gameData);
void endGame(Game *game);
void handleKey(GameData *gameData, SDL_Event *event);
float edgeLimit(float x, float maxY);
void movePaddle(Paddle *paddle, Uint32 delta_ms);
void moveBall(Ball *ball, Uint32 delta_ms);
void playMovements(GameData *gameData, Uint32 delta_ms);
bool paddleBallCollision(Ball *ball, Paddle *paddle);
void scorePoint(Ball *ball, int *scorer);
void ballCollision(GameData *gameData, Uint32 delta_ms);
void resetGame(GameData *gameData);
void play(GameData *gameData, Uint32 now_ms);
void renderPaddle(SDL_Renderer *renderer, int x, int y);
void renderBall(SDL_Renderer *renderer, Ball *ball);
void isRoundDone(Game *game);
void renderGame(SDL_Renderer *renderer, GameData *gameData);
void gameMainWhile(Game *game);

// Main function
int main(int argc, char **argv)
{
  Game game;

  initialiseWindow(&game.gameVideo);
  initialisePlay(&game.gameData);
  gameMainWhile(&game);
  endGame(&game);

  return 0;
}

// Function definitions

void initialiseWindow (GameVideo *gameVideo)
{
  if (SDL_Init(SDL_INIT_VIDEO) == 0)
  {
    if (((gameVideo->window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS)) == NULL)
      || ((gameVideo->renderer = SDL_CreateRenderer(gameVideo->window, -1, SDL_RENDERER_ACCELERATED)) == NULL))
    {
      fprintf(stderr, "Could not set SDL video mode: %s\n", SDL_GetError());
      SDL_Quit();
    }
    SDL_ShowCursor(SDL_DISABLE);
  }
}

void toggleBall (Ball *ball)
{
  float angle = rand()/(float)RAND_MAX * PADDLE_MAXANGLE*2 - PADDLE_MAXANGLE;
  ball->dy = sinf(angle);
  if (ball->dx < 0)
    ball->dx = cosf(angle);
  else
    ball->dx = -cosf(angle);
}

void resetBall (Ball *ball)
{
  ball->x = (SCREEN_WIDTH/2 - BALL_SIZE/2);
  ball->y = (SCREEN_HEIGHT/2 - BALL_SIZE/2);
  toggleBall(ball);
}

void initialisePlay(GameData *gameData)
{
  gameData->player1.y = gameData->player2.y = (SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2);
  gameData->ball.dx = 0;
  gameData->ball.dy = 0;
  resetBall(&gameData->ball);
  gameData->score = (Score) {
    .player1 = 0,
    .player2 = 0
  };
}

void endGame(Game *game)
{
  SDL_DestroyRenderer(game->gameVideo.renderer);
  SDL_DestroyWindow(game->gameVideo.window);
  SDL_Quit();
}

void handleKey(GameData *gameData, SDL_Event *event)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_UP])
  {
    gameData->player1.dy = -1;
    gameData->player2.dy = -1;
  }
  else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_DOWN])
  {
    gameData->player1.dy = -1;
    gameData->player2.dy = 1;
  }
  else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_UP])
  {
    gameData->player1.dy = 1;
    gameData->player2.dy = -1;
  }
  else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_DOWN])
  {
    gameData->player1.dy = 1;
    gameData->player2.dy = 1;
  }
  else if (state[SDL_SCANCODE_W])
    gameData->player1.dy = -1;
  else if (state[SDL_SCANCODE_S])
    gameData->player1.dy = 1;
  else if (state[SDL_SCANCODE_UP])
    gameData->player2.dy = -1;
  else if (state[SDL_SCANCODE_DOWN])
    gameData->player2.dy = 1;
  else
  {
    gameData->player1.dy = 0;
    gameData->player2.dy = 0;
  }
}

float edgeLimit(float x, float max)
{
  if (x < 0)
    return 0;
  if (x > max)
    return max;
  else
    return x;
}

void movePaddle(Paddle *paddle, Uint32 delta_ms)
{
  paddle->y += delta_ms * PADDLE_SPEED/1000 * paddle->dy;
  paddle->y = edgeLimit(paddle->y, (SCREEN_HEIGHT - PADDLE_HEIGHT));
}

void moveBall(Ball *ball, Uint32 delta_ms)
{
  ball->x += ball->dx * delta_ms * BALL_SPEED/1000;
  ball->y += ball->dy * delta_ms * BALL_SPEED/1000;
  ball->x = edgeLimit(ball->x, SCREEN_WIDTH);
  ball->y = edgeLimit(ball->y, (SCREEN_HEIGHT - BALL_SIZE));
}

void playMovements(GameData *gameData, Uint32 delta_ms)
{
  movePaddle(&gameData->player1, delta_ms);
  movePaddle(&gameData->player2, delta_ms);
  moveBall(&gameData->ball, delta_ms);
}

bool paddleBallCollision(Ball *ball, Paddle *paddle)
{
  float ballY0, ballY1, ballMidY;
  float paddleY0, paddleY1, paddleMidY;
  float midYDiff;
  float bounceAngle;

  ballY0 = ball->y;
  ballY1 = ball->y + BALL_SIZE;
  paddleY0 = paddle->y;
  paddleY1 = paddle->y + PADDLE_HEIGHT;

  if ((paddleY0 < ballY0 && ballY0 < paddleY1) || (paddleY0 < ballY1 && ballY1 < paddleY1))
  {
    ballMidY = (ballY0 + BALL_SIZE/2);
    paddleMidY = (paddleY0 + PADDLE_HEIGHT/2);

    midYDiff = (paddleMidY - ballMidY);
    bounceAngle = (PADDLE_MAXANGLE * (midYDiff/MAX_HIT_DISTANCE));

    if (ball->dy < 0)
      ball->dy = sinf(bounceAngle);
    else
      ball->dy = -sinf(bounceAngle);

    if (ball->dx < 0)
      ball->dx = -cosf(bounceAngle);
    else
      ball->dx = cosf(bounceAngle);

    return true;
  }
  else
    return false;
}

void scorePoint(Ball *ball, int *scorer)
{
  ++*scorer;
  resetBall(ball);
}

void ballCollision(GameData *gameData, Uint32 delta_ms)
{
  Ball *ball;
  float nextX, nextY;

  ball = &gameData->ball;
  nextX = (ball->x + ball->dx*delta_ms*BALL_SPEED/1000);
  nextY = (ball->y + ball->dy*delta_ms*BALL_SPEED/1000);

  if (nextY > (SCREEN_HEIGHT - BALL_SIZE) || nextY < 0)
    ball->dy = -ball->dy;

  if (nextX < PADDLE_WIDTH)
  {
    if (!paddleBallCollision(ball, &gameData->player1))
      scorePoint(ball, &gameData->score.player2);
  }
  else if (nextX > (SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE))
  {
    if (!paddleBallCollision(ball, &gameData->player2))
      scorePoint(ball, &gameData->score.player1);
  }
}

void resetGame(GameData *gameData)
{
  gameData->last_update_ms = SDL_GetTicks();
  initialisePlay(gameData);
}

void play(GameData *gameData, Uint32 now_ms)
{
  Uint32 delta_ms = (now_ms - gameData->last_update_ms);

  ballCollision(gameData, delta_ms);
  playMovements(gameData, delta_ms);
  gameData->last_update_ms = now_ms;
}

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

void isRoundDone(Game *game)
{
  Score *score;

  score = &game->gameData.score;
  if ((score->player1 == MAX_SCORE) || (score->player2 == MAX_SCORE))
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "Game Over",
                             score->player1 == MAX_SCORE ? "Player 1 wins" : "Player 2 wins",
                             game->gameVideo.window);
    resetGame(&game->gameData);
  }
}

void renderGame(SDL_Renderer *renderer, GameData *gameData)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  renderPaddle(renderer, 0, gameData->player1.y);
  renderPaddle(renderer, (SCREEN_WIDTH - PADDLE_WIDTH), gameData->player2.y);
  renderBall(renderer, &gameData->ball);
  SDL_RenderPresent(renderer);
}

void gameMainWhile(Game *game)
{
  enum {
    FPS = 60,
    SEC = 1000,
    MAX_WAIT_MS = SEC/FPS
  };

  SDL_Event event;
  Uint32 last_ms, delta_ms;

  game->gameData.last_update_ms = SDL_GetTicks();
  while (1)
  {
    last_ms = SDL_GetTicks();
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        return;
      handleKey(&game->gameData, &event);
    }
    play(&game->gameData, SDL_GetTicks());
    renderGame(game->gameVideo.renderer, &game->gameData);

    delta_ms = (SDL_GetTicks() - last_ms);
    if (delta_ms < MAX_WAIT_MS)
      SDL_Delay(MAX_WAIT_MS - delta_ms);
    isRoundDone(game);
  }
}
