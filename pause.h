//==================================================================
//
//キャラクターをキー入力で操作できるようにしよう
//Author:Saeki Takuto
//
//==================================================================

#ifndef _PAUSE_H_	//このマクロ定義がされてなかったら
#define _PAUSE_H_	//2重インクルード防止のマクロ定義

//ポーズメニュー
typedef enum
{
	PAUSE_MENU_CONTINUE=0,//ゲームに戻る
	PAUSE_MENU_RETRY,//ゲームをやり直す
	PAUSE_MENU_QUIT,//タイトル画面に戻る
	PAUSE_MENU_MAX
}PAUSE_MENU;

//typedef struct
//{
//	D3DXVECTOR3 pos;
//	D3DXVECTOR3 col;
//};
//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif
