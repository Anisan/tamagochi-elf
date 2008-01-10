#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

//Оставляем только один дефайн иначе будет кака
//#define LANG_RU
#define LANG_EN

<<<<<<< .mine
#ifdef LANG_RU

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
#define LG_GFGOPACITY "Прозрачность (%)"

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
#define LG_GFGSNDENABLE "Включить звуки"
#define LG_GFGVOLUME "Громкость звуков"
#define LG_GFGVBRENABLE "Включить вибру"
#define LG_GFGVBRSETUP "Настройка вибры"
#define LG_GFGVBRPOWER "Мощность"
#define LG_GFGVBRTYPE "Тип вибрации"
#define LG_GFGVBRTYPE1 "Одиночный"
#define LG_GFGVBRTYPE2 "Двойной"

#define LG_GFGBLINKENABLE "Включить моргалку"
#define LG_GFGBLINKSETUP "Настройка моргалки"
#define LG_GFGBLINKLOCKED "Только когда залочен"
#define LG_GFGBLINKPERIOD "Blink period, 1/10 sec"
#define LG_GFGBLINKCOUNT "Blink сount"
#define LG_GFGBLINKBRIGHT "Brightness"
#define LG_GFGBLINKKBRD "Blink keyboard"
#define LG_GFGBLINKDISPL "Blink display"
#define LG_GFGBLINKDYN "Blink dynlights"
#define LG_GFGBLINKLIGHT "Blink lighter"

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
// сообщений может быть 2,4,8,16,32,64,...
static const int mess_mask=15;
static const char * const SpeakMessage[16]=
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
  "Шозонах?!",
  "Яибу!"
};

#endif


=======
>>>>>>> .r21
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
<<<<<<< .mine
// count message = 2,4,8,16,32,64,...
static const int mess_mask=32-1;
static const char * const SpeakMessage[37]=
=======

static const int mess_mask=37-1;
static const char * const SpeakMessage[37]=
>>>>>>> .r21
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


