#pragma once
#include <memory>
#include <vector>
#include <map>
#include <Vector2.h>
#include <object/Obj.h>
#include <EFFECT.h>
#include "../scene/SceneMng.h"
#include "../scene/LAYER.h"

#define lpEffectMng EffectMng::GetInstance()
#define EFFECT_ID(KEY) EffectMng::GetInstance().GetID(KEY)

enum class EFFECT_QUE
{
	OBJ,			// 自分が再生しているｴﾌｪｸﾄを保存したい
	POS,
	RAD,			// 角度
	PLAY_ID,
	ZORDER			// 描画優先度
};

using EffectQueT = std::tuple <Obj&, Vector2Dbl, double, EFFECT, int>;			// <ｴﾌｪｸﾄID、座標、角度、再生したいｴﾌｪｸﾄ、描画優先度>

class EffectMng
{
public:
	static EffectMng& GetInstance(void)
	{
		return *sInstance;
	}

	const int& GetID(EFFECT id);												// 取得
	const int& GetID(EFFECT id, const std::string& key, float magnification);	// 登録
	bool AddEffectQue(EffectQueT eQue);											// ｴﾌｪｸﾄｷｭｰの追加
	void Draw(void);															// 描画
	void Init(void);
	void Clear(void);

	const Vector2Dbl _screenSize;												// ｴﾌｪｸﾄのｽｸﾘｰﾝｻｲｽﾞ
private:
	// ｶｽﾀﾑﾃﾞﾘｰﾀｰ
	struct EffectMngDeleter
	{
		void operator ()(EffectMng* EffectMng)
		{
			delete EffectMng;
		}
	};

	static std::unique_ptr<EffectMng, EffectMngDeleter> sInstance;

	std::map<EFFECT,int> _effectMap;						// 登録用
	std::vector<EffectQueT> _effectList;					// ｴﾌｪｸﾄｷｭｰの管理配列
	EffectMng();
	~EffectMng();
};

