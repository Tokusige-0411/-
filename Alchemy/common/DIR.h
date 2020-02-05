#pragma once

#define PI 3.1415926
#define DEG(rad) (static_cast<int>(rad * 180.0 / PI))
#define RAD(deg) (static_cast<double>(deg) * PI / 180.0)

enum class DIR
{
	LEFT,
	UP,
	RIGHT,
	DOWN,
	MAX
};

DIR begin(DIR);
DIR end(DIR);

DIR operator++(DIR& dir);
DIR operator*(DIR dir);

DIR convToDir(double rad);		// ���W�A����Ή�����DIR�̒l�ɕϊ�
DIR convToDir(int angle);		// �x����Ή�����DIR�̒l�ɕϊ�

double convToRad(DIR dir);		// DIR�����W�A���ɕύX
int convToAngle(DIR dir);

