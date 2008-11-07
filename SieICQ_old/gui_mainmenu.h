#define _GUI_MAINMENU_H_
 #ifdef _GUI_MAINMENU_H_

// Запустить гуй меню
int RUN_GUI_MAINMENU();

enum LGP_ID_MAIN_MENU
{
  LGP_ToConnect=0,
  LGP_Exit,
  
  LGP_DATA_NUM
};
  

typedef struct 
{
  int max_num;
  char header_text;
  int type_header_text;
  char *name;
} MENU_STRUCT;

#endif
