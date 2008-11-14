#include "include.h"
#include "items.h"
#include "c_list_util.h"

typedef struct
{
  GUI gui;
} GUI_C_LIST_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
} GUI_C_LIST_CSM;
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

static int CList_MoveCursor(int mode_key, int type_key)
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
      
      case RIGHT_SOFT: return 1;
      }
    }
    break;
  }
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
  c_list_max_contacts = data->max_nums = TotalItems();
  c_list_max_show_n = (ScrH - head_h - soft_h)/ (2*( data->y_disp) + Font_H);
}

int GetShowNumGroups()
{
}
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
    
    if(!it->ID)
    {
    char * newname = convUTF8_to_ANSI_STR(it->Nick);
        
    wsprintf(item_data, percent_t, newname);
    DrawString(item_data, 0, NEW_Y + Y_DISP, ScrW, NEW_Y + Y_DISP + Font_H, FONT, 32 , COLOUR, COLOUR_FRING);
    }
    
  }
  
  FreeWS(item_data); 
}
  


static void OnRedraw(GUI_C_LIST_GUI *data)
{
  if (data->gui.state==2)
  {
    DrawCListFon();
    DrawContactList(&main_c_list);
    DrawSoftButton(&c_list_soft);
  }
}

static void OnCreate(GUI_C_LIST_GUI *data,void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

static void OnClose(GUI_C_LIST_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

static void OnFocus(GUI_C_LIST_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  DisableIDLETMR();
  data->gui.state=2;
}

static void OnUnfocus(GUI_C_LIST_GUI *data,void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(0);
#endif
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


static int OnKey(GUI_C_LIST_GUI *data, GUI_MSG *msg)
{
  //if(Quit_GUI_C_LIST)return 1;
  int sh=msg->gbsmsg->msg;
  switch(sh)
  {
  case KEY_DOWN:
    switch(msg->gbsmsg->submess)
    {
    case '1': 
      CList_MoveCursorHome();
      break;
    
    case '7':
      CList_MoveCursorEnd();
      break;
    
    case RIGHT_SOFT:return 1;
    
    }
    
  }
  CList_MoveCursor(sh, msg->gbsmsg->submess);
  DirectRedrawGUI();
  
  return 0;
}


static int met0(void){return(0);}
static const void * const GUI_C_LIST_GUI_methods[11]={
  (void *)OnRedraw,
  (void *)OnCreate,
  (void *)OnClose,
  (void *)OnFocus,
  (void *)OnUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data,
  (void *)met0,
  (void *)met0,
  0
};


          
void InitContactList(CONTACT_LIST_DESC *data)
{
  data->y_disp=header_height;
}

void RUN_GUI_C_LIST(int mode)
{
  InitHeaderData(&head_c_list, "Контакты", 2, FONT, 4, COLOUR, COLOUR_FRING);
  
  GetShowsNumContacts(&main_c_list, Get_HeaderText_Height(&head_c_list),Get_SoftButton_Height(&c_list_soft));
  
  
  header_height = Get_HeaderText_Height(&head_c_list);
  //InitContactList(&main_c_list);
    
  static const RECT Canvas={0,0,0,0};
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScrW-1,ScrH-1);
  GUI_C_LIST_GUI *main_gui=malloc(sizeof(GUI_C_LIST_GUI));
  zeromem(main_gui,sizeof(GUI_C_LIST_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)GUI_C_LIST_GUI_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
}

