#include "include.h"
#include "iconpack.h"
  
static const char * const img_names[TOTAL_ICONS_NUM]=
{
  "offline.png",
  "online.png",
  "away.png",
  "dnd.png",
  "na.png",
  "occupied.png",
  "ffc.png",
  "invisible.png",
  "EVIL.png",
  "DEPRESSION.png",	   
  "HOME.png",   
  "WORK.png",
  "LUNCH.png"
};

int IconPack[TOTAL_ICONS_NUM];

void LoadIcon()
{
  FreeIcon();
  for(int i=0; i<TOTAL_ICONS_NUM; i++)
  {
    IconPack[i] = (int)MakeGlobalString(ICON_PATH,'\\',img_names[i]);
  }
}

void FreeIcon()
{
  for(int i=0; i < TOTAL_ICONS_NUM; i++)
  {
    if(IconPack[i])
      mfree ((void *)IconPack[i]);
    IconPack[i] = 0;
  }
}


