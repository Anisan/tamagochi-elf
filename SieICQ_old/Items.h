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
  unsigned short istyping; // набирает сообщение  //0x0000 - typing finished
                                                  //0x0001 - text typed
                                                  //0x0002 - typing begun
  unsigned short iscollapsed;
 
}ITEM;


extern ITEM *Itemtop;

void AddItem(unsigned int ID,  unsigned int GroupID, unsigned int UIN, unsigned short Type,  char* Nick);
void DeleteItem(int curitem);

ITEM *GetItem(int curitem);
ITEM *GetItemByID(int ID); // возвращает контакт по ID
ITEM *GetGroupByGroupID(int GroupID); // группу по GroupID
ITEM *GetItemByUIN(int UIN); // контакт по его UIN(число)
ITEM *GetItemByUINstr(char *UIN); // контакт по его Uin строке

int GetKontactInGroup(int GroupID);

void LoadCL(char *pathfile);
void SaveCL(char *pathfile);

int TotalItems();
void FreeItemsList();

#endif
