#pragma once

#include "core/os/mutex.h"
#include "servers/audio_server.h"

#include <ohaudio/native_audiorenderer.h>
#include <ohaudio/native_audiostreambuilder.h>

class AudioDriverOpenHarmony : public AudioDriver {
	bool active = false;
	Mutex mutex;
	bool pause = false;

	uint32_t buffer_size = 0;
	int32_t *mixdown_buffer = nullptr;

	OH_AudioStreamBuilder *audio_stream_builder = nullptr;
	OH_AudioRenderer *audio_renderer = nullptr;

	OH_AudioData_Callback_Result _buffer_callback(OH_AudioRenderer *renderer, void *userData, void *audioData, int32_t audioDataSize);
	static OH_AudioData_Callback_Result _buffer_callbacks(OH_AudioRenderer *renderer, void *userData, void *audioData, int32_t audioDataSize);

public:
	virtual const char *get_name() const override {
		return "OpenHarmony";
	}

	virtual Error init() override;
	virtual void start() override;
	virtual int get_mix_rate() const override;
	virtual SpeakerMode get_speaker_mode() const override;

	virtual void lock() override;
	virtual void unlock() override;
	virtual void finish() override;

	void set_pause(bool p_pause);

	AudioDriverOpenHarmony();
};