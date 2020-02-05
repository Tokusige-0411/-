#pragma once
#include "../InputState.h"
#include "../AI_Ctl.h"

class Slime :
	public InputState
{
public:
	Slime(Obj& obj);
	~Slime();
	void Update(std::vector<sharedObj>& objList) override;			// ����

private:
	Obj & _masterObj;
	AI_Ctl _aiCtl{ _masterObj,*this };
};

