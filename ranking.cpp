#include "ranking.h"
#include "rankingscore.h"
#include "rankset.h"
#include "score.h"
#include "fade.h"
#include "sound.h"
int nCntTimeRanking = NULL;

//ランキングの初期化処理
void InitRanking(void)
{
	InitRankSet();//順位の初期化

	InitRankingScore();//ランキングのスコアの初期化

	ResetRanking();//ランキングのリセット

	SetRankingScoreTexture();//ランキングのテクスチャ処理

	//サウンドの設定
	PlaySound(SOUND_LABEL_BGM02);

	nCntTimeRanking = NULL;
}

//ランキングの終了処理
void UninitRanking(void)
{
	UninitRankSet();//順位の終了
	
	UninitRankingScore();//ランキングのスコアの初期化

	StopSound();
}

//ランキングの更新処理
void UpdateRanking(void)
{
	UpdateRankSet();//順位の更新

	UpdateRankingScore();//ランキングのスコアの更新

	nCntTimeRanking++;

	if (nCntTimeRanking >= (60 * 10))
	{
		nCntTimeRanking = 0;
		SetFade(MODE_TITLE);
	}

}

//ランキングの描画処理
void DrawRanking(void)
{
	DrawRankSet();//順位の描画

	DrawRankingScore();//ランキングのスコアの更新
}