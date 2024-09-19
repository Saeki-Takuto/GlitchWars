#ifndef _GAUGE_H_//このマクロ定義がされてなかったら
#define _GAUGE_H_//2重インクルード防止のマクロ定義

#include "main.h"
//プロトタイプ宣言
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

#endif

