//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _BACK_H_//このマクロ定義がされてなかったら
#define _BACK_H_//2重インクルード防止のマクロ定義

#include "main.h"
//プロトタイプ宣言
void InitBack(void);
void UninitBack(void);
void UpdateBack(void);
void DrawBack(void);

#endif

