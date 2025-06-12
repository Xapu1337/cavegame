
int entry(int argc, char **argv) {
	
	
	u32 w = 128;
	u32 h = 128;
	u32 tile_w = w/4;
	u32 tile_h = w/4;
	
	uint8_t *pixels = (uint8_t*)Alloc(GetHeapAllocator(), w * h * 4);

	for (u32 y = 0; y < h; y += 1) {
	    for (u32 x = 0; x < w; x += 1) {
	        u32 tile_x = x / tile_w;
	        u32 tile_y = y / tile_h;
	
	        bool is_black_tile = (tile_x % 2 == tile_y % 2);
	
	        for (u32 c = 0; c < 4; c += 1) {
	            if (is_black_tile) {
	                pixels[(y * w + x) * 4 + c] = 127;
	            } else {
	                pixels[(y * w + x) * 4 + c] = 255;
	            }
	        }
	    }
	}
	
	Gfx_Image *img = make_image(w, h, 4, pixels, GetHeapAllocator());
	
	uint8_t *read_pixels = (uint8_t*)Alloc(GetHeapAllocator(), w*h*4);
	
	gfx_read_image_data(img, 0, 0, w, h, read_pixels);
	
	gfx_set_image_data(img, 0, 0, w, h, read_pixels);
	
	assert(memcmp(read_pixels, pixels, w*h*4) == 0);
	
	float64 last_time = OsGetElapsedSeconds();
	while (!window.should_close) {
		ResetTemporaryStorage();
		
		float64 now = OsGetElapsedSeconds();
		if ((int)now != (int)last_time) log("%.2f FPS\n%.2fms", 1.0/(now-last_time), (now-last_time)*1000);
		last_time = now;
		
		Matrix4 rect_xform = M4Scalar(1.0);
		rect_xform         = M4RotateZ(rect_xform, (f32)now);
		rect_xform         = M4Translate(rect_xform, v3(-.25f, -.25f, 0));
		draw_image_xform(img, rect_xform, v2(.5f, .5f), COLOR_GREEN);
		
		draw_rect(v2(sin(now), -.8), v2(.5, .25), COLOR_RED);
		
		float aspect = (f32)window.width/(f32)window.height;
		float mx = (inputFrame.mouse_x/(f32)window.width  * 2.0 - 1.0)*aspect;
		float my = inputFrame.mouse_y/(f32)window.height * 2.0 - 1.0;
		
		draw_line(v2(-.75, -.75), v2(mx, my), 0.005, COLOR_WHITE);
		
		if (IsKeyJustPressed('F')) {
			window.fullscreen = !window.fullscreen;
		}
		
		OsUpdate(); 
		GfxUpdate();
	}

	return 0;
}