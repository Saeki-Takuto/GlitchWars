#ifndef _BSOD_H_
#define _BSOD_H_

#include "main.h"

typedef enum
{
	BSOD_CLEAR = 0,//�^�C�g�����
	BSOD_GAMEOVER,//�Q�[�����
	BSOD_MAX
}BSOD;

//�v���g�^�C�v�錾
void InitBsod(void);
void UninitBsod(void);
void UpdateBsod(void);
void DrawBsod(void);

#endif

