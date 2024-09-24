//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "time.h"
#include "main.h"
#include "wave.h"
//マクロ定義
#define MAX_DIGIT (3)//最大桁

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;//頂点バッファへのポインタ
D3DXVECTOR3 g_posTime;//スコアの位置
int g_nTime;//スコアの値
int nCntTimeSecond;

//スコアの初期化処理
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTime;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&g_pTextureTime);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);


	g_posTime = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置を初期化する
	g_nTime = 120;//値を初期化する
	nCntTime=0;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_DIGIT; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - 900 + nCntTime * 30.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - 900 + nCntTime * 30.0f + 30.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 900 + nCntTime * 30.0f, 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 900 + nCntTime * 30.0f + 30.0f, 60.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//スコアの終了処理
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//スコアの更新処理
void UpdateTime(void)
{
	int aPosTexU[MAX_DIGIT];//各桁の数字を格納
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	int nCntTime;
	int nData1 = 100, nData2 = 10;

	nCntTimeSecond++;

	if (nCntTimeSecond >= 60)
	{
		nCntTimeSecond = 0;
		if (g_nTime != 0)
		{
			g_nTime--;
			HitWave(1);
		}
	}

	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntTime = 0; nCntTime < MAX_DIGIT; nCntTime++)
	{
		if (nCntTime == 0)
		{
			aPosTexU[0] = g_nTime / nData1;
		}
		else
		{
			aPosTexU[nCntTime] = (g_nTime % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntTime]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntTime]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntTime]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntTime]), 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffTime->Unlock();

}

//スコアの描画処理
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	int nCntTime;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTime = 0; nCntTime < MAX_DIGIT; nCntTime++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTime);

		////テクスチャの設定
		////pDevice->SetTexture(0, NULL);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTime, 2);
	}
}

//スコアの設定処理
void SetTime(int nTime)
{
	int aPosTexU[MAX_DIGIT];//各桁の数字を格納
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_nTime = nTime;

	int nCntTime;
	int nData1 = 100, nData2 = 10;

	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntTime = 0; nCntTime < MAX_DIGIT; nCntTime++)
	{
		if (nCntTime == 0)
		{
			aPosTexU[0] = nTime / nData1;
		}
		else
		{
			aPosTexU[nCntTime] = (nTime % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntTime]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntTime]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntTime]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntTime]), 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffTime->Unlock();

}

//スコアの加算処理
void AddTime(int nValue)
{
	int aPosTexU[MAX_DIGIT];//各桁の数値を格納

	g_nTime += nValue;
	int nCntTime;
	int nData1 = 100, nData2 = 10;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntTime = 0; nCntTime < MAX_DIGIT; nCntTime++)
	{
		if (nCntTime == 0)
		{
			aPosTexU[0] = g_nTime / nData1;
		}
		else
		{
			aPosTexU[nCntTime] = (g_nTime % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntTime]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntTime]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntTime]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntTime]), 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffTime->Unlock();
}

//スコア取得
int GetTime(void)
{
	return g_nTime;
}