

// #Global
ogb_instance String_Builder _profile_output;
ogb_instance bool profiler_initted;
ogb_instance Spinlock _profiler_lock;

#if !OOGABOOGA_LINK_EXTERNAL_INSTANCE
String_Builder _profile_output = {0};
bool profiler_initted = false;
Spinlock _profiler_lock;
#endif

void dump_profile_result() {
	File file = os_file_open("google_trace.json", O_CREATE | O_WRITE);
	
	os_file_write_string(file, STR("["));
	os_file_write_string(file, _profile_output.result);
	os_file_write_string(file, STR("{}]"));
	
	os_file_close(file);
	
	log_verbose("Wrote profiling result to google_trace.json");
}
void _profiler_report_time(string name, f64 count, f64 start) {
	if (!profiler_initted) {
		spinlock_init(&_profiler_lock);
		profiler_initted = true;
		
		string_builder_init_reserve(&_profile_output, 1024*1000, GetHeapAllocator());	
		
	}
	
	spinlock_acquire_or_wait(&_profiler_lock);
	
	string fmt = STR("{\"cat\":\"function\",\"dur\":%.3f,\"name\":\"%s\",\"ph\":\"X\",\"pid\":0,\"tid\":%zu,\"ts\":%.3f},");
    string_builder_print(
        &_profile_output,
        fmt,
        (float64)(count * 1000000),  
        name,                  
        get_context().thread_id,
        start * 1000000              
    );
	spinlock_release(&_profiler_lock);
}
#if ENABLE_PROFILING
#define tm_scope(name) \
    for (f64 start_time = OsGetElapsedSeconds(), end_time = start_time, elapsed_time = 0; \
         elapsed_time == 0; \
         elapsed_time = (end_time = OsGetElapsedSeconds()) - start_time, _profiler_report_time(STR(name), elapsed_time, start_time))
#define tm_scope_var(name, var) \
    for (f64 start_time = OsGetElapsedSeconds(), end_time = start_time, elapsed_time = 0; \
         elapsed_time == 0; \
         elapsed_time = (end_time = OsGetElapsedSeconds()) - start_time, var=elapsed_time)
#define tm_scope_accum(name, var) \
    for (f64 start_time = OsGetElapsedSeconds(), end_time = start_time, elapsed_time = 0; \
         elapsed_time == 0; \
         elapsed_time = (end_time = OsGetElapsedSeconds()) - start_time, var+=elapsed_time)
#else
	#define tm_scope(...)
	#define tm_scope_var(...)
	#define tm_scope_accum(...)
#endif