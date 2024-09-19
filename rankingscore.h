#ifndef _RANKINGSCORE_H_
#define _RANKINGSCORE_H_

#include "main.h"

//マクロ定義
#define WIDTHRANKINGSCORE (60.0f)
#define HEIGHTRANKINGSCORE (100.0f)

//プロトタイプ宣言
void InitRankingScore(void);//スコアの初期化
void UninitRankingScore(void);//スコアの終了
void UpdateRankingScore(void);//スコアの更新
void DrawRankingScore(void);//スコアの描画
void SetRankingScore(int nScore);//スコアの設定
void ResetRanking(void);//ランキング情報の初期化
void WriteScore(void);//スコアの書き込み
void SetRankingScoreTexture(void);//テクスチャの処理


#endif