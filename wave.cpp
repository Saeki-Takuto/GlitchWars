#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "wave.h"
#include "enemy.h"
int nCntTimeWave;

void InitWave(void)
{
	nCntTimeWave=0;

}

void UninitWave(void)
{

}

void UpdateWave(void)
{
	int nNum;
	Enemy* pEnemy = GetEnemy();
	int nPosX, nPosY;
	int nEneKAZU,nEneTAIRYOKU;

	nNum = GetNumEnemy();

	//if (nNum <= 0)//�G���S�����񂾏ꍇ
	//{
		nCntTimeWave++;

		if (nCntTimeWave >=10)
		{
			nCntTimeWave = 0;


			nPosX = rand() % SCREEN_WIDTH + 1;
			nPosY = rand() % SCREEN_HEIGHT + 1;

			if (nPosX >= SCREEN_WIDTH-75)
			{
				nPosX -= 75;
			}
			else if (nPosX <= 75)
			{
				nPosX += 75;
			}

			if (nPosY >= SCREEN_HEIGHT-75)
			{
				nPosY -= 75;
			}
			else if (nPosY <= 75)
			{
				nPosY += 75;
			}

			SetEnemy(D3DXVECTOR3(nPosX, nPosY, 0.0f),0,100);
			PlaySound(SOUND_LABEL_SE02);


		}

	//}


}