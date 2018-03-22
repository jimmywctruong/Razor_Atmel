/**********************************************************************************************************************
File: game_variables.h                                                                

Description:
This is the header file used to store the game variables.

**********************************************************************************************************************/

#ifndef __GAME_VARIABLES_H
#define __GAME_VARIABLES_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
u32 U32_GAME_PADDLE_LEFT_BOUND;
u32 U32_GAME_PADDLE_RIGHT_BOUND;
u32 U32_SCORE = 0;
u32 U32_LIVES = 3;

u8 au8GameScreen[U32_GAME_BOARD_YAXIS][U32_GAME_BOARD_XAXIS + 1];

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/  


#endif /* __GAME_VARIABLES_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
