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
  ball->x = U32_GAME_BOARD_XAXIS/2; 
  ball->y = U32_GAME_BOARD_YAXIS - 1;
  
  DrawBall(ball);
}

void MoveBall(struct Ball* ball) 
{
  if ((ball->x == 1)) 
  {
    ball->vx = 1;
  }
  else if (ball->x == U32_GAME_BOARD_XAXIS)
  {
    ball->vx = -1;
  }
  
  if(topCollision(ball))
  {
    ball->vy = 1;
  }
  if (sideCollision(ball))
  {
    U32_SCORE += 1;
  }
  else if (bottomCollision(ball))
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
  RedrawRow((u8)(ball->y));
  SetCursor((u8)(ball->x), (u8)(ball->y));
  DebugPrintf("o");
  
  ball->x += ball->vx;
  ball->y += ball->vy;
  
  correctBall(ball);   
}

static void correctBall(struct Ball* ball) 
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
    
static u32 topCollision(struct Ball* ball)
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

static u32 sideCollision(struct Ball* ball)
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

static u32 bottomCollision(struct Ball* ball)
{
  // Ball is moving down
  if ((ball->vy > 0) && (au8GameScreen[ball->y][ball->x - 1] == U8_PADDLE))
  {
    return 1;
  }
  return 0;
}

u32 isMoving(struct Ball* ball) 
{
  return !((ball->vx == 0) && (ball->vy == 0));
  
}

void setMoving(struct Ball* ball)
{
  //u32 oldRow = ball->y;
  
  //ball->x = U32_GAME_BOARD_XAXIS/2;
  //ball->y = U32_GAME_BOARD_YAXIS - 1;
  
  ball->vy = 1;
  ball->vx = (G_u32SystemTime1ms % 2 == 0) ? -1 : 1;
  
  //RedrawRow(oldRow - 1);
}
