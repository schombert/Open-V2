#include "common\\common.h"
#include "sound.h"
#include "world_state\\world_state.h"
#include <Windows.h>
#include "Dshow.h"
#include "graphics\\v2_window.h"
#include <random>


namespace sound {
	audio_instance::audio_instance(audio_instance && o) noexcept : filename(std::move(o.filename)), 
		graph_interface(o.graph_interface), control_interface(o.control_interface), audio_interface(o.audio_interface),
		seek_interface(o.seek_interface), event_interface(o.event_interface), volume_multiplier(o.volume_multiplier) {
		
		o.graph_interface = nullptr;
		o.control_interface = nullptr;
		o.audio_interface = nullptr;
		o.seek_interface = nullptr;
		o.event_interface = nullptr;
	}
	audio_instance::~audio_instance() {
		if(audio_interface) {
			((IBasicAudio*)audio_interface)->Release();
			audio_interface = nullptr;
		}
		if(control_interface) {
			((IMediaControl*)control_interface)->Release();
			control_interface = nullptr;
		}
		if(seek_interface) {
			((IMediaSeeking*)seek_interface)->Release();
			seek_interface = nullptr;
		}
		if(event_interface) {
			((IMediaEventEx*)event_interface)->SetNotifyWindow(NULL, 0, NULL);
			((IMediaEventEx*)event_interface)->Release();
			event_interface = nullptr;
		}
		if(graph_interface) {
			((IGraphBuilder*)graph_interface)->Release();
			graph_interface = nullptr;
		}
	}

	constexpr int32_t volume_function(float v) {
		return std::clamp(int32_t((v + -1.0f) * 4'500.0f), -10'000, 0);
	}

	void audio_instance::play(float volume, bool as_music, void* window_handle) {
		if(volume * volume_multiplier <= 0.0f || filename.size() == 0)
			return;

		if(!graph_interface) {
			IGraphBuilder *pGraph = nullptr;

			HRESULT hr = CoCreateInstance(CLSID_FilterGraph, nullptr,
				CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
			if(FAILED(hr)) {
				MessageBox(nullptr, L"failed to create graph builder", L"Audio error", MB_OK);
				std::abort();
			}

			std::thread creation_dispatch([pGraph, as_music, volume, window_handle, _this = this]() {
				HRESULT hr = pGraph->RenderFile((wchar_t*)(_this->filename.c_str()), nullptr);
				if(FAILED(hr)) {
					MessageBox(nullptr, L"unable to play audio file", L"Audio error", MB_OK);
					_this->volume_multiplier = 0.0f;
					return;
				}

				IMediaControl *pControl = nullptr;
				hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);

				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to get control interface", L"Audio error", MB_OK);
					std::abort();
				}

				if(as_music) {
					IMediaEventEx *pEvent = nullptr;
					hr = pGraph->QueryInterface(IID_IMediaEventEx, (void **)&pEvent);

					if(FAILED(hr)) {
						MessageBox(nullptr, L"failed to get event interface", L"Audio error", MB_OK);
						std::abort();
					}
					auto const res_b = pEvent->SetNotifyWindow((OAHWND)window_handle, WM_GRAPHNOTIFY, NULL);
					if(FAILED(res_b)) {
						MessageBox(nullptr, L"failed to set notification window", L"Audio error", MB_OK);
						std::abort();
					}
					_this->event_interface = pEvent;
				}

				IBasicAudio *pAudio = nullptr;
				hr = pGraph->QueryInterface(IID_IBasicAudio, (void **)&pAudio);

				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to get audio interface", L"Audio error", MB_OK);
					std::abort();
				}

				IMediaSeeking *pSeek = nullptr;
				hr = pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pSeek);

				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to get seeking interface", L"Audio error", MB_OK);
					std::abort();
				}

				_this->graph_interface = pGraph;
				_this->control_interface = pControl;
				_this->audio_interface = pAudio;
				_this->seek_interface = pSeek;

				hr = ((IBasicAudio*)pAudio)->put_Volume(volume_function(volume * _this->volume_multiplier));
				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to put_Volume", L"Audio error", MB_OK);
				}

				LONGLONG new_position = 0;
				hr = ((IMediaSeeking*)pSeek)->SetPositions(&new_position, AM_SEEKING_AbsolutePositioning, nullptr, AM_SEEKING_NoPositioning);
				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to SetPositions", L"Audio error", MB_OK);
				}
				hr = ((IMediaControl*)pControl)->Run();
				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to Run", L"Audio error", MB_OK);
				}
			});
			creation_dispatch.detach();
		} else {
			HRESULT hr;
			if(audio_interface) {
				hr = ((IBasicAudio*)audio_interface)->put_Volume(volume_function(volume * volume_multiplier));
				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to put_Volume", L"Audio error", MB_OK);
				}
			}
			if(seek_interface) {
				LONGLONG new_position = 0;
				hr = ((IMediaSeeking*)seek_interface)->SetPositions(&new_position, AM_SEEKING_AbsolutePositioning, nullptr, AM_SEEKING_NoPositioning);
				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to SetPositions", L"Audio error", MB_OK);
				}
			}

			if(control_interface) {
				hr = ((IMediaControl*)control_interface)->Run();
				if(FAILED(hr)) {
					MessageBox(nullptr, L"failed to Run", L"Audio error", MB_OK);
				}
			}
		}

		
	}

	void audio_instance::stop() const {
		if(control_interface)
			((IMediaControl*)control_interface)->Pause();
	}

	bool audio_instance::is_playing() const {
		if(seek_interface && volume_multiplier > 0.0f) {
			LONGLONG end_position = 0;
			LONGLONG current_position = 0;
			auto const result = ((IMediaSeeking*)seek_interface)->GetPositions(&current_position, &end_position);

			return !(FAILED(result) || current_position >= end_position);
		} else {
			return false;
		}
	}

	void audio_instance::change_volume(float new_volume) const {
		if(new_volume * volume_multiplier > 0.0f) {
			if(audio_interface)
				((IBasicAudio*)audio_interface)->put_Volume(volume_function(new_volume * volume_multiplier));
		} else {
			if(control_interface)
				((IMediaControl*)control_interface)->Pause();
		}
	}

	void sound_manager::play_effect(audio_instance & s, float volume) {
		if(!current_effect || current_effect->is_playing() == false) {
			s.play(volume, false, window_handle);
			current_effect = &s;
		}
	}

	void sound_manager::play_interface_sound(audio_instance & s, float volume) {
		if(current_interface_sound)
			current_interface_sound->stop();
		current_interface_sound = &s;
		s.play(volume, false, window_handle);
	}

	void sound_manager::play_music(int32_t track, float volume) {
		const auto lm = last_music;
		if(lm != -1)
			music_list[lm].stop();
		last_music = track;
		if(track != -1)
			music_list[track].play(volume, true, window_handle);
	}

	void sound_manager::change_effect_volume(float v) const {
		if(current_effect)
			current_effect->change_volume(v);
	}

	void sound_manager::change_interface_volume(float v) const {
		if(current_interface_sound)
			current_interface_sound->change_volume(v);
	}

	void sound_manager::change_music_volume(float v) const {
		const auto lm = last_music;
		if(lm != -1) {
			music_list[lm].change_volume(v);
		}
	}

	bool sound_manager::music_finished() const {
		const auto lm = last_music;
		if(lm == -1)
			return false;

		long evCode;
		LONG_PTR param1, param2;
		const auto event_interface = music_list[lm].event_interface;
		if(event_interface) {
			while(SUCCEEDED(((IMediaEventEx*)event_interface)->GetEvent(&evCode, &param1, &param2, 0))) {
				((IMediaEventEx*)event_interface)->FreeEventParams(evCode, param1, param2);
				switch(evCode) {
					case EC_COMPLETE:  // Fall through.
					case EC_USERABORT: // Fall through.
						return true;
					default:
						;
						//nothing
				}
			}
			return false;
		} else {
			return false;
		}
	}

	sound_manager::sound_manager() {
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	}

	sound_manager::~sound_manager() {
		CoUninitialize();
	}

	void init_sound_manager(sound_manager & s, const directory& root, void * window_handle) {
		const auto music_directory = root.get_directory(u"\\music");

		s.window_handle = window_handle;
		for(const auto& mp3_file : music_directory.list_files(u".mp3")) {
			s.music_list.emplace_back(mp3_file.file_path() + u'\\' + mp3_file.file_name());
			if(mp3_file.file_name() == u"thecoronation_titletheme.mp3")
				s.first_music = int32_t(s.music_list.size()) - 1;
		}

		const auto sound_directory = root.get_directory(u"\\sound");
		auto click_peek = sound_directory.peek_file(u"GI_ValidClick.wav");

		s.click_sound.set_file(click_peek ? click_peek->file_path() + u'\\' + click_peek->file_name() : std::u16string());
	}

	void play_effect(world_state & ws, audio_instance & s) {
		ws.s.sound_m.play_effect(s, ws.s.settings.effects_volume * ws.s.settings.master_volume);
	}

	void play_interface_sound(world_state & ws, audio_instance & s) {
		ws.s.sound_m.play_interface_sound(s, ws.s.settings.interface_volume * ws.s.settings.master_volume);
	}

	void play_new_track(world_state & ws) {
		auto& gen = get_local_generator();
		std::uniform_int_distribution<int32_t> dist(0, int32_t(ws.s.sound_m.music_list.size()) - 1);
		int32_t result = dist(gen);
		while(result == ws.s.sound_m.last_music)
			result = dist(gen);
		ws.s.sound_m.play_music(result, ws.s.settings.music_volume * ws.s.settings.master_volume);
	}

}
