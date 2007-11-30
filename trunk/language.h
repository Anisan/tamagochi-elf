#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

//Оставляем только один дефайн иначе будет кака
//#define LANG_RU
#define LANG_EN

#ifdef LANG_RU

//===============================

#define LG_GFGHELLO "Приветствие"
#define LG_GFGYES "Да"
#define LG_GFGNO "Нет"

#define LG_GFGSETSCREEN "Настройки смайла"
#define LG_GFGUPDATE "Обновление, 1/10 сек"
#define LG_GFGVIEW "Показовать на"
#define LG_GFGLOCK "Блокирован"
#define LG_GFGUNLOCK "Разблокирован"
#define LG_GFGBOTH "На обоих"
#define LG_GFGPOSITION "Координаты на экране"
#define LG_GFGMINSIZE "Min Size (%)"
#define LG_GFGMAXSIZE "Max Size (%)"
#define LG_GFGOPACITY "Opacity (%)"

#define LG_GFGSETPATH "Настройка путей"
#define LG_GFGPATHPET "Путь к питомцу"
#define LG_GFGPATHROOM "Путь к комнате"
#define LG_GFGPATHIMG "Путь к картинкам"
#define LG_GFGPATHSOUND "Путь к звукам"

#define LG_GFGSETPET "Настройки развития"
#define LG_GFGSPEED "Скорость"
#define LG_GFGFAST "Быстро"
#define LG_GFGMEDIUM "Средне"
#define LG_GFGSLOW "Медленно"

#define LG_GFGSETALARM "Оповещение"
#define LG_GFGVOLUME "Громкость звуков"

#define LG_GFGSETKEY "Вызов меню"
#define LG_GFGMNUENA "Тип вызова"
#define LG_GFGKEY "Клавиша"
//===============================
#define LG_COOL "Клева!"
#define LG_IAMSLEEP "Не мешай спать!"
#define LG_PAIN "Больна же!"
#define LG_SHREW "Злюка!"
#define LG_IAMDIE "Я уже умер!Поздняк метаться!"

#define LG_MSGSLEEP "Нехочу спать!"
#define LG_MSGNOTSLEEP "У блин разбудили!"

#define LG_ALREADY_STARTED "Уже запущена!"

#define LG_MENU    "Меню"
#define LG_GAMEPLAY "Игра"
#define LG_VACCINATE  "Лечение"
#define LG_DISCIPLINE  "Дисциплина"
#define LG_SLEEP  "Спать"
#define LG_NOTSLEEP  "Проснись"
#define LG_STATUS  "Статус"
#define LG_EDCFG   "Настройки"
#define LG_ABOUT   "Об эльфе"

#define LG_AGE "Возраст"
#define LG_HEALTH  "Здоровье"
#define LG_HUNGER  "Голод"
#define LG_HAPPINESS  "Счастье"
#define LG_DIRTINESS  "Чистота"
#define LG_BOREDOM  "Скука"
#define LG_BEHAVIOUR  "Мораль"
    
#define LG_MSGAGE "Ура! Днюха!"
#define LG_MSGHEALTH  "Мало жизни! Сдохну скоро!"
#define LG_MSGHUNGER  "Жрать давай! А то скоро загнусь!"
#define LG_MSGHAPPINESS  "Жизнь гавно!"
#define LG_MSGDIRTINESS  "Живу как свинья!"
#define LG_MSGBOREDOM  "Скука смертная!"

//-------------
#define LG_SELECT  "Выбор"
#define LG_CLOSE   "Закрыть"
#define LG_BACK    "Назад"
//--------------

//================================

#define LG_LOADING "Tamagochi загружен!"
#define LG_UPDSETTING "Настройки Tamagochi применены!"

//++++++++++++++++++++++++++++++++++++++++

static const int mess_mask=17-1;
static const char * const SpeakMessage[17]=
{
  "Аффтар жжет, пеши исчо!",
  "Ф бабруйск животнайе!",
  "Аццкий сотона!",
  "Выпей йаду!",
  "Отъебитесь бляди, йа выучил албанский!",
  "Блиа!! Ниибаццо жжош!!!",
  "Бу-га-га!",
  "Жжошь как агнимьот!!!",
  "Напирисдачунах!",
  "Ктулху зохавал твой моск!",
  "ЙА КРИВЕТКО!",
  "Нубляващще?!",
  "Убей сибя апстену!",
  "Учи албанский!",
  "Фублянах!",
  "Шозонах?!",
  "Яибу!"
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


