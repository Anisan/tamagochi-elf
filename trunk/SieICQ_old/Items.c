#include "include.h"
#include "Items.h"


ITEM *Itemtop;

void AddItem(unsigned int ID,  unsigned int GroupID, unsigned int UIN, unsigned short Type,  char* Nick)
{

 ITEM *bmk=malloc(sizeof(ITEM));
 bmk->next=NULL;
 bmk->prev=NULL;
 bmk->ID=ID;
 bmk->GroupID=GroupID;
 bmk->UIN=UIN;
 bmk->Type=Type;
 sprintf(bmk->Nick,Nick);
 
 bmk->StatusText=NULL;
 bmk->XStatusText=NULL;
             
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
   bmk->prev=Item;
 } 
}

void FreeItemsList()
{  
  LockSched();
  ITEM *bmk=(ITEM *)Itemtop;
  Itemtop=NULL;
  UnlockSched();
  while(bmk)
  {
    ITEM *bmk_prev=bmk;
    bmk=bmk->next;
     if (bmk_prev->StatusText)
       mfree(bmk_prev->StatusText);
     if (bmk_prev->XStatusText)
       mfree(bmk_prev->XStatusText);
   
    mfree(bmk_prev);
  }
}

void DeleteItem(int curitem)
{
  ITEM *bmk=GetItem(curitem);
  if(bmk!=0)
  {
      ITEM *bmk_prev=bmk->prev;
      ITEM *bmk_next=bmk->next;
      bmk_prev->next=bmk_next;
      bmk_next->prev=bmk_prev;
      
     if (bmk->StatusText)
       mfree(bmk->StatusText);
     if (bmk->XStatusText)
       mfree(bmk->XStatusText);
      mfree(bmk);
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

ITEM *GetItemByID(int ID)
{
  ITEM *bmk;
  bmk=Itemtop;
  while(bmk)
  {
    if(ID==bmk->ID)
      return bmk;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}

ITEM *GetGroupByID(int GroupID)
{
  ITEM *bmk;
  bmk=Itemtop;
  while(bmk)
  {
    if((GroupID==bmk->GroupID)&&(bmk->ID==0))
      return bmk;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}

ITEM *GetItemByUIN(int UIN)
{
  ITEM *bmk;
  bmk=Itemtop;
  while(bmk)
  {
    if(UIN==bmk->UIN)
      return bmk;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}

ITEM *GetItemByUINstr(char* UIN)
{
  int _UIN=str2int(UIN);
  ITEM *bmk;
  bmk=Itemtop;
  while(bmk)
  {
    if(_UIN==bmk->UIN)
      return bmk;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}


int GetKontactInGroup(int GroupID)
{
  if(!Itemtop) return 0;
  ITEM *bmk;
  bmk=Itemtop;
  int i=-1; // c учетом самой группы
  while(bmk) 
  {
    if (bmk->GroupID=GroupID) i++;
    bmk=bmk->next;  
  }
  return i;
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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

  typedef struct
  {
    unsigned int ID;
    unsigned int GroupID;
    unsigned int UIN;
    unsigned short Type;
    char Nick[64];
  }FILE_ITEM;
  
void LoadCL(char *pathfile)
{
  FSTATS stat;
  int f;
  unsigned int ul;
  int i;
  int fsize;
  int c;
  FreeItemsList();

  if (GetFileStats(pathfile,&stat,&ul)==-1) return;
  if ((fsize=stat.size)<=0) return;
  if ((f=fopen(pathfile,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return;

  c=fsize/sizeof(FILE_ITEM);

  LockSched();
  FILE_ITEM *ri=malloc(sizeof(FILE_ITEM));
  for (i=0;i<c;i++)
  {
   fread(f,ri,sizeof(FILE_ITEM),&ul);
   AddItem(ri->ID,ri->GroupID,ri->UIN,ri->Type,ri->Nick);
  }
  fclose(f,&ul);
  UnlockSched();
  mfree(ri);
}

void SaveCL(char *pathfile)
{
      int f;
      unsigned int err;
      if ((f=fopen(pathfile,A_WriteOnly|A_BIN|A_Truncate|A_Create,P_WRITE,&err))!=-1)
      {
        for (int i=0; i<TotalItems(); i++)
        {
          ITEM *bmk=GetItem(i);
          FILE_ITEM *si = malloc(sizeof(FILE_ITEM));
            zeromem(si->Nick,64);
            si->ID=bmk->ID;
            si->GroupID=bmk->GroupID;
            si->UIN=bmk->UIN;
            si->Type=bmk->Type;
            sprintf(si->Nick,bmk->Nick);
          fwrite(f,si,sizeof(FILE_ITEM),&err);
          mfree(si);
        }
        fclose(f,&err);
      }
}
//////////////////////////////////////////////////////
