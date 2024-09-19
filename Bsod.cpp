#include "bsod.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBsod= {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBsod = NULL;//頂点バッファへのポインタ
int nCntTimeBsod;

//タイトル画面の初期化処理
void InitBsod(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BSOD.png",
		&g_pTextureBsod);

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBsod,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBsod->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffBsod->Unlock();
	nCntTimeBsod = 0;
	//サウンドの設定
	PlaySound(SOUND_LABEL_SE03);
}

//タイトル画面の終了処理
void UninitBsod(void)
{
		//テクスチャの破棄
		if (g_pTextureBsod != NULL)
		{
			g_pTextureBsod->Release();
			g_pTextureBsod = NULL;
		}

	//頂点バッファの破棄
	if (g_pVtxBuffBsod != NULL)
	{
		g_pVtxBuffBsod->Release();
		g_pVtxBuffBsod = NULL;
	}

	StopSound();

}

//タイトル画面の更新処理
void UpdateBsod(void)
{
	nCntTimeBsod++;

	if (nCntTimeBsod >= 300)
	{
		nCntTimeBsod=0;
		SetFade(MODE_RESULT);
	}

	if (KeyboardTrigger(DIK_RETURN) == true || GetJoypadPress(JOYKEY_A))
	{//決定キー(ENTERキー)が押された
		//モード設定(タイトル画面に移行)
		SetFade(MODE_RESULT);
	}
}

//タイトル画面の描画処理
void DrawBsod(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBsod, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBsod);

	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}