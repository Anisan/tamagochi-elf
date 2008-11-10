#ifndef _ITEMS_H_
  #define _ITEMS_H_


enum TypeItem {GROUP,BUDDY};

typedef struct
{
  void *next;
  void *prev;
  unsigned int ID;
  unsigned int GroupID;
  unsigned int UIN;
  unsigned short Type;
  char Nick[64];
  unsigned short Status;
  char *StatusText;
  unsigned short XStatus;
  char* XStatusText;
  unsigned short isunread;
  unsigned short iswriting;
}ITEM;


//extern ITEM *Itemtop;

void AddItem(unsigned int ID,  unsigned int GroupID, unsigned int UIN, unsigned short Type,  char* Nick);
void DeleteItem(int curitem);

ITEM *GetItem(int curitem);
ITEM *GetItemByID(int ID);
ITEM *GetItemByUIN(int UIN);
ITEM *GetItemByUINstr(char *UIN);

//void LoadCL(char *pathfile);
//void SaveCL(char *pathfile);

int TotalItems();
void FreeItemsList();

#endif
