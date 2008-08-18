#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "local_ipc.h"
#include "..\inc\xtask_ipc.h"
#include "main.h"
#include "..\inc\cfg_items.h"
#include "Items.h"


#define isElka() (isnewSGold()==2)
#define UNI_YDISP (isElka()?24:0)

const char *config_filename="4:\\Zbin\\etc\\menu.bcfg";

//--------------------------------------------------------------------//
//                           READ CONFIGS                             //
//--------------------------------------------------------------------//

//--------------------------------------------------------------------//
//------------------------  MAIN     CONFIG  -------------------------//
//--------------------------------------------------------------------//

typedef struct 
{
CFG_HDR cfghdr0;
int ACTIVE_KEY;
CFG_HDR cfghdr7_2;
char CSMText[32];
CFG_HDR cfghdrm_m1;
int closeMenu;
//==============================================================
CFG_HDR cfghdr_l21;
CFG_HDR cfghdr10_1;
int Animation;
CFG_HDR cfghdr10_2;
unsigned int SpeedAnim;
CFG_HDR cfghdr_l20;
//==============================================================
CFG_HDR cfghdr_l11;
CFG_HDR cfghdr2_2;
char MENU_PATH[64];
CFG_HDR cfghdr2_3;
char CURSOR_PATH[64];
CFG_HDR cfghdr2_4;
char IMGMENU_PATH[64];
CFG_HDR cfghdr_l10;
  //==============================================================
CFG_HDR cfghdr_j11;
CFG_HDR cfghdr3_1;
char TextRight[16];
CFG_HDR cfghdr3_2;
char TextLeft[16];
CFG_HDR cfghdr3_3;
char RunLeft[16]; //A06E835B MY_MENU
CFG_HDR cfghdr_j10;
}mainConfig;





///////////////////////////////////////////
// MAIN VALUE
int ACTIVE_KEY;
char CSMText[32];
int closeMenu;
//==============================================================
int Animation;
unsigned int SpeedAnim;
//==============================================================
char MENU_PATH[64];
char CURSOR_PATH[64];
char IMGMENU_PATH[64];
  //==============================================================
char TextRight[16];
char TextLeft[16];
char RunLeft[16]; //A06E835B MY_MENU



GBSTMR mytmr;


const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ gipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

#define TMR_SECOND 216

void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

int m; //коэффициент для формулы = isnewSGold

#pragma inline=forced
int toupper(int c)
{
  if((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

#pragma inline
void ElfKiller()
{
  extern void kill_data(void *p, void (*func_p)(void *));
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void _WriteLog(char *buf)
{
  int flog=-1;
  unsigned int err;
  flog = fopen("4:\\ZBin\\menu\\menu.log",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
        if (flog!=-1)
	{
		char msg[512];

		TTime t;
		TDate d;
		GetDateTime(&d,&t);
		sprintf(msg, "%02d:%02d:%02d %s\n", t.hour,t.min,t.sec,buf);
		//  sprintf(msg, "%s\n", buf);
		fwrite(flog,msg,strlen(msg),&err);
	}
  fclose(flog,&err);      
}


//-----------------CSM-----------------
const int minus11=-11;
unsigned short maincsm_name_body[140];
int MCSMid;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

extern int hook(int submsg, int msg);
extern void InitSettings();
extern int LoadItems(const char *);

void MOnCreate(CSM_RAM *data)
{
  InitSettings();
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
  AddKeybMsgHook((void *)hook);
}

int MOnMsg(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *config_filename;
    if (strcmp_nocase(config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"Menu config updated!");
      LockSched();
      InitSettings();
      UnlockSched();
    }
  }
    
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
        case IPC_UPDATE_STAT:
          GBS_StartTimerProc(&mytmr, SpeedAnim*TMR_SECOND/10, TimerProc);break;
        }
      }
    }
  }
  

  return(1);
}


void MOnClose(CSM_RAM *csm)
{
  RemoveKeybMsgHook((void *)hook);
  GBS_DelTimer(&mytmr);
  SUBPROC((void *)ElfKiller);
}

const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  MOnMsg,
  MOnCreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  MOnClose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};
//-------------------------------------

GUI * errgui;

#pragma swi_number=0x37
__swi __arm void Ringtones_SetState(unsigned char state);


extern void ShowMenu(void);

int hook(int submsg, int msg)
{
 
  if (IsUnlocked() && submsg==ACTIVE_KEY && msg==KEY_DOWN)
    if (IsGuiOnTop(((int *)FindCSMbyID(CSM_root()->idle_id))[DISPLACE_OF_IDLEGUI_ID/4]))
    {
      ShowMenu();
      return(2);
      
    }
  return (0);
}

__thumb unsigned int GetBLAddr(unsigned int adr)
{
  short _1_11=((short*)adr)[0];
  int _1=(_1_11&0x7FF)<<11|(_1_11&0x0400?0xFFC00000:0);
  short _2=((short*)adr)[1]&0x7FF;
  return(unsigned int)(adr+_1*2+_2*2+4+(_1_11>>12&1));
}

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"MenuDaemon");
}

void InitSettings()
{
// загрузка настроек
 int mainConfigSize = sizeof(mainConfig);
 mainConfig* mainConf = (mainConfig*)malloc(mainConfigSize);
 extern const CFG_HDR cfghdr0;
 int newCfgFile = InitConfig(mainConf, mainConfigSize, "4:\\Zbin\\etc\\Menu.bcfg",(void*)&cfghdr0);
 if(newCfgFile!=-1)//Если конфиг есть
{
  ACTIVE_KEY=mainConf->ACTIVE_KEY;
  sprintf(CSMText,"%s",mainConf->CSMText);
  closeMenu=mainConf->closeMenu;
  Animation=mainConf->Animation;
  SpeedAnim=mainConf->SpeedAnim;
//==============================================================
  sprintf(MENU_PATH,"%s",mainConf->MENU_PATH);
  sprintf(CURSOR_PATH,"%s",mainConf->CURSOR_PATH);
  sprintf(IMGMENU_PATH,"%s",mainConf->IMGMENU_PATH);
  //==============================================================
  sprintf(TextRight,"%s",mainConf->TextRight);
  sprintf(TextLeft,"%s",mainConf->TextLeft);
  sprintf(RunLeft,"%s",mainConf->RunLeft);
    
  mfree(mainConf);
}
else
  {
     LockSched();
     ShowMSG(1,(int)"Can't open configs!");
     UnlockSched();
     void ElfKiller(void);
     SUBPROC((void *)ElfKiller);
  }
}


void main()
{

  InitSettings();
 
  CSM_RAM *save_cmpc;
  MAIN_CSM main_csm;
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  UpdateCSMname();
  UnlockSched();
  
}
