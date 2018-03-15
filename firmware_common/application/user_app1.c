/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 UserApp1au8GameStartMessageTop[] 
= "                    Welcome to Brick Breaker                    ";
static u8 UserApp1au8GameStartMessageBottom[] 
= "                    Press any key to Start                    ";

static u8 UserApp1au8GameOverlayTopScore[]
= "Score:              ";

static u8 UserApp1au8GameOverlayBottom[]
= "<                  >";

static u8 UserApp1au8GameScreen[48][32];

u32 U32_GAME_PADDLE_LEFT_BOUND;
u32 U32_GAME_PADDLE_RIGHT_BOUND;



/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Swap

Description:
Swaps two elements in an array at the given indices

Requires:
  - charArray the array to swap
  - first the index of the first element
  - second the index of the second element

Promises:
  - performs an in place swap of the first and second element
*/
void UserApp1Swap(u8* charArray, u32 first, u32 second)
{
  u8 temp = charArray[first];
  charArray[first] = charArray[second];
  charArray[second] = temp;
}

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1PrepareLCDMessage

Description:
Resets a message to be used for scrolling text

Requires:
  - charArray the array to prepare
  - size the size of the array

Promises:
  - Shifts all characters in an array after the 20th character
    to the right and places a \0 in the first position.
*/
void UserApp1PrepareLCDMessage(u8* charArray, u32 size)
{
  for (u32 i = size;
       i > 19; i--) 
  {
    charArray[i] = 
      charArray[i-1];
  }
  charArray[20] = '\0';
}

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  
  
  /* Initialize the Game Start Screen */
  
  LCDCommand(LCD_CLEAR_CMD);
  /* Swap the '\0' at the end of the string and put it after the 20th char */
  
  UserApp1PrepareLCDMessage
    (UserApp1au8GameStartMessageTop, U32_GAME_START_MESSAGE_TOP_SIZE);
  UserApp1PrepareLCDMessage
    (UserApp1au8GameStartMessageBottom, U32_GAME_START_MESSAGE_BOTTOM_SIZE);
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_GameMenu;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void UserApp1MakeBoard(){
  static u16 brickLayers = 8;
  
  for (int i = 0; i < U32_GAME_SCREEN_YAXIS; i++) 
  {
    if (i < brickLayers)
    {
      for (int j = 0; j < U32_GAME_BOARD_XAXIS; j++)
      {
        UserApp1au8GameScreen[i][j] = '=';
      }
    } else {
      for (int j = 0; j < U32_GAME_BOARD_XAXIS; j++)
      {
        UserApp1au8GameScreen[i][j] = ' ';
      }
    } 
  }
  
  return;
}
// Draws the board every three cycles to allow DebugPrintf time
static void UserApp1SM_DrawBoard() {
  static u32 line = -1;
  static u32 skip = 0;
  if (skip == 2)
  {
    skip = 0;
    if (line == -1)
    {
      DebugPrintf("\033[2J\033[H");
      line++;
    }
    if (line == U32_GAME_BOARD_YAXIS) 
    {
      line = -1;
      skip = 0;
      UserApp1_StateMachine = UserApp1SM_GameStart;
    } 
    else 
    {
      DebugPrintf(UserApp1DisplayLine(line++));
    }
  } 
  else 
  {
    skip++;
  }
}

static void UserApp1DrawPaddle(void) 
{
  DebugPrintf("\033[48;0H");
  DebugPrintf(UserApp1DisplayLine(U32_GAME_BOARD_YAXIS - 1));
}

static void UserApp1DrawBall(struct Ball ball) 
{
  static u32 row = 5;
  static u32 col = 7;
  static u8* command = "\033[r;cHo\0";
  
  command[row] = ((u8) ball.x) + '0';
  command[col] = ((u8) ball.y) + '0';
  DebugPrintf(command);
}

static u8* UserApp1DisplayLine(int line) {
  static u8 HorizontalMessage[33];
  for (u32 i = 0; i < U32_GAME_BOARD_XAXIS; i++)
  {
    HorizontalMessage[i] = UserApp1au8GameScreen[line][i];
  }
  HorizontalMessage[32] = '\0';
  
  return HorizontalMessage;
}

static void UserApp1MovePaddle(u32 U32_GAME_DIRECTION) 
{
  static u8 leftChar;
  static u8 rightChar;
  if ((U32_GAME_DIRECTION == U32_GAME_LEFT) && (0 < U32_GAME_PADDLE_LEFT_BOUND)) 
  {
    leftChar = '-';
    rightChar = ' ';
    U32_GAME_PADDLE_LEFT_BOUND += U32_GAME_DIRECTION;
    UserApp1au8GameScreen[U32_GAME_BOARD_YAXIS - 1][U32_GAME_PADDLE_LEFT_BOUND] = leftChar;
    UserApp1au8GameScreen[U32_GAME_BOARD_YAXIS - 1][U32_GAME_PADDLE_RIGHT_BOUND] = rightChar;
    U32_GAME_PADDLE_RIGHT_BOUND += U32_GAME_DIRECTION;
  }
  else if ((U32_GAME_DIRECTION == U32_GAME_RIGHT) && (U32_GAME_PADDLE_RIGHT_BOUND 
                                                    < U32_GAME_BOARD_XAXIS - 1))
  {
    leftChar = ' ';
    rightChar = '-';
    
    U32_GAME_PADDLE_RIGHT_BOUND += U32_GAME_DIRECTION;
    UserApp1au8GameScreen[U32_GAME_BOARD_YAXIS - 1][U32_GAME_PADDLE_RIGHT_BOUND] = rightChar;
    UserApp1au8GameScreen[U32_GAME_BOARD_YAXIS - 1][U32_GAME_PADDLE_LEFT_BOUND] = leftChar;
    U32_GAME_PADDLE_LEFT_BOUND += U32_GAME_DIRECTION;
    
  } 
    
  
  UserApp1DrawPaddle();
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{

  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Game Menu */
static void UserApp1SM_GameMenu(void)
{
  static u32 UserApp1u32MsgScrollCounter = 0;
  
  static u32 UserApp1u32MsgTopStartPosition = 0;
  static u32 UserApp1u32MsgBottomStartPosition = 0;
  
  UserApp1u32MsgScrollCounter++;
  
  /* If 100 milliseconds have passed, advance the message text */
  if (UserApp1u32MsgScrollCounter == 150) 
  {
    UserApp1u32MsgScrollCounter = 0;
    
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, UserApp1au8GameStartMessageTop 
               + UserApp1u32MsgTopStartPosition);
    LCDMessage(LINE2_START_ADDR, UserApp1au8GameStartMessageBottom 
               + UserApp1u32MsgBottomStartPosition);
    
    if (UserApp1u32MsgTopStartPosition + 20 
        == U32_GAME_START_MESSAGE_TOP_SIZE) 
    {
      UserApp1u32MsgTopStartPosition = 0;
      UserApp1PrepareLCDMessage
        (UserApp1au8GameStartMessageTop, 
         U32_GAME_START_MESSAGE_TOP_SIZE);
      
    } else 
    {
      UserApp1Swap(UserApp1au8GameStartMessageTop, 
                 UserApp1u32MsgTopStartPosition + 20,
                 UserApp1u32MsgTopStartPosition + 21); 
      UserApp1u32MsgTopStartPosition++;
    }
    
    if (UserApp1u32MsgBottomStartPosition + 20 
        == U32_GAME_START_MESSAGE_BOTTOM_SIZE) 
    {
      UserApp1u32MsgBottomStartPosition = 0;
      UserApp1PrepareLCDMessage
        (UserApp1au8GameStartMessageBottom, 
         U32_GAME_START_MESSAGE_BOTTOM_SIZE);
    } else 
    {
      UserApp1Swap(UserApp1au8GameStartMessageBottom, 
                 UserApp1u32MsgBottomStartPosition + 20,
                 UserApp1u32MsgBottomStartPosition + 21);
      UserApp1u32MsgBottomStartPosition++;
    }
    
    if (WasButtonPressed(BUTTON0) || WasButtonPressed(BUTTON1) 
        || WasButtonPressed(BUTTON2) || WasButtonPressed(BUTTON3))
    {
      if (WasButtonPressed(BUTTON0))
      {
        ButtonAcknowledge(BUTTON0);
      }
          
      if (WasButtonPressed(BUTTON1))
      {
        ButtonAcknowledge(BUTTON1);
      }
          
      if (WasButtonPressed(BUTTON2))
      {
        ButtonAcknowledge(BUTTON2);
      }
      if (WasButtonPressed(BUTTON3))
      {
        ButtonAcknowledge(BUTTON3);
      }
          
        /* If good initialization, set state to GameInitialize */
      if( 1 )
      {
        UserApp1_StateMachine = UserApp1SM_GameInitialize;
      }
      else
      {
        /* The task isn't properly initialized, so shut it down and don't run */
        UserApp1_StateMachine = UserApp1SM_Error;
      }
    }
  } 
  
} /* end UserApp1SM_GameMenu() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Game Initialize */
static void UserApp1SM_GameInitialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, UserApp1au8GameOverlayTopScore);
  LCDMessage(LINE2_START_ADDR, UserApp1au8GameOverlayBottom);
  
  UserApp1MakeBoard();
  
  struct Ball ball;
  
  UserApp1SetBall(ball);
  
  for(int i = 0; i < U32_GAME_PADDLE_SIZE/2; i++) 
  {
    UserApp1au8GameScreen[U32_GAME_BOARD_YAXIS - 1][U32_GAME_BOARD_XAXIS/2 + i] = '-';
    UserApp1au8GameScreen[U32_GAME_BOARD_YAXIS - 1][U32_GAME_BOARD_XAXIS/2 - i - 1] = '-';
    U32_GAME_PADDLE_LEFT_BOUND = U32_GAME_BOARD_XAXIS/2 - i - 1;
    U32_GAME_PADDLE_RIGHT_BOUND = U32_GAME_BOARD_XAXIS/2 + i;
  }
  
  
  
  
  
  
  UserApp1_StateMachine = UserApp1SM_DrawBoard;
  
  
  
  
  
  
  
} /* end UserApp1SM_GameInitialize */          



static void UserApp1CenterBall(struct Ball ball) 
{
  ball.x = U32_GAME_BOARD_XAXIS/2;
  ball.y = U32_GAME_BOARD_YAXIS - 1;
  ball.vy = 1;
  ball.vx = (G_u32SystemTime1ms % 2 == 0) ? -1 : 1;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Game Start */
static void UserApp1SM_GameStart(void)
{
  if (WasButtonPressed(BUTTON0) && WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON0);
    ButtonAcknowledge(BUTTON3);
  } 
  else if (WasButtonPressed(BUTTON0)) 
  {
    ButtonAcknowledge(BUTTON0);
    UserApp1MovePaddle(U32_GAME_LEFT);
    
  } else if (WasButtonPressed(BUTTON3)) 
  {
    ButtonAcknowledge(BUTTON3);
    UserApp1MovePaddle(U32_GAME_RIGHT);
  }
} /* end UserApp1SM_GameStart */ 
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
