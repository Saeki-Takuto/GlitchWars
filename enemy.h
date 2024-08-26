//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _ENEMY_H_//このマクロ定義がされてなかったら
#define _ENEMY_H_//2重インクルード防止のマクロ定義
#include "main.h"

//マクロ定義
#define MAX_ENEMY (128)
#define WIDTHENEMY (300)
#define HEIGHTENEMY (300)

typedef enum
{
	ENEMYSTATE_NORMAL = 0,//通常状態
	ENEMYSTATE_DAMAGE,//ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nType;
	bool bUse;
	int nCntBull;
	ENEMYSTATE state;
	int nCounterState;
	int nLife;
	LPDIRECT3DTEXTURE9 pTexture;
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
Enemy* GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
int GetNumEnemy();

#endif
