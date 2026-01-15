#pragma once

#define NOMINMAX
#include <windows.h>
#include "xaudio2.h"

class Sound {
	public:
		static constexpr int MAX_SOUND_NUM = 100;

		int loadSound(const char* filename);
		void playSound(int index, int loopCount);
		void stopSound(int index);
		void stopSoundAll();
		void setVolume(int index, float vol);

	private:
		IXAudio2* _xAudio2 = NULL;
		IXAudio2MasteringVoice* _masteringVoice = NULL;
		IXAudio2SourceVoice* _apSourceVoice[MAX_SOUND_NUM] = {};
		BYTE* _apDataAudio[MAX_SOUND_NUM] = {};
		DWORD _aSizeAudio[MAX_SOUND_NUM] = {};
		char _soundName[MAX_SOUND_NUM][256] = {};
		DWORD _soundIndex = 0;

	public:
		static void CreateInstance(HWND hWnd) {
			DeleteInstance();
			s_instance = new Sound(hWnd);
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static Sound& GetInstance() {
			return *s_instance;
		}
	private:
		Sound(HWND hWnd);
		~Sound();
		static inline Sound* s_instance;
};

#define SOUND Sound::GetInstance()
