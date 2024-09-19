#ifndef _BSOD_H_
#define _BSOD_H_

#include "main.h"

typedef enum
{
	BSOD_CLEAR = 0,//タイトル画面
	BSOD_GAMEOVER,//ゲーム画面
	BSOD_MAX
}BSOD;

//プロトタイプ宣言
void InitBsod(void);
void UninitBsod(void);
void UpdateBsod(void);
void DrawBsod(void);

#endif

