//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "bullet.h"
#include "effect.h"
#include "player.h"
#include "particle.h"
#include "enemy.h"
#include "score.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTutorial = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;//頂点バッファへのポインタ
int nCntTimeTutorial;

//タイトル画面の初期化処理
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial.png",
		&g_pTextureTutorial);

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(000.0f, 000.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;


	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
	nCntTimeTutorial = 0;
	//サウンドの設定
	PlaySound(SOUND_LABEL_BGM06);

	//弾の初期化処理
	InitBullet();

	//エフェクトの初期化処理
	InitEffect();

	//プレイヤーの初期化処理
	InitPlayer();

	InitParticle();

	InitScore();

	//敵の初期化処理
	InitEnemy();

	SetEnemy(D3DXVECTOR3(200.0f, 250.0f, 0.0f), 1, 50);
	SetEnemy(D3DXVECTOR3(200.0f, 650.0f, 0.0f), 2, 50);
	SetEnemy(D3DXVECTOR3(1400.0f, 250.0f, 0.0f), 3, 50);
	SetEnemy(D3DXVECTOR3(1400.0f, 650.0f, 0.0f), 4, 50);
}

//タイトル画面の終了処理
void UninitTutorial(void)
{
	//敵の終了処理
	UninitEnemy();

	//プレイヤーの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//スコアの終了処理
	UninitScore();

	//テクスチャの破棄
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//サウンドの停止
	StopSound();
}

//タイトル画面の更新処理
void UpdateTutorial(void)
{
	int nNum = GetNumEnemy();

	if (KeyboardTrigger(DIK_RETURN) == true || GetJoypadPress(JOYKEY_Y)||nNum<=0)
	{//決定キー(ENTERキー)が押された
		//モード設定(タイトル画面に移行)
		SetFade(MODE_GAME);
	}

	//弾の更新処理
	UpdateBullet();

	//エフェクトの更新処理
	UpdateEffect();

	//プレイヤーの更新処理
	UpdatePlayer();

	//敵の更新処理
	UpdateEnemy();

	//パーティクルの更新処理
	UpdateParticle();

	//スコアの更新処理
	UpdateScore();
}

//タイトル画面の描画処理
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorial);

	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//弾の描画処理
	DrawBullet();

	//エフェクトの描画処理
	DrawEffect();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

}