#include <DxLib.h>
#include "PlayerDeath.h"
#include <scene/SceneMng.h>

void PlayerDeath::operator()(Obj & player, std::vector<sharedObj>& objList)
{
	// 影も描写
		// 高さによって大きさを変えたりするかも
	lpSceneMng.AddDrawQue({ player._shadowImg,
		player._pos.x,
		player._pos.y + player._size.y / 2.0,
		0.0,
		player._extRate - (player._height / 5000.0),
		0.0,
		INT_MIN,
		LAYER::CHAR ,
		DX_BLENDMODE_NOBLEND,
		255 });
	
}
