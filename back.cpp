//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "main.h"
#include "back.h"
#include "wave.h"

//マクロ定義
#define NUM_BG (2)//背景の数


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBack[NUM_BG] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBack = NULL;		//頂点バッファへのポインタ
float g_aPosTexU[NUM_BG];//テクスチャ座標の開始位置(U値)
int g_CounterAnimBack;								//アニメーションカウンター
int g_nPatternAnimBack;								//アニメーションパターンNo.

//プレイヤーの初期化処理
void InitBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	int nCntBG;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ1の読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data/TEXTURE/bg103.jpg",
							  &g_pTextureBack[0]);

	//テクスチャ2の読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data/TEXTURE/001.png",
							  &g_pTextureBack[1]);
	//テクスチャ3の読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data/TEXTURE/bg102.png",
							  &g_pTextureBack[2]);

	//テクスチャ座標の開始位置(U値)の初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aPosTexU[nCntBG] = 0.0f;
	}


	g_CounterAnimBack = 0;						//アニメーションカウンターを初期化
	g_nPatternAnimBack = 0;						//アニメーションパターンNo.を初期化

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBack,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
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
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 1.0f);	

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBack->Unlock();
}

//プレイヤーの終了処理
void UninitBack(void)
{
	int nCntBG;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//テクスチャの破棄
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{

		if (g_pTextureBack[nCntBG] != NULL)
		{
			g_pTextureBack[nCntBG]->Release();
			g_pTextureBack[nCntBG] = NULL;
		}

	}
	//頂点バッファの破棄
	if (g_pVtxBuffBack != NULL)
	{
		g_pVtxBuffBack->Release();
		g_pVtxBuffBack = NULL;
	}
}

//プレイヤーの更新処理
void UpdateBack(void)
{
	int nCntBG;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aPosTexU[nCntBG] += 0.0001f * (nCntBG)*GetWave();

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexU[nCntBG], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexU[nCntBG] + 1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBack->Unlock();
}

//プレイヤーの描画処理
void DrawBack(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ
	int nCntBG;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBack, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{

		if (g_pTextureBack[nCntBG] != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBack[nCntBG]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG*4, 2);
		}
	}
}

