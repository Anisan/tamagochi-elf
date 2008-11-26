#include "include.h"
#include "items.h"
#include "c_list_util.h"
#include "iconpack.h"
#include "icq.h"


// Переменные - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 
  c_list_cursor_pos, // Позиция курсора
  c_list_max_contacts,   // Общее число контактов
  c_list_max_show_n, // Максимальное число контактов на экране
  header_height=0,
  items_y_disp=2;


// Структуры - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
extern 
  ITEM *Itemtop;
  CONTACT_LIST_DESC main_c_list={
    2,
    0,
    0,
    0,
    11,
    2
  };
  HEADER_ITEM head_c_list;
  SOFT_BUTTON_STRUCT c_list_soft={3, 3, "Выбор", "Меню", 1, 0};


// Функции - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Работа с курсором //
  
void CList_MoveCursorHome()
{
  if(!c_list_max_show_n)return;
  c_list_cursor_pos = 0;
  REDRAW();
}

void CList_MoveCursorEnd()
{
  if(!c_list_max_show_n)return;
  c_list_cursor_pos = c_list_max_contacts;
  REDRAW();
}

int MoveCursor_C_List(int mode_key, int type_key)
{
  switch(mode_key)
  {
  case KEY_DOWN:
    {
      switch(type_key)
      {
      case UP_BUTTON:
        
        if(!c_list_cursor_pos) c_list_cursor_pos = c_list_max_contacts;
        else c_list_cursor_pos--;
        
      break;
      
      case DOWN_BUTTON:
        
        if(c_list_cursor_pos==c_list_max_contacts) c_list_cursor_pos = 0;
        else c_list_cursor_pos++;

      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:

      break;
      
      case '1': 
      CList_MoveCursorHome();
      break;
    
      case '7':
      CList_MoveCursorEnd();
      break;
      
      case RIGHT_SOFT: TYPE_DRAW = Draw_MainMenu;
      break;
      }
    }
    break;
  }
  
//  DirectRedrawGUI();
  return 0;
}
  // Работа с интерфейсом //

// Задаём параметры шрифта
void CLIST_SetItemsFont(CONTACT_LIST_DESC * data, int sets_font, int sets_font_type)
{
  data->items_font = sets_font;
  data->items_font_type = sets_font_type;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void DrawCListFon()
{
    DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
}


void GetShowsNumContacts(CONTACT_LIST_DESC * data, int head_h, int soft_h)
{
  data->y_disp = 2;
  c_list_max_contacts = data->max_nums = TotalContact();
  c_list_max_show_n = (ScrH - head_h - soft_h)/ (2*( data->y_disp) + Font_H);
}



#define SHOW_WITH_GROUP 1
#define SHOW_WITHNOT_GROUP 0
int ShowType=0;

static void DrawContactList(CONTACT_LIST_DESC *data)
{
  int 
    ALT_disp = 0,
    ALT_clist_count,
    
    ALT_cursor_pos,
    
    ALT_max_count=0,
    NEW_Y;
  
  int Y_DISP = data->y_disp;
  
  ALT_max_count = c_list_max_contacts;
  
  if( c_list_max_contacts > c_list_max_show_n ) 
    ALT_max_count = c_list_max_show_n;
  else
    ALT_max_count = c_list_max_contacts;
    
  if(c_list_cursor_pos > c_list_max_show_n )
  {
    ALT_cursor_pos = c_list_max_show_n;
    ALT_disp = c_list_cursor_pos  - c_list_max_show_n;
  }
  else  ALT_cursor_pos = c_list_cursor_pos;
  
  
  

  char header_text[128]="";
  sprintf(header_text,"%d/%d", c_list_cursor_pos, c_list_max_contacts);
  InitHeaderText(&head_c_list, header_text);
  DrawHeaderText(&head_c_list);
  

  WSHDR *item_data = AllocWS(128);
  
  
  for(ALT_clist_count = 0; ALT_clist_count <= ALT_max_count; ALT_clist_count++)
  {

    NEW_Y =  header_height + (2*Y_DISP + Font_H) * (ALT_clist_count);

    if(ALT_clist_count==ALT_cursor_pos) 
      DrawRoundedFrame(0, NEW_Y+Y_DISP, ScrW, NEW_Y + Font_H+2*Y_DISP, 0, 0, 0 ,CURSOR_COLOUR, CURSOR_COLOUR_FRING);
      
    ITEM *it=GetItem(ALT_clist_count + ALT_disp);
    
    
    if(it->ID!=0)
    {
    char * newname = convUTF8_to_ANSI_STR(it->Nick);
        
    int img=0;
    switch (it->Status)
    {
    case ICQ_STATUS_OFFLINE:   img=IMG_OFFLINE;break;
    case ICQ_STATUS_ONLINE:    img=IMG_ONLINE;break;
    case ICQ_STATUS_AWAY:      img=IMG_AWAY;break;
    case ICQ_STATUS_DND:       img=IMG_DND;break; 
    case ICQ_STATUS_NA:        img=IMG_NA;break;
    case ICQ_STATUS_OCCUPIED:  img=IMG_OCCUPIED;break;
    case ICQ_STATUS_FREE4CHAT: img=IMG_FFC;break;
    case ICQ_STATUS_INVISIBLE: img=IMG_INVISIBLE;break;
    case ICQ_STATUS_EVIL     : img=IMG_EVIL;break;
    case ICQ_STATUS_DEPRESSION:img=IMG_DEPRESSION;break;
    case ICQ_STATUS_HOME     : img=IMG_HOME;break;
    case ICQ_STATUS_WORK     : img=IMG_WORK;break;
    case ICQ_STATUS_LUNCH    : img=IMG_LUNCH;break;
    }
    DrawImg(0, NEW_Y + Y_DISP, IconPack[img]);

    wsprintf(item_data, percent_t, newname);
    DrawString(item_data, 16, NEW_Y + Y_DISP, ScrW, NEW_Y + Y_DISP + Font_H, FONT, 32 , COLOUR, COLOUR_FRING);
    }
    
  }
  
  FreeWS(item_data); 
}
  


void OnRedraw_C_List()
{
    DrawCListFon();
    DrawContactList(&main_c_list);
    DrawSoftButton(&c_list_soft);
}

          
void Init_C_List()
{
  InitHeaderData(&head_c_list, "Контакты", 2, FONT, 4, COLOUR, COLOUR_FRING);

  
  GetShowsNumContacts(&main_c_list, Get_HeaderText_Height(&head_c_list),Get_SoftButton_Height(&c_list_soft));
  header_height = Get_HeaderText_Height(&head_c_list);
     
  main_c_list.y_disp=header_height;
}



