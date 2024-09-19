#pragma once
//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _EFFECT_H_	//このマクロ定義がされてなかったら
#define _EFFECT_H_	//2重インクルード防止のマクロ定義

#include "main.h"

//弾の種類
typedef enum
{
	EFFECTTYPE_PLAYER = 0,//プレイヤーの取得
	EFFECTTYPE_ENEMY,//敵の弾
	EFFECTTYPE_MAX
}EFFECTTYPE;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife,D3DXVECTOR3 move);



//マクロ定義
#define WIDTHEFFECT (1000)
#define HEIGHTEFFECT (1000)

#endif
