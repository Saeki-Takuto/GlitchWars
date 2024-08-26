//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _INPUT_H_	//このマクロ定義がされてなかったら
#define _INPUT_H_	//2重インクルード防止のマクロ定義

#include "main.h"

//キーの種類
typedef enum
{
	JOYKEY_UP = 0,//十字キー(上)
	JOYKEY_DOWN,//十字キー(下)
	JOYKEY_LEFT,//十字キー(左)
	JOYKEY_RIGHT,
	JOYKEY_START,
	JOYKEY_BACK,
	JOYKEY_L3,
	JOYKEY_R3,
	JOYKEY_L1,
	JOYKEY_R1,
	JOYKEY_L2,
	JOYKEY_R2,
	JOYKEY_A,
	JOYKEY_B,
	JOYKEY_X,
	JOYKEY_Y,
	JOYKEY_MAX
}JOYKEY;

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool KeyboardTrigger(int nKey);
bool KeyboardRelease(int nKey);
bool KeyboardRepeat(int nKey);



HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool JoypadTrigger(JOYKEY key);
bool JoypadRelease(JOYKEY key);
bool JoypadRepeat(JOYKEY key);
bool JoypadStickLX0(void);
bool JoypadStickLX1(void);
bool JoypadStickLY0(void);
bool JoypadStickLY1(void);
bool JoypadStickRX0(void);
bool JoypadStickRX1(void);
bool JoypadStickRY0(void);
bool JoypadStickRY1(void);


#endif