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
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
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


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u32 u32Password = 1122;
  static u32 u32Input = 0;
  static u32 u32InputCount = 0;
  static bool bRED = FALSE;
  static bool bGREEN = FALSE;
  static u32 u32BlinkCount = 0;
  static bool bBlinking = FALSE;
  static u32 startupTimer = 0;
  static bool pw_change = FALSE;
  
  startupTimer++;
  if (startupTimer < 3000 && IsButtonHeld(BUTTON3, 100))
  {
    //bGREEN = TRUE;
    LedBlink(GREEN, LED_8HZ);
    LedBlink(RED, LED_8HZ);
    ButtonAcknowledge(BUTTON3);
    u32Input = 0;
    u32InputCount = 0;
    pw_change = TRUE;
  }
  
  
  if (WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    u32Input *= 10;
    u32Input += 1;
    u32InputCount++;
  } else if (WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    u32Input *= 10;
    u32Input += 2;

    u32InputCount++;
  } else if (WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    u32Input *= 10;
    u32Input += 3;

    u32InputCount++;
  } else if (WasButtonPressed(BUTTON3) && startupTimer > 3000)
  {
    if (startupTimer > 3000)
      ButtonAcknowledge(BUTTON3);
    
    if(pw_change == TRUE)
    {
      LedOff(RED);
      LedOff(GREEN);
      bGREEN = TRUE;
      pw_change = FALSE;
      u32Password = u32Input;
      u32InputCount = 0;
      u32Input = 0;
    }
    else if (u32Input == u32Password)
    {
      bGREEN = TRUE;
      u32Input = 0;
      u32InputCount = 0;
      
    } else {
      bRED = TRUE;
      u32Input = 0;
      u32InputCount = 0;
    }
  }
  if (u32InputCount == 11)
  {
    bRED = TRUE;
    u32Input = 0;
    u32InputCount = 0;
  }
  
  if (bBlinking == TRUE)
  {
    u32BlinkCount++;
    if (u32BlinkCount == 1000)
    {
      bBlinking = FALSE;
      u32BlinkCount = 0;
      LedOff(RED);
      LedOff(GREEN);
      
    }
    
  }
  
  if (bGREEN == TRUE)
  {
    u32BlinkCount = 0;
    LedBlink(GREEN, LED_4HZ);
    bBlinking = TRUE;
    bGREEN = FALSE;
    LedOff(RED);
  } else if (bRED == TRUE)
  {
    u32BlinkCount = 0;
    LedBlink(RED, LED_4HZ);
    bBlinking = TRUE;
    bRED = FALSE;
    LedOff(GREEN);
  }
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
