//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256)								//キーの最大数

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;							//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;				//入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];							//キーボードのプレス情報
BYTE g_aOldState[NUM_KEY_MAX];							//キーボードのプレス情報

XINPUT_STATE g_joykeyState;//ジョイパッドのプレス情報
XINPUT_STATE g_joykeyOldState;


//キーボードの初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードのへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//キーボードの終了処理
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();					//キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard=NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//キーボードの更新処理
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];						//キーボードの入力情報
	int nCntKey;

	for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		g_aOldState[nCntKey] = g_aKeyState[nCntKey];//ひとつ前のキーボードプレス情報を保存
	}

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();						//キーボードへのアクセス権を獲得
	}
}

//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//キーを押した瞬間の処理
bool KeyboardTrigger(int nKey)
{
	bool Trigger = false;
	if (g_aKeyState[nKey] & 0x80 && !(g_aOldState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//キーを話した瞬間の処理
bool KeyboardRelease(int nKey)
{
	bool Trigger = false;
	if (g_aOldState[nKey] & 0x80 && !(g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//キーが押されている間の処理
bool KeyboardRepeat(int nKey)
{
	bool Trigger = false;
	if (g_aOldState[nKey] & 0x80 && (g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//ジョイパッドの初期化処理
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joykeyState, 0, sizeof(XINPUT_STATE));
	
	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//ジョイパッドの終了処理
void UninitJoypad(void)
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//ジョイパッドの更新処理
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;//ジョイパッドの入力機能
	int nCntJoy;

	for (nCntJoy = 0; nCntJoy < JOYKEY_MAX; nCntJoy++)
	{
		g_joykeyOldState = g_joykeyState;//ひとつ前のキーボードプレス情報を保存
	}

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joykeyState = joykeyState;//ジョイパッドのプレス情報を保存
	}
}

//ジョイパッドのプレス情報を保存
bool GetJoypadPress(JOYKEY key)
{
	return (g_joykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//ジョイパッドを押した瞬間の処理
bool JoypadTrigger(JOYKEY key)
{
	bool TriggerPad = false;
	if (g_joykeyState.Gamepad.wButtons & (0x01 << key) && !(g_joykeyOldState.Gamepad.wButtons&(0x01<<key)))
	{
		TriggerPad = true;
	}
	return TriggerPad;
}

//ジョイパッドを離した瞬間の処理
bool JoypadRelease(JOYKEY key)
{
	bool TriggerPad = false;
	if (g_joykeyOldState.Gamepad.wButtons & (0x01 << key) && !(g_joykeyState.Gamepad.wButtons & (0x01 << key)))
	{
		TriggerPad = true;
	}
	return TriggerPad;
}

//ジョイパッドが押されている間の処理
bool JoypadRepeat(JOYKEY key)
{
	bool TriggerPad = false;
	if (g_joykeyOldState.Gamepad.wButtons & (0x01 << key) && (g_joykeyState.Gamepad.wButtons & (0x01 << key)))
	{
		TriggerPad = true;
	}
	return TriggerPad;
}

bool JoypadStickLX0(void)
{
	return (g_joykeyState.Gamepad.sThumbLX >=10000);
}

bool JoypadStickLX1(void)
{
	return (g_joykeyState.Gamepad.sThumbLX <= -10000);
}

bool JoypadStickLY0(void)
{
	return (g_joykeyState.Gamepad.sThumbLY >= 10000);
}

bool JoypadStickLY1(void)
{
	return (g_joykeyState.Gamepad.sThumbLY <= -10000);
}
