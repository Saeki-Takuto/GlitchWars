//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#include "bullet.h"
#include "main.h"
#include "Bullet.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "score.h"
#include "effect.h"

//マクロ定義
#define MAX_BULLET (128)//弾の最大数
#define MAX_SPEED (5)	//アニメーションスピード

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	int nLife;			//寿命
	BULLETTYPE type;	//弾を出してる物のタイプ
	bool bUse;			//使用しているかどうか
}Bullet;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];					//弾の情報

//弾の初期化処理
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
							  "data/TEXTURE/bullet001.png",
							  &g_pTextureBullet);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 50;
		g_aBullet[nCntBullet].bUse = false;//使用していない状態にする

	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBullet,
								NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x- (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y- (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x+ (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y- (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x- (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y+ (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x+ (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y+ (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);

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
	g_pVtxBuffBullet->Unlock();
}

//弾の終了処理
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	int nCntBullet;

	int nDeb = 1;


	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

				//頂点バッファをロックし、頂点データへのポインタを取得
				g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

				//g_aBullet[nCntBullet].move.x += 0.3f;
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;

				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0, 10);

				Enemy* pEnemy;//敵の情報へのポインタ
				Player* pPlayer;
				int nCntEnemy;
				int nCntPlayer;

				//敵の取得
				pEnemy = GetEnemy();

				pPlayer = GetPlayer();

				pVtx += 4 * nCntBullet;

				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y - (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y - (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y + (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y + (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);



				g_aBullet[nCntBullet].nLife--;

				if (0 >= g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || 0 >= g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT)
				{
					g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}

				if (g_aBullet[nCntBullet].nLife == 0)
				{
					g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}

				if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
				{//プレイヤーの弾
					for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{//敵が使用されている
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - WIDTHENEMY / 2 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + WIDTHENEMY / 2 && g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - HEIGHTENEMY / 2 && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + HEIGHTENEMY / 2)
							{
								HitEnemy(nCntEnemy, 50);
								AddScore(10);
								g_aBullet[nCntBullet].bUse = false;//弾を使用してない状態にする
							}
						}
					}
				}
				else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
				{//敵の弾
						if (pPlayer->bUse == true)
						{//プレイヤーが使用されている
							if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - WIDTH / 2 && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + WIDTH / 2 && g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - HEIGHT / 2 && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + HEIGHT / 2)
							{
								HitPlayer(50);
								AddScore(1);
								g_aBullet[nCntBullet].bUse = false;//弾を使用してない状態にする
							}
						}
				}

				g_pVtxBuffBullet->Unlock();

		}
	}
}

//弾の描画処理
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの処理
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4*nCntBullet,//頂点インデックス
				2);
		}
	}
}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type)
{
	int nCntBullet;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].bUse = true;//使用している状態にする
			g_aBullet[nCntBullet].type = type;

			//頂点バッファをロックし、頂点データへのポインタを取得
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y - (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y - (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y + (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (WIDTHBULLET / 2), g_aBullet[nCntBullet].pos.y + (HEIGHTBULLET / 2), g_aBullet[nCntBullet].pos.z);
			break;
		}

		pVtx += 4;


	}
	g_pVtxBuffBullet->Unlock();

}