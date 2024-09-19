#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//マクロ定義
#define WIDTHTIME (50)
#define HEIGHTTIME (50)

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
int GetTime(void);

#endif
