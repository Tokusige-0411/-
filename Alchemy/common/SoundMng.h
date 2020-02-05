#pragma once
#include <memory>
#include <map>
#include <vector>

enum class SOUND
{
	TITLE_BGM,
	GAME_BGM,
	THROW,
	HOLD,
	COMBO,
	PUT,
	DAMAGE,
	MENU_DEC
};

#define lpSoundMng SoundMng::GetInstance()
#define SOUND_KEY(KEY) (SoundMng::GetInstance().GetID(KEY))

class SoundMng
{
public:
	// �ݽ�׸�
	static SoundMng & GetInstance(void)
	{
		return *_sInstance;
	}

	const std::vector<int>& GetID(const SOUND& key);
	const std::vector<int>& GetID(const SOUND& key, const std::string& fileName);
	const std::vector<int>& GetID(const SOUND& key, const std::string& fileName, const int num);

private:
	struct SoundMngDeleter
	{
		void operator()(SoundMng* soundMng) const
		{
			delete soundMng;
		}
	};
	SoundMng();
	~SoundMng();

	static std::unique_ptr<SoundMng, SoundMngDeleter> _sInstance;

	// ����ޏ����Ǘ����Ă���ϯ��
	std::map<SOUND, std::vector<int>> _soundMap;
};

