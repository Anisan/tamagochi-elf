#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

//��������� ������ ���� ������ ����� ����� ����
//#define LANG_RU
#define LANG_EN

#ifdef LANG_RU

//===============================

#define LG_GFGHELLO "�����������"
#define LG_GFGYES "��"
#define LG_GFGNO "���"

#define LG_GFGSETSCREEN "��������� ������"
#define LG_GFGUPDATE "����������, 1/10 ���"
#define LG_GFGVIEW "���������� ��"
#define LG_GFGLOCK "����������"
#define LG_GFGUNLOCK "�������������"
#define LG_GFGBOTH "�� �����"
#define LG_GFGPOSITION "���������� �� ������"
#define LG_GFGMINSIZE "Min Size (%)"
#define LG_GFGMAXSIZE "Max Size (%)"
#define LG_GFGOPACITY "Opacity (%)"

#define LG_GFGSETPATH "��������� �����"
#define LG_GFGPATHPET "���� � �������"
#define LG_GFGPATHROOM "���� � �������"
#define LG_GFGPATHIMG "���� � ���������"
#define LG_GFGPATHSOUND "���� � ������"

#define LG_GFGSETPET "��������� ��������"
#define LG_GFGSPEED "��������"
#define LG_GFGFAST "������"
#define LG_GFGMEDIUM "������"
#define LG_GFGSLOW "��������"

#define LG_GFGSETALARM "����������"
#define LG_GFGVOLUME "��������� ������"

#define LG_GFGSETKEY "����� ����"
#define LG_GFGMNUENA "��� ������"
#define LG_GFGKEY "�������"
//===============================
#define LG_COOL "�����!"
#define LG_IAMSLEEP "�� ����� �����!"
#define LG_PAIN "������ ��!"
#define LG_SHREW "�����!"
#define LG_IAMDIE "� ��� ����!������� ��������!"

#define LG_MSGSLEEP "������ �����!"
#define LG_MSGNOTSLEEP "� ���� ���������!"

#define LG_ALREADY_STARTED "��� ��������!"

#define LG_MENU    "����"
#define LG_GAMEPLAY "����"
#define LG_VACCINATE  "�������"
#define LG_DISCIPLINE  "����������"
#define LG_SLEEP  "�����"
#define LG_NOTSLEEP  "��������"
#define LG_STATUS  "������"
#define LG_EDCFG   "���������"
#define LG_ABOUT   "�� �����"

#define LG_AGE "�������"
#define LG_HEALTH  "��������"
#define LG_HUNGER  "�����"
#define LG_HAPPINESS  "�������"
#define LG_DIRTINESS  "�������"
#define LG_BOREDOM  "�����"
#define LG_BEHAVIOUR  "������"
    
#define LG_MSGAGE "���! �����!"
#define LG_MSGHEALTH  "���� �����! ������ �����!"
#define LG_MSGHUNGER  "����� �����! � �� ����� �������!"
#define LG_MSGHAPPINESS  "����� �����!"
#define LG_MSGDIRTINESS  "���� ��� ������!"
#define LG_MSGBOREDOM  "����� ��������!"

//-------------
#define LG_SELECT  "�����"
#define LG_CLOSE   "�������"
#define LG_BACK    "�����"
//--------------

//================================

#define LG_LOADING "Tamagochi ��������!"
#define LG_UPDSETTING "��������� Tamagochi ���������!"

//++++++++++++++++++++++++++++++++++++++++

static const int mess_mask=17-1;
static const char * const SpeakMessage[17]=
{
  "������ ����, ���� ����!",
  "� �������� ���������!",
  "������ ������!",
  "����� ����!",
  "���������� �����, �� ������ ���������!",
  "����!! �������� ����!!!",
  "��-��-��!",
  "����� ��� ��������!!!",
  "��������������!",
  "������ ������� ���� ����!",
  "�� ��������!",
  "����������?!",
  "���� ���� �������!",
  "��� ���������!",
  "��������!",
  "�������?!",
  "����!"
};
#endif



#ifdef LANG_EN

#define LG_GFGHELLO "Hello message"
#define LG_GFGYES "Yes"
#define LG_GFGNO "No"

#define LG_GFGSETSCREEN "Setting screen"
#define LG_GFGUPDATE "Refresh period, 1/10 sec"
#define LG_GFGVIEW "Show In"
#define LG_GFGLOCK "Lock"
#define LG_GFGUNLOCK "Unlock"
#define LG_GFGBOTH "Both"
#define LG_GFGPOSITION "Position"
#define LG_GFGMINSIZE "Min Size (%)"
#define LG_GFGMAXSIZE "Max Size (%)"
#define LG_GFGOPACITY "Opacity (%)"

#define LG_GFGSETPATH "Setting path"
#define LG_GFGPATHPET "Path pet"
#define LG_GFGPATHROOM "Path room"
#define LG_GFGPATHIMG "Path image"
#define LG_GFGPATHSOUND "Peth sound"

#define LG_GFGSETPET "Setting Pet"
#define LG_GFGSPEED "Speed"
#define LG_GFGFAST "Fast"
#define LG_GFGMEDIUM "Medium"
#define LG_GFGSLOW "Slow"

#define LG_GFGSETALARM "Notify"
#define LG_GFGVOLUME "Volume sound"

#define LG_GFGSETKEY "Activation menu"
#define LG_GFGMNUENA "Activation style"
#define LG_GFGKEY "Activation key"
//===============================

#define LG_COOL "Cool!"
#define LG_IAMSLEEP "I am sleep!"
#define LG_PAIN "PAIN!"
#define LG_SHREW "SHREW!"

#define LG_MSGSLEEP "I am sleep!"
#define LG_MSGNOTSLEEP "I am wakeup!"

#define LG_ALREADY_STARTED "Already started!"

#define LG_MENU    "Menu"
#define LG_GAMEPLAY "Game"
#define LG_VACCINATE  "Vaccinate"
#define LG_DISCIPLINE  "Discipline"
#define LG_SLEEP  "Sleep"
#define LG_NOTSLEEP  "Wakeup"
#define LG_STATUS  "Status"
#define LG_EDCFG   "Setting"
#define LG_ABOUT   "About"

#define LG_AGE "Age"
#define LG_HEALTH  "Health"
#define LG_HUNGER  "Hunger"
#define LG_HAPPINESS  "Happiness"
#define LG_DIRTINESS  "Cleanness"
#define LG_BOREDOM  "Boredom"
#define LG_BEHAVIOUR  "Behavior"
    
#define LG_IAMDIE "Death!"
#define LG_MSGAGE "Birthday!"
#define LG_MSGHEALTH  "HEALTH!"
#define LG_MSGHUNGER  "HUNGER!"
#define LG_MSGHAPPINESS  "Low HAPPINESS!"
#define LG_MSGDIRTINESS  "DIRTINESS!"
#define LG_MSGBOREDOM  "BOREDOM!"

//-------------
#define LG_SELECT  "Select"
#define LG_CLOSE   "Close"
#define LG_BACK    "Back"
//--------------

//================================

#define LG_LOADING "Tamagochi loading!"
#define LG_UPDSETTING "Setting Tamagochi updated!"



//++++++++++++++++++++++++++++++++++++++++

static const int mess_mask=2-1;
static const char * const SpeakMessage[2]=
{
  "Fuck you!",
  "Bitch!"
};


#endif

#endif


