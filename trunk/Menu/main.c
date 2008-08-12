#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "local_ipc.h"
#include "..\inc\xtask_ipc.h"
#include "main.h"
#include "Items.h"

//#include "encode.c"

extern void ascii2ws(WSHDR* ws, const char* s);

#define isElka() (isnewSGold()==2)
#define UNI_YDISP (isElka()?24:0)

GBSTMR mytmr;

//extern ITEM *Itemtop;
extern void AddToItem(char *name,char *description,char *iconsmall,char *iconbig, char *run);
extern ITEM *GetItem(int curitem);
extern void FreeItemsList();
extern int TotalAnim(int curitem);
extern char *IconAnim(int curitem, int pic);

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

int CellX;
int CellY;

typedef struct
{
  int flag; //0, 8, 16 ��
  int (*onKey)(GUI *gui, GUI_MSG *msg);
  void (*global_hook_proc)(GUI *gui, int cmd);
  void *locret; //0
  const int *softkeys;
  const SOFTKEYSTAB *softkeystab;
  int unkflag; //1
  int lgp_null;
  const int *icons; //&x52A
  int pos; //0 (pos?)
  int font; //4
  int textcolor; //0x64
  int _101;
  int timeout; //0-forever
} MSG_BOX;


int(*MsgBox)(int _1, int, MSG_BOX*, int lgp_id);
void(*AddHeader)(GUI*,HEADER_DESC*,void* malloc_adr);
int m; //����������� ��� ������� = isnewSGold

#pragma inline
void patch_header(const HEADER_DESC* headc)
{
  HEADER_DESC *head=(HEADER_DESC *)headc;
  head->rc.x=0;
  head->rc.y=UNI_YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+UNI_YDISP;
}

void ElfKiller()
{
  extern void kill_data(void *p, void (*func_p)(void *));
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

CSM_DESC icsmd;

int (*oldIcsmOnMessage)(CSM_RAM*,GBS_MSG*);
void (*oldIcsmOnClose)(CSM_RAM*);


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

//----------------MMenu----------------
int MHico;
HEADER_DESC MHeader={0, 0, 0, 0, &MHico, 0, LGP_NULL};
int MSoftKeys[]={0,1,2};

SOFTKEY_DESC Msk[]=
{
  {0x0018,0x0000,(int)TextLeft},
  {0x0001,0x0000,(int)TextRight},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB Mskt=
{
  Msk,0
};

GBSTMR tmr;

int pic_n=0;
int pos=4;
int ItemsCount=0;
#define TIME 262/5

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

extern int LoadItems(const char *);

//����� � ���������� ��� sieelf.googlecode.com (r)BingK
#define FUNC_PT_START	0xA085DEA4
#define FUNC_PT_END	0xA0865BA3
#define FUNC_PT_PSIZE	0x80
#define FUNC_PT_OFFSET	0x10
#define MAX_FUNC	0xFA

#ifdef NEWSGOLD
unsigned int GetFunctionPointByName(char *name)
{
  char *p=(char *)FUNC_PT_START;
  int len=strlen(name);
  if(len==0)
     return 0;
  while((char *)FUNC_PT_END-p>0)
   {
     if(!strncmpNoCase(p, name, len))
	return (*(unsigned int *)(p+FUNC_PT_OFFSET));
     p+=FUNC_PT_PSIZE;
   }
  return 0;
}
#endif


void run_address(unsigned int address) 
{
  void (*runaddr)(void);
  runaddr=(void (*)())address;
  if (runaddr) 
  {
    runaddr();
  }
}

void run_shortcut(char *shortcut) 
{
#ifdef NEWSGOLD
   run_address(GetFunctionPointByName(shortcut));
#else
   run_address((unsigned int)GetFunctionPointer(shortcut));
#endif
}

int chr2num(char chr)
{
  if(chr>='0'&&chr<='9')
     return (chr-('0'-0));
  if(chr>='A'&&chr<='Z')
     return (chr-('A'-0xA));
  if(chr>='a'&&chr<='z')
     return (chr-('a'-0xA));
  return 0;
}


int RunBM2(const char *s)
{
// const char *s=items[bm].Run;
 if((s)&&strlen(s))
 {
   // �������
   if ((s[(strlen(s))-4]=='.')&&(s[(strlen(s))-3]=='c')&&
     (s[(strlen(s))-2]=='f')&&(s[(strlen(s))-1]=='g'))
   {
      LockSched();
      LoadItems(s);
      pos=0;
      UnlockSched();
      return(1);
   }
  
   // ������ ������ � �������� ������
  if ((s[2]=='\\')&&((s[(strlen(s))-3]=='.')||//��������� ������ �� ������� �������� '\\' � '.'
     (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
       {
        WSHDR *ws;
        ws=AllocWS(256);
        str_2ws(ws,s,256);
        if(ExecuteFile(ws,0,0))
        {
          FreeWS(ws);
          return(1);
        }
        else
        {
          FreeWS(ws);
          return 0;
        }
       }
    
    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       ((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))  
        {
          int i;
          unsigned int addr=0;
          char *p=(char*)s;
          for(i=0;i<strlen(s);i++)
          {
             addr=chr2num(p[i])+addr*0x10;
          }
          run_address(addr);
          return 1;
        }
    else
    {
          char ShortCut[32];
          char *p=(char*)s;
          int i=0;
          int j=0;
          for(;j<strlen(s);j++)
          {
            if(p[j]>='A')
            {
              ShortCut[i]=p[j];
              i++;
            }
          }
          ShortCut[i]=0;
          run_shortcut(ShortCut);     
          return 1;
    }  
  }
 return(0);
}
// ----------------------------------------------

void PickNumTmr()
{
  pic_n++;
  if(pic_n>=TotalAnim(pos))
    pic_n=0;
  DirectRedrawGUI();
  GBS_StartTimerProc(&tmr, SpeedAnim*TMR_SECOND/10, PickNumTmr);
}


void TmrReset()
{
  GBS_DelTimer(&tmr);
  pic_n=0;
  GBS_StartTimerProc(&tmr, SpeedAnim*TMR_SECOND/10, PickNumTmr);
}

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
int CalcPic(char* picture)
{
  int pic = strtoul(picture, 0, 10);
  if ( (pic <= 4) && (strlen(picture) > 1) )
  {
     pic = (int)picture;
  }
  return pic;
}

void UpdateHeader(GUI* gui)
{
  ITEM *Item=GetItem(pos);
  MHeader.lgp_id=(int)Item->Text;
  MHico=CalcPic(Item->IconSmall); //������ ���������
  ((int*)GetDataOfItemByID(gui, 2))[13]=(int)Item->Text; //����� ���������
}


char transparent[]={0x00,0x00,0x00,0x00};
const int font=FONT_SMALL;

void (*OOnRedraw)(GUI *data);
void NOnRedraw(GUI *data)
{
  OOnRedraw(data); //������� ������ OnRedraw
  
  int delta=0;
  if (pos>(Columns*Rows-1))
  {
   delta=(pos-(Columns*Rows-1))/Columns;
   if ((pos-(Columns*Rows-1))%Columns!=0)
   ++delta;
  }

  int setka=Columns*Rows;
  if ((ItemsCount-delta*Columns)<(Columns*Rows))
    setka=ItemsCount-delta*Columns;
  
  for(int i=0;i<setka;i++)
  {
    
/* --==�������==-- */
    int x=menuRect.x+OffsetX+CellX*(i%Columns);
    int y=menuRect.y+OffsetY+CellY*(i/Columns);
      
    if((i+delta*Columns)==pos)
    DrawImg(x,y,CalcPic((char*)CURSOR_PATH));
    
    ITEM *Item=GetItem(i+delta*Columns);
      
    if ((pic_n==0)||(!Animation)||((i+delta*Columns)!=pos))
      DrawImg(x,y,CalcPic(Item->IconBig));
     else
      DrawImg(x,y,CalcPic(IconAnim(pos,pic_n)));

  }

  ITEM *Item=GetItem(pos);
  if (descShow)
    {
      WSHDR *ws=AllocWS(128);
      ascii2ws(ws,Item->Description);
      DrawString(ws,descRect.x,descRect.y,descRect.x2,descRect.y2,font,TEXT_ALIGNMIDDLE,deskColor,0);
    }
}

const void * NGuiMeths[11];

void MGHook(GUI *gui, int cmd)
{
  switch(cmd)
  {
  case 1://��������: ������� onRedraw + ���������� ������
    {
      AddHeader(gui,&MHeader,malloc_adr());
      memcpy(NGuiMeths,gui->methods,11*sizeof(void*));
      gui->methods=(void*)NGuiMeths;
      OOnRedraw=(void(*)(GUI*))NGuiMeths[0];//(gui->methods[0]);
      NGuiMeths[0]=(void*)NOnRedraw;
    }
    break;
  case 5://��������� ������ (?)
    UpdateHeader(gui);
    TmrReset();
    break;
  case 6://������ ������
    UpdateHeader(gui);
    GBS_DelTimer(&tmr);//�������� ������� (����)
    break;
  }
}




int MOnKey(GUI *gui, GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  
  if(msg->gbsmsg->msg==LONG_PRESS)
  {
     if (key=='*')
      {
        char s[256];
        sprintf(s,"Menu v0.4\n(c)Eraser\n%s at %s",__DATE__,__TIME__);
        ShowMSG(2,(int)s);
      }
     if (key=='#')
      {
        // �������������� ��������
        WSHDR *ws;
        ws=AllocWS(150);
        extern const char *successed_config_filename;
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
     if (key=='0')
      {
        // �������������� ����
        char s[256];
        sprintf(s,"Info");
        ShowMSG(2,(int)s);
      }
     
  }
  
  if(msg->gbsmsg->msg==KEY_DOWN)
  {
           
    
    switch(key)
    {
    case ENTER_BUTTON:
      goto run;
    case LEFT_BUTTON:
      pos--;
      break;
    case RIGHT_BUTTON:
      pos++;
      break;
    case UP_BUTTON:
      pos-=Columns;
      break;
    case DOWN_BUTTON:
      pos+=Columns;
      break;
    case RIGHT_SOFT:
      GBS_DelTimer(&tmr);
      return(0);
    case LEFT_SOFT:
      RunBM2(RunLeft);
      return(0);
/*      
    case '*':
      pos=9;
      goto run;
    case '0':
      pos=10;
      goto run;
    case '#':
      pos=11;
      goto run;
   default:
      if(key>='1' && key<='9')
      {
        pos=key-1-'0';
        goto run;
      }
      else
        return(0);*/
    }
      
    if(pos<0)
      pos=ItemsCount-1;
    if(pos>ItemsCount-1)
      pos=0;

  //end:
    UpdateHeader(gui);
    TmrReset();
    RefreshGUI();
    return(0);
  run:
    {
    ITEM *Item=GetItem(pos);
    RunBM2(Item->Run);
    }
    return(0);
  }
  return(0);
}

MSG_BOX MMenu=
{
  0,
  MOnKey,
  MGHook,
  0,
  MSoftKeys,
  &Mskt,
  1,
  LGP_NULL,
  0,
  0,
  0,
  0x64,
  0x65,
  0
};
//-----------------CSM-----------------
const int minus11=-11;
unsigned short maincsm_name_body[140];
int MCSMid;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

void MOnCreate(CSM_RAM *data)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
//  MHico=*items[pos].point.IconSmall;
  csm->gui_id=MsgBox(0,0,&MMenu,LGP_NULL);
  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}


extern void InitSettings();

int MOnMsg(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
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
  GBS_DelTimer(&tmr);
  //pos=4;
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

int newIcsmOnMessage(CSM_RAM* data,GBS_MSG* msg)
{
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"Menu config updated!");
      LockSched();
      InitSettings();
      UnlockSched();
    }
  }

  return oldIcsmOnMessage(data,msg);
}


GUI * errgui;

#pragma swi_number=0x37
__swi __arm void Ringtones_SetState(unsigned char state);



int hook(int submsg, int msg)
{
 
  if (IsUnlocked() && submsg==ACTIVE_KEY && msg==KEY_DOWN)
    if (IsGuiOnTop(((int *)FindCSMbyID(CSM_root()->idle_id))[DISPLACE_OF_IDLEGUI_ID/4]))
    {
      if(FindCSMbyID(MCSMid))
        CloseCSM(MCSMid);
      char dummy[sizeof(MAIN_CSM)];
      LockSched();
      LoadItems(MENU_PATH);
      pos=Start;
      MCSMid=CreateCSM(&MAINCSM.maincsm,dummy,0);
      UnlockSched();
      return(2);
      
    }
  return (0);
}


void newIcsmOnClose(CSM_RAM *data)
{
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  RemoveKeybMsgHook((void *)hook);
  seqkill(data,oldIcsmOnClose,&ELF_BEGIN,SEQKILLER_ADR());
  SUBPROC((void *)ElfKiller);
}

unsigned int GetBLAddr(unsigned int adr)
{
  short _1_11=((short*)adr)[0];
  int _1=(_1_11&0x7FF)<<11|(_1_11&0x0400?0xFFC00000:0);
  short _2=((short*)adr)[1]&0x7FF;
  return(unsigned int)(adr+_1*2+_2*2+4+(_1_11>>12&1));
}

int LoadItems(const char *menu_path)
{
  FSTATS stat;
  char *fn;
  int f;
  unsigned int ul;
  int i;
  int fsize;
  int c;
  TMenuPoint *itemsF;

  //FreeLines();
 
  fn=(char *)menu_path;
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;

//  _WriteLog("Menu.cfg find");
  c=fsize/sizeof(TMenuPoint);
  char msg[512];
  sprintf(msg, "%02d %s\n", c,"item");
//  _WriteLog(msg);

  FreeItemsList();
  
//  _WriteLog("Menu read");
  LockSched();
  itemsF=malloc(c*sizeof(TMenuPoint));
  for (i=0;i<c;i++)
  {
    //itemsF=realloc(itemsF,(i+1)*sizeof(TMenuPoint));
    fread(f,&itemsF[i], sizeof(TMenuPoint),&ul);
    AddToItem(itemsF[i].Text,itemsF[i].Description,itemsF[i].IconSmall,itemsF[i].IconBig, itemsF[i].Run);
  }
  fclose(f,&ul);
  
  mfree(itemsF);
  UnlockSched();

  return c;
}

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Menu");
}

void InitSettings()
{
// �������� ��������
  InitConfig();
 
  int w = menuRect.x2 - menuRect.x;
  int h = menuRect.y2 - menuRect.y;
  
  CellX=w/Columns;
  CellY=h/Rows;
  
  pos=Start;
  TmrReset();
  wsprintf((WSHDR*)&MAINCSM.maincsm_name,"%t",CSMText);
}


void main()
{
  unsigned int adr;

  //������ ������� MsgBox
  #pragma swi_number=0x8050
  __swi __arm unsigned int MsgBoxOkCancel_adr();
  adr=MsgBoxOkCancel_adr()+7; //��� ����� ���������� "B MsgBox"
  MsgBox=(int(*)(int,int,MSG_BOX*,int))(adr+(short)((*(short*)adr|0xF800))*2+5);

  //������ ������� AddHeader
  #pragma swi_number=0x8152
  __swi __arm unsigned int CreateMenu_adr();
  AddHeader=(void(*)(GUI*,HEADER_DESC*,void*))GetBLAddr(GetBLAddr(CreateMenu_adr()+33)+123);

  InitSettings();
 
  patch_header(&MHeader);
//  MHeader.lgp_id=(int)items[pos].point.Text;

//---------------
//  wsprintf((WSHDR*)&MAINCSM.maincsm_name,"%t",CSMText);

  LockSched();
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  memcpy(&icsmd,icsm->constr,sizeof(icsmd));
  oldIcsmOnClose=icsmd.onClose;
  icsmd.onClose=newIcsmOnClose;
  oldIcsmOnMessage=icsmd.onMessage;
  icsmd.onMessage=newIcsmOnMessage;
  icsm->constr=&icsmd;
  AddKeybMsgHook((void *)hook);
  UnlockSched();
}
