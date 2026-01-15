#include "Sound.h"

HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

Sound::Sound(HWND hWnd) {
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	XAudio2Create(&_xAudio2, 0);
	_xAudio2->CreateMasteringVoice(&_masteringVoice);
}

Sound::~Sound() {
	for (int nCntSound = 0; nCntSound < (int)_soundIndex; nCntSound++) {
		if (_apSourceVoice[nCntSound]) {
			// 一時停止
			_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			_apSourceVoice[nCntSound]->DestroyVoice();
			_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(_apDataAudio[nCntSound]);
			_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	_masteringVoice->DestroyVoice();
	_masteringVoice = NULL;

	if (_xAudio2) {
		_xAudio2->Release();
		_xAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

int Sound::loadSound(const char* filename) {
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	for (unsigned int i = 0; i < _soundIndex; i++) {
		if (strcmp(_soundName[i], filename) == 0) {
			return i;
		}
	}

	if (_soundIndex == MAX_SOUND_NUM) {
		return -1;
	}

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	// WAVEファイルのチェック
	CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);

	// フォーマットチェック
	CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	// オーディオデータ読み込み
	CheckChunk(hFile, 'atad', &_aSizeAudio[_soundIndex], &dwChunkPosition);

	_apDataAudio[_soundIndex] = (BYTE*)malloc(_aSizeAudio[_soundIndex]);
	ReadChunkData(hFile, _apDataAudio[_soundIndex], _aSizeAudio[_soundIndex], dwChunkPosition);

	// ソースボイスの生成
	_xAudio2->CreateSourceVoice(&_apSourceVoice[_soundIndex], &(wfx.Format));

	//読み込んだサウンド名を保存する
	strcpy_s(_soundName[_soundIndex], 256, filename);

	int retIndex = _soundIndex;

	//インデックスを一つ進める
	_soundIndex++;

	return retIndex;
}

void Sound::setVolume(int index, float vol) {
	_apSourceVoice[index]->SetVolume(vol);
}

void Sound::playSound(int index, int loopCount) {
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = _aSizeAudio[index];
	buffer.pAudioData = _apDataAudio[index];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (loopCount < 0)
		loopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopCount = loopCount;

	_apSourceVoice[index]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0) {
		_apSourceVoice[index]->Stop(0);
		_apSourceVoice[index]->FlushSourceBuffers();
	}
	_apSourceVoice[index]->SubmitSourceBuffer(&buffer);
	_apSourceVoice[index]->Start(0);
}

void Sound::stopSound(int index) {
	XAUDIO2_VOICE_STATE xa2state;
	_apSourceVoice[index]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0) {
		_apSourceVoice[index]->Stop(0);
		_apSourceVoice[index]->FlushSourceBuffers();
	}
}

void Sound::stopSoundAll() {
	for (int nCntSound = 0; nCntSound < (int)_soundIndex; nCntSound++) {
		if (_apSourceVoice[nCntSound]) {
			_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

/*------------------------------------------------------------------------------
	 WAVEフォーマットのチェック
------------------------------------------------------------------------------*/
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{
			// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{
				// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

/*------------------------------------------------------------------------------
	 WAVEフォーマットの読み込み
------------------------------------------------------------------------------*/
HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{
		// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

