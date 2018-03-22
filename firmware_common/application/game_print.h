/**********************************************************************************************************************
File: game_print.h                                                                

Description:
This is the header file used to print various game elements.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


**********************************************************************************************************************/
#ifndef __GAME_PRINT_H
#define __GAME_PRINT_H

void DrawPaddle(void);
void DrawBall(struct Ball* ball);
void RedrawRow(u8 row);
void SetCursor(u8 row, u8 col);
void UpdateTopRow(void);
void UpdateBottomRow(void);

static u8* numToString(u32 number);

#endif /* __GAME_PRINT_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/