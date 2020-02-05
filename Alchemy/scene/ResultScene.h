#pragma once
#include "BaseScene.h"
#include <object/Obj.h>
#include <Input/InputState.h>

class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();

	UniqueBase Update(UniqueBase own)override;

private:

	void Init(void);
	void DrawInit(void);

	int _gameOver;
	int _gameClear;

	int _cnt;								// ÌÚ°Ñ¶³İÄ

	std::shared_ptr<InputState> _input;		// “ü—ÍŠÇ—ƒNƒ‰ƒX
};

