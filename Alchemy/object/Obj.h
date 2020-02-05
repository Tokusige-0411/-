#pragma once
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <Vector2.h>
#include <ImageMng.h>
#include <SoundMng.h>
#include <DIR.h>
#include "UNIT_ID.h"
#include "STATE.h"

/* ----------�I�u�W�F�N�g���N���X--------- */


// �w�c
enum class TEAM_TAG
{
	NON,
	ALLY_TEAM,		// ����
	ENEMY_TEAM		// �G
};

class Obj;
class InputState;
class Camera;
class NetWork;
class NetState;
class EffectMng;
struct PlayerNormal;
struct PlayerHold;
struct PlayerThrow;
struct PlayerDeath;
struct PotNormal;
struct PotHold;
struct PotThrow;

struct FuncHold;
struct FuncThrow;
struct FuncIntoPot;
struct FuncPut;
struct ThrownUpdate;
struct HoldenUpdate;
struct TestEnemyUpdate;

struct HeadPot;
struct HealTagDec;
struct LookForEnemy;

struct BatAttack;
struct MushroomAttack;
struct Slime_Attack;
struct GhostAttack;
struct DemonAttack;
struct OctpusAttack;
struct BeeAttack;
struct BossAttack;
struct PotAttack;

struct SlimeInit;
struct BatInit;
struct MushroomInit;
struct GhostInit;
struct DemonInit;

struct ItemNormal;

struct OctpusInit;
struct BeeInit;
struct FollowMushInit;
struct FollowGhostInit;
struct FollowDemonInit;
struct FollowPotInit;

using AnimVector = std::vector<std::pair<int, unsigned int>>;
using sharedObj = std::shared_ptr<Obj>;
using AnimKey = std::pair<STATE, DIR>;
using funcObj = std::function<void(Obj&,std::vector<sharedObj>& objList)>;

class Obj
{
public:
	Obj();
	virtual void Update(std::vector<sharedObj>& objList) = 0;
	void Draw(void);		// �A�j���[�V�����̕`��	GameScene�ȊO
	void Draw(int id);		// �D���ȉ摜���w�肵�ĕ`��
	virtual ~Obj();

	// get,set�֐� : �ϐ�����_���O�����̂��֐���
	STATE state(void)const;							
	bool state(STATE newState);

	DIR dir(void)const;
	bool dir(DIR newDir);
	
	bool alive(void) { return _alive; }
	bool alive(bool alive);
	bool dead(void) { return _dead; }
	bool dead(bool dead);

	const Vector2Dbl& pos(void);
	const Vector2Int& size(void);
	const double& rad(void);
	
	virtual void exFlag(bool isEX);
	virtual int getType(void);
	void glowFlag(bool isGlow);

	const int weight(void);
	const UNIT_ID& unitID(void);
	const TEAM_TAG& teamTag(void);

	std::weak_ptr<Obj>& tageObj(void);
	void SetTageObj(sharedObj obj);

	const double& searchRange(void);
	const double& attackRange(void);

	virtual void DoDamage(int power);		// �U�������HP������A�����͍U����
	const int& hp(void);
	const int& hpMax(void);

	bool isAnimEnd(void);								// �A�j���[�V�������I�����Ă��邩
	bool SetAnim(const AnimKey key,AnimVector& data);	// �A�j���[�V�����̃f�[�^��o�^
	void LoadStaticImg(void);								// HP�o�[�̓ǂݍ���

	const bool resultFlg(void);
	void resultFlg(bool flag);
	void SetresultFlg(bool id);

private:
	friend EffectMng;
	friend NetWork;
	friend NetState;
	friend PlayerNormal;
	friend PlayerHold;
	friend PlayerThrow;
	friend PlayerDeath;
	friend PotNormal;
	friend PotHold;
	friend PotThrow;
	friend FuncHold;
	friend FuncThrow;
	friend FuncIntoPot;
	friend FuncPut;
	friend ThrownUpdate;
	friend HoldenUpdate;
	friend TestEnemyUpdate;

	// AI
	friend HeadPot;
	friend HealTagDec;
	friend LookForEnemy;

	// �U��
	friend BatAttack;
	friend MushroomAttack;
	friend Slime_Attack;
	friend GhostAttack;
	friend DemonAttack;
	friend OctpusAttack;
	friend BeeAttack;
	friend BossAttack;
	friend PotAttack;

	// Enemy��init
	friend SlimeInit;
	friend BatInit;
	friend MushroomInit;
	friend GhostInit;
	friend DemonInit;

	// Item
	friend ItemNormal;

	// Follower��Init
	friend OctpusInit;
	friend BeeInit;
	friend FollowMushInit;
	friend FollowGhostInit;
	friend FollowDemonInit;
	friend FollowPotInit;

	std::map<AnimKey, AnimVector> _animMap;			// �A�j���[�V�����i�[�}�b�v
	STATE _state;									// ���
	DIR _dir;										// ����
	unsigned int _animFrame;						// ���݂̃R�}��
	unsigned int _animCount;						// �A�j���[�V�������n�܂��Ă���̃t���[�����̃J�E���g
	bool _resultFlg;								// ���U���g�V�[���Ɉڍs���邽�߂��׸�
protected:
	
	bool DestroyProc(void);			// ������Ԋm�F,���S��Ԑݒ�
	virtual void DrawHP(void);				// HP�o�[�`��֐��A���삷��Player�ł�UI��ɕ`�悵�����̂�override����
	static int _hpBarImg[2];		// Obj���ʂ�HP�o�[�摜
	static int _shadowImg;			// �e�摜ID

	bool _alive;					// �������Ă邩(���������Ă��Ȃ�)
	bool _dead;						// ���S���Ă邩(���������I������)
	bool _exFlag;					// ����s���t���O(Enemy:�ˌ��J�n,Player:�L���v�`�������,�Ȃ�)
	bool _holdFlag;					// ������Ă��邩
	bool _glowFlag;					// �G�t�F�N�g�������邩
	int _hp;						// �̗�
	int _hpMax;						// �ő�̗�
	int _power;						// �U����
	UNIT_ID _unitID;				// ���
	Vector2Dbl _pos;				// ���W
	Vector2Int _size;				// �T�C�Y
	Vector2Dbl _vel;				// ���x�A���͓�������Ƃ��Ɏg��

	int _zOrder;					// �`��D��x
	int _weight;					// �d��
	int _damageCnt;					// �_���[�W���󂯂����HP�o�[�\�����Ԓ����p
	int _glowID;					// �`��G�t�F�N�g�������邽�߂̃X�N���[��
	int _hpID;						// HP�o�[�\���̂��߂̃X�N���[��
	int _effectScreen;				// �G�t�F�N�g�\���p�̃X�N���[��
	int _serialNum;					// �ʂ��ԍ�
	double _height;					// ����
	double _gravity;				// �������Ă�Ƃ��̏d��
	double _rad;					// ��]�p
	double _speed;					// ����
	double _searchRange;			// ���G�͈�
	double _attackRange;			// �U���͈�
	double _extRate;				// �g�嗦
	TEAM_TAG _team;					// �w�c
	std::weak_ptr<Obj> _tageObj;	// �ΏۂƂȂ�obj
	std::weak_ptr<Obj> _holdPlayer;	// ������Ă���v���C���[�A������ĂȂ��Ƃ��͊Ď��Ώۖ���
	std::weak_ptr<Obj> _potObj;		// ��
	std::map<STATE, funcObj> _funcState;	// �I�u�W�F�N�g�̏�Ԃ��L�[�ɂ���Update�ōs���֐�������
	std::shared_ptr<InputState> _input;			// ���͊Ǘ��N���X
	bool _effectFlg;							// �̪�Ă�A���Đ����Ă������ǂ���
	int _coolCnt;								// �����їp
	int _coolCntMax;							// �����т̍ő�l
	int _effectID;									// �̪�Ă�����ٕۑ����Ă���
};

