#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

//Оставляем только один дефайн иначе будет кака
//#define LANG_RU
#define LANG_EN

#ifdef LANG_EN

#define LG_GFGHELLO "Hello message"
#define LG_GFGYES "Yes"
#define LG_GFGNO "No"

#define LG_GFGSETSCREEN "Screen settings"
#define LG_GFGUPDATE "Refresh period, 1/10 sec"
#define LG_GFGVIEW "Show In"
#define LG_GFGLOCK "Lock"
#define LG_GFGUNLOCK "Unlock"
#define LG_GFGBOTH "Both"
#define LG_GFGPOSITION "Position"
#define LG_GFGMINSIZE "Min Size (%)"
#define LG_GFGMAXSIZE "Max Size (%)"
#define LG_GFGOPACITY "Opacity (%)"

#define LG_GFGSETPATH "Paths settings"
#define LG_GFGPATHPET "Path to Pet.cfg"
#define LG_GFGPATHROOM "Path to room"
#define LG_GFGPATHIMG "Path to images"
#define LG_GFGPATHSOUND "Path to sounds"

#define LG_GFGSETPET "Pet settings"
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

#define LG_COOL "Cool game! :)"
#define LG_IAMSLEEP "I'm sleeping! Zzz.."
#define LG_PAIN "Au! Painful! :'("
#define LG_SHREW "You are shrew! :/"

#define LG_MSGSLEEP "I am sleep!"
#define LG_MSGNOTSLEEP "I am wakeup!"

#define LG_ALREADY_STARTED "I'm already here!"

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
    
#define LG_IAMDIE "I'm dead!  +/-("
#define LG_MSGAGE "Birthday moment! ^^"
#define LG_MSGHEALTH  "I'm sick and tired... =("
#define LG_MSGHUNGER  "I'm hunger! Give me new food!"
#define LG_MSGHAPPINESS  "Life is sad.. Nothing helps.."
#define LG_MSGDIRTINESS  "Too much nasty scraps in my room! :/"
#define LG_MSGBOREDOM  "I'm really BORED!"

//-------------
#define LG_SELECT  "Select"
#define LG_CLOSE   "Close"
#define LG_BACK    "Back"
//--------------

//================================

#define LG_LOADING "Tamagochi loading!"
#define LG_UPDSETTING "Tamagochi settings updated!"

//++++++++++++++++++++++++++++++++++++++++

static const int mess_mask=37-1;
static const char * const SpeakMessage[37]=
{
  "Filesystem error!",
  "I know bad words!",
  "Filesystem error!",
  "Bitch! :)",
  "Filesystem error!",
  "Gay! :)",
  "Filesystem error!",
  "Moron! :)",
  "Filesystem error!",
  "I want sex! :)",
  "Filesystem error!",
  "I like big tits! :)",
  "Filesystem error!",
  "Did you masturbate today? :)",
  "Filesystem error!",
  "I've deleted some files from zbin/etc.. :)",
  "Filesystem error!",
  "May i format mmcard now?",
  "Filesystem error!",
  "BlueTooth service stopped!",
  "Filesystem error!",
  "Network is lost!",
  "Filesystem error!",
  "Sim card is blocked!",
  "Filesystem error!",
  "PIN is unavailable now!",
  "Filesystem error!",
  "Master reset starts!",
  "Filesystem error!",
  "Give me your mama! :)",
  "Filesystem error!",
  "Press on me! Lower.. Lower...",
  "Filesystem error!",
  "Teach me how to steal money!",
  "Filesystem error!",
  "New system time accepted!",
  "Filesystem error!"
};


#endif

#endif


