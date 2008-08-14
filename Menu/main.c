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
extern int TotalItems();

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
int pic_n=0;
int pos=0;

// список вложенных меню
LIST *MenuTop;


typedef struct
{
  int flag; //0, 8, 16 хз
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

int m; //коэффициент для формулы = isnewSGold

#pragma inline
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
int MSoftKeys[]={0,1,2};

SOFTKEY_DESC Msk[]=
{
  {0x0018,0x0000,(int)TextLeft},
  {0x0000,0x0000,(int)TextRight},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB Mskt=
{
  Msk,0
};

GBSTMR tmr;


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


int isFile(char *fname)
{
  return (strstr(fname,".")!=0);
}

int isSub(char *fname)
{
  return (strstr(fname,".cfg")!=0);
}

int strh2int(char* str)
{
  int n,c=0;
  if (*(str+strlen(str)-1)=='h') *(str+strlen(str)-1)='\0';
  while ((*str!='h')&&(*str))
  {
    if ((*str>='a')&&(*str <='f')) n=*str-('A'-('9'+1))-('a'-'A');
    else if ((*str>='A')&&(*str<='F')) n=*str-('A'-('9'+1));
    else if ((*str>='0')&&(*str<='9')) n=*str;
    c=c*16+(n-'0');
    str++;
  }
  return c;
}

int str2int(const char *str)
{
  int n,c=0;
  while(*str!='\0')
  {
    n=*str;
    c=c*10+(n-'0');
    str++;
  }
  return c;
}

void RunEntry(char *entry)
{
  typedef int (*func)(void);
  func ff;
  ff=(func)strh2int(entry);
  if (ff!=NULL) SUBPROC((void *)(*ff));
}

void RunShort(char *fname)
{
  typedef void (*voidfunc)(); 
  voidfunc pp=(voidfunc)GetFunctionPointer(fname); 
  if(pp!=0) 
    SUBPROC((void*)pp);
  else
    ShowMSG(1,(int)"NULL pointer function!");
}

char *nill="";
void RunFile(char *fname)
{
  WSHDR *ws=AllocWS(256);
  str_2ws(ws,fname,strlen(fname)+1);
  ExecuteFile(ws,0,nill);
  FreeWS(ws);
}


void RunSub(char *sub_name)
{
      LockSched();
      LoadItems(sub_name);
      pos=0;
      LIST *sub=malloc(sizeof(LIST));
      sub->next=NULL;
      sprintf(sub->item,sub_name); 
      if(!MenuTop)
          MenuTop=sub; 
       else
        {
         sub->next=MenuTop;
         MenuTop=sub;
        }
      UnlockSched();
}


int Run(const char *s)
{
  char *file=malloc(strlen(s)+1);
  strcpy(file,s);
  
  if (isSub(file)) RunSub(file);
  else
  if (isFile(file)) RunFile(file);
  else
  if (strstr(file,"_")!=0)  
     RunShort(file); 
    else 
     RunEntry(file);

  mfree(file);  
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



char transparent[]={0x00,0x00,0x00,0x00};
const int font=FONT_SMALL;

void (*OOnRedraw)(GUI *data);
void NOnRedraw(GUI *data)
{
  OOnRedraw(data); //Сначала старый OnRedraw

  int TI=TotalItems();
  int delta=0;
  
  switch (styleMenu)
  {
    // отрисовка сетки
  case 0: 
    {
      
      if (pos>(Columns*Rows-1))
      {
       delta=(pos-(Columns*Rows-1))/Columns;
       if ((pos-(Columns*Rows-1))%Columns!=0)
       ++delta;
      }
    
      
      int setka=Columns*Rows;
      if ((TI-delta*Columns)<(Columns*Rows))
        setka=TI-delta*Columns;
      
      for(int i=0;i<setka;i++)
      {
        
        int cursor=CalcPic((char*)CURSOR_PATH);
        int icon;
        int x=0;
        int y=0;
        int cx=0;
        int cy=0;
    
       ITEM *Item=GetItem(i+delta*Columns);
        if ((!Animation)||((i+delta*Columns)!=pos))
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    
        
    /* --==Формула==-- */
        if (position_type==0)
        {
        x=menuRect.x+(CellX-GetImgWidth(icon))/2+CellX*(i%Columns);
        y=menuRect.y+(CellY-GetImgHeight(icon))/2+CellY*(i/Columns);
        cx=menuRect.x+(CellX-GetImgWidth(cursor))/2+CellX*(i%Columns);
        cy=menuRect.y+(CellY-GetImgHeight(cursor))/2+CellY*(i/Columns);
        }
        else
        {
        x=menuRect.x+OffsetX+CellX*(i%Columns);
        y=menuRect.y+OffsetY+CellY*(i/Columns);
        cx=x;
        cy=y;
        }
        
        if(((i+delta*Columns)==pos)&&cursorShow)
        DrawImg(cx,cy,cursor);
        
        DrawImg(x,y,icon);
    
      }
      
      // scrollbar
      int rowScroll=TI/Columns;
      if (TI>rowScroll*Columns) ++rowScroll;
      if ((scrollShow==1)||((rowScroll>Rows)&&(scrollShow==0)))
      {
        int posScroll=(pos)/Columns;
        //if (pos>posScroll*Columns) ++posScroll;
        int sh=(menuRect.y2-menuRect.y)/rowScroll;
        DrawLine(menuRect.x2-1,menuRect.y,menuRect.x2-1,menuRect.y2, LINE_DOTTED,scrollColor);
        DrawRectangle(menuRect.x2,menuRect.y+sh*posScroll,menuRect.x2-2,menuRect.y+sh*posScroll+sh,
                      0,0,scrollColor);
      }
   
      
      break;
    }
  // отрисовка списка
  case 1:
    {
      if (pos>(Rows-1))
       delta=(pos-(Rows-1));
      
      for(int i=0;i<Rows;i++)
      {
        
        int cursor=CalcPic((char*)CURSOR_PATH);
        int icon;
        int x=0;
        int y=0;
        int cx=0;
        int cy=0;
    
       ITEM *Item=GetItem(i+delta);
        if ((!Animation)||((i+delta)!=pos))
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    
        
    /* --==Формула==-- */
        if (position_type==0)
        {
        x=menuRect.x+(CellY-GetImgHeight(icon))/2;
        y=menuRect.y+(CellY-GetImgHeight(icon))/2+CellY*i;
        cx=menuRect.x+(CellY-GetImgHeight(cursor))/2;
        cy=menuRect.y+(CellY-GetImgHeight(cursor))/2+CellY*i;
        }
        else
        {
        x=menuRect.x+OffsetX;
        y=menuRect.y+OffsetY+CellY*i;
        cx=x;
        cy=y;
        }
        
        if(((i+delta)==pos)&&cursorShow)
        DrawImg(cx,cy,cursor);
        
        DrawImg(x,y,icon);
        
        WSHDR *ws=AllocWS(128);
        ascii2ws(ws,Item->Text);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*i+((CellY/2-GetFontYSIZE(listNameFont))/2),
                   menuRect.x2,menuRect.y+CellY*i+CellY/2,listNameFont,listNameStyle,listNameColor,0);
        ascii2ws(ws,Item->Description);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*i+CellY/2+((CellY/2-GetFontYSIZE(listDescFont))/2),
                   menuRect.x2,menuRect.y+CellY*i+CellY,
                   listDescFont,listDescStyle,listDescColor,0);
      }
      // scrollbar
      if ((scrollShow==1)||((TI>Rows)&&(scrollShow==0)))
      {
        int sh=(menuRect.y2-menuRect.y)/TI;
        DrawLine(menuRect.x2-1,menuRect.y,menuRect.x2-1,menuRect.y2, LINE_DOTTED,scrollColor);
        DrawRectangle(menuRect.x2,menuRect.y+sh*pos,menuRect.x2-2,menuRect.y+sh*pos+sh,
                      0,0,scrollColor);
      }
   
      break;
    }
  }
  
 
  
  ITEM *Item=GetItem(pos);
  if (descShow)
    {
      WSHDR *ws=AllocWS(128);
      ascii2ws(ws,Item->Description);
      DrawString(ws,descRect.x,
                 descRect.y+((descRect.y2-descRect.y-GetFontYSIZE(descFont))/2),
                 descRect.x2,descRect.y2,descFont,descStyle,deskColor,0);
    }
  if (headShow)
    {
      int headIcon=CalcPic(Item->IconSmall);
      int hi=(headRect.y2-headRect.y-GetImgHeight(headIcon))/2;
      DrawImg(headRect.x+hi,headRect.y+hi,headIcon);

      WSHDR *ws=AllocWS(128);
      ascii2ws(ws,Item->Text);
      DrawString(ws,headRect.x+2*hi+2+GetImgWidth(headIcon),
                 headRect.y+((headRect.y2-headRect.y-GetFontYSIZE(headFont))/2),
                 headRect.x2,headRect.y2,headFont,headStyle,headColor,0);
    }
  //_WriteLog("end draw");
}

const void * NGuiMeths[11];

void MGHook(GUI *gui, int cmd)
{
  switch(cmd)
  {
  case 1://Создание: подмена onRedraw
    {
      memcpy(NGuiMeths,gui->methods,11*sizeof(void*));
      gui->methods=(void*)NGuiMeths;
      OOnRedraw=(void(*)(GUI*))NGuiMeths[0];//(gui->methods[0]);
      NGuiMeths[0]=(void*)NOnRedraw;
    }
    break;
  case 5://Получение фокуса (?)
    TmrReset();
    break;
  case 6://Потеря фокуса
    GBS_DelTimer(&tmr);//Экономим ресурсы (типо)
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
        sprintf(s,"Menu v0.6\n(c)Eraser\n%s at %s",__DATE__,__TIME__);
        ShowMSG(2,(int)s);
      }
     if (key=='#')
      {
        // редактирование настроек
        WSHDR *ws;
        ws=AllocWS(150);
        extern const char *successed_config_filename;
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
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
      if (styleMenu==0)
      pos-=Columns;
      else
        pos--;
      break;
    case DOWN_BUTTON:
      if (styleMenu==0)
      pos+=Columns;
      else
        pos++;
      break;
    case RIGHT_SOFT:
      if (!MenuTop)
      {
        GBS_DelTimer(&tmr);
        return 1;
      }
      else
      {
        LIST *List=(LIST *)MenuTop->next;
        mfree(MenuTop);
        MenuTop=List;
        LockSched();
        if (MenuTop)
        LoadItems(MenuTop->item);
        else
        LoadItems(MENU_PATH);
        pos=0;
        UnlockSched();
      }
      
      break;
      //return(0);
    case LEFT_SOFT:
      Run(RunLeft);
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
      pos=TotalItems()-1;
    if(pos>TotalItems()-1)
      pos=0;

  //end:
    TmrReset();
    RefreshGUI();
    return(0);
  run:
    {
    ITEM *Item=GetItem(pos);
    Run(Item->Run);
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
__thumb unsigned int GetBLAddr(unsigned int adr)
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
  

  fn=(char *)menu_path;
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;

//  _WriteLog("Menu.cfg find");
  c=fsize/sizeof(TMenuPoint);
//  char msg[512];
//  sprintf(msg, "%02d %s\n", c,"item");
//  _WriteLog(msg);

  LockSched();
  FreeItemsList();
  TMenuPoint *itemsF;
  itemsF=malloc(c*sizeof(TMenuPoint));
  for (i=0;i<c;i++)
  {
   fread(f,&itemsF[i],sizeof(TMenuPoint),&ul);
   AddToItem(itemsF[i].Text,itemsF[i].Description,itemsF[i].IconSmall,itemsF[i].IconBig, itemsF[i].Run);
  }
  fclose(f,&ul);
  UnlockSched();
  
  mfree(itemsF);
//_WriteLog("Menu load");
  return c;
}

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Menu");
}

void InitSettings()
{
// загрузка настроек
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

    //Нахожу функцию MsgBox
  #pragma swi_number=0x8050
  __swi __arm unsigned int MsgBoxOkCancel_adr();
  adr=MsgBoxOkCancel_adr()+7; //Тут лежит инструкция "B MsgBox"
  MsgBox=(int(*)(int,int,MSG_BOX*,int))(adr+(short)((*(short*)adr|0xF800))*2+5);
  
 

  InitSettings();
  MenuTop=NULL;
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
