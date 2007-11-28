#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "rect_patcher.h"
#include "..\inc\pnglist.h"
#include "revision.h"
#include "Tamagochi.h"
#include "language.h"


int StatusMenu_ID;

extern int M_ICONS[];
extern TStatusPet StatusPet;

void ViewAge()
{
  char s[256];
  snprintf(s,255,"%s:%d\n%d/%d",LG_AGE,
            StatusPet.Age,
            (int)StatusPet.TimeAge,
            StatusPet.Age*10+100 );
   ShowMSG(2, (int)s);
}
void ViewHealth()
{
  char s[256];
   snprintf(s,255,"%s:%d",LG_HEALTH,
            (int)StatusPet.Health*100/StatusPet.MaxHealth
            );
   ShowMSG(2, (int)s);
}
void ViewHunger()
{
  char s[256];
  snprintf(s,255,"%s:%d",LG_HUNGER,
            (int)StatusPet.Hunger*100/StatusPet.MaxHunger
            );
  ShowMSG(2, (int)s);
}
void ViewHappiness()
{
  char s[256];
  snprintf(s,255,"%s:%d",LG_HAPPINESS,
            (int)StatusPet.Happiness*100/StatusPet.MaxHappiness
            );
  ShowMSG(2, (int)s);
}
void ViewDirtiness()
{
  char s[256];
  snprintf(s,255,"%s:%d",LG_DIRTINESS,
            100-(int)StatusPet.Dirtiness*100/StatusPet.MaxDirtiness
            );
  ShowMSG(2, (int)s);
}
void ViewBoredom()
{
  char s[256];
  snprintf(s,255,"%s:%d",LG_BOREDOM,
            (int)StatusPet.Boredom*100/StatusPet.MaxBoredom
            );
  ShowMSG(2, (int)s);
}
void ViewBehaviour()
{
  char s[256];
  snprintf(s,255,"%s:%d",LG_BEHAVIOUR,
            (int)StatusPet.Behaviour*100/StatusPet.MaxBehaviour
            );
  ShowMSG(2, (int)s);
}


///-----------------------

static const HEADER_DESC smenuhdr={0,0,131,21,NULL,(int)LG_STATUS,LGP_NULL};

static const int smenusoftkeys[]={0,1,2};

static const char * const smenutexts[7]=
{
  LG_AGE,
  LG_HEALTH,
  LG_HUNGER,
  LG_HAPPINESS,
  LG_DIRTINESS,
  LG_BOREDOM,
  LG_BEHAVIOUR
};



static const void *smenuprocs[7]=
{
  (void *)ViewAge,
  (void *)ViewHealth,
  (void *)ViewHunger,
  (void *)ViewHappiness,
  (void *)ViewDirtiness,
  (void *)ViewBoredom,
  (void *)ViewBehaviour
 
};

static const SOFTKEY_DESC smenu_sk[]=
{
  {0x0018,0x0000,(int)LG_SELECT},
  {0x0001,0x0000,(int)LG_BACK},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB smenu_skt=
{
  smenu_sk,0
};

void smenu_ghook(void *data, int cmd)
{
  PNGTOP_DESC *pltop=PNG_TOP();
  if (cmd==9)
  {
    pltop->dyn_pltop=NULL;
  }
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

static int smenu_keyhook(void *data, GUI_MSG *msg)
{
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    ((void (*)(void))(smenuprocs[GetCurMenuItem(data)]))();
  }
  return(0);
}

static void smenuitemhandler(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  switch(curitem)
  {
  case 0:{
    ws=AllocMenuWS(data,strlen(smenutexts[curitem])+6);
    wsprintf(ws,"%t: %d",smenutexts[curitem],StatusPet.Age);
    SetMenuItemIconArray(data,item,M_ICONS+7);
    SetMenuItemText(data, item, ws, curitem);
    break;}
  case 1:
    ws=AllocMenuWS(data,strlen(smenutexts[curitem])+5);
    wsprintf(ws,"%t:%d",smenutexts[curitem],(int)StatusPet.Health*100/StatusPet.MaxHealth);
    SetMenuItemIconArray(data,item,M_ICONS+8);
    SetMenuItemText(data, item, ws, curitem);
    break;
  case 2:
    ws=AllocMenuWS(data,strlen(smenutexts[curitem])+5);
    wsprintf(ws,"%t:%d",smenutexts[curitem],(int)StatusPet.Hunger*100/StatusPet.MaxHunger);
    SetMenuItemIconArray(data,item,M_ICONS+9);
    SetMenuItemText(data, item, ws, curitem);
    break;  
  case 3:
    ws=AllocMenuWS(data,strlen(smenutexts[curitem])+5);
    wsprintf(ws,"%t:%d",smenutexts[curitem],(int)StatusPet.Happiness*100/StatusPet.MaxHappiness);
    SetMenuItemIconArray(data,item,M_ICONS+10);
    SetMenuItemText(data, item, ws, curitem);
    break;  
  case 4:
    ws=AllocMenuWS(data,strlen(smenutexts[curitem])+5);
    wsprintf(ws,"%t:%d",smenutexts[curitem],100-(int)StatusPet.Dirtiness*100/StatusPet.MaxDirtiness);
    SetMenuItemIconArray(data,item,M_ICONS+11);
    SetMenuItemText(data, item, ws, curitem);
    break;  
  case 5:
    ws=AllocMenuWS(data,strlen(smenutexts[curitem])+5);
    wsprintf(ws,"%t:%d",smenutexts[curitem],(int)StatusPet.Boredom*100/StatusPet.MaxBoredom);
    SetMenuItemIconArray(data,item,M_ICONS+12);
    SetMenuItemText(data, item, ws, curitem);
    break;  
  case 6:
    ws=AllocMenuWS(data,strlen(smenutexts[curitem])+5);
    wsprintf(ws,"%t:%d",smenutexts[curitem],(int)StatusPet.Behaviour*100/StatusPet.MaxBehaviour);
    SetMenuItemIconArray(data,item,M_ICONS+13);
    SetMenuItemText(data, item, ws, curitem);
    break;  
  }
}

static const MENU_DESC smenu=
{
  8,smenu_keyhook,smenu_ghook,NULL,
  smenusoftkeys,
  &smenu_skt,
  0x11,//MENU_FLAG,
  smenuitemhandler,
  NULL,//menuitems,
  NULL,//menuprocs,
  0  //count items
};

void ShowStatus(void)
{
  patch_header(&smenuhdr);
  StatusMenu_ID=CreateMenu(0,0,&smenu,&smenuhdr,0,7,0,0);
  
}
