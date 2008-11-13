#ifndef _ITEMS_H_
  #define _ITEMS_H_


enum TypeItem {GROUP, // группа
               BUDDY  // контакт
              };

typedef struct
{
  void *next;
  void *prev;
  unsigned int ID;            // ID контакта , для группы = 0
  unsigned int GroupID;       // ID группы , для контакта - в какой группе
  unsigned int UIN;
  unsigned short Type;
  char Nick[64];
  unsigned short Status;
  char *StatusText;
  unsigned short XStatus;
  char* XStatusText;
  unsigned short isunread;  // есть непрочитанные
  unsigned short iswriting; // набирает сообщение
 
}ITEM;


extern ITEM *Itemtop;

void AddItem(unsigned int ID,  unsigned int GroupID, unsigned int UIN, unsigned short Type,  char* Nick);
void DeleteItem(int curitem);

ITEM *GetItem(int curitem);
ITEM *GetItemByID(int ID);
ITEM *GetGroupByGroupID(int GroupID);
ITEM *GetItemByUIN(int UIN);
ITEM *GetItemByUINstr(char *UIN);

void LoadCL(char *pathfile);
void SaveCL(char *pathfile);

int TotalItems();
void FreeItemsList();

#endif
