//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _BULLET_H_	//このマクロ定義がされてなかったら
#define _BULLET_H_	//2重インクルード防止のマクロ定義

#include "main.h"

//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER=0,//プレイヤーの取得
	BULLETTYPE_ENEMY,//敵の弾
	BULLETTYPE_HOMING,
	BULLETTYPE_MAX
}BULLETTYPE;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos,D3DXVECTOR3 move,int nLife,BULLETTYPE type);



//マクロ定義
#define WIDTHBULLET (30)
#define HEIGHTBULLET (30)

#endif
