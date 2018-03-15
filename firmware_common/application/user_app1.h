/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
struct Ball 
{
  u32 x;
  u32 y;

  u32 vx;
  u32 vy;
};

/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
static u32 U32_GAME_START_MESSAGE_TOP_SIZE = 64;
static u32 U32_GAME_START_MESSAGE_BOTTOM_SIZE = 62;

static u32 U32_GAME_SCREEN_XAXIS = 32;
static u32 U32_GAME_SCREEN_YAXIS = 48;
static u32 U32_GAME_BOARD_XAXIS = 32;
static u32 U32_GAME_BOARD_YAXIS = 48;

static u32 U32_GAME_PADDLE_SIZE = 6;

static u32 U32_GAME_LEFT = -1;
static u32 U32_GAME_RIGHT = 1;



/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);
void UserApp1PrepareLCDMessage(u8* charArray, u32 size);
void UserApp1Swap(u8* charArray, u32 first, u32 second);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserApp1MakeBoard(void);
static u8* UserApp1DisplayLine(int line);
static void UserApp1DrawPaddle(void);
static void UserApp1CenterBall();

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         

static void UserApp1SM_GameMenu(void);

static void UserApp1SM_GameInitialize(void);

static void UserApp1SM_GameStart(void);

static void UserApp1SM_DrawBoard(void);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
