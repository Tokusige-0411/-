#pragma once
#include <string>
#include <map>
#include <Vector2.h>

#define FONT_DIV_CNT_Y 18
#define FONT_DIV_CNT_X 16

#define FONT_SIZE_X 32
#define FONT_SIZE_Y 32

enum class TEXT_ICON
{
	PL1,
	PL2,
	PL3,
	PL4,
	OCT,
	BEE,
	GHOSTH,
	MUSH,
	DEMON,
	POT_JR,
	POT,
	A,
	B,
	X,
	Y,
};

class Text
{
public:
	Text();
	~Text();

	// 指定の文字列を自分のスクリーンに描画する
	// 全角文字のみ
	void MakeText(const std::string&);

	// 指定の文字列を自分のスクリーンに描画する
	// 全角文字のみ
	// string:描画したい文字,space:行間
	void MakeText(const std::string&, int space);
	void AddIcon(TEXT_ICON icon, Vector2Int pos, double exRate);
	bool isDraw(void);
	void LoadFontImg(void);
	int screen(void);
	Vector2Int& size(void);
private:
	int _screen;	// 文字描画用のスクリーン
	Vector2Int _size;		// スクリーンのサイズ
	bool _isDraw;	// 描画されているか
	static int _fontImg[FONT_DIV_CNT_Y][FONT_DIV_CNT_X];
	static std::map<TEXT_ICON, int> _iconMap;		// icon画像格納マップ
};

