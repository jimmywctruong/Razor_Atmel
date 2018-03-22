/**********************************************************************************************************************
File: game_print.c                                                                

Description:
This is the source file used to print various game elements.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:

**********************************************************************************************************************/

#include "game_print.h"
#include "user_app1.h"



void DrawPaddle() 
{
  RedrawRow(U32_GAME_BOARD_YAXIS-U32_GAME_PADDLE_VERTTICAL_OFFSET);
}

void DrawBall(struct Ball* ball) 
{
  SetCursor((u8) (ball->x), (u8) (ball->y));
  DebugPrintf("O");
}


void RedrawRow(u8 row) {
  SetCursor(1, row);
  DebugPrintf(au8GameScreen[row - 1]);
}

void SetCursor(u8 col, u8 row)
{
    static u8 command [16]= {'\033', '['};
    u32 commandSize = 2;
    
    //Transmit the 10's digit of the y position, if it exists.
    if(row > 9)
    {
        command[commandSize++] = row/10 + '0';
        row%=10;
    }
    
    //Transmit the 1's digit of the y position
    command[commandSize++] = row + '0';
    
    //Transmit the ; between the x and y values
    command[commandSize++] = ';';
    
    //Transmit the 10's digit of the x position, if it exists.   
    
    if(col > 9)
    {
        command[commandSize++] = col/10 + '0';
        col%=10;
    }
    
    //Transmit the 1's digit of the x position
    command[commandSize++] = col + '0';
    
    //Transmit the end of the movement command
    command[commandSize++] = 'H';
    command[commandSize++] = '\0';
    DebugPrintf(command);
}

void updateTopRow(void)
{ 
  
  LCDMessage(LINE1_START_ADDR + 7, numToString(U32_SCORE));
  LCDMessage(LINE1_END_ADDR, numToString(U32_LIVES));
}

static u8* numToString(u32 number)
{
  static u8 au8Number[6] = {'\0', '\0', '\0', '\0', '\0', '\0'}; 
  u32 au8NumberSize = 0;

  if (number > 9999)
  {
    au8Number[au8NumberSize++] = number/10000 + '0';
    number%=10000;
  }
  if (number > 999)
  {
    au8Number[au8NumberSize++] = number/1000 + '0';
    number%=1000;
  }
  if (number > 99)
  {
    au8Number[au8NumberSize++] = number/100 + '0';
    number%=100;
  }
  if(number > 9)
  {
    au8Number[au8NumberSize++] = number/10 + '0';
    number%=10;
  }
    
  //Transmit the 1's digit of the y position
  au8Number[au8NumberSize++] = number + '0';
  
  return au8Number;
  
}
    

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/