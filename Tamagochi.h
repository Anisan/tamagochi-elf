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
  int Age;          //возраст
  int TimeAge;      //минут после увеличения возраста
  int Health;       //здоровье
  int MaxHealth;    //максимальное здоровье
  int Hunger;       //голод
  int MaxHunger;    //максимальный голод
  int Happiness;    //счастье
  int MaxHappiness; //максимальное счастье
  int Dirtiness;    //грязь
  int MaxDirtiness; //максимальная грязь
  int Boredom;      //скука
  int MaxBoredom;   //максимальная скука
  int Behaviour;    //мораль
  int MaxBehaviour; //максимальная мораль
  int StatusDeath;  //статус смерти
  int ImageStatus;  //индекс статус-картинки на главный экран
} TStatusPet;

#endif
