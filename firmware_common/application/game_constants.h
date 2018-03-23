/**********************************************************************************************************************
File: game_constants.h                                                                

Description:
This is a header file that contains all the constants used in the brick breaker game.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


**********************************************************************************************************************/
#ifndef __GAME_CONSTANTS_H
#define __GAME_CONSTANTS_H

/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/

#define U32_GAME_START_MESSAGE_TOP_SIZE (u32) 64
#define U32_GAME_START_MESSAGE_BOTTOM_SIZE (u32) 62

#define U32_GAME_BOARD_XAXIS (u32) 20
#define U32_GAME_BOARD_YAXIS (u32) 30

#define U32_BRICKLAYERS (u32) 5

#define U8_BRICK (u8) '='
#define U8_BLANK (u8) ' '
#define U8_PADDLE (u8) '-'
#define U8_BALL (u8) 'o'
#define U8_SMALL_BALL (u8) '.'
#define AU8_BALL "o"
#define AU8_SMALL_BALL "."

#define U32_GAME_PADDLE_SIZE (u32) 6
#define U32_GAME_PADDLE_VERTTICAL_OFFSET (u32) 3

#define U32_GAME_LEFT (u32) -1
#define U32_GAME_RIGHT (u32) 1

#define U32_GAME_LEFT_WALL (u32) 1
#define U32_GAME_RIGHT_WALL (u32) U32_GAME_BOARD_XAXIS
#define U32_GAME_TOP_WALL (u32) 1
#define U32_GAME_BOTTOM_WALL (u32) U32_GAME_BOARD_YAXIS

u8 au8GameOverlayTopScore[]
= "Score:       Lives: ";

u8 au8GameOverlayBottom[]
= "<                  >";

#endif /* __GAME_CONSTANTS_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/