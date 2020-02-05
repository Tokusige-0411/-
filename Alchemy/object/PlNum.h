#pragma once

enum class PlNum
{
	Pl_00 = -1, 
	PL_01 = 0, 
	PL_02,
	PL_03,
	PL_04,
	PL_MAX,
	PL_NON						// ｹﾞｽﾄ側未接続時設定用
};

PlNum begin(PlNum);
PlNum end(PlNum);
PlNum operator*(PlNum num);
PlNum operator++(PlNum& num);
PlNum operator--(PlNum& num);
