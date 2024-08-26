//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#include "main.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "fade.h"


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;//頂点バッファへのポインタ
int g_CounterAnimPlayer;						//アニメーションカウンター
int g_nPatternAnimPlayer;						//アニメーションパターンNo.
float g_fLengthPlayer;//対角線の長さ
float g_fAnglePlayer;//対角線の角度
float g_Direction;	//方向
Player g_player;

//プレイヤーの初期化処理
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/rocket_02.png",
		&g_pTexturePlayer);

	g_CounterAnimPlayer=0;						//アニメーションカウンターを初期化
	g_nPatternAnimPlayer=0;						//アニメーションパターンNo.を初期化
	g_player.pos = D3DXVECTOR3(300.0f,300.0f,0.0f);		//位置を初期化
	g_player.move= D3DXVECTOR3(0.0f,0.0f,0.0f);			//移動量を初期化
	g_Direction = 0.2;							//方向を初期化
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きを初期化する
	g_player.bDisp = true;//画面表示状態を初期化する
	g_player.bUse = true;
	g_player.nLife = 10;

	//対角線の長さを算出する
	g_fLengthPlayer = sqrtf(WIDTH * WIDTH + HEIGHT * HEIGHT) / 2.0f;

	//対角線の角度を算出する
	g_fAnglePlayer = atan2f(WIDTH, HEIGHT);


	//頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z + (0.0f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z + (0.0f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + (0.0f + g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + (0.0f + g_fAnglePlayer)) * g_fLengthPlayer;
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

	////テクスチャ座標の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	//pVtx[3].tex = D3DXVECTOR2(0.2f, 0.5f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//プレイヤーの終了処理
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//プレイヤーの更新処理
void UpdatePlayer(void)
{
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.nCounterState = PLAYERSTATE_NORMAL;
			//頂点カラーの設定(通常職に戻す)(後で打ち込む)
		}
		break;
	case PLAYERSTATE_DEATH:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			//モード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);
			return;
		}
		break;
	}

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	if (KeyboardRepeat(DIK_A) == true||JoypadRepeat(JOYKEY_LEFT)||JoypadStickLX1())
	{//Aキーが押された
		g_Direction = 0.2;
		if (KeyboardRepeat(DIK_W) == true|| JoypadRepeat(JOYKEY_UP) || JoypadStickLY0())
		{//AとW同時押し(左上)
			g_player.move.x += sinf(-D3DX_PI * 0.75f) * 3.0f;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * 3.0f;
		}
		else if (KeyboardRepeat(DIK_S) == true|| JoypadRepeat(JOYKEY_DOWN) || JoypadStickLY1())
		{//AとS同時押し(左下)
			g_player.move.x += sinf(-D3DX_PI * 0.25f) * 3.0f;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * 3.0f;
		}
		else if (KeyboardRepeat(DIK_D) == true|| JoypadRepeat(JOYKEY_RIGHT) || JoypadStickLX0())
		{//AとD同時押し(動かない)
		}
		else
		{//Aしか押されない(左)
			g_player.move.x -= 3.0f;
		}
	}
	else if (KeyboardRepeat(DIK_D) == true|| JoypadRepeat(JOYKEY_RIGHT)|| JoypadStickLX0())
	{//Dキーが押された
		g_Direction = -0.2;
		if (KeyboardRepeat(DIK_W) == true|| JoypadRepeat(JOYKEY_UP) || JoypadStickLY0())
		{//DとW同時押し(右上)
			g_player.move.x += sinf(+D3DX_PI * 0.75f) * 3.0f;
			g_player.move.y += cosf(+D3DX_PI * 0.75f) * 3.0f;
		}
		else if (KeyboardRepeat(DIK_S) == true|| JoypadRepeat(JOYKEY_DOWN) || JoypadStickLY1())
		{//DとS同時押し(右下)
			g_player.move.x += sinf(+D3DX_PI * 0.25f) * 3.0f;
			g_player.move.y += cosf(+D3DX_PI * 0.25f) * 3.0f;
		}
		else if (KeyboardRepeat(DIK_A) == true|| JoypadRepeat(JOYKEY_LEFT) || JoypadStickLX1())
		{//DとA同時押し(動かない)
		}
		else
		{//Dしか押されない(右)
			g_player.move.x += 3.0f;
		}
	}
	else if (KeyboardRepeat(DIK_W) == true|| JoypadRepeat(JOYKEY_UP) || JoypadStickLY0())
	{//Wキーが押された
		if (KeyboardRepeat(DIK_S) == true|| JoypadRepeat(JOYKEY_DOWN) || JoypadStickLY1())
		{//WとS同時押し(動かない)
		}
		else
		{//Wだけ押した(上)
			g_player.move.y -= 3.0;
		}
	}
	else if (KeyboardRepeat(DIK_S) == true|| JoypadRepeat(JOYKEY_DOWN) || JoypadStickLY1())
	{//Sキーが押された
		if (KeyboardRepeat(DIK_W) == true|| JoypadRepeat(JOYKEY_UP) || JoypadStickLY0())
		{//SとW同時押し(動かない)
		}
		else
		{//Sだけ押した(下)
			g_player.move.y += 3.0f;
		}
	}

	if (KeyboardRepeat(DIK_UP) == true|| JoypadRepeat(JOYKEY_L3))
	{//UPキーが押された(拡大)
		if (g_fLengthPlayer < 400)
		{
			g_fLengthPlayer += 10.0f;
		}
	}
	else if (KeyboardRepeat(DIK_DOWN) == true|| JoypadRepeat(JOYKEY_R3))
	{//DOWNキーが押された(縮小)
		if (g_fLengthPlayer > 40)
		{
			g_fLengthPlayer -= 10.0f;
		}
	}
	else if (KeyboardRepeat(DIK_LEFT) == true|| JoypadRepeat(JOYKEY_L1) == true)
	{//LEFTキーが押された(反時計回り)
		g_player.rot.z += 0.1f;
	}
	else if (KeyboardRepeat(DIK_RIGHT) == true|| JoypadRepeat(JOYKEY_R1) == true)
	{//RIGHTキーが押された(時計回り)
		g_player.rot.z -= 0.1f;
	}
	else if(KeyboardTrigger(DIK_R) == true)
	{
		int nID = MessageBox(NULL,"リセットしますか?", "リセッシュ", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			g_CounterAnimPlayer = 0;												//アニメーションカウンターを初期化
			g_nPatternAnimPlayer = 0;												//アニメーションパターンNo.を初期化
			g_player.pos = D3DXVECTOR3(300.0f, 300.0f, 0.0f);						//位置を初期化
			g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//移動量を初期化
			g_Direction = 0.2;														//方向を初期化
			g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//向きを初期化する
			g_fLengthPlayer = sqrtf(WIDTH * WIDTH + HEIGHT * HEIGHT) / 2.0f;		//対角線の長さを再び算出する
		}
	}

	if (KeyboardTrigger(DIK_SPACE) == true|| JoypadTrigger(JOYKEY_A))
	{//SPACE
		//弾の設定
		//SetBullet(g_player.pos, g_player.move);

		SetBullet(g_player.pos, D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI) * 20.0f, cosf(g_player.rot.z + D3DX_PI) * 20.0f, 0.0f),0,BULLETTYPE_PLAYER);
		PlaySound(SOUND_LABEL_SE01);
		//SetBullet(g_player.pos, D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI*0.75) * 5.0f, cosf(g_player.rot.z + D3DX_PI * 0.75) * 5.0f, 0.0f));
		//SetBullet(g_player.pos, D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI*-0.75) * 5.0f, cosf(g_player.rot.z + D3DX_PI * -0.75) * 5.0f, 0.0f));
	}



	//位置を更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	//移動量を更新(減衰させる)
	g_player.move.x += (0.0f - g_player.move.x) * 0.05f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.05f;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z + (0.0f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z + (0.0f - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + (0.0f + g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + (0.0f + g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//g_CounterAnimPlayer++;							//アニメーションカウンターを加算

	//if ((g_CounterAnimPlayer%SPEED)==0)
	//{//一定時間経過

	//	//アニメーションパターンNo.を更新
	//	g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % FLAME;

	//		//テクスチャ座標の設定
	//		pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimPlayer * 0.2f, (g_nPatternAnimPlayer/5)*0.5);
	//		pVtx[1].tex = D3DXVECTOR2(g_Direction + g_nPatternAnimPlayer * 0.2f,(g_nPatternAnimPlayer / 5)*0.5);
	//		pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimPlayer * 0.2f, 0.5+(g_nPatternAnimPlayer / 5)*0.5);
	//		pVtx[3].tex = D3DXVECTOR2(g_Direction + g_nPatternAnimPlayer * 0.2f,0.5+(g_nPatternAnimPlayer / 5)*0.5);

	//	if (g_nPatternAnimPlayer>=10)
	//	{
	//		g_nPatternAnimPlayer = 0;				//パターンNo.を初期値に戻す
	//	}
	//}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

}

//プレイヤーの描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;						//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_player.bDisp == true)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}

//プレイヤーの取得
Player* GetPlayer(void)
{
	return &g_player;
}

//プレイヤーのヒット処理
void HitPlayer(int nDamage)
{
	g_player.nLife -= nDamage;
	if (g_player.nLife <= 0)
	{
		//爆発の設定(後で打ち込む)
		g_player.bDisp = false;
		g_player.state = PLAYERSTATE_DEATH;
		g_player.nCounterState = 60;
	}
	else
	{
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 5;
		//頂点カラーの設定(後で打ち込む)
	}
}