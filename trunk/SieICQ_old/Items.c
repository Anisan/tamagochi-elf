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

int TotalItems()
{
  if(!Itemtop) return 0;
  ITEM *bmk;
  bmk=Itemtop;
  int i=1; 
  while(bmk=bmk->next) i++;
  return i;
}


