#ifndef _MAIN_H_
  #define _MAIN_H_


extern const unsigned  int Rows;
extern const unsigned  int Columns;
extern const int position_type;
extern const unsigned int OffsetX;
extern const unsigned int OffsetY;
extern const char MENU_PATH[64];
extern const char CURSOR_PATH[64];
extern const char IMGMENU_PATH[64];
extern const int cursorShow;

extern const int scrollShow;
extern const char scrollColor[4];

extern const RECT menuRect;
extern const int styleMenu;
extern const unsigned int Start;

extern const unsigned int listNameFont;
extern const unsigned int listNameStyle;
extern const char listNameColor[4];
extern const unsigned int listDescFont;
extern const unsigned int listDescStyle;
extern const char listDescColor[4];

extern const int descShow;
extern const RECT descRect;
extern const char deskColor[4];
extern const unsigned int descFont;
extern const unsigned int descStyle;


extern const int headShow;
extern const int headIconShow;
extern const RECT headRect;
extern const char headColor[4];
extern const unsigned int headFont;
extern const unsigned int headStyle;

extern const char TextRight[16];
extern const char TextLeft[16];
extern const char RunLeft[16];

extern const int ACTIVE_KEY;
extern const int Animation;
extern const unsigned int SpeedAnim;

extern const char CSMText[32];

// для чтения файла меню
typedef struct{
  char Text[32];
  char Description[128];
  char IconBig[128];
  char IconSmall[128];
  char Run[128];
} TMenuPoint;

#endif
