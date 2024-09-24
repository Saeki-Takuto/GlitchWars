#ifndef _UI_H_
#define _UI_H_

#include "main.h"

typedef enum
{
	UI_CLEAR = 0,//タイトル画面
	UI_GAMEOVER,//ゲーム画面
	UI_MAX
}UI;

//プロトタイプ宣言
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);

#endif

