#include "include.h"
#include "iconpack.h"
  
static const char * const img_names[TOTAL_ICONS_NUM]=
{
  // Status ; Start = 0
  
  "online.png",
  "ffc.png",
  "away.png",
  "dnd.png",
  "occupied.png",
  "na.png",
  "invisible.png",
  "EVIL.png",
  "DEPRESSION.png",	   
  "HOME.png",   
  "WORK.png",
  "LUNCH.png",
  "offline.png",
  "groupon.png",
  "groupoff.png",
    
  // MainMenu ; Start = 13
  "toconnect.png",
  "changes.png",
  "changesX.png",
  "clist.png",
  "settings.png",
  "about.png",
  "exit.png"
};

int IconPack[TOTAL_ICONS_NUM + XIMGCount];

void LoadIcon()
{
  FreeIcon();
  for(int i=0; i<TOTAL_ICONS_NUM; i++)
  {
    IconPack[i] = (int)MakeGlobalString(ICON_PATH,'\\',img_names[i]);
  }
  for(int i=0; i<XIMGCount; i++)
  {
    char img[10];
    sprintf(img,"%d.png",i);
    IconPack[TOTAL_ICONS_NUM + i] = (int)MakeGlobalString(ICON_PATH,'\\',img);
  }
}

void FreeIcon()
{
  for(int i=0; i < TOTAL_ICONS_NUM + XIMGCount; i++)
  {
    if(IconPack[i])
      mfree ((void *)IconPack[i]);
    IconPack[i] = 0;
  }
}


