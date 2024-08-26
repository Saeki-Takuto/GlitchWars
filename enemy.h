//==================================================================
//
//�L�����N�^�[���L�[���͂ő���ł���悤�ɂ��悤
//Author:Saeki Takuto
//
//==================================================================

#ifndef _ENEMY_H_//���̃}�N����`������ĂȂ�������
#define _ENEMY_H_//2�d�C���N���[�h�h�~�̃}�N����`
#include "main.h"

//�}�N����`
#define MAX_ENEMY (128)
#define WIDTHENEMY (300)
#define HEIGHTENEMY (300)

typedef enum
{
	ENEMYSTATE_NORMAL = 0,//�ʏ���
	ENEMYSTATE_DAMAGE,//�_���[�W���
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nType;
	bool bUse;
	int nCntBull;
	ENEMYSTATE state;
	int nCounterState;
	int nLife;
	LPDIRECT3DTEXTURE9 pTexture;
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
Enemy* GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);
int GetNumEnemy();

#endif
