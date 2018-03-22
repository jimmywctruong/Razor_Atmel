/**********************************************************************************************************************
File: game_movement.h                                                                

Description:
This is the header file used to store the game movement logic elements.

**********************************************************************************************************************/

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


#ifndef __GAME_MOVEMENT_H
#define __GAME_MOVEMENT_H

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void MovePaddle(u32 U32_GAME_DIRECTION);
void StartMovingBall(struct Ball* ball);
void StopBall(struct Ball* ball);
void SetBall(struct Ball* ball);
void MoveBall(struct Ball* ball);
u32 IsBallMoving(struct Ball* ball);



/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

static void CheckVerticalWalls(struct Ball* ball);
static void CheckHorizontalWalls(struct Ball* ball);
static void CheckTopCollision(struct Ball* ball);
static void CheckLeftCollision(struct Ball* ball);
static void CheckRightCollision(struct Ball* ball);
static void CheckBottomCollision(struct Ball* ball);


static void CheckTopLeftCollision(struct Ball* ball);
static void CheckTopRightCollision(struct Ball* ball);

static void CheckBottomLeftCollision(struct Ball* ball);
static void CheckBottomRightCollision(struct Ball* ball);

static void CorrectBall(struct Ball* ball) ;

static void MoveBallUp(struct Ball* ball);
static void MoveBallDown(struct Ball* ball);
static void MoveBallLeft(struct Ball* ball);
static void MoveBallRight(struct Ball* ball);
#endif /* __GAME_MOVEMENT_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/