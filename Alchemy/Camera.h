#pragma once
#include <vector2.h>
#include "object/Obj.h"

#define CAMERA_MOVE_CNT 60

#define lpCamera Camera::GetInstance()

class Camera
{
public:
	static Camera& GetInstance(void)
	{
		return *sInstance;
	}
	Vector2Dbl pos(void);
	Vector2Dbl size(void);
	Vector2Dbl exMovePos(void);
	void exMoveFlag(bool);
	bool exMoveFlag(void);
	void UpDate(void);
	void ReSetList(void);
	void SearchObj(std::vector<sharedObj> List);
	void SetMoveData(Vector2Dbl aimPos);		// �J�����̓���ړ��̎��Ɏg���f�[�^���Z�b�g����
	bool MoveData();							// �J�����̓���ړ����֐�����������
	void Draw(int drawnLayer);					// ����������C���[���v���C���[�ɒ��ڂ��ĕ`��
	bool isPlay(void);							// �Q�[���V�[����
	void FadeInPos(Vector2Dbl pos);				// ���
	void gameScene(bool isPlay);
private:
	Vector2Dbl _pos;
	Vector2Dbl _size;
	Vector2Dbl _unitVel;	// ����ړ�����Ƃ��̑��x�̒P�ʃx�N�g��
	int _cntSum;			// CAMERA_MOVE_CNT�̑��a
	double _length;			// �ړ�����
	double _a;				// �����̕ω��֐�v = a * (x - CAMERA_MOVE_CNT / 2) + b�@��a
	double _b;				// �����̕ω��֐�v = a * (x - CAMERA_MOVE_CNT / 2) + b�@��b
	bool _exMoveFlag;		// ����ړ����׸�
	bool _gameScene;		// �Q�[���V�[����
	Vector2Dbl _exMovePos;	// ����ړ�����Ƃ��Ɉړ����钼�O�̍��W���i�[���Ă���
	Vector2Dbl _fadeInPos;	// ̪��ރC�����̍��W
	int _moveCnt;
	std::vector<int> _cameraScreen;
	std::vector<std::weak_ptr<Obj>> _plObjList;
	struct CameraDeleter
	{
		void operator()(Camera *camera)
		{
			delete camera;
		}
	};
	static std::unique_ptr<Camera, CameraDeleter> sInstance;
	Camera();
	~Camera();
};

