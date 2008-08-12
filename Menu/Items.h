

typedef struct
{
  void *next;
  char Icon[128];
}LIST;

typedef struct
{
  void *next;
  char Text[128];
  char Description[128];
  char IconSmall[128];
  char IconBig[128];
  LIST *IconAnim;
  char Run[128];
}ITEM;


