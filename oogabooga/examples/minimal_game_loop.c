
int entry(int argc, char **argv) {
	
	// This is how we (optionally) configure the window.
	// To see all the settable window properties, ctrl+f "struct Os_Window" in os_interface.c
	window.title = STR("Minimal Game Example");
	
	while (!window.should_close) {
		ResetTemporaryStorage();
		
		float64 now = OsGetElapsedSeconds();
		
		Matrix4 rect_xform = M4Scalar(1.0);
		rect_xform         = M4RotateZ(rect_xform, (f32)now);
		rect_xform         = M4Translate(rect_xform, v3(-125, -125, 0));
		DrawRectXform(rect_xform, v2(250, 250), COLOR_GREEN);
		
		draw_rect(v2(sin(now)*window.width*0.4-60, -60), v2(120, 120), COLOR_RED);
		
		OsUpdate(); 
		GfxUpdate();
	}

	return 0;
}