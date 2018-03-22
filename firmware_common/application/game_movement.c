/**********************************************************************************************************************
File: game_print.c                                                                

Description:
This is the source file used to move various game elements.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:

**********************************************************************************************************************/

#include "game_constants.h"
#include "game_movement.h"
#include "game_variables.h"

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */

void MovePaddle(u32 U32_GAME_DIRECTION) 
{
  static u8 leftChar;
  static u8 rightChar;
  if ((U32_GAME_DIRECTION == U32_GAME_LEFT) && (0 < U32_GAME_PADDLE_LEFT_BOUND)) 
  {
    leftChar = U8_PADDLE;
    rightChar = U8_BLANK;
    
    U32_GAME_PADDLE_LEFT_BOUND += U32_GAME_DIRECTION;
    au8GameScreen[U32_GAME_BOARD_YAXIS - 1 - U32_GAME_PADDLE_VERTTICAL_OFFSET][U32_GAME_PADDLE_LEFT_BOUND] = leftChar;
    au8GameScreen[U32_GAME_BOARD_YAXIS - 1 - U32_GAME_PADDLE_VERTTICAL_OFFSET][U32_GAME_PADDLE_RIGHT_BOUND] = rightChar;
    U32_GAME_PADDLE_RIGHT_BOUND += U32_GAME_DIRECTION;
  }
  else if ((U32_GAME_DIRECTION == U32_GAME_RIGHT) && (U32_GAME_PADDLE_RIGHT_BOUND 
                                                    < U32_GAME_BOARD_XAXIS - 1))
  {
    leftChar = U8_BLANK;
    rightChar = U8_PADDLE;
    
    U32_GAME_PADDLE_RIGHT_BOUND += U32_GAME_DIRECTION;
    au8GameScreen[U32_GAME_BOARD_YAXIS - 1 - U32_GAME_PADDLE_VERTTICAL_OFFSET][U32_GAME_PADDLE_RIGHT_BOUND] = rightChar;
    au8GameScreen[U32_GAME_BOARD_YAXIS - 1 - U32_GAME_PADDLE_VERTTICAL_OFFSET][U32_GAME_PADDLE_LEFT_BOUND] = leftChar;
    U32_GAME_PADDLE_LEFT_BOUND += U32_GAME_DIRECTION;
    
  } 
  DrawPaddle();
}


void SetBall(struct Ball* ball)
{
  // Sets the ball to the left or right of the center of the paddle.
  ball->x = (U32_GAME_PADDLE_RIGHT_BOUND + U32_GAME_PADDLE_LEFT_BOUND 
             + (G_u32SystemTime1ms % 2 ? 0 : 1))/2; 
  ball->y = U32_GAME_BOARD_YAXIS - 1 - U32_GAME_PADDLE_VERTTICAL_OFFSET;
  
  DrawBall(ball);
}

static void CheckVerticalWalls(struct Ball* ball)
{
  if (ball -> x == U32_GAME_LEFT_WALL)
  {
    MoveBallRight(ball);
  }
  else if (ball -> x == U32_GAME_RIGHT_WALL)
  {
    MoveBallLeft(ball);
  }
}

static void CheckHorizontalWalls(struct Ball* ball)
{
  if (ball -> y == U32_GAME_BOTTOM_WALL)
  {
    MoveBallUp(ball);
    
  }
  else if (ball -> y == U32_GAME_TOP_WALL)
  {
    MoveBallDown(ball);
  }
}

static void MoveBallUp(struct Ball* ball)
{
    // Up is negative, down is positive
    // If ball is travelling down, ensure the ball is moving up
    ball -> vy = (ball -> vy > 0) ? -1 * (ball -> vy) : (ball -> vy);
}

static void MoveBallDown(struct Ball* ball)
{
    // Up is negative, down is positive
    // If ball is travelling up, ensure the ball is moving down
    ball -> vy = (ball -> vy < 0) ? -1 * (ball -> vy) : (ball -> vy);
}

static void MoveBallRight(struct Ball* ball)
{
    // Left is negative, right is positive
    // If ball is travelling left, ensure the ball is moving right
    ball -> vx = (ball -> vx < 0) ? -1 * (ball -> vx) : (ball -> vx);
}

static void MoveBallLeft(struct Ball* ball)
{
    // Left is negative, right is positive
    // If ball is travelling right, ensure the ball is moving left
    ball -> vx = (ball -> vx > 0) ? -1 * (ball -> vx) : (ball -> vx);
}


static void CheckTopCollision(struct Ball* ball)
{
  // Skip if the ball collided with the top wall.
  if (ball -> y == U32_GAME_TOP_WALL)
  {
    return;
  }
  // The item above the ball is at ball -> y - 1 visually, 
  // and ball -> y - 2'th row index in the array.
  u8 col = ball -> y - 2;
  u8 tileAbove = au8GameScreen[col][ball -> x];
  if (tileAbove != U8_BLANK)
  {
    if (tileAbove == U8_BRICK)
    {
      au8GameScreen[(ball -> y) - 2][ball -> x] = U8_BLANK;
    }
    MoveBallDown(ball);
  }
}

static void CheckBottomCollision(struct Ball* ball)
{
  // Skip if the ball collided with the bottom wall.
  if (ball -> y == U32_GAME_BOTTOM_WALL)
  {
    return;
  }
  // The item below the ball is at ball -> y + 1 visually, 
  // and ball -> y'th row index in the array.
  u8 tileAbove = au8GameScreen[(ball -> y)][ball -> x];
  if (tileAbove != U8_BLANK)
  {
    if (tileAbove == U8_BRICK)
    {
      au8GameScreen[(ball -> y)][ball -> x] = U8_BLANK;
    }
    MoveBallUp(ball);
  }
}


static void CheckLeftCollision(struct Ball* ball)
{
  // Skip if the ball collided with the left wall.
  if (ball -> x == U32_GAME_LEFT_WALL)
  {
    return;
  }
  
  // The item to the left of the ball is at ball -> x - 1 visually, 
  // and ball -> x -2'th col index in the array.
  u8 tileLeft = au8GameScreen[ball -> y][ball -> x - 2];
  if (tileLeft != U8_BLANK)
  {
    if (tileLeft == U8_BRICK)
    {
      au8GameScreen[ball -> y][ball -> x - 2] = U8_BLANK;
    }
    MoveBallRight(ball);
  }
}

static void CheckRightCollision(struct Ball* ball)
{
  // Skip if the ball collided with the left wall.
  if (ball -> x == U32_GAME_RIGHT_WALL)
  {
    return;
  }
  
  // The item to the right of the ball is at ball -> x + 1 visually, 
  // and ball -> x + 2'th col index in the array.
  u8 tileLeft = au8GameScreen[ball -> y][ball -> x + 2];
  if (tileLeft != U8_BLANK)
  {
    if (tileLeft == U8_BRICK)
    {
      au8GameScreen[ball -> y][ball -> x + 2] = U8_BLANK;
    }
    MoveBallLeft(ball);
  }
}



void MoveBall(struct Ball* ball) 
{
  CheckVerticalWalls(ball);
  CheckHorizontalWalls(ball);
  
  CheckTopCollision(ball);
  CheckLeftCollision(ball);
  CheckRightCollision(ball);
  CheckBottomCollision(ball);
  
  RedrawRow((u8) (ball->y - ball->vy)); // Clear the old shadow
  
  SetCursor((u8)(ball->x), (u8)(ball->y));
  DebugPrintf(AU8_SMALL_BALL);
  
  ball->x += ball->vx;
  ball->y += ball->vy;
  
  CorrectBall(ball);  
  
#if 0 
  if ((ball->x == 1)) 
  {
    ball->vx = 1;
  }
  else if (ball->x == U32_GAME_BOARD_XAXIS)
  {
    ball->vx = -1;
  }
  
  if(TopCollision(ball))
  {
    ball->vy = 1;
  }
  if (SideCollision(ball))
  {
    U32_SCORE += 1;
  }
  else if (BottomCollision(ball))
  {
    ball->vy = -1;
  }
  else if ((ball->y == 1)) 
  {
    ball->vy = 1;
  }
  else if (ball->y == U32_GAME_BOARD_YAXIS)
  {
    ball->vy = -1;
  }
#endif   
}

static void CorrectBall(struct Ball* ball) 
{
  if (ball->x < 1)
  {
    ball->x = 1;
  }
  else if (ball->x > U32_GAME_BOARD_XAXIS)
  {
    ball->x = U32_GAME_BOARD_XAXIS;
  }
  
  if (ball->y < 1)
  {
    ball->y = 1;
  }
  else if (ball->y > U32_GAME_BOARD_YAXIS)
  {
    ball->y = U32_GAME_BOARD_YAXIS;
  }
}
    
static u32 TopCollision(struct Ball* ball)
{
  // Ball is moving up
  if (ball->vy < 0)
  {
    if (au8GameScreen[ball->y - 2][ball->x - 1] == U8_BRICK)
    {
      if (ball->x - 1 > 0)
      {
        au8GameScreen[ball->y - 2][ball->x - 2] = U8_BLANK;
        U32_SCORE += 1;
      }
  
      if (ball->x - 1 < U32_GAME_BOARD_XAXIS - 1)
      {
        au8GameScreen[ball->y - 2][ball->x] = U8_BLANK;
        U32_SCORE += 1;
      }
      au8GameScreen[ball->y - 2][ball->x - 1] = U8_BLANK;
      RedrawRow(ball->y - 1);
      U32_SCORE += 1;
      
      return 1;
    } 
  }
  return 0;
}

static u32 SideCollision(struct Ball* ball)
{
  if ((ball->x == 1) || (ball->x == U32_GAME_BOARD_XAXIS) 
      || (ball->y == U32_GAME_BOARD_YAXIS))
  {
    return 0;
  }
  // Moving left
  if ((ball->vx < 0) && (au8GameScreen[ball->y - 1][ball->x - 2] == U8_BRICK))
  {
    au8GameScreen[ball->y - 1][ball->x - 2] = U8_BLANK;
    ball->vx = 1;
    return 1;
  } 
  // Moving right
  else if ((ball->vx > 0) && (au8GameScreen[ball->y - 1][ball->x] == U8_BRICK))
  {
    au8GameScreen[ball->y - 1][ball->x] = U8_BLANK;
    ball->vx = -1;
    return 1;
  }
  return 0;
    
}

static u32 BottomCollision(struct Ball* ball)
{
  // Ball is moving down
  if ((ball->vy > 0) && (au8GameScreen[ball->y][ball->x - 1] == U8_PADDLE))
  {
    return 1;
  }
  return 0;
}

u32 IsBallMoving(struct Ball* ball) 
{
  return !((ball->vx == 0) && (ball->vy == 0));
  
}

void StartMovingBall(struct Ball* ball)
{
  ball->vy = 1;
  ball->vx = (G_u32SystemTime1ms % 2 == 0) ? -1 : 1;
}
  
void StopBall(struct Ball* ball)
{
  ball->vx = 0;
  ball->vy = 0;
}
