#ifndef _MAIN_H_
  #define _MAIN_H_


extern const unsigned  int Rows;
extern const unsigned  int Columns;
extern const unsigned int OffsetX;
extern const unsigned int OffsetY;
extern const char MENU_PATH[64];
extern const char CURSOR_PATH[64];
extern const char IMGMENU_PATH[64];

extern const RECT menuRect;
extern const int descShow;
extern const RECT descRect;
extern const char deskColor[4];
extern const unsigned int Start;

extern const char TextRight[16];
extern const char TextLeft[16];
extern const char RunLeft[16];

extern const int ACTIVE_KEY;
extern const int Animation;
extern const unsigned int SpeedAnim;

extern const char CSMText[32];




typedef struct{
  char Text[32];
  char Description[128];
  char IconBig[128];
  char IconSmall[128];
  char Run[128];
} TMenuPoint;

#endif
