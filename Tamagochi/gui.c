#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "rect_patcher.h"
#include "..\inc\pnglist.h"
#include "revision.h"
#include "Tamagochi.h"
#include "language.h"



int MainMenu_ID;
const char percent_t[]="%t";


extern int M_ICONS[];

#define LG_COPYRIGHT     "Tamagochi v%s\nRevision %d\n(C) by Eraser"

extern void GamePlay();
extern void Vaccinate();
extern void Discipline();
extern void SleepProc();
extern void InfoStatus();

static void EditConfig(void)
{
  extern const char *successed_config_filename;
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,successed_config_filename,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  GeneralFuncF1(1);
}

static void AboutDlg()
{
  char s[256];
  snprintf(s,255,LG_COPYRIGHT,__VERSION__,__SVN_REVISION__);
  ShowMSG(2, (int)s);
}

///-----------------------

static const HEADER_DESC menuhdr={0,0,131,21,NULL,(int)LG_MENU,LGP_NULL};

static const int mmenusoftkeys[]={0,1,2};

static const char * const menutexts[7]=
{
  LG_GAMEPLAY,
  LG_VACCINATE,
  LG_DISCIPLINE,
  LG_SLEEP,
  LG_STATUS,
  LG_EDCFG,
  LG_ABOUT
};



static const void *menuprocs[7]=
{
  (void *)GamePlay,
  (void *)Vaccinate,
  (void *)Discipline,
  (void *)SleepProc,
  (void *)InfoStatus,
  (void *)EditConfig,
  (void *)AboutDlg,
};

static const SOFTKEY_DESC mmenu_sk[]=
{
  {0x0018,0x0000,(int)LG_SELECT},
  {0x0001,0x0000,(int)LG_CLOSE},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB mmenu_skt=
{
  mmenu_sk,0
};
void tmenu_ghook(void *data, int cmd)
{
  PNGTOP_DESC *pltop=PNG_TOP();
  if (cmd==9)
  {
    pltop->dyn_pltop=NULL;
  }
  if (cmd==0x0A)
  {
    //pltop->dyn_pltop=XStatusesImgList;
    DisableIDLETMR();
  }
}

static int tmenu_keyhook(void *data, GUI_MSG *msg)
{
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    ((void (*)(void))(menuprocs[GetCurMenuItem(data)]))();
  }
  return(0);
}

static void menuitemhandler(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  extern const char percent_t[];
  if (curitem!=3){
  ws=AllocMenuWS(data,strlen(menutexts[curitem]));
  wsprintf(ws,percent_t,menutexts[curitem]);
  }
  else
  { extern int Sleep;
    if (Sleep==0){
      ws=AllocMenuWS(data,strlen(LG_SLEEP));
      wsprintf(ws,percent_t,LG_SLEEP);
    }
    else
    {
      ws=AllocMenuWS(data,strlen(LG_NOTSLEEP));
      wsprintf(ws,percent_t,LG_NOTSLEEP);
    }
  }
  switch(curitem)
  {
  case 0:
    SetMenuItemIconArray(data,item,M_ICONS+0);
    break;
  case 1:
    SetMenuItemIconArray(data,item,M_ICONS+1);
    break;
  case 2:
    SetMenuItemIconArray(data,item,M_ICONS+2);
    break;  
  case 3:
    SetMenuItemIconArray(data,item,M_ICONS+3);
    break;  
  case 4:
    SetMenuItemIconArray(data,item,M_ICONS+4);
    break;  
  case 5:
    SetMenuItemIconArray(data,item,M_ICONS+5);
    break;  
  case 6:
    SetMenuItemIconArray(data,item,M_ICONS+6);
    break;  
  }
  SetMenuItemText(data, item, ws, curitem);
}

static const MENU_DESC tmenu=
{
  8,tmenu_keyhook,tmenu_ghook,NULL,
  mmenusoftkeys,
  &mmenu_skt,
  0x11,//MENU_FLAG,
  menuitemhandler,
  NULL,//menuitems,
  NULL,//menuprocs,
  0  //count items
};




void ShowMenu(void)
{

  patch_header(&menuhdr);
  MainMenu_ID=CreateMenu(0,0,&tmenu,&menuhdr,0,7,0,0);

}
