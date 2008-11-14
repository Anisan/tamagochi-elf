#ifndef _ITEMS_H_
  #define _ITEMS_H_


enum TypeItem {GROUP, // ������
               BUDDY  // �������
              };

typedef struct
{
  void *next;
  void *prev;
  unsigned int ID;            // ID �������� , ��� ������ = 0
  unsigned int GroupID;       // ID ������ , ��� �������� - � ����� ������
  unsigned int UIN;
  unsigned short Type;
  char Nick[64];
  unsigned short Status;
  char *StatusText;
  unsigned short XStatus;
  char* XStatusText;
  unsigned short isunread;  // ���� �������������
  unsigned short istyping; // �������� ���������  //0x0000 - typing finished
                                                  //0x0001 - text typed
                                                  //0x0002 - typing begun
  unsigned short iscollapsed;
 
}ITEM;


extern ITEM *Itemtop;

void AddItem(unsigned int ID,  unsigned int GroupID, unsigned int UIN, unsigned short Type,  char* Nick);
void DeleteItem(int curitem);

ITEM *GetItem(int curitem);
ITEM *GetItemByID(int ID); // ���������� ������� �� ID
ITEM *GetGroupByGroupID(int GroupID); // ������ �� GroupID
ITEM *GetItemByUIN(int UIN); // ������� �� ��� UIN(�����)
ITEM *GetItemByUINstr(char *UIN); // ������� �� ��� Uin ������

int GetKontactInGroup(int GroupID);

void LoadCL(char *pathfile);
void SaveCL(char *pathfile);

int TotalItems();
void FreeItemsList();

#endif
