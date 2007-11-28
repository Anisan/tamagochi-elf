#ifndef _TAMAGOCHI_H_
  #define _TAMAGOCHI_H_

#define __VERSION__ "0.2"

extern const unsigned int REFRESH;
extern const int cfgShowIn;

extern const unsigned int POS_X;
extern const unsigned int POS_Y;

extern const char PET_PATH[64];
extern const char PIC_PATH[64];
extern const char ROOM_PATH[64];
extern const char SOUND_PATH[64];



static const char * const SoundName[2]=
{
  "global.wav",
  "hello.wav"

};


static const char * const icons_names[9]=
{
  "smile.png",
  "death.png",
  "dream.png",
  "hunger.png",
  "Health.png",
  "Happiness.png",
  "Dirtiness.png",
  "Boredom.png",
  "Behaviour.png"
};

static const char * const icons_menu[14]=
{
  "game.png",
  "vaccinate.png",
  "discipline.png",
  "sleep.png",
  "status.png",
  "settings.png",
  "about.png",
  "sm_smile.png",
  "heart.png",
  "sm_hunger.png",
  "sm_Happiness.png",
  "sm_Dirtiness.png",
  "sm_Boredom.png",
  "sm_Behaviour.png"
    
};


typedef struct{
  int Age;          //�������
  int TimeAge;      //����� ����� ���������� ��������
  int Health;       //��������
  int MaxHealth;    //������������ ��������
  int Hunger;       //�����
  int MaxHunger;    //������������ �����
  int Happiness;    //�������
  int MaxHappiness; //������������ �������
  int Dirtiness;    //�����
  int MaxDirtiness; //������������ �����
  int Boredom;      //�����
  int MaxBoredom;   //������������ �����
  int Behaviour;    //������
  int MaxBehaviour; //������������ ������
  int StatusDeath;  //������ ������
  int ImageStatus;  //������ ������-�������� �� ������� �����
} TStatusPet;

#endif
