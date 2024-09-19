//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _MAIN_H_					//このマクロ定義がされてなかったら
#define _MAIN_H_					//2重インクルード防止のマクロ定義
#include <windows.h>
#include "d3dx9.h"
#include "stdio.h"
#define DIRECTINPUT_VERSION (0x0800)//ビルドの警告対処用マクロ
#include "dinput.h"					//入力処理に必要
#include "Xinput.h"//ジョイパッド処理に必要
#include "xaudio2.h"//サウンド処理に必要



//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")											//入力処理に必要
#pragma comment(lib,"xinput.lib")											//ジョイパッド処理に必要

//マクロ定義
#define CLASS_NAME "WindowClass"											//ウィンドウクラスの名前
#define WINDOW_NAME "Glitch Wars"		//ウィンドウの名前(キャプションに表示)
#define SCREEN_WIDTH (1600)													//ウィンドウの幅
#define SCREEN_HEIGHT (900)													//ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

typedef enum
{
	MODE_TITLE=0,//タイトル画面
	MODE_GAME,//ゲーム画面
	MODE_RESULT,//リザルト画面
	MODE_BSOD,
	MODE_RANKING,
	MODE_MAX
}MODE;

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;				//頂点座標
	float rhw;						//座標変換用係数(1.0fで固定)
	D3DCOLOR col;					//頂点カラー
	D3DXVECTOR2 tex;				//テクスチャ座標
}VERTEX_2D;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void SetMode(MODE mode);
MODE GetMode(void);
void DrawDEBUG(float Playerposx,float Playerposy,float Playerposz);

#endif

