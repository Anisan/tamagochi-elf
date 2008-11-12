#include "include.h"

#include "gui_mainmenu.h"

// Developer - Vedan, 2008

void DrawHeaderText(HEADER_ITEM *data)
{
  WSHDR *ws_head_text = AllocWS(128);

  wsprintf(ws_head_text, percent_t, data->text);
  DrawString(ws_head_text, 0, data->y_disp, ScrW , 2*data->y_disp + Font_H, FONT,32,COLOUR, COLOUR_FRING);
  FreeWS(ws_head_text);
};

int Get_HeaderText_Height(HEADER_ITEM *data)
{
  int hh = 2 * (data->y_disp) + GetFontYSIZE(data->font);
  return hh;
}

void InitHeaderText(HEADER_ITEM *data, char *txt)
{
  sprintf(data->text, percent_s, txt);
}

void InitHeaderData(HEADER_ITEM *data, char *txt, int y, int f, int f_t, char *c, char *f_c)
{
  if(strlen(txt)!=0) sprintf(data->text, percent_s, txt);
  if(y!=0) data->y_disp = y;
  if(f!=0) data->font = f;
  if(f_t!=0) data->font_type = f_t;
  if(strlen(c)!=0) memcpy(&(data->colour),c,4);
  if(strlen(f_c)!=0) memcpy(&(data->fring_colour),f_c,4);
   
  // Проверка на пустоту для того,чтобы можно было устнавливать отдельные параметры, не дёргая остальные
  // Например нужно только высоту изменить 
}


char *dday[7]={"Понедельник","Вторник","Среда","Четверг","Пятница","Суббота","Воскресение"};

void DrawDataTime(DATA_TIME *data)
{
  int Y_POS = data->y_position;
  int Font = data->font;
  
  TDate date;
  TTime time;
  GetDateTime(&date, &time);
           
  WSHDR *ws_data_time = AllocWS(128);

  wsprintf(ws_data_time, 
           "%02d.%02d.%02d  %02d:%02d\n%t", 
           date.day, 
           date.month, 
           date.year, 
           time.hour,
           time.min,
           dday[GetWeek(&date)]
             );
  
  DrawString(ws_data_time,0, Y_POS , ScrW, ScrH, Font, 32 + 2, data->colour, data->fring_colour);
  
  FreeWS(ws_data_time);
  
}


void InitDataTime(DATA_TIME *data, int y, int dt_font, char * c1, char * c2)
{
  data->y_position=y;
  data->font=dt_font;
  memcpy(&(data->colour),c1,4);
  memcpy(&(data->fring_colour),c2,4);
}
                   
void DrawMenuList(MENU_STRUCT *data, const MENU_ITEM *hdr, int cur_count)
{
  int Y_DISP = data->y_disp;
  
  WSHDR *ws_head_text = AllocWS(128);
  wsprintf(ws_head_text, percent_t, data->header_text);
  DrawString(ws_head_text, 0,Y_DISP , ScrW, Font_H + Y_DISP, FONT, 32 + 2, COLOUR, COLOUR_FRING);
  int start;
  for(start=0; start <(data-> max_num); start++)
  {
    int NEW_Y = 2*Y_DISP + Font_H + start*(Font_H+Y_DISP);
    
    if(cur_count==start) 
      DrawRoundedFrame(0, NEW_Y + Y_DISP, ScrW, NEW_Y + Font_H+2*Y_DISP, 0, 0, 0 ,CURSOR_COLOUR, CURSOR_COLOUR_FRING);
    
    wsprintf(ws_head_text, percent_t, (data->items)[start]);
    DrawString(ws_head_text, 0, NEW_Y + Y_DISP, ScrW, NEW_Y + Y_DISP + Font_H, FONT, 32 , COLOUR, COLOUR_FRING);
    
  }
  

    
  FreeWS(ws_head_text);
};

void InitMenuList(MENU_STRUCT *data, char *head, int y_dsp)
{
  data->y_disp=y_dsp;
  //memcpy(&(data->items_name), name, sizeof(name));
  sprintf(data->header_text, percent_s, head);
};

// Soft Button Functions =======================================================

void DrawSoftButton(SOFT_BUTTON_STRUCT *data)
{
  int Main_Y = ScrH - Font_H - data->y_disp;
  WSHDR *ws_soft_text = AllocWS(128);
  wsprintf(ws_soft_text, percent_t, data->text_left);
  DrawString(ws_soft_text, data->x_disp, Main_Y, ScrW, ScrH - data->y_disp, FONT, 32, COLOUR, COLOUR_FRING);
  wsprintf(ws_soft_text, percent_t, data->text_right);
  DrawString(ws_soft_text, 0, Main_Y, ScrW - data->x_disp, Main_Y + Font_H, FONT, 32 + 4,COLOUR, COLOUR_FRING);
  FreeWS(ws_soft_text);
};

int Get_SoftButton_Height(SOFT_BUTTON_STRUCT *data)
{
  int hh = 2 *(data->y_disp) + Font_H;
  return hh;
}

void InitSoftButton(SOFT_BUTTON_STRUCT *data, int x_dsp, int y_dsp, char * left, char * right, int ena_t, int ena_t_s)
{
  data->y_disp=y_dsp;
  data->x_disp=x_dsp;
  data->ena_time=ena_t;
  data->ena_time_sec=ena_t_s;
  sprintf(data->text_left, percent_s, left);
  sprintf(data->text_right, percent_s, right);
};

// =============================================================================


