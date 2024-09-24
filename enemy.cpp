//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "main.h"
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "particle.h"
#include "score.h"
#include "player.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];
int g_nNumEnemy;//敵の総数

//敵の初期化処理
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ
	int nCntENEMY;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy000.jpg",
		&g_pTextureEnemy[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial001.png",
		&g_pTextureEnemy[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial002.png",
		&g_pTextureEnemy[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial003.png",
		&g_pTextureEnemy[3]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial004.png",
		&g_pTextureEnemy[4]);

	for (nCntENEMY = 0; nCntENEMY < MAX_ENEMY; nCntENEMY++)
	{

		g_aEnemy[nCntENEMY].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置の初期化

		g_aEnemy[nCntENEMY].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きを初期化する

		g_aEnemy[nCntENEMY].nCntBull = 0;						//弾のカウントの初期化

		g_aEnemy[nCntENEMY].nLife = 500;						//ライフを500に

		g_aEnemy[nCntENEMY].bUse = false;						//不使用にする

		g_aEnemy[nCntENEMY].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //カラーの初期化

		g_aEnemy[nCntENEMY].nType = 0;								//タイプの初期化

		g_aEnemy[nCntENEMY].state = {};								//ステートの初期化

		int nCounterState = 0;										//ステートカウントの初期化

		g_nNumEnemy = 0;
	}

	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntENEMY = 0; nCntENEMY < MAX_ENEMY; nCntENEMY++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_aEnemy[nCntENEMY].col);
		pVtx[1].col = D3DXCOLOR(g_aEnemy[nCntENEMY].col);
		pVtx[2].col = D3DXCOLOR(g_aEnemy[nCntENEMY].col);
		pVtx[3].col = D3DXCOLOR(g_aEnemy[nCntENEMY].col);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

}

//敵の終了処理
void UninitEnemy(void)
{
	int nCntENEMY;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//テクスチャの破棄
	for (nCntENEMY = 0; nCntENEMY < NUM_ENEMY; nCntENEMY++)
	{

		if (g_pTextureEnemy[nCntENEMY] != NULL)
		{
			g_pTextureEnemy[nCntENEMY]->Release();
			g_pTextureEnemy[nCntENEMY] = NULL;
		}

	}
	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

}

//敵の描画処理
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_aEnemy[nCntEnemy].bUse==true)
		{
			int nType = g_aEnemy[nCntEnemy].nType;

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemy[nType]);

			//プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}

	}

}

void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	Player* pPlayer;

	pPlayer = GetPlayer();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{	
		if (g_aEnemy[nCntEnemy].bUse == true)
		{

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				g_aEnemy[nCntEnemy].nCntBull++;
				g_aEnemy[nCntEnemy].col.r = 1.0f;
				g_aEnemy[nCntEnemy].col.g = 1.0f;
				g_aEnemy[nCntEnemy].col.b = 1.0f;
				g_aEnemy[nCntEnemy].col.a = 1.0f;

				if (g_aEnemy[nCntEnemy].nCntBull >= 100)
				{
						//弾の発射方向を計算
						D3DXVECTOR3 direction = pPlayer->pos - g_aEnemy[nCntEnemy].pos;
						D3DXVec3Normalize(&direction, &direction);

						// 弾の設定
						SetBullet(g_aEnemy[nCntEnemy].pos, direction * 5.0f, 100, BULLETTYPE_ENEMY);
						g_aEnemy[nCntEnemy].nCntBull = 0;
				}
				break;
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].col.r = 1.0f;
					g_aEnemy[nCntEnemy].col.g = 1.0f;
					g_aEnemy[nCntEnemy].col.b = 1.0f;
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}
				break;
			case ENEMYSTATE_DIE:
				g_aEnemy[nCntEnemy].col.a -= 0.1f;
				if (g_aEnemy[nCntEnemy].col.a <= 0.0f)
				{
					g_aEnemy[nCntEnemy].col.a = 0.0f;
					g_aEnemy[nCntEnemy].bUse = false;
					g_aEnemy[nCntEnemy].nCntBull = 0;
					g_nNumEnemy--;//敵の総数カウントダウン
					SetParticle(g_aEnemy[nCntEnemy].pos, 30,10);
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}
				break;
			}
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntEnemy;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(g_aEnemy[nCntEnemy].col);
			pVtx[1].col = D3DXCOLOR(g_aEnemy[nCntEnemy].col);
			pVtx[2].col = D3DXCOLOR(g_aEnemy[nCntEnemy].col);
			pVtx[3].col = D3DXCOLOR(g_aEnemy[nCntEnemy].col);

		}
	}
	g_pVtxBuffEnemy->Unlock();

}

//敵の設定処理
void SetEnemy(D3DXVECTOR3 pos, int nType,int nLife)
{
	int nCntEnemy;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].bUse = true;//使用している状態にする
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nType = nType; // 敵の種類を設定
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

			if (g_aEnemy[nCntEnemy].nType < 1)
			{
				//頂点バッファをロックし、頂点データへのポインタを取得
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y - (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y - (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y + (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y + (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);
			}
			else
			{
				//頂点バッファをロックし、頂点データへのポインタを取得
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (WIDTHENEMY), g_aEnemy[nCntEnemy].pos.y - (HEIGHTENEMY), g_aEnemy[nCntEnemy].pos.z);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (WIDTHENEMY), g_aEnemy[nCntEnemy].pos.y - (HEIGHTENEMY), g_aEnemy[nCntEnemy].pos.z);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (WIDTHENEMY), g_aEnemy[nCntEnemy].pos.y + (HEIGHTENEMY), g_aEnemy[nCntEnemy].pos.z);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (WIDTHENEMY), g_aEnemy[nCntEnemy].pos.y + (HEIGHTENEMY), g_aEnemy[nCntEnemy].pos.z);

			}

			g_nNumEnemy++;//敵の総数カウントアップ
			break;

		}
		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock();
}

//敵の取得
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}

void HitEnemy(int nCntEnemy, int nDamage)
{
	//VERTEX_2D* pVtx;							//頂点情報へのポインタ
		g_aEnemy[nCntEnemy].nLife -= nDamage;
		AddScore(100);


		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{
			g_aEnemy[nCntEnemy].nLife = 0;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DIE;
		}
		else
		{
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			g_aEnemy[nCntEnemy].col.r = 1.0f;
			g_aEnemy[nCntEnemy].col.g = 0.0f;
			g_aEnemy[nCntEnemy].col.b = 0.0f;
		}
}

int GetNumEnemy()
{
	return g_nNumEnemy;
}