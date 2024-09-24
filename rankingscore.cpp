//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "input.h"
#include "fade.h"
#include "game.h"
#include "score.h"
#include "rankingscore.h"
#include <stdio.h>
#include "input.h"

//マクロ定義
#define MAX_RANK (5)
#define MAX_DIGIT (8)
#define MAX_POLYGON (MAX_RANK*MAX_DIGIT)

typedef struct
{
	D3DXVECTOR3 pos;
	int nScore;
}RankScore;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRankingScore = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingScore = NULL;//頂点バッファへのポインタ
RankScore g_aRankScore[MAX_RANK];

int g_nRankUpdate = -1;//ランキングの更新
int g_nTimeRanking;//タイマー

//ランキングの初期化処理
void InitRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureRankingScore);

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingScore,
		NULL);

	//初期化
	g_nTimeRanking = 0;

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		g_aRankScore[nCntRank].pos = D3DXVECTOR3(590.0f, 250.0f + (nCntRank * 80.0f)+(20.0f*nCntRank), 0.0f);
		g_aRankScore[nCntRank].nScore = 0;
		for (int nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			//頂点座標の設定
			pVtx[0].pos.x = g_aRankScore[nCntRank].pos.x - WIDTHRANKINGSCORE / 2 + (WIDTHRANKINGSCORE * nCntScore);
			pVtx[0].pos.y = g_aRankScore[nCntRank].pos.y - HEIGHTRANKINGSCORE / 2;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aRankScore[nCntRank].pos.x + WIDTHRANKINGSCORE / 2 + (WIDTHRANKINGSCORE * nCntScore);
			pVtx[1].pos.y = g_aRankScore[nCntRank].pos.y - HEIGHTRANKINGSCORE / 2;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aRankScore[nCntRank].pos.x - WIDTHRANKINGSCORE / 2 + (WIDTHRANKINGSCORE * nCntScore);
			pVtx[2].pos.y = g_aRankScore[nCntRank].pos.y + HEIGHTRANKINGSCORE / 2;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aRankScore[nCntRank].pos.x + WIDTHRANKINGSCORE / 2 + (WIDTHRANKINGSCORE * nCntScore);
			pVtx[3].pos.y = g_aRankScore[nCntRank].pos.y + HEIGHTRANKINGSCORE / 2;
			pVtx[3].pos.z = 0.0f;

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
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRankingScore->Unlock();
}

//ランキングの終了処理
void UninitRankingScore(void)
{
	//テクスチャの破棄
	if (g_pTextureRankingScore != NULL)
	{
		g_pTextureRankingScore->Release();
		g_pTextureRankingScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRankingScore != NULL)
	{
		g_pVtxBuffRankingScore->Release();
		g_pVtxBuffRankingScore = NULL;
	}
}

//ランキングの更新処理
void UpdateRankingScore(void)
{
	static int nCounter{};

	if (g_nRankUpdate != -1)
	{
		nCounter++;   //加算

		//該当スコアを点滅
		if (nCounter == 10)
		{
			VERTEX_2D* pVtx{};       //頂点情報のポインタ

		//頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
				pVtx += 4;
			}

			g_pVtxBuffRankingScore->Unlock();
		}
		else if (nCounter == 20)
		{
			VERTEX_2D* pVtx{};       //頂点情報のポインタ

		//頂点バッファをロックし,頂点情報へのポインタを取得
			g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (32 * g_nRankUpdate);

			for (int nCnt1 = 0; nCnt1 < MAX_DIGIT; nCnt1++)
			{
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

				pVtx += 4;
			}

			g_pVtxBuffRankingScore->Unlock();

			nCounter = 0;
		}

	}
	if (KeyboardTrigger(DIK_RETURN) == true|| JoypadTrigger(JOYKEY_A))
	{
		//タイトル
		SetFade(MODE_TITLE);
	}
}

//ランキングの描画処理
void DrawRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ
	int nCntRankScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntRankScore = 0; nCntRankScore < MAX_POLYGON; nCntRankScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRankingScore);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRankScore, 2);
	}
}

//ランキングの設定処理
void SetRankingScore(int nScore)
{
	int nCntRank;
	int nCntScore;
	int nData{};

	g_nRankUpdate = -1;//初期化

	//5番目のスコアと比較
	if (g_aRankScore[MAX_RANK - 1].nScore < nScore)
	{
		g_aRankScore[MAX_RANK - 1].nScore = nScore;
		g_nRankUpdate = MAX_RANK - 1;
	}

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (nCntScore = nCntRank + 1; nCntScore < MAX_RANK; nCntScore++)
		{
			if (g_aRankScore[nCntRank].nScore <= g_aRankScore[nCntScore].nScore)
			{
				nData = g_aRankScore[nCntRank].nScore;//上書き前のスコアを保存
				g_aRankScore[nCntRank].nScore = g_aRankScore[nCntScore].nScore;//置き換え
				g_aRankScore[nCntScore].nScore = nData;//置き換えデータを保存
			}
		}
	}

	//スコアが入っていたら
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (nScore == g_aRankScore[nCntRank].nScore)
		{
			g_nRankUpdate = nCntRank;
			break;
		}
	}

	WriteScore();//スコアの書き出し
}

void ResetRanking(void)
{
	FILE* pFile;//ファイルの読み込み

	//ファイルを開く
	pFile = fopen("data/RANKING/ranking.txt", "r");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			//ファイルを開けた場合
			fscanf(pFile, "%d", &g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;
	}

	fclose(pFile);

}

//スコア書き出し
void WriteScore(void)
{
	FILE* pFile;//ファイルの読み込み

	//ファイルを開く
	pFile = fopen("data/RANKING/ranking.txt", "w");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			//ファイルを開けた場合
			fprintf(pFile, "%d\n", g_aRankScore[nCnt].nScore);
		}
	}
	else
	{
		MessageBox(NULL, "開けません", "エラー", MB_OK);

		return;
	}
	fclose(pFile);
}

//ランキングのスコアテクスチャ処理
void SetRankingScoreTexture(void)
{
	int aPosTexU[MAX_DIGIT] = {};//各桁の数字を格納
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	int nCntScore;
	int nData1 = 10000000, nData2 = 1000000;

	//頂点バッファのロック
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	//桁ごとに分割する
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
		{
			if (nCntScore == 0)
			{
				aPosTexU[0] = g_aRankScore[nCntRank].nScore / nData1;
			}
			else
			{
				aPosTexU[nCntScore] = g_aRankScore[nCntRank].nScore % nData1 / nData2;
				nData1 = nData1 / 10;
				nData2 = nData2 / 10;
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntScore]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntScore]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0 + (0.1 * aPosTexU[nCntScore]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1 + (0.1 * aPosTexU[nCntScore]), 1.0f);

			pVtx += 4;
		}
		nData1 = 10000000, nData2 = 1000000;
	}
	//アンロック
	g_pVtxBuffRankingScore->Unlock();
}