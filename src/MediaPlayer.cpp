#include "MediaPlayer.h"

MediaPlayer::MediaPlayer() {
	setenv("VLC_PLUGIN_PATH", "/Applications/VLC.app/Contents/MacOS/plugins", 1);
	vlc_instance = libvlc_new(0, NULL);
	if (vlc_instance == NULL) {
		std::cout << "Error occured while initializing vlc." << std::endl;
		std::cout << libvlc_printerr("%s") << std::endl;
		return;
	}
	
	vlc_mp = libvlc_media_player_new(vlc_instance);
}

MediaPlayer::~MediaPlayer() {
	if (vlc_instance == NULL) return;
	libvlc_media_player_stop(vlc_mp);
	libvlc_media_player_release(vlc_mp);
	libvlc_release(vlc_instance);
}

void MediaPlayer::SetVideoWidget(QFrame* widget) {
	videoWidget = widget;
}

void MediaPlayer::LoadMedia(const QString& filepath) {
	libvlc_media_t* media = libvlc_media_new_path(vlc_instance, filepath.toStdString().c_str());
	libvlc_media_player_set_media(vlc_mp, media);
	
	vlc_current_media = media;
	loadedMedia = filepath;
}

void MediaPlayer::Play() {
	if (videoWidget) {
		currentWId = videoWidget->winId();
	} else {
		currentWId = 0;
	}
	
	#if defined(Q_OS_WIN32)
			libvlc_media_player_set_hwnd(vlc_mp, (void *)currentWId);
	#elif defined(Q_OS_DARWIN)
			libvlc_media_player_set_nsobject(vlc_mp, (void *)currentWId);
	#elif defined(Q_OS_UNIX)
			libvlc_media_player_set_xwindow(vlc_mp, currentWId);
	#endif
	
	libvlc_media_player_play(vlc_mp);
}

void MediaPlayer::Pause() {
	libvlc_media_player_pause(vlc_mp);
}

void MediaPlayer::Stop() {
	libvlc_media_player_stop(vlc_mp);
}

void MediaPlayer::ChangePosition(float newPosition) {
	float pos = newPosition / (float) MP_POSITION_RESOLUTION;
	libvlc_media_player_set_position(vlc_mp, pos);
}

// Audio specific
void MediaPlayer::Mute(bool mute) {
	libvlc_audio_set_mute(vlc_mp, mute);
}

void MediaPlayer::ToggleMute() {
	libvlc_audio_toggle_mute(vlc_mp);
}

void MediaPlayer::ChangeVolume(int newVolume) {
	libvlc_audio_set_volume(vlc_mp, newVolume);
}
