//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

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
#include "pause.h"
#include "particle.h"
#include "rankingscore.h"
#include "time.h"
#include "wave.h"
#include "memory.h"
#include "gauge.h"
#include "ui.h"

//グローバル変数宣言
GAMESTATE g_gameState = GAMESTATE_NONE;//ゲームの状態
int g_nCounterGameState=0;//状態管理カウンター
bool g_bPause = false;//ポーズ中かどうか

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

	//パーティクルの初期化処理
	InitParticle();

	//敵の初期化処理
	InitEnemy();

	//爆発の初期化処理
	InitExplosion();

	//スコアの初期化処理
	InitScore();

	//タイムの初期化処理
	InitTime();

	//メモリ使用量の初期化処理
	InitMemory();

	//ウェーブの初期化処理
	InitWave();

	//ゲージの初期化処理
	InitGauge();

	//UIの初期化処理
	InitUi();

	//ポーズの初期化処理
	InitPause();

	g_gameState = GAMESTATE_NORMAL;//通常状態に設定
	g_nCounterGameState = 0;

	g_bPause = false;//ポーズ解除

	PlaySound(SOUND_LABEL_BGM03);
}

//ゲーム画面の終了処理
void UninitGame(void)
{
	//ポーズの終了処理
	UninitPause();

	//UIの終了処理
	UninitUi();

	//ゲージの初期化処理
	UninitGauge();

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

	//タイムの終了処理
	UninitTime();

	//メモリ使用量の初期化処理
	UninitMemory();

	//ウェーブの終了処理
	UninitWave();

	//音楽の停止
	StopSound();
}

//ゲーム画面の更新処理
void UpdateGame(void)
{
	int nNum;
	int nNum2;
	int nNum3;
	if (KeyboardTrigger(DIK_P) == true|| JoypadTrigger(JOYKEY_START)==true)
	{//ポーズキー(P)が押された

		if (g_bPause == false)
		{
			//サウンドの設定
			PlaySound(SOUND_LABEL_SE04);
		}
		else
		{
			//サウンドの設定
			PlaySound(SOUND_LABEL_SE05);

		}
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//ポーズ中
		UpdatePause();
	}

	if (g_bPause == false)
	{//ポーズ中でなければ
		//背景の更新処理
		UpdateBack();

		//弾の更新処理
		UpdateBullet();

		//エフェクトの更新処理
		UpdateEffect();

		//ゲージの更新処理
		UpdateGauge();

		//プレイヤーの更新処理
		UpdatePlayer();

		//敵の更新処理
		UpdateEnemy();

		//爆発の更新処理
		UpdateExplosion();

		//パーティクルの初期化処理
		UpdateParticle();

		//スコアの更新処理
		UpdateScore();

		//タイムの更新処理
		UpdateTime();

		//メモリ使用量の更新処理
		UpdateMemory();

		//UIの更新処理
		UpdateUi();

		//ウェーブの更新処理
		UpdateWave();


		Enemy* pEnemy = GetEnemy();
		Player* pPlayer = GetPlayer();

		nNum2 = GetTime();
		nNum = GetNumEnemy();
		nNum3 = GetMemory();

		if (nNum <= 0)//敵が全員死んだ場合
		{
		}
		else if (pPlayer->bUse == false)
		{
			g_gameState = GAMESTATE_GAMEOVER;
		}
		else if (nNum2 <= 0)//時間が0になった場合
		{
			g_gameState = GAMESTATE_END;
		}
		else if (nNum3>=100)
		{
			g_gameState = GAMESTATE_GAMEOVER;
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

				ResetRanking();

				//ランキングの設定
				SetRankingScore(GetScore());

				g_nCounterGameState = 0;
				g_gameState = GAMESTATE_NONE;//何もしていない状態に設定

			}
			break;
		case GAMESTATE_GAMEOVER://終了状態
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				SetMode(MODE_BSOD);

				ResetRanking();

				//ランキングの設定
				SetRankingScore(GetScore());

				g_nCounterGameState = 0;
				g_gameState = GAMESTATE_NONE;//何もしていない状態に設定
			}
			break;
		}
	}
}

//ゲーム画面の描画処理
void DrawGame(void)
{
	//背景の描画処理
	DrawBack();

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

	//タイムの描画処理
	DrawTime();

	//メモリ使用量の描画処理
	DrawMemory();

	//ゲージの描画処理
	DrawGauge();

	//UIの描画処理
	DrawUi();


	if (g_bPause == true)
	{
		//ポーズの描画処理
		DrawPause();
	}
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

//ポーズの有効無効設定
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}