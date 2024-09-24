//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[2] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//頂点バッファへのポインタ

//タイトル画面の初期化処理
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/CLEAR.png",
		&g_pTextureResult[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/OVER.png",
		&g_pTextureResult[1]);


	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffResult->Unlock();

	if (GetGameState() == GAMESTATE_END)
	{
		//サウンドの設定
		PlaySound(SOUND_LABEL_BGM04);
	}
	else
	{
		//サウンドの設定
		PlaySound(SOUND_LABEL_BGM05);
	}

}

//タイトル画面の終了処理
void UninitResult(void)
{
	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt]= NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	StopSound();

}

//タイトル画面の更新処理
void UpdateResult(void)
{
	if (KeyboardTrigger(DIK_RETURN) == true|| GetJoypadPress(JOYKEY_A))
	{//決定キー(ENTERキー)が押された
		//モード設定(タイトル画面に移行)
		SetFade(MODE_RANKING);
	}
}

//タイトル画面の描画処理
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (GetGameState() == GAMESTATE_END)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[0]);
	}
	else
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[1]);
	}

	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}