#include "memory.h"
#include "main.h"
#include "enemy.h"
//マクロ定義
#define MAX_KETA (3)//最大桁

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMemory = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMemory = NULL;//頂点バッファへのポインタ
D3DXVECTOR3 g_posMemory;//スコアの位置
int g_nMemory;//スコアの値
int nCntMemorySecond;

//スコアの初期化処理
void InitMemory(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntMemory;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureMemory);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_KETA,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMemory,
		NULL);


	g_posMemory = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置を初期化する
	g_nMemory = 0;//値を初期化する
	nCntMemory = 0;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMemory->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMemory = 0; nCntMemory < MAX_KETA; nCntMemory++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - 800 + nCntMemory * 30.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - 800 + nCntMemory * 30.0f + 30.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 800 + nCntMemory * 30.0f, 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 800 + nCntMemory * 30.0f + 30.0f, 50.0f, 0.0f);

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
	g_pVtxBuffMemory->Unlock();
}

//スコアの終了処理
void UninitMemory(void)
{
	//テクスチャの破棄
	if (g_pTextureMemory != NULL)
	{
		g_pTextureMemory->Release();
		g_pTextureMemory = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMemory != NULL)
	{
		g_pVtxBuffMemory->Release();
		g_pVtxBuffMemory = NULL;
	}
}

//スコアの更新処理
void UpdateMemory(void)
{
	int aPosTexU[MAX_KETA];//各桁の数字を格納
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	int nCntMemory;
	int nData1 = 100, nData2 = 10;

	//nCntMemorySecond++;

	//if (nCntMemorySecond >= 60)
	//{
	//	nCntMemorySecond = 0;
	//	if (g_nMemory != 0)
	//	{
	//		g_nMemory--;
	//	}
	//}

	g_nMemory=GetNumEnemy()*5;

	if (g_nMemory >= 100)
	{
		g_nMemory = 100;
	}

	g_pVtxBuffMemory->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntMemory = 0; nCntMemory < MAX_KETA; nCntMemory++)
	{
		if (nCntMemory == 0)
		{
			aPosTexU[0] = g_nMemory / nData1;
		}
		else
		{
			aPosTexU[nCntMemory] = (g_nMemory % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntMemory]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntMemory]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntMemory]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntMemory]), 1.0f);




		pVtx += 4;
	}
	g_pVtxBuffMemory->Unlock();

}

//スコアの描画処理
void DrawMemory(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	int nCntMemory;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMemory, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMemory = 0; nCntMemory < MAX_KETA; nCntMemory++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMemory);

		////テクスチャの設定
		////pDevice->SetTexture(0, NULL);


		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMemory, 2);
	}
}

//スコアの設定処理
void SetMemory(int nMemory)
{
	int aPosTexU[MAX_KETA];//各桁の数字を格納
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_nMemory = nMemory;

	int nCntMemory;
	int nData1 = 100, nData2 = 10;

	g_pVtxBuffMemory->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntMemory = 0; nCntMemory < MAX_KETA; nCntMemory++)
	{
		if (nCntMemory == 0)
		{
			aPosTexU[0] = nMemory / nData1;
		}
		else
		{
			aPosTexU[nCntMemory] = (nMemory % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntMemory]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntMemory]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntMemory]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntMemory]), 1.0f);




		pVtx += 4;
	}
	g_pVtxBuffMemory->Unlock();

}

//スコアの加算処理
void AddMemory(int nValue)
{
	int aPosTexU[MAX_KETA];//各桁の数値を格納

	g_nMemory += nValue;
	int nCntMemory;
	int nData1 = 100, nData2 = 10;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffMemory->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (nCntMemory = 0; nCntMemory < MAX_KETA; nCntMemory++)
	{
		if (nCntMemory == 0)
		{
			aPosTexU[0] = g_nMemory / nData1;
		}
		else
		{
			aPosTexU[nCntMemory] = (g_nMemory % nData1 / nData2);
			nData1 /= 10;
			nData2 /= 10;
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntMemory]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntMemory]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntMemory]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntMemory]), 1.0f);

		pVtx += 4;
	}
	g_pVtxBuffMemory->Unlock();


}

//スコア取得
int GetMemory(void)
{
	return g_nMemory;
}