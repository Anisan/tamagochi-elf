#include "include.h"
#include "gui_begin.h"
#include "gui_mainmenu.h"
#include "main.h"
// (c) Vedan

typedef struct
{
  GUI gui;
} GUI_MAINMENU_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
} GUI_MAINMENU_CSM;

int TYPE_MENU;
// ���������

// MAIN_MENU !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define ALL_MENU_NUM 3 // ���������� ���� ����

#define MAIN_MENU_NUMS 4 //���������� ������� �������� ����
#define MAIN_MENU_Y_DISP 3
#define MAIN_MENU_X_DISP 3

//unsigned short int MAIN_MENU_NUMS = 3;

static void ToConnect(GUI *data)
{
  //ShowMSG(1,(int)"Connect");
  RUN_GUI_BEGIN(1);
  Create_Connect();
}

static void EditConfig(GUI *data)
{
  //ShowMSG(1,(int)"Settings");
  OpenSettings();
}

static void ExitSieICQ(GUI *data)
{
  //ShowMSG(1,(int)"Exit");
  Close_MAIN_CSM();
}

static void AboutSieICQ(GUI *data)
{
  ShowMSG(1,(int)"SieISQ\n(c) Anisan, Vedan");
}

MENU_ITEM mainmenu_text[MAIN_MENU_NUMS]=
{
  (int)"������������",
  (int)"���������",
  (int)"� ���������",
  (int)"�����"
};



const MENU_PROCS mainmenu_procs[MAIN_MENU_NUMS]=
{
  ToConnect,
  EditConfig,
  AboutSieICQ,
  ExitSieICQ
};
  

MENU_STRUCT main_menu_struct=
{
  MAIN_MENU_NUMS,
  2,
  MAIN_MENU_X_DISP,
  "����",
  0,
  mainmenu_text,
  mainmenu_procs
};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// S O F T   B U T T O N =======================================================

// ���������������� ���� ������� �������� ����
SOFT_BUTTON_STRUCT mainmemu_soft={3, 3, "�����", "�����", 1, 0};

//==============================================================================

#define IS_MAIN_MENU 0

//CURSOR_ITEM menu_cursors[ALL_MENU_NUM]={0,0,0};

int menu_cursors[ALL_MENU_NUM]=
{
  0,
  0,
  0
};

int max_num_menus[ALL_MENU_NUM]=
{
  MAIN_MENU_NUMS,
  0,
  0
};

// K E Y  C O N T R O L ========================================================

int CountMenuCursors(int type_menu, int mode_key, int type_key)
{
  switch(mode_key)
  {
  case KEY_DOWN:
    {
      switch(type_key)
      {
      case UP_BUTTON:
        if(menu_cursors[type_menu]==0)menu_cursors[type_menu]=max_num_menus[type_menu]-1;
        else menu_cursors[type_menu]--;
      break;
      
      case DOWN_BUTTON:
        if((menu_cursors[type_menu])==max_num_menus[type_menu]-1)menu_cursors[type_menu]=0;
        else menu_cursors[type_menu]++;
      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:
        ((void (*)(void))(mainmenu_procs[menu_cursors[type_menu]]))();
      break;
      
      case RIGHT_SOFT: return 1;
      }
    }
    break;
  }
  return 0;
}

//==============================================================================
unsigned int GUI_MAINMENU_ID = 0,
             Quit_GUI_MAINMENU = 0;
             


static void DrawMenuFon()
{
  DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
}


static void OnRedraw(GUI_MAINMENU_GUI *data)
{
  if (data->gui.state==2)
  {
    DrawMenuFon();
    DrawMenuList(&main_menu_struct, &mainmenu_text[MAIN_MENU_NUMS], menu_cursors[IS_MAIN_MENU]);
    DrawSoftButton(&mainmemu_soft);
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
      case RIGHT_SOFT: if(TYPE_MENU==IS_MAIN_MENU)Close_MAIN_CSM(); else return 1;
    }
    
  }
  
  CountMenuCursors(IS_MAIN_MENU, sh, msg->gbsmsg->submess);
    
  DirectRedrawGUI();
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





// ������������� ���������� ����
void InitMenu()
{
  // ���� ������ ������ �� �������
  //InitSoftButton(&mainmemu_soft,3,3, "�����", "�����", 1, 0);
  //InitMenuList(&main_menu_struct, "����", 3, (char*)mainmenu_text);
}

void RUN_GUI_MAINMENU(int menu_mode)
{
  Quit_GUI_MAINMENU = 0;
  TYPE_MENU = menu_mode;
  InitMenu();
  static const RECT Canvas={0,0,0,0};
  patch_rect((RECT*)&Canvas,0,0,ScrW-1,ScrH-1);
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  GUI_MAINMENU_GUI *main_gui=malloc(sizeof(GUI_MAINMENU_GUI));
  zeromem(main_gui,sizeof(GUI_MAINMENU_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)GUI_MAINMENU_GUI_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
  //DirectRedrawGUI();
  ///return (0);
}
