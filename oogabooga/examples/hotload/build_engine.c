
///
// Build config stuff

#define OOGABOOGA_BUILD_SHARED_LIBRARY 1

#include "oogabooga/oogabooga.c"

///
///
// This is the "engine" part of your game, which will call into your game.dll


typedef void (*Game_Update_Proc)(f64);
Game_Update_Proc game_update;

Dynamic_Library_Handle dll = 0;

void load_game_dll(char **argv) {

    // Here we load all the game symbols

    if (dll) {
        os_unload_dynamic_library(dll);
    }

    string exe_path = STR(argv[0]);
    string exe_dir = get_directory_of(exe_path);
    
    // We need to copy the original and open the copy, so we can recompile the original and then close & replace
    // the copy.
    string dll_path = StringConcat(exe_dir, STR("/game.dll"), GetTemporaryAllocator());
    string used_dll_path = StringConcat(exe_dir, STR("/game-in-use.dll"), GetTemporaryAllocator());
    
    bool ok = os_file_copy(dll_path, used_dll_path, true);
    assert(ok, "Could not copy %s to %s", dll_path, used_dll_path);
    
    dll = os_load_dynamic_library(used_dll_path);
    assert(dll, "Failed loading game dll");

    game_update = os_dynamic_library_load_symbol(dll, STR("game_update"));
    assert(game_update, "game is missing game_update()");
    
    log("Loaded game procedures");
}

int entry(int argc, char **argv) {
	
    load_game_dll(argv);
	
	window.title = STR("Minimal Game Example");
	window.point_width = 1280; // We need to set the scaled size if we want to handle system scaling (DPI)
	window.point_height = 720; 
	window.x = 200;
	window.y = 90;
	window.clearColor = hex_to_rgba(0x6495EDff);

	float64 last_time = OsGetElapsedSeconds();
	while (!window.should_close) {
		float64 now = OsGetElapsedSeconds();
		float64 delta = now-last_time;
		if ((int)now != (int)last_time) log("%.2f FPS\n%.2fms", 1.0/(delta), (delta)*1000);
		last_time = now;
		
		ResetTemporaryStorage();
		
		game_update(delta);
		
		if (IsKeyJustPressed('R')) {
            load_game_dll(argv);
            play_one_audio_clip(STR("oogabooga/examples/bruh.wav"));
		}
		
		OsUpdate(); 
		GfxUpdate();
	}

	return 0;
}