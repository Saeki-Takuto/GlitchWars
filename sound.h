//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 高宮祐翔
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM01 = 0,
	SOUND_LABEL_BGM02,
	SOUND_LABEL_BGM03,
	SOUND_LABEL_BGM04,
	SOUND_LABEL_BGM05,
	SOUND_LABEL_BGM06,
	SOUND_LABEL_SE01,
	SOUND_LABEL_SE02,
	SOUND_LABEL_SE03,
	SOUND_LABEL_SE04,
	SOUND_LABEL_SE05,
	SOUND_LABEL_SE06,
	SOUND_LABEL_SE07,
	SOUND_LABEL_SE08,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
