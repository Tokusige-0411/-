#pragma once

enum class EFFECT
{
	AT_BITE,			// 噛みつき
	HEAL,				// 回復
	AT_SLIME,			// ｽﾗｲﾑ
	GHOST_CHARGE,		// ﾁｬｰｼﾞ
	GHOST_SHOT,			// 弾表示用
	GHOST_BLAST,		// 爆発
	DEMON_CHARGE,		// ﾁｬｰｼﾞ
	DEMON_WAR,			// 警告
	INK,				// 墨攻撃
	FIRE,				// 炎攻撃
	AT_BEE,				// 蜂のこうげき
	SMOKE,				// ｷﾉｺの煙攻撃
	LANDING,			// 着地
	DARK,				// ﾎﾞｽ攻撃１
	BOSS_WAR,			// ﾎﾞｽﾁｬｰｼﾞ
	METEO,				// めてお
	FIRE_TWO,			
	DEATH,
	POT,				// ミニツボのこうげき
	BOHU,				// ぼふっ
	MAX
};

EFFECT begin(EFFECT);
EFFECT end(EFFECT);

EFFECT operator ++(EFFECT& effect);
EFFECT operator *(EFFECT effect);