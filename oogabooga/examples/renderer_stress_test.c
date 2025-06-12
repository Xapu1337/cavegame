

int entry(int argc, char **argv) {
	
	window.title = STR("My epic game");
	window.point_width = 1280;
	window.point_height = 720;
	window.x = 200;
	window.y = 200;

	window.clearColor = hex_to_rgba(0x2a2d3aff);
	
	Gfx_Image *bush_image = load_image_from_disk(STR("oogabooga/examples/berry_bush.png"), GetHeapAllocator());
	assert(bush_image, "Failed loading berry_bush.png");
	Gfx_Image *hammer_image = load_image_from_disk(STR("oogabooga/examples/hammer.png"), GetHeapAllocator());
	assert(hammer_image, "Failed loading hammer.png");
	
	Custom_Mouse_Pointer hammer_pointer 
	   = os_make_custom_mouse_pointer_from_file(STR("oogabooga/examples/hammer.png"), 16, 16, GetHeapAllocator());
	assert(hammer_pointer != 0, "Could not load hammer pointer");
	
	
	void *my_data = Alloc(GetHeapAllocator(), 32*32*4);
	memset(my_data, 0xffffffff, 32*32*4);
	Gfx_Image *my_image = make_image(32, 32, 4, my_data, GetHeapAllocator());
	for (int *c = (int*)my_data; c < (int*)my_data+16*16; c += 1) {
		*c = 0xff0000ff;
	}
	gfx_set_image_data(my_image, 0, 0, 16, 16, my_data);
	
	Gfx_Font *font = load_font_from_disk(STR("C:/windows/fonts/arial.ttf"), GetHeapAllocator());
	assert(font, "Failed loading arial.ttf, %d", GetLastError());
	
	// This makes sure atlas is rendered for ascii.
	// You might want to do this if your game lags the first time you render text because it renders
	// the atlas on the fly.
	render_atlas_if_not_yet_rendered(font, 32, 'A'); 
	
	seed_for_random = rdtsc();
	
	const float64 fps_limit = 69000;
	const float64 min_frametime = 1.0 / fps_limit;
	
	Matrix4 cameraXform = M4Scalar(1.0);
	
	float64 last_time = OsGetElapsedSeconds();
	while (!window.should_close) tm_scope("Frame") {
		ResetTemporaryStorage();
		
		float64 now = OsGetElapsedSeconds();
		float64 delta = now - last_time;
		if (delta < min_frametime) {
			os_high_precision_sleep((min_frametime-delta)*1000.0);
			now = OsGetElapsedSeconds();
			delta = now - last_time;
		}
		last_time = now;
		
		float32 aspect = (float32)window.width/(float32)window.height;
	
		drawFrame.projection = m4_make_orthographic_projection(-aspect, aspect, -1, 1, -1, 10);
		
		const float32 cam_move_speed = 4.0;
		Vector2 cam_move_axis = v2(0, 0);
		if (IsKeyDown('A')) {
			cam_move_axis.x -= 1.0;
		}
		if (IsKeyDown('D')) {
			cam_move_axis.x += 1.0;
		}
		if (IsKeyDown('S')) {
			cam_move_axis.y -= 1.0;
		}
		if (IsKeyDown('W')) {
			cam_move_axis.y += 1.0;
		}
		
		Vector2 cam_move = V2Mulf(cam_move_axis, delta * cam_move_speed);
		cameraXform = M4Translate(cameraXform, v3(v2_expand(cam_move), 0));
		drawFrame.cameraXform = cameraXform;

		seed_for_random = 69;
		for (u64 i = 0; i < 150000; i++) {
			float32 aspect = (float32)window.width/(float32)window.height;
			float min_x = -aspect;
			float max_x = aspect;
			float min_y = -1;
			float max_y = 1;
			
			float x = get_random_float32() * (max_x-min_x) + min_x;
			float y = get_random_float32() * (max_y-min_y) + min_y;

			draw_image(bush_image, v2(x, y), v2(0.1, 0.1), COLOR_WHITE);
		}
		
		if (is_key_just_released('E')) {
			log("FPS: %.2f", 1.0 / delta);
			log("ms: %.2f", delta*1000.0);
		}
		
		GfxUpdate();
		OsUpdate();
	}

	return 0;
}

