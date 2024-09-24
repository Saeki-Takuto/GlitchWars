//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "score.h"
#include "main.h"
//マクロ定義
#define MAX_DIGIT (8)//最大桁

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;//スコアの位置
int g_nScore;//スコアの値

//スコアの初期化処理
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&g_pTextureScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);


	g_posScore = D3DXVECTOR3(0.0f,0.0f,0.0f);//位置を初期化する
	g_nScore = 0;//値を初期化する

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH-250+nCntScore*30.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - 250 +nCntScore*30.0f+30.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 250 +nCntScore*30.0f, 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 250 +nCntScore*30.0f+30.0f, 60.0f, 0.0f);

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
	g_pVtxBuffScore->Unlock();
}

//スコアの終了処理
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//スコアの更新処理
void UpdateScore(void)
{
}

//スコアの描画処理
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		////テクスチャの設定
		////pDevice->SetTexture(0, NULL);


		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*nCntScore, 2);
	}
}

//スコアの設定処理
void SetScore(int nScore)
{
	int aPosTexU[MAX_DIGIT];//各桁の数字を格納
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_nScore = nScore;

	int nCntScore;
	int nData1 = 10000000, nData2 = 1000000;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		if (nCntScore==0)
		{
			aPosTexU[0] = nScore / nData1;
		}
		else
		{
			aPosTexU[nCntScore] = (nScore % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0+(0.1*aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1+(0.1 * aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffScore->Unlock();

}

//スコアの加算処理
void AddScore(int nValue)
{
	int aPosTexU[MAX_DIGIT];//各桁の数値を格納

	g_nScore += nValue;
	int nCntScore;
	int nData1 = 10000000, nData2 = 1000000;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		if (nCntScore == 0)
		{
			aPosTexU[0] = g_nScore / nData1;
		}
		else
		{
			aPosTexU[nCntScore] = (g_nScore % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffScore->Unlock();
}

//スコア取得
int GetScore(void)
{
	return g_nScore;
}