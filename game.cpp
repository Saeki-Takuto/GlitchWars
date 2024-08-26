#include "player.h"
#include "back.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "game.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "effect.h"

//グローバル変数宣言
GAMESTATE g_gameState = GAMESTATE_NONE;//ゲームの状態
int g_nCounterGameState=0;//状態管理カウンター

//ゲーム画面の初期化処理
void InitGame(void)
{
	//背景の初期化処理
	InitBack();

	//弾の初期化処理
	InitBullet();

	//エフェクトの初期化処理
	InitEffect();

	//プレイヤーの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();
	SetEnemy(D3DXVECTOR3(200.0f, 200.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(600.0f, 200.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(900.0f, 200.0f, 0.0f), 0);

	//爆発の初期化処理
	InitExplosion();

	//スコアの初期化処理
	InitScore();

	g_gameState = GAMESTATE_NORMAL;//通常状態に設定
	g_nCounterGameState = 0;

	PlaySound(SOUND_LABEL_BGM03);
}

//ゲーム画面の終了処理
void UninitGame(void)
{
	//爆発の終了処理
	UninitExplosion();

	//敵の終了処理
	UninitEnemy();

	//プレイヤーの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//背景の終了処理
	UninitBack();

	//スコアの終了処理
	UninitScore();

	StopSound();
}

//ゲーム画面の更新処理
void UpdateGame(void)
{
	int nNum;

	//背景の更新処理
	UpdateBack();

	//弾の更新処理
	UpdateBullet();

	//エフェクトの更新処理
	UpdateEffect();

	//プレイヤーの更新処理
	UpdatePlayer();

	//敵の更新処理
	UpdateEnemy();

	//爆発の更新処理
	UpdateExplosion();

	//スコアの更新処理
	UpdateScore();

	//プレイヤーの取得
	Player* pPlayer = GetPlayer();
	Enemy* pEnemy = GetEnemy();

	nNum = GetNumEnemy();

	if (nNum <= 0)//敵が全員死んだ場合
	{
		g_gameState = GAMESTATE_END;
	}
	else if (pPlayer->bUse == false)
	{
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;
	case GAMESTATE_END://終了状態
   		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			//モード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);
			g_nCounterGameState = 0;
			g_gameState = GAMESTATE_NONE;//何もしていない状態に設定

		}
		break;
	}


}

//ゲーム画面の描画処理
void DrawGame(void)
{
	//背景の描画処理
	DrawBack();//back

	//弾の描画処理
	DrawBullet();

	//エフェクトの描画処理
	DrawEffect();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//爆発の描画処理
	DrawExplosion();

	//スコアの描画処理
	DrawScore();
}

//ゲームの状態の設定
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//ゲームの状態の取得
GAMESTATE GetGameState(void)
{
	return g_gameState;
}