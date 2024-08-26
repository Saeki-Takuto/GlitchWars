//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _EXPLOSION_H_	//このマクロ定義がされてなかったら
#define _EXPLOSION_H_	//2重インクルード防止のマクロ定義

#include "main.h"
//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col);

//マクロ定義
#define WIDTHEXPLOSION (50)
#define HEIGHTEXPLOSION (50)
#define FLAMEEX (8)
#define SPEEDEX (5)


#endif
