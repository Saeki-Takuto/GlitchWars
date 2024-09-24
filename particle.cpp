//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "particle.h"
#include "main.h"
#include "effect.h" 

//マクロ定義
#define MAX_PARTICLE (256)//パーティクルの最大数

//パーティクル構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置(発生位置)
	int nLife;//寿命(発生時間)
	bool bUse;
	int nNumber;//粒子の数
}Particle;

//グローバル変数
Particle g_aParticle[MAX_PARTICLE];

//初期化処理
void InitParticle(void)
{
	VERTEX_2D* pVtx;

	LPDIRECT3DDEVICE9 pDevice;

	int nCntParticle;

	//デバイスの取得
	pDevice = GetDevice();

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].nLife=0;
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].nNumber = 0;
	}
}

//更新処理
void UpdateParticle(void)
{
	VERTEX_2D* pVtx;
	int nCntParticle;
	int nCntApper;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	float fAngle;
	float fLength;
	int nLife;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//パーティクル生成
			for (nCntApper = 0; nCntApper < g_aParticle[nCntParticle].nNumber; nCntApper++)//発生させたい粒子の数
			{
				pos = g_aParticle[nCntParticle].pos;

				//移動量の設定
				fAngle=(float)(rand() % 629 - 314) / 100.0f;
				fLength = (float)(rand() % 20) / 10.0f + 0.2f;

				move.x = sinf(fAngle) * fLength;
				move.y = cosf(fAngle) * fLength;

				//色の指定
				col = (1.0f, 1.0f, 1.0f, 1.0f);

				//半径の設定
				fRadius = (float)(rand() % 100) / 10.0f + 0.5f;

				//寿命の設定
				nLife = (float)(rand() % 1200) / 10.0f + 0.2;

				//エフェクトの設定
				SetEffect(pos, col, fRadius, nLife, move);
			}
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aParticle[nCntParticle].nLife = 0;
				g_aParticle[nCntParticle].bUse = false;
				g_aParticle[nCntParticle].nNumber = 0;

			}
		}
	}
}

//設定処理
void SetParticle(D3DXVECTOR3 pos, int nLife,int nNumber)
{
	VERTEX_2D* pVtx;

	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;
			g_aParticle[nCntParticle].nNumber = nNumber;

			break;
		}
	}
}
