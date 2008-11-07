#include "include.h"
#include "gui_mainmenu.h"

#define MAIN_MENU_TEXT_NUM 3
char * main_menu_text[MAIN_MENU_TEXT_NUM] =
{
  "Подключиться",
  "Настройки",
  "Выход"
};

typedef struct
{
  GUI gui;
} GUI_MAINMENU_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
} GUI_MAINMENU_CSM;

unsigned int GUI_MAINMENU_ID = 0,
             Quit_GUI_MAINMENU = 0;
             
MENU_STRUCT main_menu_struct;

static void DrawMenuFon()
{
  DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
}


static void OnRedraw(GUI_MAINMENU_GUI *data)
{
  if (data->gui.state==2)
  {
    DrawMenuFon();
  }
}

static void OnCreate(GUI_MAINMENU_GUI *data,void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

static void OnClose(GUI_MAINMENU_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

static void OnFocus(GUI_MAINMENU_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  DisableIDLETMR();
  data->gui.state=2;
}

static void OnUnfocus(GUI_MAINMENU_GUI *data,void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(0);
#endif
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


static int OnKey(GUI_MAINMENU_GUI *data, GUI_MSG *msg)
{
  if(Quit_GUI_MAINMENU)return 1;
  int sh=msg->gbsmsg->msg;
  switch(sh)
  {
  case KEY_DOWN:
    switch(msg->gbsmsg->submess)
    {
    }
    DirectRedrawGUI();
  }
  return 0;
}


static int met0(void){return(0);}
static const void * const GUI_MAINMENU_GUI_methods[11]={
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





int RUN_GUI_MAINMENU()
{
  Quit_GUI_MAINMENU = 0;
  static const RECT Canvas={0,0,0,0};
  patch_rect((RECT*)&Canvas,0,0,ScrW-1,ScrH-1);
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  GUI_MAINMENU_GUI *main_gui=malloc(sizeof(GUI_MAINMENU_GUI));
  zeromem(main_gui,sizeof(GUI_MAINMENU_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)GUI_MAINMENU_GUI_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  return ( CreateGUI(main_gui) );
}
