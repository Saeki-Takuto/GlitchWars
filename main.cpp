//==================================================================
//
//GlitchWars
//Author:Saeki Takuto
//
//==================================================================

#include "main.h"
#include "player.h"
#include "back.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "Bsod.h"
#include "tutorial.h"

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;				//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;//現在のモード
LPD3DXFONT g_pFont = NULL;//フォントへのポインタ
int g_nCountFPS = 0;//FPSカウンタ

//================================================
//メイン関数
//================================================
int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	DWORD dwCurrentTime;							//現在時刻
	DWORD dwExecLastTime;							//最後に処理した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,									//ウィンドウのスタイル
		WindowProc,									//ウィンドウプロシージャ
		0,											//0にする(通常は使用しない)
		0,											//0にする(通常は使用しない)
		hInstance,									//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),				//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),					//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),					//クライアント領域の背景色
		NULL,										//メニューバー
		CLASS_NAME,									//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)				//ファイルのアイコン
	};

	HWND hWnd;//ウィンドウハンドル(識別子)
	MSG msg;//メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//画面サイズの構造体

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,						//拡張ウィンドウスタイル
						  CLASS_NAME,				//ウィンドウクラスの名前
						  WINDOW_NAME,				//ウィンドウの名前
						  WS_OVERLAPPEDWINDOW,		//ウィンドウスタイル
					      CW_USEDEFAULT,			//ウィンドウの左上X座標
						  CW_USEDEFAULT,			//ウィンドウの左上Y座標
						  (rect.right-rect.top),	//ウィンドウの幅
						  (rect.bottom-rect.top),	//ウィンドウの高さ
						  NULL,						//親ウィンドウのハンドル
						  NULL,						//メニューハンドルまたは小ウィンドウID
						  hInstance,				//インスタンスハンドル
						  NULL);					//ウィンドウ作成データ

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return-1;
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;								//初期化する
	dwExecLastTime = timeGetTime();					//現在時刻を取得(保存)

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);						//ウィンドウの表示状態を設定
	UpdateWindow(hWnd);								//クライアント領域を更新

	DWORD dwFrameCount;//フレームカウント
	DWORD dwFPSLastTime;//最後にFPSを計測した時刻

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);				//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);				//ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;//FPSを測定した時刻を保存
				dwFrameCount = 0;//フレームカウントをクリア

			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存
				dwFrameCount++;//フレームカウントを加算
				//更新処理
				Update();

				//描画処理
				Draw();
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:								//ウィンドウ破棄のメッセージ
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:								//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:								//[Esc]キーが押された
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);//規定の処理を返す
}

//初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;							//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;					//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;			//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			//バックバッファの形式
	d3dpp.BackBufferCount = 1;						//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;			//デブスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		//デブスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{//Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 32, 0, 0, 0,
				   FALSE, SHIFTJIS_CHARSET,
				   OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,
				   "Terminal",&g_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	//サウンドの初期化処理
	InitSound(hWnd);
	
	//モードの設定
	SetMode(g_mode);

	//フェードの設定
	InitFade(g_mode);

	return S_OK;
}

//終了処理
void Uninit(void)
{
	//フェードの終了処理
	UninitFade();
	
	//タイトル画面の終了処理
	UninitTitle();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();

	//ランキングの終了処理
	UninitRanking();

	//サウンドの終了処理
	UninitSound();

	//キーボードの終了処理
	UninitKeyboard();

	//ジョイパッドの終了処理
	UninitJoypad();

	//デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//更新処理
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッドの更新処理
	UpdateJoypad();

	switch (g_mode)
	{
	case MODE_TITLE://タイトル画面
		UpdateTitle();
		break;
	case MODE_GAME://ゲーム画面
		UpdateGame();
		break;
	case MODE_RESULT://リザルト画面
		UpdateResult();
		break;
	case MODE_RANKING://ランキング
		UpdateRanking();
		break;
	case MODE_BSOD://ブルースクリーン
		UpdateBsod();
		break;
	case MODE_TUTORIAL://チュートリアル
		UpdateTutorial();
		break;
	}

	//フェードの更新処理
	UpdateFade();

}

//描画処理
void Draw(void)
{
	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		switch (g_mode)
		{
		case MODE_TITLE://タイトル画面
			DrawTitle();
			break;
		case MODE_GAME://ゲーム画面
			DrawGame();
			break;
		case MODE_RESULT://リザルト画面
			DrawResult();
			break;
		case MODE_RANKING://ランキング画面
			DrawRanking();
			break;
		case MODE_BSOD://ブルースクリーン
			DrawBsod();
			break;
		case MODE_TUTORIAL://チュートリアル
			DrawTutorial();
			break;
		}

		//フェードの描画処理
		DrawFade();

#ifdef _DEBUG

		Player* pPlayer;

		int nCntPlayer;

		pPlayer = GetPlayer();

#endif

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//モードの設定
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_TITLE://タイトル画面
		UninitTitle();
		break;
	case MODE_GAME://ゲーム画面
		UninitGame();
		break;
	case MODE_RESULT://リザルト画面
		UninitResult();
		break;
	case MODE_RANKING://ランキング画面
		UninitRanking();
		break;
	case MODE_BSOD://ブルースクリーン
		UninitBsod();
		break;
	case MODE_TUTORIAL://チュートリアル
		UninitTutorial();
		break;
	}

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE://タイトル画面
		InitTitle();
		break;
	case MODE_GAME://ゲーム画面
		InitGame();
		break;
	case MODE_RESULT://リザルト画面
		InitResult();
		break;
	case MODE_RANKING://ランキング画面
		InitRanking();
		break;
	case MODE_BSOD://ブルースクリーン
		InitBsod();
		break;
	case MODE_TUTORIAL://チュートリアル
		InitTutorial();
		break;
	}

	g_mode = mode;//現在の画面(モード)を切り替える
}

//モードの取得
MODE GetMode(void)
{
	return g_mode;
}

//デバッグの表示
void DrawDEBUG(float Playerposx, float Playerposy, float Playerposz)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "FPS:%d\n",g_nCountFPS,Playerposx,Playerposy,Playerposz);

	//テキストの描画
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}