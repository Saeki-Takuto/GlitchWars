//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#include "main.h"
#include "player.h"
#include "back.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "pause.h"

//マクロ定義
#define NUM_PA (3)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTexturePause[3] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//頂点バッファへのポインタ
PAUSE_MENU g_PauseMenu;//ポーズメニュー
int PauseSelect;

//プレイヤーの初期化処理
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	int nCntPause;
	PauseSelect = 0;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ1の読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/continue.png",
		&g_apTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/RETRY.png",
		&g_apTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/MENU.png",
		&g_apTexturePause[2]);

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PA,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < NUM_PA; nCntPause++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(000.0f, 000.0+(100*nCntPause), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(300.0f, 0.0 + (100 * nCntPause), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 100.0 + (100 * nCntPause), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(300.0f, 100.0 + (100 * nCntPause), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}

void UninitPause(void)
{
	int nCntPA;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//テクスチャの破棄
	for (nCntPA = 0; nCntPA < NUM_PA; nCntPA++)
	{

		if (g_apTexturePause[nCntPA] != NULL)
		{
			g_apTexturePause[nCntPA]->Release();
			g_apTexturePause[nCntPA] = NULL;
		}

	}
	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

}

void UpdatePause(void)
{
	VERTEX_2D* pVtx;							//頂点情報へのポインタ
	int nCntPause;

	if (KeyboardTrigger(DIK_DOWN) == true)
	{//下が押された
		PauseSelect++;
		if (PauseSelect >= PAUSE_MENU_MAX)
		{
			PauseSelect = 0;
		}
	}
	else if (KeyboardTrigger(DIK_UP) == true)
	{//上が押された
		PauseSelect--;
		if (PauseSelect < 0)
		{
			PauseSelect = PAUSE_MENU_MAX-1;

		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < NUM_PA; nCntPause++)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * PauseSelect;
	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_pVtxBuffPause->Unlock();


	if (KeyboardTrigger(DIK_RETURN) == true)
	{
		switch (PauseSelect)
		{
		case PAUSE_MENU_CONTINUE://コンテニュー
			SetEnablePause(false);
			break;
		case PAUSE_MENU_RETRY://リトライ
			SetFade(MODE_GAME);
			break;
		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE);
			break;
		}
	}

}

void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ
	int nCntPA;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPA = 0; nCntPA < NUM_PA; nCntPA++)
	{

		if (g_apTexturePause[nCntPA] != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePause[nCntPA]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPA * 4, 2);
		}

	}

}