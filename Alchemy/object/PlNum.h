#pragma once

enum class PlNum
{
	Pl_00 = -1, 
	PL_01 = 0, 
	PL_02,
	PL_03,
	PL_04,
	PL_MAX,
	PL_NON						// �޽đ����ڑ����ݒ�p
};

PlNum begin(PlNum);
PlNum end(PlNum);
PlNum operator*(PlNum num);
PlNum operator++(PlNum& num);
PlNum operator--(PlNum& num);
