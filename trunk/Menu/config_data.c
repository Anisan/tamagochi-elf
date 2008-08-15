#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "main.h"

//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif



 __root const CFG_HDR cfghdr4_1={CFG_UINT,"Hotkey (dec)",0,99};
 __root const int ACTIVE_KEY=26;
 
 __root const CFG_HDR cfghdr7_2={CFG_STR_WIN1251,"CSM name",0,31};
 __root const char CSMText[32]="Главное меню";

//==============================================================
__root const CFG_HDR cfghdr_si11={CFG_LEVEL,"Setting interface",1,0};


 //menu
  __root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Menu",1,0};

  __root const CFG_HDR cfghdr1_4 = {CFG_UINT, "Rows", 0, 30};
  __root const unsigned int Rows = 4;

  __root const CFG_HDR cfghdr1_5 = {CFG_UINT, "Colunms", 0, 20};
  __root const unsigned int Columns = 3;

  __root const CFG_HDR cfghdr1_3 = {CFG_UINT, "Start item", 0, 600};
  __root const unsigned int Start = 4;

  __root const CFG_HDR cfghdr1_10 = {CFG_CHECKBOX, "Show cursor", 0, 2};
  __root const int cursorShow = 1;
  
  __root const CFG_HDR cfghdr1_6={CFG_RECT, "Position menu", 0, 0};
  __root const RECT menuRect = {0, 48, 240, 320-32-16};
  
  __root const CFG_HDR cfghdr1_7={CFG_CBOX, "Style menu", 0, 2};
  __root const int styleMenu = 0;
  __root const CFG_CBOX_ITEM cfgcbox0[2] = {"Matrix", "List"};

  __root const CFG_HDR cfghdr_f11={CFG_LEVEL,"Fonts list",1,0};

    __root const CFG_HDR cfghdrfl_h0={CFG_UINT,"Font name",0,65535};
    __root const unsigned int listNameFont=FONT_MEDIUM;

    __root const CFG_HDR cfghdrfl_h1={CFG_UINT,"Style name",0,65535};
    __root const unsigned int listNameStyle=TEXT_ALIGNLEFT;
    
    __root const CFG_HDR cfghdrfl_h2={CFG_COLOR,"Color name",0,0};
    __root const char listNameColor[4]={0xFF,0xFF,0xFF,0x64};
    
    __root const CFG_HDR cfghdrfl_h3={CFG_UINT,"Font description",0,65535};
    __root const unsigned int listDescFont=FONT_SMALL;

    __root const CFG_HDR cfghdrfl_h4={CFG_UINT,"Style description",0,65535};
    __root const unsigned int listDescStyle=TEXT_ALIGNLEFT;
    
    __root const CFG_HDR cfghdrfl_h5={CFG_COLOR,"Color description",0,0};
    __root const char listDescColor[4]={0xFF,0xFF,0xFF,0x64};
  
  __root const CFG_HDR cfghdr_f10={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_pi11={CFG_LEVEL,"Position icon",1,0};

    __root const CFG_HDR cfghdr1_8={CFG_CBOX, "Type", 0, 2};
    __root const int position_type = 0;
    __root const CFG_CBOX_ITEM cfgcboxpi0[2] = {"Center", "Offset"};
    
    __root const CFG_HDR cfghdr1_9 = {CFG_UINT, "OffsetX", 0, 100};
    __root const unsigned int OffsetX = 3;
  
    __root const CFG_HDR cfghdr1pi_10 = {CFG_UINT, "OffsetY", 0, 100};
    __root const unsigned int OffsetY = 3;
  
  __root const CFG_HDR cfghdr_pi10={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

  
  //header
  __root const CFG_HDR cfghdr_h11={CFG_LEVEL,"Header",1,0};

    __root const CFG_HDR cfghdr0_h1 = {CFG_CHECKBOX, "Show", 0, 2};
    __root const int headShow = 1;

    __root const CFG_HDR cfghdr0_h11 = {CFG_CHECKBOX, "Show icon", 0, 2};
    __root const int headIconShow = 1;

    __root const CFG_HDR cfghdr0_h2={CFG_RECT, "Position Header", 0, 0};
    __root const RECT headRect = {0, 24, 240, 24+24};
    
    __root const CFG_HDR cfghdr0_h3={CFG_UINT,"Font",0,65535};
    __root const unsigned int headFont=FONT_MEDIUM;

    __root const CFG_HDR cfghdr0_h4={CFG_UINT,"Style",0,65535};
    __root const unsigned int headStyle=TEXT_ALIGNLEFT;
    
    __root const CFG_HDR cfghdr0_h5={CFG_COLOR,"Color",0,0};
    __root const char headColor[4]={0xFF,0xFF,0xFF,0x64};

  __root const CFG_HDR cfghdr_h10={CFG_LEVEL,"",0,0};

  //description
  __root const CFG_HDR cfghdr_k11={CFG_LEVEL,"Description",1,0};

    __root const CFG_HDR cfghdr0_1 = {CFG_CHECKBOX, "Show", 0, 2};
    __root const int descShow = 1;

    __root const CFG_HDR cfghdr0_2={CFG_RECT, "Position description", 0, 0};
    __root const RECT descRect = {0, 320-32-16, 240, 320-32};
    
    __root const CFG_HDR cfghdr0_3={CFG_UINT,"Font",0,65535};
    __root const unsigned int descFont=FONT_SMALL;

    __root const CFG_HDR cfghdr0_4={CFG_UINT,"Style",0,65535};
    __root const unsigned int descStyle=TEXT_ALIGNMIDDLE;
    
    __root const CFG_HDR cfghdr0_5={CFG_COLOR,"Color",0,0};
    __root const char deskColor[4]={0xFF,0xFF,0xFF,0x64};

  __root const CFG_HDR cfghdr_k10={CFG_LEVEL,"",0,0};
  
    //description
  __root const CFG_HDR cfghdr_s11={CFG_LEVEL,"Scrolling",1,0};

    __root const CFG_HDR cfghdrs1_8={CFG_CBOX, "Show", 0, 3};
    __root const int scrollShow = 0;
    __root const CFG_CBOX_ITEM cfgcboxs0[3] = {"Auto", "Alltime", "No"};
    
    __root const CFG_HDR cfghdrs0_5={CFG_COLOR,"Color",0,0};
    __root const char scrollColor[4]={0xFF,0xFF,0xFF,0x64};

  __root const CFG_HDR cfghdr_s10={CFG_LEVEL,"",0,0};
  
__root const CFG_HDR cfghdr_si10={CFG_LEVEL,"",0,0};
  //==============================================================
__root const CFG_HDR cfghdr_l21={CFG_LEVEL,"Animation",1,0};
 
  __root const CFG_HDR cfghdr10_1 = {CFG_CHECKBOX, "Enable", 0, 2};
  __root const int Animation = 1;
    
  __root const CFG_HDR cfghdr10_2 = {CFG_UINT, "Speed 1/10s", 0, 600};
  __root const unsigned int SpeedAnim = 2;

__root const CFG_HDR cfghdr_l20={CFG_LEVEL,"",0,0};
  
  //==============================================================
__root const CFG_HDR cfghdr_l11={CFG_LEVEL,"Paths",1,0};

  __root const CFG_HDR cfghdr2_2={CFG_STR_UTF8,"Path menu cfg",0,63};
  __root const char MENU_PATH[64]=DEFAULT_DISK ":\\ZBin\\Menu\\Menu.cfg";
  
  __root const CFG_HDR cfghdr2_3={CFG_STR_UTF8,"Path cursor",0,63};
  __root const char CURSOR_PATH[64]=DEFAULT_DISK ":\\ZBin\\Menu\\cursor.png";

  __root const CFG_HDR cfghdr2_4={CFG_STR_UTF8,"Path img menu",0,63};
  __root const char IMGMENU_PATH[64]=DEFAULT_DISK ":\\ZBin\\Menu\\img\\";

__root const CFG_HDR cfghdr_l10={CFG_LEVEL,"",0,0};
  //==============================================================
__root const CFG_HDR cfghdr_j11={CFG_LEVEL,"Softkey",1,0};

  __root const CFG_HDR cfghdr3_1={CFG_STR_WIN1251,"Text right soft ",0,15};
  __root const char TextRight[16]="Назад";
  
  __root const CFG_HDR cfghdr3_2={CFG_STR_WIN1251,"Text left soft",0,15};
  __root const char TextLeft[16]="Мое меню";

  __root const CFG_HDR cfghdr3_3={CFG_STR_WIN1251,"Run left soft",0,15};
  __root const char RunLeft[16]="A06E835B"; //A06E835B MY_MENU

__root const CFG_HDR cfghdr_j10={CFG_LEVEL,"",0,0};
  //====================================================









