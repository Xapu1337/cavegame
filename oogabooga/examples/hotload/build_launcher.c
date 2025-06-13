

// !!!!!!!! BUILD CONFIG SHOULD BE DONE IN build_engine.c

#define OOGABOOGA_LINK_EXTERNAL_INSTANCE 1
#include "oogabooga/oogabooga.c"


// All we do with the launcher is to launch the engine
// We need to be careful to use oogabooga things because it has not yet been initialized.
// We can use GetTemporaryAllocator() because that actually gives us the initialization allocator.
// We cannot use log() but we can use print()
int main(int argc, char **argv) {
    string exe_path = STR(argv[0]);
    string exe_dir = get_directory_of(exe_path);
    
    Allocator a = get_initialization_allocator();
    string dll_path = StringConcat(exe_dir, STR("/engine.dll"), a);
    
    Dynamic_Library_Handle dll = os_load_dynamic_library(dll_path);
    if (!dll) {
        OsWriteStringToStdout(STR("Failed loading engine dll from "));
        OsWriteStringToStdout(dll_path);
        OsWriteStringToStdout(STR("\n"));
        return -1;
    }
    
    int (*engine_main)(int, char**) = os_dynamic_library_load_symbol(dll, STR("main"));
    if (!engine_main) {
        OsWriteStringToStdout(STR("Failed loading engine main\n"));
        return -1;
    }
    
    print("Launcher found engine main(), running...\n");
    return engine_main(argc, argv);
}