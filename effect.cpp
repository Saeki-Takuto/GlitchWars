//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#include "effect.h"
#include "main.h"
#include "Effect.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "score.h"

//マクロ定義
#define MAX_EFFECT (4096)//エフェクトの最大数
#define MAX_SPEED (5)	//アニメーションスピード

//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 col;	//移動量
	D3DXVECTOR3 rot;//向き
	float fRadius;	//エフェクトを出してる物のタイプ
	float fLength;//対角線の長さ
	float fAngle;
	int nLife;			//寿命
	bool bUse;			//使用しているかどうか
}Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];					//エフェクトの情報

//エフェクトの初期化処理
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&g_pTextureEffect);

	//エフェクトの情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].fLength= sqrtf(WIDTHEFFECT * WIDTHEFFECT + HEIGHTEFFECT * HEIGHTEFFECT) / 2.0f;
		g_aEffect[nCntEffect].fAngle = atan2f(WIDTHEFFECT, HEIGHTEFFECT);
		g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
		
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (WIDTHEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.y - (HEIGHTEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.z);
		//pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (WIDTHEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.y - (HEIGHTEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.z);
		//pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - (WIDTHEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.y + (HEIGHTEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.z);
		//pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + (WIDTHEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.y + (HEIGHTEFFECT / 2)* g_aEffect[nCntEffect].fLength, g_aEffect[nCntEffect].pos.z);

		//頂点座標の設定
		pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z - (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z - (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z + (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z + (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z + (0.0f - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z + (0.0f - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z + (0.0f + g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z + (0.0f + g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
		pVtx[3].pos.z = 0.0f;


		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの終了処理
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//エフェクトの更新処理
void UpdateEffect(void)
{
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	int nCntEffect;

	int nDeb = 1;


	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			//g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;
			//g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

			Enemy* pEnemy;//敵の情報へのポインタ
			Player* pPlayer;
			int nCntEnemy;

			//敵の取得
			pEnemy = GetEnemy();

			pPlayer = GetPlayer();

			pVtx += 4 * nCntEffect;

			g_aEffect[nCntEffect].fLength-=2.0f;

			if (g_aEffect[nCntEffect].fLength <= 30)
			{
				g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
			}

			//頂点座標の設定
			pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z - (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z - (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z + (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z + (D3DX_PI - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z + (0.0f - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z + (0.0f - g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + sinf(g_aEffect[nCntEffect].rot.z + (0.0f + g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + cosf(g_aEffect[nCntEffect].rot.z + (0.0f + g_aEffect[nCntEffect].fAngle)) * g_aEffect[nCntEffect].fLength;
			pVtx[3].pos.z = 0.0f;


			g_aEffect[nCntEffect].nLife--;

			if (0 >= g_aEffect[nCntEffect].pos.x >= SCREEN_WIDTH || 0 >= g_aEffect[nCntEffect].pos.y >= SCREEN_HEIGHT)
			{
				g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
			}

			if (g_aEffect[nCntEffect].nLife == 0)
			{
				g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
			}



				//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				//{
				//	if (pEnemy->bUse == true)
				//	{//敵が使用されている
				//		if (g_aEffect[nCntEffect].pos.x >= pEnemy->pos.x - WIDTHENEMY / 2 && g_aEffect[nCntEffect].pos.x <= pEnemy->pos.x + WIDTHENEMY / 2 && g_aEffect[nCntEffect].pos.y >= pEnemy->pos.y - HEIGHTENEMY / 2 && g_aEffect[nCntEffect].pos.y <= pEnemy->pos.y + HEIGHTENEMY / 2)
				//		{
				//			HitEnemy(nCntEnemy, 50);
				//			AddScore(10);
				//			g_aEffect[nCntEffect].bUse = false;//エフェクトを使用してない状態にする
				//		}
				//	}
				//}
			g_pVtxBuffEffect->Unlock();

		}
	}
}

//エフェクトの描画処理
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの処理
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCntEffect,//頂点インデックス
				2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 col, float fRadius, int nLife)
{
	int nCntEffect;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//エフェクトが使用されていない
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;//使用している状態にする
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].fLength = sqrtf(WIDTHEFFECT * WIDTHEFFECT + HEIGHTEFFECT * HEIGHTEFFECT) / 2.0f;
			g_aEffect[nCntEffect].fAngle = atan2f(WIDTHEFFECT, HEIGHTEFFECT);

			break;
		}

		pVtx += 4;


	}
	g_pVtxBuffEffect->Unlock();

}