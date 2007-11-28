// ����-��������
// Eraser @2007
// anis_1981@mail.ru
// ICQ:308911945

#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "Tamagochi.h"
#include "RANDOM.h"
#include "conf_loader.h"
#include "language.h"


#ifdef NEWSGOLD
#define USE_ONE_KEY
#endif

#define TMR_SECOND 216
unsigned int fShow;

extern const int ACTIVE_KEY;
extern const int ACTIVE_KEY_STYLE;

// -1 - XTask GUI present
// 0 - XTask GUI absent
// 1 - IBUT longpressed, ready for exit
int mode;

// 0 - no press
// 1 - long press ENTER_BUTTON
// 2 - disable KEY_UP process
int mode_enter;

int my_csm_id;
CSM_RAM *under_idle;

extern void kill_data(void *p, void (*func_p)(void *));

#define IPC_TAMAGOCHI_NAME "Tamagochi"
#define IPC_UPDATE_STAT 1

extern const int SpeedLife;

const char ipc_my_name[]=IPC_TAMAGOCHI_NAME;

CSM_DESC icsmd;

GBSTMR start_tmr;

GBSTMR mytmr;
GBSTMR mytmr2;
GBSTMR savetmr;

const IPC_REQ my_ipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

TStatusPet StatusPet; 
int Sleep=0;
int Behavior=0;

#define TOTAL_ICONS=9;

///----------------------------------
int S_ICONS[9+1];
int M_ICONS[14+1];

char *MakeGlobalString(const char *first, int breakchar, const char *last)
{
  int l;
  char *p;
  p=malloc((l=strlen(first))+strlen(last)+2);
  strcpy(p,first);
  if (breakchar)
    p[l++]=breakchar;
  strcpy(p+l,last);
  return p;
}

void setup_ICONS(void)
{
  int i=0;
  do
  {
    if (!S_ICONS[i]) S_ICONS[i]=(int)MakeGlobalString(PIC_PATH,'\\',icons_names[i]);
    i++;
  }
  while(i<9);
  i=0;
  do
  {
    if (!M_ICONS[i]) M_ICONS[i]=(int)MakeGlobalString(PIC_PATH,'\\',icons_menu[i]);
    i++;
  }
  while(i<14);
  return;
}

void free_ICONS(void)
{
  int i=0;
  do
  {
    mfree((void*)S_ICONS[i]);
    S_ICONS[i]=0;
    i++;
  }
  while(i<9);
  i=0;
  do
  {
    mfree((void*)M_ICONS[i]);
    M_ICONS[i]=0;
    i++;
  }
  while(i<14);
  

  return;
}

// ----------------------------------------------------------------------------
#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}


#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int strncmp_nocase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}
//------------------------------------------------
extern const unsigned int sndVolume;

void Play(const char *fname)
{
  if ((!IsCalling()))//&&Is_Sounds_Enabled)
  {
    FSTATS fstats;
    unsigned int err;
    if (GetFileStats(fname,&fstats,&err)!=-1)
    {
      PLAYFILE_OPT _sfo1;
      WSHDR* sndPath=AllocWS(128);
      WSHDR* sndFName=AllocWS(128);
      char s[128];
      const char *p=strrchr(fname,'\\')+1;
      str_2ws(sndFName,p,128);
      strncpy(s,fname,p-fname);
      s[p-fname]='\0';
      str_2ws(sndPath,s,128);
      
      zeromem(&_sfo1,sizeof(PLAYFILE_OPT));
      _sfo1.repeat_num=1;
      _sfo1.time_between_play=0;
      _sfo1.play_first=0;
      _sfo1.volume=sndVolume;
#ifdef NEWSGOLD
      _sfo1.unk6=1;
      _sfo1.unk7=1;
      _sfo1.unk9=2;
      PlayFile(0x10, sndPath, sndFName, GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1);
#else
#ifdef X75
      _sfo1.unk4=0x80000000;
      _sfo1.unk5=1;
      PlayFile(0xC, sndPath, sndFName, 0,GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1);
#else
      _sfo1.unk5=1;
      PlayFile(0xC, sndPath, sndFName, GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1);
#endif
#endif
      FreeWS(sndPath);
      FreeWS(sndFName);
    }
  }
}

extern void ChangeStatusImage();
extern void VerifyStatus();

//------------------------------------------------
void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}

//------------------------------------------------
void TimerSave(void)
{
  Randomize();
  
  int f;
  unsigned int ul;
  if ((f=fopen(PET_PATH,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
   {
    fwrite(f,&StatusPet, sizeof(StatusPet),&ul);
    fclose(f,&ul);
    
   }
  GBS_StartTimerProc(&savetmr,TMR_SECOND*60*5,TimerSave);
}

//������� ��������� ���������� ����� �������
void TimerProc2(void)
{
  if (StatusPet.StatusDeath==1) return ;
  
  ++Behavior;
  ++StatusPet.TimeAge;
  if (StatusPet.TimeAge>=StatusPet.Age*10+100)
  {
    ++StatusPet.Age;
    StatusPet.TimeAge=0;
    StatusPet.MaxHealth=StatusPet.MaxHealth+2;
    StatusPet.MaxHunger=StatusPet.MaxHunger+2;
    StatusPet.MaxHappiness=StatusPet.MaxHappiness+2;
    StatusPet.MaxDirtiness=StatusPet.MaxHappiness+2;
    StatusPet.MaxBoredom=StatusPet.MaxBoredom+2;
    StatusPet.MaxBehaviour=StatusPet.MaxBehaviour+2;
  }

  // ��� - �������� �����������
  if ((Sleep!=0)&&(Sleep!=3))
  {
    ++ Sleep;
    StatusPet.ImageStatus=2;
     GBS_StartTimerProc(&mytmr2,TMR_SECOND*60*(SpeedLife+1),TimerProc2);
    return;
  }
  if (Sleep!=0)Sleep=1;
  
  
  // �������
  ++StatusPet.Hunger;
  // �������� ������� ������ � ������� ������

 int Eat=0;
  if (StatusPet.Hunger>(int)StatusPet.MaxHunger/3) Eat=1;
  
  DIR_ENTRY de;
  unsigned int err;
  char name[256];
  char * ext;
  int findgvn=0;
  strcpy(name,ROOM_PATH);
  strcat(name,"*.*");
  char *ptr=name+strlen(name)+1; 
  if (FindFirstFile(&de,name,&err))
  {
    do
    { strcpy(ptr,ROOM_PATH);
      strcat(ptr,de.file_name);
      
      if(!isdir(name,&err))
      {
        if((ext=strrchr(ptr,'.')))
        {
          if(!strncmp_nocase(ext+1,"gvn",3))
          {
            ++findgvn;
          }
          else
          {
            //��������
            if (Eat)
            if (StatusPet.Hunger>0)
            if (de.file_size<(StatusPet.Age+1)*1024)
            {
              int in = (int)de.file_size/1024;
              if (in==0) in=1;
              StatusPet.Hunger=StatusPet.Hunger-in;
              // ���� �������������� � �����
              char name2[256];
              strcpy(name2,ROOM_PATH);
              strcat(name2,de.file_name);
              strcat(name2,".gvn");
             
              fmove(ptr, name2, &err);

            }
          }
        }
      }
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
  
  // �����
  if (StatusPet.Hunger>(int)StatusPet.MaxHunger/2) --StatusPet.Happiness;
  // �����
  if (StatusPet.Hunger==0) ++StatusPet.Happiness;
  
  // ������� 
  StatusPet.Dirtiness=findgvn;
  if (StatusPet.Dirtiness>(int)StatusPet.MaxDirtiness) --StatusPet.Happiness;
  
  ++StatusPet.Boredom; // �����
  if (StatusPet.Boredom>(int)StatusPet.MaxBoredom/2) --StatusPet.Happiness;
  
  --StatusPet.Behaviour; // ����������
  
  // �������� ������������ - ����������� ��������
  VerifyStatus();
    
  // ������ � ������
  if ((StatusPet.Health==0)||(StatusPet.Hunger>=StatusPet.MaxHunger)||(StatusPet.Happiness==0))
  {
    StatusPet.StatusDeath=1;
  }
  
  ChangeStatusImage();
  
  // ����� � ���������
  // ����������
  if (Sleep==0)
  if (Behavior>=StatusPet.Behaviour)
  {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[0]);
  Play(sound_name);
  Behavior=0;
  ShowMSG(2, (int)SpeakMessage[Random()&mess_mask]); 
  }
  
  GBS_StartTimerProc(&mytmr2,TMR_SECOND*60*(SpeedLife+1),TimerProc2);
}

// ---------------------------------------------------------------------------
extern void ShowStatus(void);

void InfoStatus()
{
  ShowStatus();
}

// ----------------------------------------------------------------------------
void GamePlay()
{
 if (Sleep!=0)
 {
 ShowMSG(2, (int)LG_IAMSLEEP);
 return;
 }

 StatusPet.Happiness = StatusPet.Happiness+10;
 StatusPet.Boredom = StatusPet.Boredom -10;
 StatusPet.Hunger = StatusPet.Hunger +10;
 ShowMSG(2, (int)LG_COOL);
 ChangeStatusImage();
 VerifyStatus();
}
// ----------------------------------------------------------------------------
void Vaccinate()
{
 if (Sleep!=0)
 {
 ShowMSG(2, (int)LG_IAMSLEEP);
 return;
 }
 StatusPet.Happiness = StatusPet.Happiness-10;
 StatusPet.Health = StatusPet.Health +10;
 StatusPet.Boredom = StatusPet.Boredom -10;
 ShowMSG(2, (int)LG_PAIN);
 ChangeStatusImage();
 VerifyStatus();
}
// ----------------------------------------------------------------------------
void Discipline()
{
 if (Sleep!=0)
 {
 ShowMSG(2, (int)LG_IAMSLEEP);
 return;
 }
 StatusPet.Happiness = StatusPet.Happiness-10;
 StatusPet.Behaviour = StatusPet.Behaviour +10;
 StatusPet.Boredom = StatusPet.Boredom -10;
 ShowMSG(2, (int)LG_SHREW);
 ChangeStatusImage();
 VerifyStatus();
}
// ----------------------------------------------------------------------------
void SleepProc()
{
  if(Sleep==0){ Sleep=1;
  ShowMSG(2, (int)LG_MSGSLEEP);}
  else {Sleep=0;
  ShowMSG(2, (int)LG_MSGNOTSLEEP);}
  ChangeStatusImage();
}
// ----------------------------------------------------------------------------
void ChangeStatusImage()
{
  // ��������� ��������
  StatusPet.ImageStatus=0;
  if (StatusPet.Behaviour<(int)StatusPet.MaxBehaviour/2) StatusPet.ImageStatus=8;
  if (StatusPet.Boredom>(int)StatusPet.MaxBoredom/2) StatusPet.ImageStatus=7;
  if (StatusPet.Dirtiness>(int)StatusPet.MaxDirtiness/2) StatusPet.ImageStatus=6;
  if (StatusPet.Hunger>(int)StatusPet.MaxHunger/2) StatusPet.ImageStatus=3;
  if (StatusPet.Health<(int)StatusPet.MaxHealth/2) StatusPet.ImageStatus=4;
  if (StatusPet.Happiness<(int)StatusPet.MaxHappiness/2) StatusPet.ImageStatus=5;
  // ���
  if (Sleep!=0) StatusPet.ImageStatus=2; 
  if (StatusPet.StatusDeath==1) StatusPet.ImageStatus=1;
}

// ----------------------------------------------------------------------------
void VerifyStatus()
{
// �������� ������������ - ����������� ��������
  if (StatusPet.Happiness>StatusPet.MaxHappiness) StatusPet.Happiness=StatusPet.MaxHappiness;
  if (StatusPet.Happiness<0) StatusPet.Happiness=0;
  if (StatusPet.Health>StatusPet.MaxHealth) StatusPet.Health=StatusPet.MaxHealth;
  if (StatusPet.Health<0) StatusPet.Health=0;
  if (StatusPet.Dirtiness>StatusPet.MaxDirtiness) StatusPet.Dirtiness=StatusPet.MaxDirtiness;
  if (StatusPet.Boredom>StatusPet.MaxBoredom) StatusPet.Boredom=StatusPet.MaxBoredom;
  if (StatusPet.Boredom<0) StatusPet.Boredom=0;
  if (StatusPet.Behaviour>StatusPet.MaxBehaviour) StatusPet.Behaviour=StatusPet.MaxBehaviour;
  if (StatusPet.Behaviour<0) StatusPet.Behaviour=0;
}
//-----------------------------------------------------------------------------

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  void *icsm;
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)LG_UPDSETTING);
      InitConfig();
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
#ifdef NEWSGOLD
          if (!getCpuUsedTime_if_ena())
          {
            StartCpuUsageCount();
          }
#endif
          GBS_StartTimerProc(&mytmr, REFRESH*TMR_SECOND/10, TimerProc);
        }
      }
    }
  }
  
   switch (cfgShowIn)
  {
  case 0:
    fShow = !IsUnlocked();
    break;
  case 1:
    fShow = IsUnlocked();
    break;
  default:
    fShow = 1;
    break;
  }
 
  
  icsm=FindCSMbyID(CSM_root()->idle_id);
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm)) && fShow) //���� IdleGui �� ����� �����
    {
      GUI *igui=GetTopGUI();
      if (igui) //� �� ����������
      {
#ifdef ELKA
        {
          void *canvasdata = BuildCanvas();
#else
        void *idata = GetDataOfItemByID(igui, 2);
        if (idata)
        {
          void *canvasdata = ((void **)idata)[DISPLACE_OF_IDLECANVAS / 4];
#endif
          char pic_name[128];
          snprintf(pic_name, 127, "%s%s", PIC_PATH, icons_names[StatusPet.ImageStatus]);
          DrawCanvas(canvasdata, POS_X, POS_Y, 
                     POS_X + GetImgWidth((int)pic_name)-1, POS_Y + GetImgHeight((int)pic_name)-1, 1);
          DrawImg(POS_X, POS_Y, (int)pic_name);   
          
         // DrawCanvas(canvasdata,POS_X, POS_Y, POS_X+GetImgWidth((int)S_ICONS[StatusPet.ImageStatus])-1,
	//	    POS_Y+GetImgHeight((int)S_ICONS[StatusPet.ImageStatus])-1,1);
	  //DrawImg(POS_X, POS_Y,S_ICONS[StatusPet.ImageStatus]);
        }
      }
    }
  }
  return 1;  
}

extern int my_keyhook(int submsg, int msg);

static void maincsm_oncreate(CSM_RAM *data)
{
   GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
 
  AddKeybMsgHook((void *)my_keyhook);

  setup_ICONS();
  
  int f;
  unsigned int ul;
 
  if ((f=fopen(PET_PATH,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
  fread(f,&StatusPet, sizeof(StatusPet), &ul);
  fclose(f,&ul);
  }
  else
  {
  StatusPet.Age=0;
  StatusPet.Health=100;
  StatusPet.MaxHealth=100;
  StatusPet.Hunger=0;
  StatusPet.MaxHunger=100;
  StatusPet.Happiness=100;
  StatusPet.MaxHappiness=100;
  StatusPet.Dirtiness=0; 
  StatusPet.MaxDirtiness=100; 
  StatusPet.Boredom=0;
  StatusPet.MaxBoredom=100;
  StatusPet.Behaviour=100;
  StatusPet.MaxBehaviour=100;
  StatusPet.StatusDeath=0;
  StatusPet.ImageStatus=0;
  
  if ((f=fopen(PET_PATH,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
   {
    fwrite(f,&StatusPet, sizeof(StatusPet),&ul);
    fclose(f,&ul);
    
   }
  }
  //������ ���������� �����
  GBS_StartTimerProc(&mytmr2,TMR_SECOND,TimerProc2);
  GBS_StartTimerProc(&savetmr,TMR_SECOND*60*5,TimerSave);
  ChangeStatusImage();
  
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[1]);
  Play(sound_name);
  
}

static void maincsm_onclose(CSM_RAM *csm)
{
  RemoveKeybMsgHook((void *)my_keyhook);
  
  GBS_DelTimer(&mytmr);
  GBS_DelTimer(&mytmr2);
  GBS_DelTimer(&savetmr);


  free_ICONS();
    
  int f;
  unsigned int ul;  
   if ((f=fopen(PET_PATH,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
   {
    fwrite(f,&StatusPet, sizeof(StatusPet),&ul);
    fclose(f,&ul);
    
   }
  
  SUBPROC((void *)ElfKiller);

}

static unsigned short maincsm_name_body[140];
const int minus11=-11;

static const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  maincsm_onclose,
  sizeof(CSM_RAM),
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
//--------------------
extern void ShowMenu(void);

int my_keyhook(int submsg, int msg)
{
#ifndef NEWSGOLD
  void *icsm=FindCSMbyID(CSM_root()->idle_id);
  if (ACTIVE_KEY_STYLE==3)
  {
    if (submsg!=ENTER_BUTTON) return KEYHOOK_NEXT;
    switch(msg)
    {
    case KEY_DOWN:
      if (mode_enter==2)
      {
	GBS_SendMessage(MMI_CEPID,KEY_UP,ENTER_BUTTON);
	return KEYHOOK_NEXT;
      }
      mode_enter=0;
      return KEYHOOK_BREAK;
    case KEY_UP:
      if (mode_enter==0)
      {
        mode_enter=2;
        GBS_SendMessage(MMI_CEPID,KEY_DOWN,ENTER_BUTTON);
        return KEYHOOK_BREAK;
      }
      if (mode_enter==2)
      {
        mode_enter=0;
        return KEYHOOK_NEXT;
      }
      mode_enter=0;
      return KEYHOOK_BREAK;      
    case LONG_PRESS:
      mode_enter=1;
      if (IsUnlocked())
      {
        ShowMenu();
      }
      mode=0;
      return KEYHOOK_BREAK;
    }
  }
  // * + # implementation
  if ((ACTIVE_KEY_STYLE==2) && !(my_csm_id))
  {
    if (msg==KEY_UP)
    {
      mode=0;
      return KEYHOOK_NEXT;
    }
    if (msg==KEY_DOWN)
    {
      switch (submsg)
      {
      case '*':
        mode=1;
        return (0);
      case '#':
        if (mode==1)
        {
          if (IsUnlocked()||ENA_LOCK)
            ShowMenu();
          else mode=0;
        }
        else return KEYHOOK_NEXT;
      }
    }
  }
#endif
  if (ACTIVE_KEY_STYLE<2)
  {
    if (submsg!=ACTIVE_KEY) return KEYHOOK_NEXT;
    if (my_csm_id)
    {
      if (msg==KEY_UP)
      {
	GBS_SendMessage(MMI_CEPID,KEY_DOWN,ENTER_BUTTON);
      }
      return KEYHOOK_BREAK;
    }
    switch(msg)
    {
    case KEY_DOWN:
      mode=0;
      if (ACTIVE_KEY_STYLE==0)
	return KEYHOOK_BREAK;
      else 
	return KEYHOOK_NEXT;
    case KEY_UP:
      if (mode==1)
      {
	//Release after longpress
	mode=0;
	if ((ACTIVE_KEY_STYLE==1))
	{
	  //Launch on LongPress
	  if (IsUnlocked())
	  {
	    ShowMenu();
	  }
	  return KEYHOOK_BREAK;
	}
        break;
      }
      if (ACTIVE_KEY_STYLE==0)
      {
	if (IsUnlocked())
	{
	  ShowMenu();
	}
	return KEYHOOK_BREAK;
      }
      break;
    case LONG_PRESS:
      mode=1;
#ifndef NEWSGOLD
      if (ACTIVE_KEY_STYLE==1)
      {
	if (ENA_LONG_PRESS)
	  return KEYHOOK_NEXT;
	else 
	  return KEYHOOK_BREAK;
      }
#else
      return KEYHOOK_BREAK;
#endif
    }
  }
  return KEYHOOK_NEXT;
  /*if (msg==KEY_UP)
   if (submsg==ACTIVE_KEY)
   { 
    //ShowMSG(1,(int)"��� pressed!"); 
    ShowMenu();
   } 
  return KEYHOOK_NEXT;*/
}

//---------------

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Tamagochi");
}
// ----------------------------------------------------------------------------

void DoSplices(void)
{

  LockSched();
  
    extern const int ENA_HELLO_MSG;
    if (ENA_HELLO_MSG) ShowMSG(1,(int)LG_LOADING);
         
      CSMROOT *csmr;
      CSM_RAM *save_cmpc;
      CSM_RAM main_csm;
      UpdateCSMname();
      csmr=CSM_root();
      save_cmpc=csmr->csm_q->current_msg_processing_csm;
      csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
      CreateCSM(&MAINCSM.maincsm,&main_csm,0);
      csmr->csm_q->current_msg_processing_csm=save_cmpc;
  
  UnlockSched();
}
  
void main(void)
{
  extern const char *successed_config_filename;
  
  InitConfig();
  DoSplices();
}



