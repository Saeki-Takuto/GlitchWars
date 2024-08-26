//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#include "main.h"
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"

//マクロ定義
#define NUM_ENEMY (3)//敵の種類

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];
int g_nNumEnemy = 0;//敵の総数

//敵の初期化処理
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ
	int nCntENEMY;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy000.jpg",
		&g_pTextureEnemy[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tex2.png",
		&g_pTextureEnemy[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/rock.png",
		&g_pTextureEnemy[2]);


	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntENEMY = 0; nCntENEMY < NUM_ENEMY; nCntENEMY++)
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_aEnemy[nCntENEMY].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きを初期化する

		g_aEnemy[nCntENEMY].nCntBull = 0;

		g_aEnemy[nCntENEMY].nLife = 500;

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

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if(g_aEnemy[nCntEnemy].bUse==true)
		{
			if (g_pTextureEnemy[nCntEnemy] != NULL)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aEnemy[nCntEnemy].pTexture);

					//プレイヤーの描画
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
			}
		}

	}

}

void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{	
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				g_aEnemy[nCntEnemy].nCntBull++;
				if (g_aEnemy[nCntEnemy].nCntBull >= 60)
				{//SPACE
					//弾の設定
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(5.0f, 0.0f, 0.0f), 0, BULLETTYPE_ENEMY);
					g_aEnemy[nCntEnemy].nCntBull = 0;
				}
				break;
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					pVtx += 4 * nCntEnemy;
					//頂点カラーの設定
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}
				break;
			}
		}
	}
	g_pVtxBuffEnemy->Unlock();

}

//敵の設定処理
void SetEnemy(D3DXVECTOR3 pos, int nType)
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

			g_aEnemy[nCntEnemy].nType = nType; // 敵の種類を設定

			// テクスチャを設定する
			if (nType >= 0 && nType < NUM_ENEMY)
			{
				g_aEnemy[nCntEnemy].pTexture = g_pTextureEnemy[nType];
			}
			else
			{
				g_aEnemy[nCntEnemy].pTexture = NULL; // 無効なタイプの場合
			}

			//頂点バッファをロックし、頂点データへのポインタを取得
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y - (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y - (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y + (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (WIDTHENEMY / 2), g_aEnemy[nCntEnemy].pos.y + (HEIGHTENEMY / 2), g_aEnemy[nCntEnemy].pos.z);

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
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_aEnemy[nCntEnemy].nLife -= nDamage;


	if (g_aEnemy[nCntEnemy].nLife == 0)
	{
		SetExplosion(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].nLife);
		g_nNumEnemy--;//敵の総数カウントダウン
		g_aEnemy[nCntEnemy].bUse = false;


		//if (g_nNumEnemy <= 0)
		//{
		//	//モード設定(リザルト画面に移行)
		//	SetMode(MODE_RESULT);
		//}

	}
	else
	{
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;
		pVtx += 4 * nCntEnemy;
		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		g_pVtxBuffEnemy->Unlock();

	}

}

int GetNumEnemy()
{
	return g_nNumEnemy;
}