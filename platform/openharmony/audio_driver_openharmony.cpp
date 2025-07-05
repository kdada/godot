#include "audio_driver_openharmony.h"

AudioDriverOpenHarmony::AudioDriverOpenHarmony() {
}

OH_AudioData_Callback_Result AudioDriverOpenHarmony::_buffer_callback(OH_AudioRenderer *renderer, void *userData, void *audioData, int32_t audioDataSize) {
	bool mix = true;

	if (pause) {
		mix = false;
	} else {
		mix = mutex.try_lock();
	}

	if (mix) {
		audio_server_process(buffer_size, mixdown_buffer);
	} else {
		int32_t *src_buff = mixdown_buffer;
		for (unsigned int i = 0; i < buffer_size * 2; i++) {
			src_buff[i] = 0;
		}
	}

	if (mix) {
		mutex.unlock();
	}

	const int32_t *src_buff = mixdown_buffer;
    int16_t *ptr = static_cast<int16_t *>(audioData);

	for (unsigned int i = 0; i < buffer_size * 2; i++) {
		ptr[i] = src_buff[i] >> 16;
	}
    return AUDIO_DATA_CALLBACK_RESULT_VALID;
}

OH_AudioData_Callback_Result AudioDriverOpenHarmony::_buffer_callbacks(OH_AudioRenderer *renderer, void *userData, void *audioData, int32_t audioDataSize) {
	AudioDriverOpenHarmony *ad = static_cast<AudioDriverOpenHarmony *>(userData);
	return ad->_buffer_callback(renderer, userData, audioData, audioDataSize);
}

Error AudioDriverOpenHarmony::init() {
	return OK;
}

void AudioDriverOpenHarmony::start() {
	if (active) {
		return;
	}
	active = false;

	if (!mixdown_buffer) {
		buffer_size = 960;
		mixdown_buffer = memnew_arr(int32_t, buffer_size * 2);
	}

	if (!audio_stream_builder) {
		OH_AudioStreamBuilder_Create(&audio_stream_builder, AUDIOSTREAM_TYPE_RENDERER);
		OH_AudioStreamBuilder_SetSamplingRate(audio_stream_builder, get_mix_rate());
		OH_AudioStreamBuilder_SetChannelCount(audio_stream_builder, 2);
		OH_AudioStreamBuilder_SetSampleFormat(audio_stream_builder, AUDIOSTREAM_SAMPLE_S16LE);
		OH_AudioStreamBuilder_SetEncodingType(audio_stream_builder, AUDIOSTREAM_ENCODING_TYPE_RAW);
		OH_AudioStreamBuilder_SetRendererInfo(audio_stream_builder, AUDIOSTREAM_USAGE_MUSIC);
		OH_AudioStreamBuilder_SetRendererWriteDataCallback(audio_stream_builder, _buffer_callbacks, this);
		OH_AudioStreamBuilder_SetLatencyMode(audio_stream_builder, AUDIOSTREAM_LATENCY_MODE_FAST);
        OH_AudioStreamBuilder_SetFrameSizeInCallback(audio_stream_builder, buffer_size);
	}

	if (!audio_renderer) {
		OH_AudioStreamBuilder_GenerateRenderer(audio_stream_builder, &audio_renderer);
		OH_AudioRenderer_Start(audio_renderer);
	}

	active = true;
}

int AudioDriverOpenHarmony::get_mix_rate() const {
	return 48000;
}

AudioDriver::SpeakerMode AudioDriverOpenHarmony::get_speaker_mode() const {
	return SPEAKER_MODE_STEREO;
}

void AudioDriverOpenHarmony::lock() {
	if (active) {
		mutex.lock();
	}
}

void AudioDriverOpenHarmony::unlock() {
	if (active) {
		mutex.unlock();
	}
}

void AudioDriverOpenHarmony::finish() {
	if (audio_renderer) {
		OH_AudioRenderer_Stop(audio_renderer);
		OH_AudioRenderer_Flush(audio_renderer);
		OH_AudioRenderer_Release(audio_renderer);
		audio_renderer = nullptr;
	}

	if (audio_stream_builder) {
		OH_AudioStreamBuilder_Destroy(audio_stream_builder);
		audio_stream_builder = nullptr;
	}

	if (!mixdown_buffer) {
		buffer_size = 1024;
		memdelete_arr(mixdown_buffer);
		mixdown_buffer = nullptr;
	}
	active = false;
}

void AudioDriverOpenHarmony::set_pause(bool p_pause) {
	pause = p_pause;

	if (active && audio_renderer) {
		if (pause) {
			OH_AudioRenderer_Pause(audio_renderer);
		} else {
			OH_AudioRenderer_Start(audio_renderer);
		}
	}
}