#include "BaseScene.h"



BaseScene::BaseScene()
{
	_screenPos = { 0.0,0.0};
}

BaseScene::~BaseScene()
{
}

void BaseScene::RunActQue(std::vector<ActQueT> actList)
{
	// ここに入ったら消す
	actList.clear();
}

void BaseScene::RunInstanceQue(std::vector<InstanceQueT> instanceList)
{
	// 何もしない
	instanceList.clear();
}

