#include "include.h"
#include "status_change.h"

#include "icq.h"

SOFT_BUTTON_STRUCT 
  status_change_soft={3, 3, "Выбор", "Назад", 1, 0};

HEADER_ITEM 
  status_change_head;


#define STATUS_CHANGE_NUMS 8

static unsigned int 
  Status_Change_Cursor = 0,
  Exit_Enter = 1;



MENU_ITEM status_change_text[STATUS_CHANGE_NUMS]=
{
  (int)"В сети",
  (int)"Готов чатиться",
  (int)"Отошел",
  (int)"Не беспокоить",
  (int)"Занят",
  (int)"Недоступен",
  (int)"Невидим",
  (int)"Отключён"
};

static void ChangeStatus(GUI *data)
{
  switch(Status_Change_Cursor)
  {
  case 0:
    SetStatus(STATUS_ONLINE);
    break;
  case 1:
    SetStatus(STATUS_FREE4CHAT);
    break;
  case 2:
    SetStatus(STATUS_AWAY);
    break;
  case 3:
    SetStatus(STATUS_DND);
    break;
  case 4:
    SetStatus(STATUS_OCCUPIED);
    break;
  case 5:
    SetStatus(STATUS_NA);
    break;
  case 6:
    SetStatus(STATUS_INVISIBLE);
    break;
  case 7:
    SetStatus(STATUS_OFFLINE);
    break;
  }
}

const MENU_PROCS status_change_procs[1]=
{
  ChangeStatus
};

MENU_STRUCT status_change_struct=
{
  STATUS_CHANGE_NUMS,
  2,
  2,
  "Сменить статус",
  0,
  status_change_text,
  status_change_procs
};

void Draw_StatusChange()
{
  DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
  DrawMenuList(&status_change_struct, &status_change_text[STATUS_CHANGE_NUMS], Status_Change_Cursor);
  DrawSoftButton(&status_change_soft);
}

int MoveCursor_StatusChange(int pressed_mode, int key_kode)
{
  switch(pressed_mode)
  {
  case KEY_DOWN:
    {
      switch(key_kode)
      {
      case UP_BUTTON:
        if(Status_Change_Cursor==0)Status_Change_Cursor=STATUS_CHANGE_NUMS-1;
        else Status_Change_Cursor--;
      break;
      
      case DOWN_BUTTON:
        if(Status_Change_Cursor==STATUS_CHANGE_NUMS-1)Status_Change_Cursor=0;
        else Status_Change_Cursor++;
      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:
        ((void (*)(void))(status_change_procs[0]))();
        if(!Exit_Enter) break; else 
          return (1);

            
      case RIGHT_SOFT: return 1;
      
      case '1': Status_Change_Cursor = 0; break;
      case '7': Status_Change_Cursor = STATUS_CHANGE_NUMS-1; break;
      }
    }
    break;
  }
  DirectRedrawGUI();
  return 0;
}

void Init_Status_Change()
{
  //InitHeaderData(&status_change_head, "Сменить статус", 2, FONT, 4, COLOUR, COLOUR_FRING);
}
