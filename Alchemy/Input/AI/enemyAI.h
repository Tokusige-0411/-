#pragma once
#include "../InputState.h"
#include "../AI_Ctl.h"

class enemyAI :
	public InputState
{
public:
	enemyAI(Obj& obj);
	~enemyAI();
	void Update(std::vector<sharedObj>& objList) override;			// ����

private:
	Obj& _masterObj;						// �Ă΂ꂽ��
	AI_Ctl _aiCtl{_masterObj,*this};
};

