//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _PLAYER_H_//このマクロ定義がされてなかったら
#define _PLAYER_H_//2重インクルード防止のマクロ定義
#define FLAME (10)
#define SPEED (5)
#define WIDTH (100)
#define HEIGHT (150)

#include"main.h"

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,//出現状態(点滅)
	PLAYERSTATE_NORMAL,//通常状態
	PLAYERSTATE_DAMAGE,//ダメージ状態
	PLAYERSTATE_DEATH,//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	PLAYERSTATE state;//プレイヤーの状態
	D3DXVECTOR3 move;//移動量
	int nCounterState;//状態管理カウンター
	int nLife;//体力
	bool bDisp;//表示するかしないか
	bool bUse;
}Player;


//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);

Player* GetPlayer(void);

#endif
