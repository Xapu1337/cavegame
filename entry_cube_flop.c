#include "oogabooga/oogabooga.h"

// Simple 3D cube flop game example

#define CELL_SIZE 1.0f
#define BOARD_SIZE 5

typedef struct { int x; int y; } Grid_Pos;

typedef struct Cube {
    Grid_Pos grid;
    Quaternion orientation;
} Cube;

static Cube cube = { {2,2}, {0,0,0,1} };
static Grid_Pos hole_pos = {4,1};

static bool rotating = false;
static float rotate_t = 0.0f;
static Vector3f32 pivot_offset;
static Vector3f32 rotate_axis;
static Grid_Pos target_pos;

Matrix4 make_cube_xform() {
    float board_half = BOARD_SIZE * CELL_SIZE * 0.5f;
    Vector3f32 pos = V3((cube.grid.x + 0.5f) * CELL_SIZE - board_half,
                           CELL_SIZE * 0.5f,
                           (cube.grid.y + 0.5f) * CELL_SIZE - board_half);

    Matrix4 m = M4FromQuatPos(cube.orientation, pos);
    if (rotating) {
        m = M4Translate(m, pivot_offset);
        m = M4Rotate(m, rotate_axis, rotate_t * PI32 * 0.5f);
        m = M4Translate(m, V3Mulf(pivot_offset, -1.0f));
    }
    return m;
}

void draw_cube(Matrix4 transform, Vector4 color) {
    Matrix4 face;
    float s = CELL_SIZE;

    // front
    face = transform;
    face = M4Translate(face, V3(-s/2, -s/2, s/2));
    DrawRectXform(face, v2(s, s), color);

    // back
    face = transform;
    face = M4RotateY(face, PI32);
    face = M4Translate(face, V3(-s/2, -s/2, s/2));
    DrawRectXform(face, v2(s, s), color);

    // left
    face = transform;
    face = M4RotateY(face, -PI32*0.5f);
    face = M4Translate(face, V3(-s/2, -s/2, s/2));
    DrawRectXform(face, v2(s, s), color);

    // right
    face = transform;
    face = M4RotateY(face, PI32*0.5f);
    face = M4Translate(face, V3(-s/2, -s/2, s/2));
    DrawRectXform(face, v2(s, s), color);

    // top
    face = transform;
    face = M4RotateX(face, -PI32*0.5f);
    face = M4Translate(face, V3(-s/2, -s/2, s/2));
    DrawRectXform(face, v2(s, s), color);

    // bottom
    face = transform;
    face = M4RotateX(face, PI32*0.5f);
    face = M4Translate(face, V3(-s/2, -s/2, s/2));
    DrawRectXform(face, v2(s, s), color);
}

Vector3f32 board_tile_world(int x, int y) {
    float board_half = BOARD_SIZE * CELL_SIZE * 0.5f;
    return V3((x + 0.5f) * CELL_SIZE - board_half, 0,
                 (y + 0.5f) * CELL_SIZE - board_half);
}

int entry(int argc, char **argv) {
    window.title = STR("Cube Flop");
    window.point_width = 1280;
    window.point_height = 720;
    window.clearColor = v4(0.05f, 0.05f, 0.05f, 1.0f);

    float cam_yaw = 0.5f;
    float cam_pitch = 0.7f;
    float cam_dist = 8.0f;

    Vector2f32 last_mouse = V2(0, 0);

    float64 last_time = OsGetElapsedSeconds();
    while (!window.should_close) {
        ResetTemporaryStorage();
        float64 now = OsGetElapsedSeconds();
        float dt = now - last_time;
        last_time = now;

        Vector2f32 mouse_delta = V2(inputFrame.mouse_x - last_mouse.x,
                                       inputFrame.mouse_y - last_mouse.y);
        last_mouse = V2(inputFrame.mouse_x, inputFrame.mouse_y);

        if (IsKeyDown(MOUSE_BUTTON_LEFT)) {
            cam_yaw   += mouse_delta.x * 0.005f;
            cam_pitch += mouse_delta.y * 0.005f;
            cam_pitch = clamp(cam_pitch, 0.2f, 1.2f);
        }

        if (!rotating) {
            if (IsKeyJustPressed('A')) {
                target_pos = (Grid_Pos){cube.grid.x-1, cube.grid.y};
                rotate_axis = V3(0,0,1);
                pivot_offset = V3(-CELL_SIZE/2, -CELL_SIZE/2, 0);
                rotating = true;
            } else if (IsKeyJustPressed('D')) {
                target_pos = (Grid_Pos){cube.grid.x+1, cube.grid.y};
                rotate_axis = V3(0,0,-1);
                pivot_offset = V3(CELL_SIZE/2, -CELL_SIZE/2, 0);
                rotating = true;
            } else if (IsKeyJustPressed('W')) {
                target_pos = (Grid_Pos){cube.grid.x, cube.grid.y-1};
                rotate_axis = V3(-1,0,0);
                pivot_offset = V3(0, -CELL_SIZE/2, -CELL_SIZE/2);
                rotating = true;
            } else if (IsKeyJustPressed('S')) {
                target_pos = (Grid_Pos){cube.grid.x, cube.grid.y+1};
                rotate_axis = V3(1,0,0);
                pivot_offset = V3(0, -CELL_SIZE/2, CELL_SIZE/2);
                rotating = true;
            }
        }

        if (rotating) {
            rotate_t += dt * 3.0f;
            if (rotate_t >= 1.0f) {
                cube.grid = target_pos;
                cube.orientation = QuatMul(cube.orientation,
                    QuatFromAxisAngle(rotate_axis, PI32*0.5f));
                rotating = false;
                rotate_t = 0.0f;
            }
        }

        float aspect = (float)window.width / (float)window.height;
        drawFrame.projection = M4MakePerspectiveProjection(PI32/4.0f, aspect, 0.1f, 100.f);

        Vector3f32 cam_pos = V3(sinf(cam_yaw)*cosf(cam_pitch)*cam_dist,
                                  sinf(cam_pitch)*cam_dist,
                                  cosf(cam_yaw)*cosf(cam_pitch)*cam_dist);
        drawFrame.cameraXform = M4MakeLookAt(cam_pos, V3(0,0,0), V3(0,1,0));

        // draw board
        for(int y=0;y<BOARD_SIZE;y++)
        for(int x=0;x<BOARD_SIZE;x++) {
            if (x==hole_pos.x && y==hole_pos.y) continue;
            Matrix4 tile = M4Scalar(1.0f);
            tile = M4RotateX(tile, -PI32*0.5f);
            tile = M4Translate(tile, board_tile_world(x,y));
            DrawRectXform(tile, v2(CELL_SIZE, CELL_SIZE), v4(0.2f,0.2f,0.2f,1.0f));
        }

        // draw cube
        Matrix4 cubeTransform = make_cube_xform();
        draw_cube(cubeTransform, COLOR_GREEN);

        if (!rotating && cube.grid.x==hole_pos.x && cube.grid.y==hole_pos.y) {
            print("You Win!\n");
        }

        OsUpdate();
        GfxUpdate();
    }
    return 0;
}
