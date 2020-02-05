#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Vector2.h"
#include <object\STATE.h>

// �摜�}�b�v�ɃA�N�Z�X����L�[
enum class IMG
{
	PLAYER_1,
	PLAYER_2,
	PLAYER_3,
	PLAYER_4,
	FOL_OCTOPUS,
	FOL_BEE,
	FOL_DAEMON,
	FOL_GHOST,
	FOL_MUSH,
	FOL_POT,
	ENEMY_SLIME,
	ENEMY_BAT,
	ENEMY_DAEMON,
	ENEMY_GHOST,
	ENEMY_MUSH,
	BOSS,
	ITEM_SLIME,
	ITEM_BAT,
	ITEM_DEMON,
	ITEM_GHOST,
	ITEM_MUSH,
	ITEM_POT,
	POT,
	HP_G,
	HP_R,
	PL_HP_G,
	PL_HP_R,
	PL_ICON,
	BOSS_HP_R,
	BOSS_HP_G,
	BOSS_HP_R_TWO,
	BOSS_HP_G_TWO,
	HEART,
	COMBO_GAUGE,
	BLAST,
	BUTTON,
	SHADOW,
	FONT,
	BG,
	BG_CURSOR,
	LANDING,
	MENU,
	MENU_CURSOL
};

#define lpImageMng ImageMng::GetInstance()
#define IMAGE_ID(KEY) (ImageMng::GetInstance().GetID(KEY))

using ImageKey = std::pair<IMG, STATE>;

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		return *sInstance;
	}

	const std::vector<int>& GetID(ImageKey key);
	const std::vector<int>& GetID(ImageKey key, const std::string& fileName);
	const std::vector<int>& GetID(ImageKey key, const std::string& fileName,const Vector2Int& divSize,const Vector2Int& divCnt);

private:
	struct ImageMngDeleter
	{
		void operator()(ImageMng *ImageMng)
		{
			delete ImageMng;
		}
	};

	static std::unique_ptr<ImageMng, ImageMngDeleter> sInstance;
	std::map < ImageKey, std::vector<int>> _imgMap;		// �摜�f�[�^���i�[����Ă���map
	ImageMng();
	~ImageMng();
};

