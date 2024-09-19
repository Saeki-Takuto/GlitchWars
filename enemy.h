//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _ENEMY_H_//このマクロ定義sがされてなかったら
#define _ENEMY_H_//2重インクルード防止のマクロ定義
#include "main.h"

//マクロ定義
#define MAX_ENEMY (128)
#define NUM_ENEMY (3)//敵の種類
#define WIDTHENEMY (150)
#define HEIGHTENEMY (150)

typedef enum
{
	ENEMYSTATE_NORMAL = 0,//通常状態
	ENEMYSTATE_DAMAGE,//ダメージ状態
	ENEMYSTATE_DIE,//死亡状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nCntBull;
	int nLife;
	bool bUse;
	D3DXCOLOR col;
	int nType;
	ENEMYSTATE state;
	int nCounterState;
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife);
Enemy* GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
int GetNumEnemy();

#endif
