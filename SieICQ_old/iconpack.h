#ifndef _ICONPACK_H_
#define _ICONPACK_H_

enum ICON_ID
{
  IMG_OFFLINE,
  IMG_ONLINE,
  IMG_AWAY,
  IMG_DND,
  IMG_NA,
  IMG_OCCUPIED,
  IMG_FFC,
  IMG_INVISIBLE,
  IMG_EVIL,		
  IMG_DEPRESSION,	   
  IMG_HOME,		   
  IMG_WORK,	
  IMG_LUNCH,	
  
  TOTAL_ICONS_NUM
};



void LoadIcon();
void FreeIcon();
  
extern int IconPack[TOTAL_ICONS_NUM];

#endif