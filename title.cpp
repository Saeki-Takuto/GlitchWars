//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle[2] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファへのポインタ
int nCntTime;
int nCntPushTime;
int nCntFlash;
int FlashTime;
bool bPush;
//タイトル画面の初期化処理
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title003.png",
		&g_pTextureTitle[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title002.png",
		&g_pTextureTitle[1]);

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

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

	pVtx += 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(200.0f,570.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(1400.0f,570.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(200.0f,770.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(1400.0f,770.0f,0.0f);

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
	g_pVtxBuffTitle->Unlock();

	//サウンドの設定
	PlaySound(SOUND_LABEL_BGM01);

	nCntTime = NULL;
	nCntFlash = NULL;
	FlashTime = 30;
	nCntPushTime=NULL;
	bPush = false;

}

//タイトル画面の終了処理
void UninitTitle(void)
{
	//サウンドの停止
	StopSound();

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	StopSound();
}

//タイトル画面の更新処理
void UpdateTitle(void)
{
	nCntTime++;
	nCntFlash++;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (nCntFlash==FlashTime)
	{
		pVtx += 4;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(0,0,0,0);
		pVtx[1].col = D3DCOLOR_RGBA(0,0,0,0);
		pVtx[2].col = D3DCOLOR_RGBA(0,0,0,0);
		pVtx[3].col = D3DCOLOR_RGBA(0,0,0,0);
	}
	else if (nCntFlash >= FlashTime*2)
	{
		nCntFlash = 0;

		pVtx += 4;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	if (KeyboardTrigger(DIK_RETURN) == true|| JoypadTrigger(JOYKEY_A))
	{//決定キー(ENTERキー)が押された
		if (bPush == false)
		{
			bPush = true;
			//サウンドの停止
			StopSound();
			//サウンドの設定
			PlaySound(SOUND_LABEL_SE08);
			////モード設定(ゲーム画面に移行)
			//SetFade(MODE_TUTORIAL);
		}
	}

	if (bPush == true)
	{
		FlashTime = 4;
		nCntPushTime++;

		if (nCntPushTime >= 180)
		{
			nCntPushTime=0;
			//モード設定(ゲーム画面に移行)
			SetFade(MODE_TUTORIAL);
		}
	}

	if (nCntTime >= (60 * 46)&&bPush==false)
	{
		nCntTime = 0;
		SetFade(MODE_RANKING);
	}
}

//タイトル画面の描画処理
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ

//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCnt]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, 2);
	}

}