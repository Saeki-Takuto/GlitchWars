//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "ui.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureUi[2] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL;//頂点バッファへのポインタ

//タイトル画面の初期化処理
void InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/memory.png",
		&g_pTextureUi[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/percent.png",
		&g_pTextureUi[1]);

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f,50.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(170.0f,50.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,102.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(170.0f,102.0f,0.0f);

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

	pVtx += 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(270.0f, 50.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(320.0f, 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(270.0f, 102.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(320.0f, 102.0f, 0.0f);

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
	g_pVtxBuffUi->Unlock();

}

//タイトル画面の終了処理
void UninitUi(void)
{
	for (int nCnt = 0; nCnt < UI_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureUi[nCnt] != NULL)
		{
			g_pTextureUi[nCnt]->Release();
			g_pTextureUi[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}

	//サウンドの停止
	StopSound();
}

//タイトル画面の更新処理
void UpdateUi(void)
{
}

//タイトル画面の描画処理
void DrawUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureUi[nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}