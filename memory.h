#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "main.h"

//マクロ定義
#define WIDTHMEMORY (50)
#define HEIGHTMEMORY (50)

//プロトタイプ宣言
void InitMemory(void);
void UninitMemory(void);
void UpdateMemory(void);
void DrawMemory(void);
void SetMemory(int nMemory);
void AddMemory(int nValue);
int GetMemory(void);

#endif
