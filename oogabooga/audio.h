#ifndef OOGABOOGA_AUDIO_H
#define OOGABOOGA_AUDIO_H

#include <stdbool.h>

typedef struct Audio_Source Audio_Source;
typedef struct Audio_Playback_Config Audio_Playback_Config;
typedef struct Audio_Player Audio_Player;
typedef enum Audio_Player_State Audio_Player_State;

bool audio_open_source_stream(Audio_Source *src, string path, Allocator allocator);
bool audio_open_source_load(Audio_Source *src, string path, Allocator allocator);
void audio_source_destroy(Audio_Source *src);

void play_one_audio_clip_source(Audio_Source source);
void play_one_audio_clip(string path);
void play_one_audio_clip_source_config(Audio_Source source, Audio_Playback_Config config);
void play_one_audio_clip_config(string path, Audio_Playback_Config config);

Audio_Player *audio_player_get_one(void);
void audio_player_release(Audio_Player *p);
void audio_player_set_state(Audio_Player *p, Audio_Player_State state);
void audio_player_set_time_stamp(Audio_Player *p, double time_in_seconds);
void audio_player_set_progression_factor(Audio_Player *p, double factor);
bool audio_player_at_source_end(Audio_Player *p);
double audio_player_get_time_stamp(Audio_Player *p);
double audio_player_get_current_progression_factor(Audio_Player *p);
void audio_player_set_source(Audio_Player *p, Audio_Source src);
void audio_player_clear_source(Audio_Player *p);
void audio_player_set_looping(Audio_Player *p, bool looping);

#endif
