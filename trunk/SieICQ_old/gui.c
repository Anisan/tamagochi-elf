#include "include.h"

#include "gui.h"

#include "status_change.h"

typedef struct
{
  GUI gui;
} GUI_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
} GUI_CSM;



static unsigned int GUI_ID, TYPE_GUI;

static void OnRedraw(GUI_GUI *data)
{
  if (data->gui.state==2)
  {
    switch(TYPE_GUI)
    {
    case GUI_STATUS_CHANGE:
      Draw_StatusChange();
      break;
    }
  }

}

static void OnCreate(GUI_GUI *data,void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

static void OnClose(GUI_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

static void OnFocus(GUI_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
  data->gui.state=2;
}

static void OnUnfocus(GUI_GUI *data,void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int OnKey(GUI_GUI *data, GUI_MSG *msg)
{
  switch(TYPE_GUI)
  {
  case GUI_STATUS_CHANGE:
    MoveCursor_StatusChange(msg->gbsmsg->msg, msg->gbsmsg->submess);
    break;
  }
  switch(msg->gbsmsg->msg)
  {
  case KEY_DOWN:
    switch(msg->gbsmsg->submess)
    {
      case RIGHT_SOFT: return 1;
    }
  }
  return 0;
}

static int met0(void){return(0);}
static const void * const GUI_GUI_methods[11]={
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
                
void Gui_Run(int GUI_MODE)
{
  TYPE_GUI = GUI_MODE;
  
  switch(GUI_MODE)
  {
  case GUI_STATUS_CHANGE:
    Init_Status_Change();
    break;
  }
  
  static const RECT Canvas={0,0,0,0};
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScrW-1,ScrH-1);
  GUI_GUI *main_gui=malloc(sizeof(GUI_GUI));
  zeromem(main_gui,sizeof(GUI_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)GUI_GUI_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
}
