#include "include.h"

#include "gui_mainmenu.h"
#include "icq.h"

typedef struct
{
  GUI gui;
} GUI_BEGIN_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
} GUI_BEGIN_CSM;


DATA_TIME begin_data_time;
SOFT_BUTTON_STRUCT begin_soft={3, 3, " ", "Отмена", 1, 0};

unsigned int GUI_BEGIN_ID = 0,
             Quit_GUI_BEGIN = 0,
             TYPE_GUI;
             
static void EndLoad()
{ 
        //Quit_GUI_BEGIN = 1;
        RUN_GUI_MAINMENU(0);
}

static void DrawBeginFon()
{
  if(!TYPE_GUI)EndLoad();
  
  DrawRectangle(0,YDISP,ScrW-1,ScrH-1,0,
		   GetPaletteAdrByColorIndex(1),
		   GetPaletteAdrByColorIndex(1));
  DrawImg(0,0,(int)"4:\\Zbin\\SieICQ\\img\\logo.png");
  
  
}

static void DrawProgressbar()
{
  
}

static void CheckConnected()
{
  //if(ICQStatus!=STATUS_OFFLINE)
    //Quit_GUI_BEGIN=1;
}
static void DrawInfo()
{
  WSHDR *ws_info = AllocWS(128);
  
  unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED; //by BoBa 10.07
  
  DrawDataTime(&begin_data_time);
  wsprintf(ws_info,"State: %d, RXstate: %d\nRx:%db,Tx:%db\nQueue: %db\n%s\n%t",
           connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);
/*
  if(pm != pl)
  {
     DrawRectangle(0,ScrH-4-2*GetFontYSIZE(FONT_SMALL_BOLD),ScrW-1,ScrH-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-2,0,
                     GetPaletteAdrByColorIndex(0),
                     GetPaletteAdrByColorIndex(0));
    pos_status = ((ScrW-1) * pl) / pm;
    DrawRectangle(1,ScrH-4-2*GetFontYSIZE(FONT_SMALL_BOLD)+1,pos_status,ScrH-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-3,0,
                     GetPaletteAdrByColorIndex(14),
                     GetPaletteAdrByColorIndex(14));  
    wstrcatprintf(data->ws1,"\nLoading images...");
  }
*/
  DrawString(ws_info,3,60,ScrW-4,ScrH-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     FONT,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  
  
  DrawSoftButton(&begin_soft);
  
  FreeWS(ws_info);
}

static void OnRedraw(GUI_BEGIN_GUI *data)
{
  if (data->gui.state==2)
  {
    CheckConnected();
    DrawBeginFon();
    DrawInfo();
    DrawProgressbar();
  }
}

static void OnCreate(GUI_BEGIN_GUI *data,void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

static void OnClose(GUI_BEGIN_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
  Quit_GUI_BEGIN = 0;
}

static void OnFocus(GUI_BEGIN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  DisableIDLETMR();
  data->gui.state=2;
}

static void OnUnfocus(GUI_BEGIN_GUI *data,void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(0);
#endif
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


static int OnKey(GUI_BEGIN_GUI *data, GUI_MSG *msg)
{
  if(Quit_GUI_BEGIN)return 1;
  int sh=msg->gbsmsg->msg;
  switch(sh)
  {
  case KEY_DOWN:
    switch(msg->gbsmsg->submess)
    {
      case RIGHT_SOFT:Disconnect(); return 1;
    case LEFT_SOFT: return 1;
    }
    
  }
  DirectRedrawGUI();
  
  return 0;
}


static int met0(void){return(0);}
static const void * const GUI_BEGIN_GUI_methods[11]={
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




void InitBegin()
{
  InitDataTime(&begin_data_time, 20, 11, COLOUR, COLOUR_FRING);
}
                
void RUN_GUI_BEGIN(int mode)
{
  InitBegin();
  Quit_GUI_BEGIN = 0;
  TYPE_GUI = mode;
  
  static const RECT Canvas={0,0,0,0};
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScrW-1,ScrH-1);
  GUI_BEGIN_GUI *main_gui=malloc(sizeof(GUI_BEGIN_GUI));
  zeromem(main_gui,sizeof(GUI_BEGIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)GUI_BEGIN_GUI_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
  //DirectRedrawGUI();
  //return (0);
}
