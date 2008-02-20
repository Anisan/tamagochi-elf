// Эльф-тамагочи
// Eraser @2007
// anis_1981@mail.ru
// ICQ:308911945

#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "..\inc\xtask_ipc.h"
#include "Tamagochi.h"
#include "RANDOM.h"
#include "conf_loader.h"
#include "language.h"


#ifdef NEWSGOLD
#define USE_ONE_KEY
#endif

#ifdef ELKA
#pragma swi_number=54
__swi __arm void SLI_SetState(unsigned char state);
#endif

#define TMR_SECOND 216
#define IN_TICKS(sec) (sec * TMR_SECOND)

unsigned int fShow;

// -1 - XTask GUI present
// 0 - XTask GUI absent
// 1 - IBUT longpressed, ready for exit
int mode;

// 0 - no press
// 1 - long press ENTER_BUTTON
// 2 - disable KEY_UP process
int mode_enter;

// для отрисовки смайла и статуса
IMGHDR *Smile=0;
IMGHDR *StatImg=0;
int old_img_status=99;
int old_simg_status=99;
int simg_status=0;

int my_csm_id;
int maincsm_id;
CSM_RAM *under_idle;


extern void kill_data(void *p, void (*func_p)(void *));

extern const int SpeedLife;

const char ipc_my_name[]=IPC_TAMAGOCHI_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
IPC_REQ gipc;

char game_list[32768];
char player_list[32768];

/*
static const char * const games_names[2]=
{
  "Tetris",
  "mc"  
};
*/
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
int Fatigue=0; //усталость
int Sleep=0;
int Behavior=0;

 extern const char *successed_config_filename;
 extern const char UNDER_IDLE_CONSTR[];
 extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

//=============================
CSM_RAM *GetUnderIdleCSM(void)
{
  CSM_RAM *u;
  CSM_DESC *UnderIdleDesc;
  if (strlen((char *)UNDER_IDLE_CONSTR)==8)
  {
    UnderIdleDesc=(CSM_DESC *)strtoul((char *)UNDER_IDLE_CONSTR,0,0x10);
  }
  else
  {
    UnderIdleDesc=((CSM_RAM *)(FindCSMbyID(CSM_root()->idle_id))->prev)->constr;
    sprintf((char *)UNDER_IDLE_CONSTR,"%08X",UnderIdleDesc);
    SaveConfigData(successed_config_filename);
  }
  LockSched();
  u=CSM_root()->csm_q->csm.first;
  while(u && u->constr!=UnderIdleDesc) u=u->next;
  UnlockSched();
  return u;
}
///----------------------------------
// загрузка иконок
int S_ICONS[9+1];
int M_ICONS[15+1];

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
    if (!S_ICONS[i]) S_ICONS[i]=(int)MakeGlobalString(PIC_PATH,0,icons_names[i]);
    i++;
  }
  while(i<9);
  i=0;
  do
  {
    if (!M_ICONS[i]) M_ICONS[i]=(int)MakeGlobalString(PIC_PATH,0,icons_menu[i]);
    i++;
  }
  while(i<15);
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
  while(i<15);
  

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
//////////////////////////////////////////////////////////////////////
// image function scale opacity
#define CTYPE1 10
#define CTYPE2 3

typedef struct {
  int x;
  int y;
}POINT;

typedef struct {
  char R;
  char G;
  char B;
  char A;
}color;

int max(int x, int y)
{
  return x > y ? x : y;
}

int min(int x, int y)
{
  return x < y ? x : y;
}

DrwImg(IMGHDR *img, int x, int y, char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}

color RGBA(char R, char G, char B, char A)
{
  color t={R,G,B,A};
  return t;
}


color getcolor(IMGHDR *img, int x, int y)
{
  color *bm=(color*)img->bitmap;
  if(x < img->w && x>=0 && y < img->h && y>=0) 
    return *(bm + x + y*img->w);
  else
    return (color){0,0,0,0};
}


void setcolor(IMGHDR *img, int x, int y, color clr)
{
  color *bm=(color*)img->bitmap;
  if(x < img->w && x>=0 && y < img->h && y>=0)
    *(bm + x + y*img->w)=clr;
}

void bmfree(IMGHDR *img)
{
  int x, y;
  for(y=0; y<img->h; y++)
    for(x=0; x<img->w; x++)
      setcolor(img, x, y, (color){0,0,0,0});
}

IMGHDR *createIMGHDR(int w, int h, int type)
{
  IMGHDR *img=malloc(sizeof(IMGHDR));
  img->w=w; 
  img->h=h; 
  img->bpnum=type;
  img->bitmap=malloc(h*w*sizeof(color));
  bmfree(img);
  return img;
}

void deleteIMGHDR(IMGHDR *img)
{
  mfree(img->bitmap);
  mfree(img); 
}

IMGHDR *alpha(IMGHDR *img, char a, int nw, int del)
{
  int i;
  color *r=(color*)img->bitmap;
  for(i=0;i<img->h*img->w; i++, r++)
    if(r->A>a)
      r->A-=a;
    else
      r->A=0;
    return img;
}


IMGHDR *resample(IMGHDR *img, int px, int py, int fast, int del)
{
  if (px==100 && py==100) return img;
  
  long newx = (img->w*px)/100,
  newy = (img->h*py)/100;
  
  float xScale, yScale, fX, fY;
  xScale = (float)img->w  / (float)newx;
  yScale = (float)img->h / (float)newy;
  
  IMGHDR *img2=createIMGHDR(newx,newy, CTYPE1);
  if (fast) {
    for(long y=0; y<newy; y++){
      fY = y * yScale;
      for(long x=0; x<newx; x++){
        fX = x * xScale;
        setcolor(img2,  x,  y, getcolor(img, (long)fX,(long)fY));
      }
    }
  } else {
    long ifX, ifY, ifX1, ifY1, xmax, ymax;
    float ir1, ir2, ig1, ig2, ib1, ib2, ia1, ia2, dx, dy;
    char r,g,b,a;
    color rgb1, rgb2, rgb3, rgb4;
    xmax = img->w-1;
    ymax = img->h-1;
    for(long y=0; y<newy; y++){
      fY = y * yScale;
      ifY = (int)fY;
      ifY1 = min(ymax, ifY+1);
      dy = fY - ifY;
      for(long x=0; x<newx; x++){
        fX = x * xScale;
        ifX = (int)fX;
        ifX1 = min(xmax, ifX+1);
        dx = fX - ifX;
        rgb1= getcolor(img, ifX,ifY);
        rgb2= getcolor(img, ifX1,ifY);
        rgb3= getcolor(img, ifX,ifY1);
        rgb4= getcolor(img, ifX1,ifY1);
        
        ir1 = rgb1.R   * (1 - dy) + rgb3.R   * dy;
        ig1 = rgb1.G * (1 - dy) + rgb3.G * dy;
        ib1 = rgb1.B  * (1 - dy) + rgb3.B  * dy;
        ia1 = rgb1.A  * (1 - dy) + rgb3.A  * dy;
        ir2 = rgb2.R   * (1 - dy) + rgb4.R   * dy;
        ig2 = rgb2.G * (1 - dy) + rgb4.G * dy;
        ib2 = rgb2.B  * (1 - dy) + rgb4.B  * dy;
        ia2 = rgb2.A  * (1 - dy) + rgb4.A  * dy;
        
        r = (char)(ir1 * (1 - dx) + ir2 * dx);
        g = (char)(ig1 * (1 - dx) + ig2 * dx);
        b = (char)(ib1 * (1 - dx) + ib2 * dx);
        a = (char)(ia1 * (1 - dx) + ia2 * dx);
        
        setcolor(img2, x,y,RGBA(r,g,b,a));
      }
    }
  }
  
  if(del)
    deleteIMGHDR(img);
  return img2;
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

//------------------------------------------------
GBSTMR tmr_vibra;
volatile int vibra_count;

void start_vibra(void)
{
  extern const int VIBR_TYPE;
  extern const int Is_Vibra_Enabled;
  void stop_vibra(void);
  if((Is_Vibra_Enabled)&&(!IsCalling()))
  {
    extern const unsigned int vibraPower;
    SetVibration(vibraPower);
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>2,stop_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,stop_vibra);
  }
}

void stop_vibra(void)
{
  extern const int VIBR_TYPE;
  SetVibration(0);
  if (--vibra_count)
  {
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>5,start_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,start_vibra);
  }
}
//------------------------------------------------
int count = 0;
//int cycl_cnt = 0;

GBSTMR tmr_light;

void LightOff(void);
void Check(void);

//SetIllumination(unsigned char dev,unsigned long param1,unsigned short bright,unsigned long delay);
//0 - дисплей, 1 - клавиатура, 2 - динамический свет (x65)
void LightOn()
{
	if (cfgDispl)
		SetIllumination(0, 1, cfgBright, 0);
	if (cfgKbd)
		SetIllumination(1, 1, cfgBright, 0);   
#ifndef NEWSGOLD
	if (cfgDyn)
		SetIllumination(2, 1, cfgBright, 0);
#else
	if (cfgLighter)
		SetIllumination(4, 1, cfgBright, 0);
#endif
        
#ifdef ELKA
        if (cfgSLI)
                SLI_SetState(1);
#endif
 
	GBS_StartTimerProc(&tmr_light, IN_TICKS(cfgPeriod) / 20, LightOff);
}

void LightOff()
{
	if (cfgDispl)
		SetIllumination(0, 1, 0, 0);
	if (cfgKbd)
		SetIllumination(1, 1, 0, 0);   
#ifndef NEWSGOLD
	if (cfgDyn)
		SetIllumination(2, 1, 0, 0);
#else
	if (cfgLighter)
		SetIllumination(4, 1, 0, 0);
#endif
#ifdef ELKA
        if (cfgSLI)
        {
                SLI_SetState(2);
                SLI_SetState(0);
        }
#endif
	if (--count)
		GBS_StartTimerProc(&tmr_light, IN_TICKS(cfgPeriod) / 20, LightOn);
}

void Light()
{
	if (!(IsUnlocked() && cfgLockOnly))
	{
	count = cfgMaxEv;
	LightOn();
	}
}
//------------------------------------------------

extern void ChangeStatusImage();
extern void VerifyStatus();
static void UpdateCSMname(void);

//------------------------------------------------
void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}

//------------------------------------------------
void _WriteLog(char *buf)
{
  int flog=-1;
  unsigned int err;
  flog = fopen("4:\\ZBin\\tamagochi.log",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
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

int GameDetected(void)
{
  extern const char CsmAdr[9];
  int find=0;
  CSM_RAM *icsm=under_idle->next; 
 // _WriteLog("Begin");
  do
  {
      WSHDR *tws=(WSHDR *)(((char *)icsm->constr)+sizeof(CSM_DESC));
      if((tws->ws_malloc==NAMECSM_MAGIC1)&&(tws->ws_mfree==NAMECSM_MAGIC2))
	{
            char s[40];
            char *p;
            ws_2str(tws, s, 64);
            p=strstr(game_list,s);
            if (p)
            {
              find=1;
              simg_status=1;
             // ShowMSG(0,(int)LG_COOL);
             // _WriteLog("Find");
            }
            p=strstr(player_list,s);
            if (p)
            {
              find=1;
              simg_status=3;
             // ShowMSG(0,(int)LG_COOL);
             // _WriteLog("Find");
            }
      }
      else
      {
        // ищем медиаплеер
        char s[40];
        char *p;
        CSM_DESC *desc=icsm->constr;
        sprintf(s,"%08X ",desc);
        p=strstr(player_list,s);
        if (p)
        {
         simg_status=3;
         find=1;
        }
      }
  }
  while((icsm=icsm->next));

  //_WriteLog("End");
  return(find);
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

void SleepProc();

//главная процедура жизненного цикла питомца
void TimerProc2(void)
{
  
  extern const int Night_Ena;
  extern const unsigned int Night_begin;
  extern const unsigned int Night_end;

  if (StatusPet.StatusDeath==1) 
  {
    simg_status=0;
    ChangeStatusImage();
    return ;
  }

  if (Night_Ena)
  {
      //проверка ночи
      TTime t;
      TDate d;
      GetDateTime(&d,&t);
      int fNight=0;
      if (Night_begin>=Night_end)
        if ((t.hour>=Night_begin)||(t.hour<Night_end))
          fNight=1;
      if (Night_begin<Night_end)
        if ((t.hour>=Night_begin)&&(t.hour<Night_end)) 
          fNight=1;
    
      if (Sleep!=5)
      {
        //не спит
        if (fNight)
        { 
        if (Sleep!=0) SleepProc(); 
        Sleep=5;
        StatusPet.ImageStatus=2;
        simg_status=2;
        ChangeStatusImage();
        GBS_StartTimerProc(&mytmr2,TMR_SECOND*60*(SpeedLife+1),TimerProc2);
        return;
        }
      }
      if (Sleep==5)
      {
        //спит
        if (!fNight)
        {  
         SleepProc(); 
         simg_status=0;
        }
        else
        {
         StatusPet.ImageStatus=2;
         simg_status=2;
         ChangeStatusImage();
         GBS_StartTimerProc(&mytmr2,TMR_SECOND*60*(SpeedLife+1),TimerProc2);
         return; 
        }
      }
  }
  
// усталость =100 сон
// усталость =0 подъем
  if ((Fatigue>=100))
    SleepProc();
  if ((Sleep!=0)&&(Fatigue<=0))
    SleepProc();  
  if (Sleep==0) ++Fatigue; else --Fatigue;
  
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
    
    //StatusPet.Health=StatusPet.MaxHealth;
    //StatusPet.Hunger=0;
    StatusPet.Happiness=StatusPet.MaxHappiness;
    StatusPet.Boredom=0;
    StatusPet.Behaviour=(int)2*StatusPet.MaxBehaviour/3;
    
    
    char sound_name[128];
    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[2]);
    Play(sound_name);
    vibra_count=3;
    start_vibra();
    Light();
    ShowMSG(0,(int)LG_MSGAGE);
    
    UpdateCSMname();
  }

    
  // сон - процессы замедляются
  if ((Sleep!=0)&&(Sleep!=3))
  {
    ++ Sleep;
    StatusPet.ImageStatus=2;
    ChangeStatusImage();
    GBS_StartTimerProc(&mytmr2,TMR_SECOND*60*(SpeedLife+1),TimerProc2);
    return;
  }
  
  
  if (Sleep!=0)Sleep=1;
//во время сна
  if (Sleep!=0) ++StatusPet.Happiness;
//когда не спит
  if (Sleep==0)
  {
    --StatusPet.Behaviour; // дисциплина
      // пакость = убивает процесс, если совсем дисциплина на нуле,
  // и чтобы сильно не ругали дисциплина увеличивается до 30%
       extern const int Malware_Ena;
       if ((StatusPet.Behaviour<=0)&&Malware_Ena)
            {
                int fkill=1;
                CSM_RAM *icsm=under_idle->next; 
                  do
                  {
                    int i=((CSM_RAM *)(icsm))->id;
                    if (fkill)
                    if (i!=CSM_root()->idle_id) 
                    {
                     CloseCSM(i);
                     StatusPet.Behaviour=(int)StatusPet.MaxBehaviour/3;
                     fkill=0;
                     char sound_name[128];
                    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[13]);
                    Play(sound_name);
                    }  
                  }
                  while((icsm=icsm->next));
            }
  // проверка на наличие в процессах игр
      extern const int Scan_Game_Ena  ;
      if (Scan_Game_Ena)
      {
        if (GameDetected())
        {
          --StatusPet.Boredom; // скука
          ++StatusPet.Happiness;
        }
        else 
        {
          //--StatusPet.Happiness;
          ++StatusPet.Boredom; // скука
          simg_status=0;
        }
      }
      else
          ++StatusPet.Boredom; // скука
      
  }  

  // питание
  ++StatusPet.Hunger;
  // проверка наличия жратвы и наличие мусора
  int Eat=0;
  if (StatusPet.Hunger>(int)StatusPet.MaxHunger/3) Eat=1;
  // ест только когда не спит
  if (Sleep!=0) Eat=0;
   
   
  DIR_ENTRY de;
  unsigned int err;
  char name[256];
  char * ext;
  int findgvn=0;
  int fEat=0;
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
            //кормимся
            if (Eat)
            if (StatusPet.Hunger>0)
            if (de.file_size<(StatusPet.Age+1+5)*1024)
            {
              int in = (int)de.file_size/1024;
              if (in==0) in=1;
              StatusPet.Hunger=StatusPet.Hunger-in;
              // файл перименовываем в мусор
              char name2[256];
              strcpy(name2,ROOM_PATH);
              strcat(name2,de.file_name);
              strcat(name2,".gvn");
              fmove(ptr, name2, &err); 
              fEat=1;
            }
          }
        }
      }
    }
    while(FindNextFile(&de,&err));
    if (fEat)
    {
    char sound_name[128];
    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[13]);
    Play(sound_name);
    }
  }  
  FindClose(&de,&err);
  
  // голод
  if (StatusPet.Hunger>(int)2*StatusPet.MaxHunger/3) 
  {
    --StatusPet.Happiness;
    if (Sleep!=0) SleepProc(); 
  }  
  // сытый
  if (StatusPet.Hunger==0) ++StatusPet.Happiness;
  
  // чистота 
  StatusPet.Dirtiness=findgvn;
  if (StatusPet.Dirtiness>(int)StatusPet.MaxDirtiness/2) --StatusPet.Health;
  if (StatusPet.Dirtiness>(int)2*StatusPet.MaxDirtiness/3) --StatusPet.Happiness;
  
  if (StatusPet.Behaviour<=(int)StatusPet.MaxBehaviour/10) --StatusPet.Health;
  if (StatusPet.Boredom>(int)StatusPet.MaxBoredom/2) --StatusPet.Happiness;
  if (StatusPet.Happiness<=(int)StatusPet.MaxHappiness/2) --StatusPet.Health;
  
  // проверки максимальных - минимальных значений
  VerifyStatus();
    
  // суицид и смерть
  if ((StatusPet.Health==0)||(StatusPet.Hunger>=StatusPet.MaxHunger)||(StatusPet.Happiness==0))
  {
    StatusPet.StatusDeath=1;
    char sound_name[128];
    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[7]);
    Play(sound_name);
    vibra_count=3;
    start_vibra();
    Light();
  }
  
  ChangeStatusImage();
  
  // звуки и сообщения
  // дисциплина
  if (Sleep==0)
  if (Behavior>=StatusPet.Behaviour)
  {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[0]);
  Play(sound_name);
  if(VIBR_TYPE)
      vibra_count=2;
    else
      vibra_count=1;
  start_vibra();
  Light();
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
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[11]);
  Play(sound_name);
  ShowMSG(2, (int)LG_IAMSLEEP);
 return;
 }
 if (StatusPet.StatusDeath==1)
 {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[8]);
  Play(sound_name);
 ShowMSG(2, (int)LG_IAMDIE);
 return;
 }

 StatusPet.Happiness = StatusPet.Happiness+10;
 StatusPet.Boredom = StatusPet.Boredom -10;
 StatusPet.Hunger = StatusPet.Hunger +10;
 Fatigue=Fatigue+10;
 char sound_name[128];
 snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[10]);
 Play(sound_name);
 ShowMSG(2, (int)LG_COOL);
 ChangeStatusImage();
 VerifyStatus();
}
// ----------------------------------------------------------------------------
void Vaccinate()
{
 if (Sleep!=0)
 {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[11]);
  Play(sound_name);
 ShowMSG(2, (int)LG_IAMSLEEP);
 return;
 }
 if (StatusPet.StatusDeath==1)
 {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[8]);
  Play(sound_name);
 ShowMSG(2, (int)LG_IAMDIE);
 return;
 }
 
 StatusPet.Happiness = StatusPet.Happiness-10;
 StatusPet.Health = StatusPet.Health +10;
 StatusPet.Boredom = StatusPet.Boredom -10;
 char sound_name[128];
 snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[15]);
 Play(sound_name);
 ShowMSG(2, (int)LG_PAIN);
 ChangeStatusImage();
 VerifyStatus();
}
// ----------------------------------------------------------------------------
void Discipline()
{
 if (Sleep!=0)
 {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[11]);
  Play(sound_name);
 ShowMSG(2, (int)LG_IAMSLEEP);
 return;
 }
 if (StatusPet.StatusDeath==1)
 {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[8]);
  Play(sound_name);
 ShowMSG(2, (int)LG_IAMDIE);
 return;
 }
 
 StatusPet.Happiness = StatusPet.Happiness-10;
 StatusPet.Behaviour = StatusPet.Behaviour +10;
 StatusPet.Boredom = StatusPet.Boredom -10;
 char sound_name[128];
 snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[17]);
 Play(sound_name);
 ShowMSG(2, (int)LG_SHREW);
 ChangeStatusImage();
 VerifyStatus();
}
// ----------------------------------------------------------------------------
void SleepProc()
{
    if (StatusPet.StatusDeath==1)
 {
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[8]);
  Play(sound_name);
 ShowMSG(2, (int)LG_IAMDIE);
 return;
 }
  if(Sleep==0){ Sleep=1;
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[14]);
  Play(sound_name);
  ShowMSG(2, (int)LG_MSGSLEEP);}
  else {Sleep=0;
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[5]);
  Play(sound_name);
  ShowMSG(2, (int)LG_MSGNOTSLEEP);}
  ChangeStatusImage();
}
// ----------------------------------------------------------------------------
void ChangeStatusImage()
{
  // Статусные картинки
  StatusPet.ImageStatus=0;
  if (StatusPet.Behaviour<(int)StatusPet.MaxBehaviour/3) StatusPet.ImageStatus=8;
  if (StatusPet.Boredom>(int)StatusPet.MaxBoredom/2) StatusPet.ImageStatus=7;
  if (StatusPet.Dirtiness>(int)StatusPet.MaxDirtiness/2) StatusPet.ImageStatus=6;
  if (StatusPet.Hunger>(int)StatusPet.MaxHunger/2) StatusPet.ImageStatus=3;
  if (StatusPet.Health<(int)StatusPet.MaxHealth/2) StatusPet.ImageStatus=4;
  if (StatusPet.Happiness<(int)StatusPet.MaxHappiness/2) StatusPet.ImageStatus=5;
  // сон
  if (Sleep!=0) StatusPet.ImageStatus=2; 
  if (StatusPet.StatusDeath==1) StatusPet.ImageStatus=1;

  if (old_simg_status!=simg_status)
  {
  if(StatImg)
    deleteIMGHDR(StatImg);
  
  StatImg=0;
  char pic_name[128];
  snprintf(pic_name, 127, "%s%s", PIC_PATH, icons_status[simg_status]);
  int SIZE=MINSIZE+StatusPet.Age;
  if (SIZE>MAXSIZE)SIZE=MAXSIZE;
  
  
    
  if(strlen(pic_name)) 
    if(StatImg=CreateIMGHDRFromPngFile(pic_name, CTYPE2))
    {
      //StatImg=resample(StatImg, SIZE, SIZE, 0, 1);
      if (Effects_Ena)
      if (OP!=100)
      StatImg=alpha(StatImg, 255-255*OP/100, 0, 0);
    }   
  old_simg_status=simg_status;
  }

  
  if (old_img_status!=StatusPet.ImageStatus)
  {
  if(Smile)
    deleteIMGHDR(Smile);
  
  Smile=0;
  char pic_name[128];
  snprintf(pic_name, 127, "%s%s", PIC_PATH, icons_names[StatusPet.ImageStatus]);
  int SIZE=MINSIZE+StatusPet.Age;
  if (SIZE>MAXSIZE)SIZE=MAXSIZE;
  
  if(strlen(pic_name)) 
    if(Smile=CreateIMGHDRFromPngFile(pic_name, CTYPE2))
    {
      if (Effects_Ena)
      {
      Smile=resample(Smile, SIZE, SIZE, 0, 1);
      if (OP!=100)
      Smile=alpha(Smile, 255-255*OP/100, 0, 0);
      }
    }   
  if(VIBR_TYPE)
      vibra_count=2;
    else
      vibra_count=1;
  start_vibra();
  Light();
  old_img_status=StatusPet.ImageStatus;
  }
  
  char sound_name[128];
  {
  if (StatusPet.Dirtiness>=StatusPet.MaxDirtiness-10) 
   { 
     ShowMSG(1,(int)LG_MSGDIRTINESS);
     snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[13]); 
   }
  if (StatusPet.Boredom>=StatusPet.MaxBoredom-10) 
  { 
    ShowMSG(1,(int)LG_MSGBOREDOM);
    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[6]);
  }
  if (StatusPet.Hunger>=StatusPet.MaxHunger-10) 
  {
    ShowMSG(1,(int)LG_MSGHUNGER);
    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[5]);
  }
  if (StatusPet.Health<=10)
  {
    ShowMSG(1,(int)LG_MSGHEALTH);
    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[4]);
  }
  if (StatusPet.Happiness<=10)
    snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[12]);
  }
  Play(sound_name);
 
}

// ----------------------------------------------------------------------------
void VerifyStatus()
{
// проверки максимальных - минимальных значений
  if (StatusPet.Hunger>StatusPet.MaxHunger) StatusPet.Hunger=StatusPet.MaxHunger;
  if (StatusPet.Hunger<0) StatusPet.Hunger=0;
  if (StatusPet.Happiness>StatusPet.MaxHappiness) StatusPet.Happiness=StatusPet.MaxHappiness;
  if (StatusPet.Happiness<0) StatusPet.Happiness=0;
  if (StatusPet.Health>StatusPet.MaxHealth) StatusPet.Health=StatusPet.MaxHealth;
  if (StatusPet.Health<0) StatusPet.Health=0;
  if (StatusPet.Dirtiness>StatusPet.MaxDirtiness) StatusPet.Dirtiness=StatusPet.MaxDirtiness;
  if (StatusPet.Dirtiness<0) StatusPet.Dirtiness=0;
  if (StatusPet.Boredom>StatusPet.MaxBoredom) StatusPet.Boredom=StatusPet.MaxBoredom;
  if (StatusPet.Boredom<0) StatusPet.Boredom=0;
  if (StatusPet.Behaviour>StatusPet.MaxBehaviour) StatusPet.Behaviour=StatusPet.MaxBehaviour;
  if (StatusPet.Behaviour<0) StatusPet.Behaviour=0;
  if (Fatigue>100)Fatigue=100;
  if (Fatigue<0)Fatigue=0;
}
//-----------------------------------------------------------------------------
void CheckDoubleRun(void)
{
  int csm_id;
  if ((csm_id=(int)(gipc.data))!=-1)
  {
    gipc.name_to=ipc_xtask_name;
    gipc.name_from=ipc_my_name;
    gipc.data=(void *)csm_id;
    LockSched();
    CloseCSM(maincsm_id);
    ShowMSG(1,(int)LG_ALREADY_STARTED);
    UnlockSched();
  }
  else
  {
    extern const int ENA_HELLO_MSG;
    if (ENA_HELLO_MSG) ShowMSG(1,(int)LG_LOADING);
  }
  
}


int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  //MAIN_CSM *csm=(MAIN_CSM*)data;
  void *icsm;
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)LG_UPDSETTING);
      InitConfig();
      old_img_status=99;
      ChangeStatusImage();
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
        case IPC_CHECK_DOUBLERUN:
       //Если приняли свое собственное сообщение, значит запускаем чекер
	  if (ipc->name_from==ipc_my_name) SUBPROC((void *)CheckDoubleRun);
          else ipc->data=(void *)maincsm_id;
	  break;
        case IPC_UPDATE_STAT:
#ifdef NEWSGOLD
          if (!getCpuUsedTime_if_ena())
          {
            StartCpuUsageCount();
          }
#endif
          GBS_StartTimerProc(&mytmr, REFRESH*TMR_SECOND/10, TimerProc);
          break;
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
    if (IsGuiOnTop(idlegui_id(icsm)) && fShow) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
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
         
          if(Effects_Ena)
          {
              if(Smile)
              DrawCanvas(canvasdata, 
                         POS_X - Smile->w/2, 
                         POS_Y - Smile->h/2, 
                         POS_X + Smile->w/2, 
                         POS_Y + Smile->h/2, 
                         1);
              //отрисовка смайла
              if(Smile)
                DrwImg(Smile,
                       POS_X-Smile->w/2,
                       POS_Y-Smile->h/2,
                       GetPaletteAdrByColorIndex(0),
                       GetPaletteAdrByColorIndex(1));
              // отрисовка статуса (в нижнем левом углу смайла)
               if((Smile)&&(StatImg))
                DrwImg(StatImg,
                       POS_X+Smile->w/2-StatImg->w,
                       POS_Y+Smile->h/2-StatImg->h,
                       GetPaletteAdrByColorIndex(0),
                       GetPaletteAdrByColorIndex(1));
          }
          else
          {
            //отрисовка смайла
            char pic_name[128];
              snprintf(pic_name, 127, "%s%s", PIC_PATH, icons_names[StatusPet.ImageStatus]);
              DrawCanvas(canvasdata, POS_X-GetImgWidth((int)pic_name)/2, POS_Y - GetImgHeight((int)pic_name)/2, POS_X+GetImgWidth((int)pic_name)/2, POS_Y + GetImgHeight((int)pic_name)/2, 1);
              DrawImg( POS_X-GetImgWidth((int)pic_name)/2, POS_Y - GetImgHeight((int)pic_name)/2, (int)pic_name);
            // отрисовка статуса (в нижнем левом углу смайла)
            char spic_name[128];
              snprintf(spic_name, 127, "%s%s", PIC_PATH, icons_status[simg_status]);
              DrawImg( POS_X+GetImgWidth((int)pic_name)/2-GetImgWidth((int)spic_name), POS_Y + GetImgHeight((int)pic_name)/2-GetImgHeight((int)spic_name), (int)spic_name);
    
          }
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
  
  int sz=0;
  extern const char GAMELIST_PATH[64];
  if ((f=fopen(GAMELIST_PATH,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    sz=fread(f,game_list,32767,&ul);
    fclose(f,&ul);
  }
  if (sz>=0) game_list[sz]=0;

  extern const char PLAYERLIST_PATH[64];
  if ((f=fopen(PLAYERLIST_PATH,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    sz=fread(f,player_list,32767,&ul);
    fclose(f,&ul);
  }
  if (sz>=0) player_list[sz]=0;
  
  //запуск жизненного цикла
  GBS_StartTimerProc(&mytmr2,TMR_SECOND,TimerProc2);
  GBS_StartTimerProc(&savetmr,TMR_SECOND*60*5,TimerSave);

  UpdateCSMname();
  ChangeStatusImage();
  
  char sound_name[128];
  snprintf(sound_name, 127, "%s%s", SOUND_PATH, SoundName[1]);
  Play(sound_name);
  if(VIBR_TYPE)
      vibra_count=2;
    else
      vibra_count=1;
  start_vibra();
  Light();
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
 
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
          if (IsUnlocked())
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

      return KEYHOOK_BREAK;

    }
  }
  return KEYHOOK_NEXT;
  
}

//---------------

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"%t - %d age",IPC_TAMAGOCHI_NAME,StatusPet.Age);
}
// ----------------------------------------------------------------------------

void DoSplices(void)
{

  LockSched();
  
         
      CSMROOT *csmr;
      CSM_RAM *save_cmpc;
      CSM_RAM main_csm;
      UpdateCSMname();
      csmr=CSM_root();
      save_cmpc=csmr->csm_q->current_msg_processing_csm;
      csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
      maincsm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
      csmr->csm_q->current_msg_processing_csm=save_cmpc;
  
      under_idle=GetUnderIdleCSM();
      
  UnlockSched();
}
  
void main(void)
{
  
  InitConfig();
  DoSplices();
}



