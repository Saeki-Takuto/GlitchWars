//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "gauge.h"
#include "bsod.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "enemy.h"
#include "memory.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGauge[MAX_UI] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;//頂点バッファへのポインタ

void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/gaugeback.png",
		&g_pTextureGauge[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/gauge.png",
		&g_pTextureGauge[1]);


	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(502.0f, 10.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 60.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(502.0f, 60.0f, 0.0f);

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
	pVtx[0].pos = D3DXVECTOR3(0.0f, 12.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 12.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 58.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 58.0f, 0.0f);

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
	g_pVtxBuffGauge->Unlock();

}

void UninitGauge(void)
{
	for (int nCnt = 0; nCnt > MAX_UI; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureGauge[nCnt] != NULL)
		{
			g_pTextureGauge[nCnt]->Release();
			g_pTextureGauge[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}

}

void UpdateGauge(void)
{
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4;

	//頂点座標の設定
	pVtx[1].pos = D3DXVECTOR3(GetMemory()*5.0f, 12.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetMemory()*5.0f, 58.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();

}

void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_pTextureGauge[nCnt] != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureGauge[nCnt]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

}
