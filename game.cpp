#include "player.h"
#include "back.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "game.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "effect.h"

//�O���[�o���ϐ��錾
GAMESTATE g_gameState = GAMESTATE_NONE;//�Q�[���̏��
int g_nCounterGameState=0;//��ԊǗ��J�E���^�[

//�Q�[����ʂ̏���������
void InitGame(void)
{
	//�w�i�̏���������
	InitBack();

	//�e�̏���������
	InitBullet();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();
	SetEnemy(D3DXVECTOR3(200.0f, 200.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(600.0f, 200.0f, 0.0f), 0);
	SetEnemy(D3DXVECTOR3(900.0f, 200.0f, 0.0f), 0);

	//�����̏���������
	InitExplosion();

	//�X�R�A�̏���������
	InitScore();

	g_gameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

	PlaySound(SOUND_LABEL_BGM03);
}

//�Q�[����ʂ̏I������
void UninitGame(void)
{
	//�����̏I������
	UninitExplosion();

	//�G�̏I������
	UninitEnemy();

	//�v���C���[�̏I������
	UninitPlayer();

	//�e�̏I������
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�w�i�̏I������
	UninitBack();

	//�X�R�A�̏I������
	UninitScore();

	StopSound();
}

//�Q�[����ʂ̍X�V����
void UpdateGame(void)
{
	int nNum;

	//�w�i�̍X�V����
	UpdateBack();

	//�e�̍X�V����
	UpdateBullet();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�G�̍X�V����
	UpdateEnemy();

	//�����̍X�V����
	UpdateExplosion();

	//�X�R�A�̍X�V����
	UpdateScore();

	//�v���C���[�̎擾
	Player* pPlayer = GetPlayer();
	Enemy* pEnemy = GetEnemy();

	nNum = GetNumEnemy();

	if (nNum <= 0)//�G���S�����񂾏ꍇ
	{
		g_gameState = GAMESTATE_END;
	}
	else if (pPlayer->bUse == false)
	{
		g_gameState = GAMESTATE_END;
	}

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;
	case GAMESTATE_END://�I�����
   		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			//���[�h�ݒ�(���U���g��ʂɈڍs)
			SetFade(MODE_RESULT);
			g_nCounterGameState = 0;
			g_gameState = GAMESTATE_NONE;//�������Ă��Ȃ���Ԃɐݒ�

		}
		break;
	}


}

//�Q�[����ʂ̕`�揈��
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBack();//back

	//�e�̕`�揈��
	DrawBullet();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�����̕`�揈��
	DrawExplosion();

	//�X�R�A�̕`�揈��
	DrawScore();
}

//�Q�[���̏�Ԃ̐ݒ�
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//�Q�[���̏�Ԃ̎擾
GAMESTATE GetGameState(void)
{
	return g_gameState;
}