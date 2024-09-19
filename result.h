#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

typedef enum
{
	RESULT_CLEAR = 0,//タイトル画面
	RESULT_GAMEOVER,//ゲーム画面
	RESULT_MAX
}RESULT;

//プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif

