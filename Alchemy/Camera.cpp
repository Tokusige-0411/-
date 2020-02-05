#include "Camera.h"
#include "scene/SceneMng.h"
#include "NetWork/NetWork.h"
#include "object/Player.h"
#include "UI_Mng.h"

std::unique_ptr<Camera, Camera::CameraDeleter> Camera::sInstance(new Camera());

Camera::Camera()
{
	_gameScene = false;
}

Camera::~Camera()
{
}

Vector2Dbl Camera::pos(void)
{
	return _pos;
}

Vector2Dbl Camera::size(void)
{
	return _size;
}

Vector2Dbl Camera::exMovePos(void)
{
	return _exMovePos;
}

void Camera::exMoveFlag(bool exMove)
{
	_exMoveFlag = exMove;
}

bool Camera::exMoveFlag(void)
{
	return _exMoveFlag;
}



void Camera::UpDate(void)
{
	if (_exMoveFlag)
	{
		/*if (_moveCnt <= CAMERA_MOVE_CNT)
		{
			_pos += _unitVel * (_b + _a * static_cast<double>((_moveCnt - CAMERA_MOVE_CNT / 2) * (_moveCnt - CAMERA_MOVE_CNT / 2)));
			_moveCnt++;
		}*/

		MoveData();
	}
	else
	{
		
		//_pos = (*_plObj.lock()).pos();
		//// ﾍﾞﾀｶﾞｷしたくない感満載
		//if (lpSceneMng.WorldSize.x < _pos.x + _size.x)
		//{
		//	_pos.x = lpSceneMng.WorldSize.x - _size.x;
		//}
		//if (0 > _pos.x - _size.x)
		//{
		//	_pos.x = _size.x;
		//}
		//if (lpSceneMng.WorldSize.y < _pos.y + _size.y)
		//{
		//	_pos.y = lpSceneMng.WorldSize.y - _size.y;
		//}
		//if (0 > _pos.y - _size.y)
		//{
		//	_pos.y = _size.y;
		//}

		//_exMovePos = _pos;
		_pos = { lpSceneMng.WorldCenter.x,lpSceneMng.WorldSize.y - lpSceneMng.ScreenCenter.y };
	}
}

void Camera::ReSetList(void)
{
	_plObjList.clear();
	_cameraScreen.clear();
}

void Camera::SearchObj(std::vector<sharedObj> List)
{
	for (auto obj : List)
	{
		if ((*obj).unitID() == UNIT_ID::PLAYER)
		{
			_plObjList.emplace_back(obj);

			_cameraScreen.emplace_back(MakeScreen(static_cast<int>(lpSceneMng.ScreenSize.x) / ((Player&)(*_plObjList[0].lock())).GetpCnt(), static_cast<int>(lpSceneMng.ScreenSize.y)));
			_size = { lpSceneMng.ScreenSize.x / static_cast<double>(((Player&)(*_plObjList[0].lock())).GetpCnt()) ,lpSceneMng.ScreenSize.y };
		}
	}

	_gameScene = true;
}

void Camera::SetMoveData(Vector2Dbl aimPos)
{
	_length = sqrt(LengthSquare(aimPos, _pos));

	_a = -6.0 * _length / static_cast<double>(CAMERA_MOVE_CNT * CAMERA_MOVE_CNT * CAMERA_MOVE_CNT);
	_b = -static_cast<double>(CAMERA_MOVE_CNT / 2 * CAMERA_MOVE_CNT / 2) * _a;
	
	_cntSum = 0;

	for (int i = 1; i < CAMERA_MOVE_CNT; i++)
	{
		_cntSum += i;
	}
	/*_b = length / 20.0;
	_a = -_b / static_cast<double>(CAMERA_MOVE_CNT /2  * CAMERA_MOVE_CNT / 2);*/
	_unitVel = (aimPos - _pos) / _length;
	_moveCnt = CAMERA_MOVE_CNT;
}

bool Camera::MoveData()
{
	if (_moveCnt > 0)
	{
		_moveCnt--;
		_pos += _unitVel * (_length * _moveCnt / _cntSum);
	}
	else
	{
		return true;
	}
	return false;
}

void Camera::Draw(int drawnLayer)
{
	if (_gameScene)
	{
		if (!_exMoveFlag)
		{
			// それぞれのプレイヤーの視点を描画
			for (int i = 0; i < _cameraScreen.size(); i++)
			{
				SetDrawScreen(_cameraScreen[i]);
				ClsDrawScreen();
				auto center = lpSceneMng.WorldCenter - (*_plObjList[i].lock()).pos() + _size / 2.0 + ((Player&)(*_plObjList[i].lock())).ShakeOffset();

				// 画面外補正
				if (center.x > lpSceneMng.WorldCenter.x)
				{
					center.x = lpSceneMng.WorldCenter.x;
				}
				else if (center.x < _size.x - lpSceneMng.WorldCenter.x)
				{
					center.x = _size.x - lpSceneMng.WorldCenter.x;
				}

				if (center.y > lpSceneMng.WorldCenter.y)
				{
					center.y = lpSceneMng.WorldCenter.y;
				}
				else if (center.y < _size.y - lpSceneMng.WorldCenter.y)
				{
					center.y = _size.y - lpSceneMng.WorldCenter.y;
				}
				DrawRotaGraph(center.x, center.y, 1.0, 0.0, drawnLayer, true);
			}

			for (int i = 0; i < _cameraScreen.size(); i++)
			{
				SetDrawScreen(DX_SCREEN_BACK);
				DrawGraph(i * static_cast<int>(_size.x), 0, _cameraScreen[i], true);
			}
		}
		else
		{
			if (!lpUI_Mng.FadeFlag())
			{
				auto pos = lpSceneMng.WorldCenter - _pos + lpSceneMng.ScreenSize / 2.0;
				// ゲームシーン以外はそのまま描画
				SetDrawScreen(DX_SCREEN_BACK);
				DrawRotaGraph(pos.x, pos.y, 1.0, 0, drawnLayer, true);
			}
			else
			{
				for (int i = 0; i < _cameraScreen.size(); i++)
				{
					SetDrawScreen(DX_SCREEN_BACK);
					DrawGraph(i * static_cast<int>(_size.x), 0, _cameraScreen[i], true);
				}
			}
		}

		lpUI_Mng.Draw();
	}
	else
	{
		// ゲームシーン以外はそのまま描画
		SetDrawScreen(DX_SCREEN_BACK);
		auto center = lpSceneMng.WorldCenter;
		DrawRotaGraph(static_cast<int>(center.x), static_cast<int>(center.y), 1.0, 0, drawnLayer, true);
	}
}

bool Camera::isPlay(void)
{
	return _gameScene;
}

void Camera::FadeInPos(Vector2Dbl pos)
{
	_fadeInPos = pos;
}

void Camera::gameScene(bool isPlay)
{
	_gameScene = isPlay;
}
