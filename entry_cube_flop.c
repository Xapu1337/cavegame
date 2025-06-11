// Simple 3D cube flop game example

#define CELL_SIZE 1.0f
#define BOARD_SIZE 5

typedef struct {
    int x;
    int y;
} Grid_Pos;

static Grid_Pos cube_pos = {2, 2};
static Grid_Pos hole_pos = {4, 1};

static bool rotating = false;
static float rotate_t = 0.0f;
static Vector3f32 pivot_offset;
static Vector3f32 rotate_axis;
static Grid_Pos target_pos;

Matrix4 make_cube_xform() {
    float board_half = BOARD_SIZE * CELL_SIZE * 0.5f;
    Vector3f32 pos = v3f32((cube_pos.x + 0.5f) * CELL_SIZE - board_half,
                           CELL_SIZE * 0.5f,
                           (cube_pos.y + 0.5f) * CELL_SIZE - board_half);

    Matrix4 m = m4_scalar(1.0f);
    m = m4_translate(m, pos);
    if (rotating) {
        m = m4_translate(m, pivot_offset);
        m = m4_rotate(m, rotate_axis, rotate_t * PI32 * 0.5f);
        m = m4_translate(m, v3_mulf(pivot_offset, -1.0f));
    }
    return m;
}

void draw_cube(Matrix4 transform, Vector4 color) {
    Matrix4 face;
    float s = CELL_SIZE;

    // front
    face = transform;
    face = m4_translate(face, v3f32(-s/2, -s/2, s/2));
    draw_rect_xform(face, v2(s, s), color);

    // back
    face = transform;
    face = m4_rotate_y(face, PI32);
    face = m4_translate(face, v3f32(-s/2, -s/2, s/2));
    draw_rect_xform(face, v2(s, s), color);

    // left
    face = transform;
    face = m4_rotate_y(face, -PI32*0.5f);
    face = m4_translate(face, v3f32(-s/2, -s/2, s/2));
    draw_rect_xform(face, v2(s, s), color);

    // right
    face = transform;
    face = m4_rotate_y(face, PI32*0.5f);
    face = m4_translate(face, v3f32(-s/2, -s/2, s/2));
    draw_rect_xform(face, v2(s, s), color);

    // top
    face = transform;
    face = m4_rotate_x(face, -PI32*0.5f);
    face = m4_translate(face, v3f32(-s/2, -s/2, s/2));
    draw_rect_xform(face, v2(s, s), color);

    // bottom
    face = transform;
    face = m4_rotate_x(face, PI32*0.5f);
    face = m4_translate(face, v3f32(-s/2, -s/2, s/2));
    draw_rect_xform(face, v2(s, s), color);
}

Vector3f32 board_tile_world(int x, int y) {
    float board_half = BOARD_SIZE * CELL_SIZE * 0.5f;
    return v3f32((x + 0.5f) * CELL_SIZE - board_half, 0,
                 (y + 0.5f) * CELL_SIZE - board_half);
}

int entry(int argc, char **argv) {
    window.title = STR("Cube Flop");
    window.point_width = 1280;
    window.point_height = 720;
    window.clear_color = v4(0.05f, 0.05f, 0.05f, 1.0f);

    float cam_yaw = 0.5f;
    float cam_pitch = 0.7f;
    float cam_dist = 8.0f;

    Vector2f32 last_mouse = v2f32(0, 0);

    float64 last_time = os_get_elapsed_seconds();
    while (!window.should_close) {
        reset_temporary_storage();
        float64 now = os_get_elapsed_seconds();
        float dt = now - last_time;
        last_time = now;

        Vector2f32 mouse_delta = v2f32(input_frame.mouse_x - last_mouse.x,
                                       input_frame.mouse_y - last_mouse.y);
        last_mouse = v2f32(input_frame.mouse_x, input_frame.mouse_y);

        if (is_key_down(MOUSE_BUTTON_LEFT)) {
            cam_yaw   += mouse_delta.x * 0.005f;
            cam_pitch += mouse_delta.y * 0.005f;
            cam_pitch = clamp(cam_pitch, 0.2f, 1.2f);
        }

        if (!rotating) {
            if (is_key_just_pressed('A')) {
                target_pos = (Grid_Pos){cube_pos.x-1, cube_pos.y};
                rotate_axis = v3f32(0,0,1);
                pivot_offset = v3f32(-CELL_SIZE/2, -CELL_SIZE/2, 0);
                rotating = true;
            } else if (is_key_just_pressed('D')) {
                target_pos = (Grid_Pos){cube_pos.x+1, cube_pos.y};
                rotate_axis = v3f32(0,0,-1);
                pivot_offset = v3f32(CELL_SIZE/2, -CELL_SIZE/2, 0);
                rotating = true;
            } else if (is_key_just_pressed('W')) {
                target_pos = (Grid_Pos){cube_pos.x, cube_pos.y-1};
                rotate_axis = v3f32(-1,0,0);
                pivot_offset = v3f32(0, -CELL_SIZE/2, -CELL_SIZE/2);
                rotating = true;
            } else if (is_key_just_pressed('S')) {
                target_pos = (Grid_Pos){cube_pos.x, cube_pos.y+1};
                rotate_axis = v3f32(1,0,0);
                pivot_offset = v3f32(0, -CELL_SIZE/2, CELL_SIZE/2);
                rotating = true;
            }
        }

        if (rotating) {
            rotate_t += dt * 3.0f;
            if (rotate_t >= 1.0f) {
                cube_pos = target_pos;
                rotating = false;
                rotate_t = 0.0f;
            }
        }

        float aspect = (float)window.width / (float)window.height;
        draw_frame.projection = m4_make_perspective_projection(PI32/4.0f, aspect, 0.1f, 100.f);

        Vector3f32 cam_pos = v3f32(sinf(cam_yaw)*cosf(cam_pitch)*cam_dist,
                                  sinf(cam_pitch)*cam_dist,
                                  cosf(cam_yaw)*cosf(cam_pitch)*cam_dist);
        draw_frame.camera_xform = m4_make_look_at(cam_pos, v3f32(0,0,0), v3f32(0,1,0));

        // draw board
        for(int y=0;y<BOARD_SIZE;y++)
        for(int x=0;x<BOARD_SIZE;x++) {
            if (x==hole_pos.x && y==hole_pos.y) continue;
            Matrix4 tile = m4_scalar(1.0f);
            tile = m4_rotate_x(tile, -PI32*0.5f);
            tile = m4_translate(tile, board_tile_world(x,y));
            draw_rect_xform(tile, v2(CELL_SIZE, CELL_SIZE), v4(0.2f,0.2f,0.2f,1.0f));
        }

        // draw cube
        Matrix4 cube = make_cube_xform();
        draw_cube(cube, COLOR_GREEN);

        if (!rotating && cube_pos.x==hole_pos.x && cube_pos.y==hole_pos.y) {
            log("You Win!");
        }

        os_update();
        gfx_update();
    }
    return 0;
}
