#define _GUI_BEGIN_H_
 #ifdef _GUI_BEGIN_H_

// Запустить начальный гуй
void RUN_GUI_BEGIN(int mode);

void SMART_REDRAW(void);

void BeginStep(int max);
void NextStep(char * msg);
void EndStep();

#endif
