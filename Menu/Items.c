#include "..\inc\swilib.h"
#include "Items.h"
#include "main.h"

ITEM *Itemtop;

extern void _WriteLog(char *buf);

void AddToItem(char *name,char *description,char *iconsmall,char *iconbig, char *run)
{

ITEM *bmk=malloc(sizeof(ITEM));
 bmk->next=0;
 sprintf(bmk->Text,name);
 sprintf(bmk->Description,description);
 sprintf(bmk->Run,run);
 
 const char *s=iconsmall;
 char icon[128];
 
    if ((s[2]!='\\')&&((s[(strlen(s))-3]=='.')||//Проверяем строку на наличие символов '\\' и '.'
     (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
     {
      snprintf(icon, 127, "%s%s", IMGMENU_PATH,iconsmall);
      sprintf(bmk->IconSmall,icon);
     }

 int find=0;
 char tmp[128];

 sprintf(tmp,iconbig);
    
    int cs=0;
      for (int j=0;j<strlen(tmp);j++)
      {
        if ((tmp[j]!=';')&&(j!=strlen(tmp)))
            icon[cs++]=tmp[j];
          else 
          {
            icon[cs++]=0;
            char tmp2[128];
            sprintf(tmp2,"%s",icon);
            s=tmp2;
            if ((s[2]!='\\')&&((s[(strlen(s))-3]=='.')||//Проверяем строку на наличие символов '\\' и '.'
             (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
                snprintf(icon, 127, "%s%s", IMGMENU_PATH,icon);
            _WriteLog(icon);
            
            if (find==0) 
              sprintf(bmk->IconBig,"%s",icon); 
             else
             {
               LIST *Anim=malloc(sizeof(LIST));
               Anim->next=0;
               sprintf(Anim->Icon,"%s",icon); 
           
                if(!bmk->IconAnim)
                  bmk->IconAnim=Anim; 
                 else
                  {
                    LIST *List=bmk->IconAnim;
                    while(List->next)
                       List=List->next;
                    List->next=Anim;   
                 }
             }
              
            find=j;
            cs=0;
          }
      }
             
 if(!Itemtop)
 {
   Itemtop=bmk; 
 }
 else
 {
   ITEM *Item=Itemtop;
   while(Item->next)
       Item=Item->next;
   Item->next=bmk;   
 } 
}

void FreeItemsList()
{  
  LockSched();
  ITEM *bmk=(ITEM *)Itemtop;
  Itemtop=0;
  UnlockSched();
  while(bmk)
  {
    if (bmk->IconAnim)
    {
      LIST *Anim=(LIST *)bmk->IconAnim;
      while (Anim)
      {
        LIST *Anim_prev=Anim;
        Anim=Anim->next;
        mfree(Anim_prev);
      }
    }
    
    ITEM *bmk_prev=bmk;
    bmk=bmk->next;
    mfree(bmk_prev);
  }
   
}

ITEM *GetItem(int curitem)
{

  ITEM *bmk;
  bmk=Itemtop;
  int i=0;
  while(bmk)
  {
    if(i==curitem)
      return bmk;
    i++;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
  
}

char *IconAnim(int curitem, int pic)
{
  ITEM *bmk;
  bmk=Itemtop;
  int i=0;
  while(bmk)
  {
    if(i==curitem)
    {
      if(!bmk->IconAnim) return 0;
      LIST *Anim=bmk->IconAnim;
      int j=0;
      while(Anim=Anim->next) 
      {
        if (j==pic)
          return Anim->Icon; 
        
        j++;
      }
    }
    i++;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}

int TotalItems()
{
  if(!Itemtop) return 0;
  ITEM *bmk;
  bmk=Itemtop;
  int i=1; 
  while(bmk=bmk->next) i++;
  return i;
}

int TotalAnim(int curitem)
{
  ITEM *bmk;
  bmk=Itemtop;
  int i=0;
  while(bmk)
  {
    if(i==curitem)
    {
      if(!bmk->IconAnim) return 0;
      LIST *Anim=bmk->IconAnim;
      int j=0;
      while(Anim=Anim->next) j++;
      return j;
    }
    i++;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}
