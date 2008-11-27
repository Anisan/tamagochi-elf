#include "include.h"
#include "capabilities.h"

int capcmp(const char *s, const char *d, int len)
{
  for (int i=0; i<len; i++)
    if (s[i]!=d[i])
      return 0;
  return 1;
}


int FindCapabilities(char * buf, int len, const char * cap)
{
  int count= len/0x10;
  for (int i=0; i<count; i++)
  {
    int f=capcmp(&buf[i*0x10] ,cap,0x10);
    if (f) return 1;
  }
  return 0;
}

int FindXStatus(char * buf, int len)
{
  int count= len/0x10;
  for (int i=0; i<count; i++)
  {
    for (int x=0; x<XSTATUS_COUNT; x++)
    {  
     int f=capcmp(&buf[i*0x10] ,&capXStatus[x*0x10],0x10);
     if (f) return x+1;
    }
  }
  return 0;
}

int ClientID(char * buf, int len)
{
  int count= len/0x10;
  for (int i=0; i<count; i++)
  {
    for (int x = 0; x < sizeof(clientDB)/(sizeof(STDCAPINFO)); ++x)
    {
       int f=capcmp(&buf[i*0x10] ,clientDB[x].caps,clientDB[x].capSize);
       if (f) return x+1;
      
    }
  }
  return 0;
}
