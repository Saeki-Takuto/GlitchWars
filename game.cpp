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
#include "pause.h"
#include "particle.h"
#include "rankingscore.h"
#include "time.h"
#include "wave.h"
#include "memory.h"

//�O���[�o���ϐ��錾
GAMESTATE g_gameState = GAMESTATE_NONE;//�Q�[���̏��
int g_nCounterGameState=0;//��ԊǗ��J�E���^�[
bool g_bPause = false;//�|�[�Y�����ǂ���

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

	InitParticle();

	//�G�̏���������
	InitEnemy();

	//�����̏���������
	InitExplosion();

	//�X�R�A�̏���������
	InitScore();

	//�^�C���̏���������
	InitTime();

	InitMemory();

	//�E�F�[�u�̏���������
	InitWave();

	//�|�[�Y�̏���������
	InitPause();

	g_gameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;

	g_bPause = false;//�|�[�Y����

	PlaySound(SOUND_LABEL_BGM03);
}

//�Q�[����ʂ̏I������
void UninitGame(void)
{
	//�|�[�Y�̏I������
	UninitPause();

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

	//�^�C���̏I������
	UninitTime();

	UninitMemory();

	//�E�F�[�u�̏I������
	UninitWave();

	StopSound();
}

//�Q�[����ʂ̍X�V����
void UpdateGame(void)
{
	int nNum;
	int nNum2;
	int nNum3;
	if (KeyboardTrigger(DIK_P) == true)
	{//�|�[�Y�L�[(P)�������ꂽ
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//�|�[�Y��
		UpdatePause();
	}

	if (g_bPause == false)
	{//�|�[�Y���łȂ����
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

		UpdateParticle();

		////�p�[�e�B�N��
		//SetParticle(D3DXVECTOR3(400.0f,150.0f,0.0f),20);

		//�X�R�A�̍X�V����
		UpdateScore();

		//�^�C���̍X�V����
		UpdateTime();

		UpdateMemory();

		//�E�F�[�u�̍X�V����
		UpdateWave();


		Enemy* pEnemy = GetEnemy();
		Player* pPlayer = GetPlayer();

		nNum2 = GetTime();
		nNum = GetNumEnemy();
		nNum3 = GetMemory();

		if (nNum <= 0)//�G���S�����񂾏ꍇ
		{
		}
		else if (pPlayer->bUse == false)
		{
			g_gameState = GAMESTATE_GAMEOVER;
		}
		else if (nNum2 <= 0)//���Ԃ�0�ɂȂ����ꍇ
		{
			g_gameState = GAMESTATE_END;
		}
		else if (nNum3>=100)
		{
			g_gameState = GAMESTATE_GAMEOVER;
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

				ResetRanking();

				//�����L���O�̐ݒ�
				SetRankingScore(GetScore());

				g_nCounterGameState = 0;
				g_gameState = GAMESTATE_NONE;//�������Ă��Ȃ���Ԃɐݒ�

			}
			break;
		case GAMESTATE_GAMEOVER://�I�����
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				////���[�h�ݒ�(���U���g��ʂɈڍs)
				//SetFade(MODE_RESULT);

				SetMode(MODE_BSOD);

				ResetRanking();

				//�����L���O�̐ݒ�
				SetRankingScore(GetScore());

				g_nCounterGameState = 0;
				g_gameState = GAMESTATE_NONE;//�������Ă��Ȃ���Ԃɐݒ�

			}
			break;

		}


	}


}

//�Q�[����ʂ̕`�揈��
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBack();

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

	//�^�C���̕`�揈��
	DrawTime();

	DrawMemory();


	if (g_bPause == true)
	{
		DrawPause();
	}
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

//�|�[�Y�̗L�������ݒ�
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}