#include "../oogabooga/oogabooga.h"

// Simple 3D cube flop game example

#define CELL_SIZE 1.0f
#define BOARD_SIZE 5

typedef struct { int x; int y; } GridPos;

typedef struct Cube {
    GridPos grid;
    Quaternion orientation;
} Cube;

static Cube cube = { {2,2}, {.x = 0, .y = 0, .z = 0, .w = 1} };
static GridPos holePos = {4,1};

static bool rotating = false;
static float rotateT = 0.0f;
static Vec3 pivotOffset;
static Vec3 rotateAxis;
static GridPos targetPos;

Matrix4 MakeCubeXform() {
    float boardHalf = BOARD_SIZE * CELL_SIZE * 0.5f;
    Vec3 pos = Vec3((cube.grid.x + 0.5f) * CELL_SIZE - boardHalf,
                           CELL_SIZE * 0.5f,
                           (cube.grid.y + 0.5f) * CELL_SIZE - boardHalf);

    Matrix4 m = M4FromQuatPos(cube.orientation, pos);
    if (rotating) {
        m = M4Translate(m, pivotOffset);
        m = M4Rotate(m, rotateAxis, rotateT * PI32 * 0.5f);
        m = M4Translate(m, V3Mulf(pivotOffset, -1.0f));
    }
    return m;
}

void DrawCube(Matrix4 transform, Vector4 color) {
    Matrix4 face;
    float s = CELL_SIZE;

    // front
    face = transform;
    face = M4Translate(face, Vec3(-s/2, -s/2, s/2));
    DrawRectXform(face, V2(s, s), color);

    // back
    face = transform;
    face = M4RotateY(face, PI32);
    face = M4Translate(face, Vec3(-s/2, -s/2, s/2));
    DrawRectXform(face, V2(s, s), color);

    // left
    face = transform;
    face = M4RotateY(face, -PI32*0.5f);
    face = M4Translate(face, Vec3(-s/2, -s/2, s/2));
    DrawRectXform(face, V2(s, s), color);

    // right
    face = transform;
    face = M4RotateY(face, PI32*0.5f);
    face = M4Translate(face, Vec3(-s/2, -s/2, s/2));
    DrawRectXform(face, V2(s, s), color);

    // top
    face = transform;
    face = M4RotateX(face, -PI32*0.5f);
    face = M4Translate(face, Vec3(-s/2, -s/2, s/2));
    DrawRectXform(face, V2(s, s), color);

    // bottom
    face = transform;
    face = M4RotateX(face, PI32*0.5f);
    face = M4Translate(face, Vec3(-s/2, -s/2, s/2));
    DrawRectXform(face, V2(s, s), color);
}

Vec3 BoardTileWorld(int x, int y) {
    float boardHalf = BOARD_SIZE * CELL_SIZE * 0.5f;
    return Vec3((x + 0.5f) * CELL_SIZE - boardHalf, 0,
                 (y + 0.5f) * CELL_SIZE - boardHalf);
}

int Entry(int argc, char **argv) {
    window.title = STR("Cube Flop");
    window.point_width = 1280;
    window.point_height = 720;
    window.clearColor = V4(0.05f, 0.05f, 0.05f, 1.0f);

    float camYaw = 0.5f;
    float camPitch = 0.7f;
    float camDist = 8.0f;

    Vector2f32 lastMouse = V2(0, 0);

    float64 last_time = OsGetElapsedSeconds();
    while (!window.should_close) {
        ResetTemporaryStorage();
        float64 now = OsGetElapsedSeconds();
        float dt = now - last_time;
        last_time = now;

        Vector2f32 mouseDelta = V2(inputFrame.mouse_x - lastMouse.x,
                                   inputFrame.mouse_y - lastMouse.y);
        lastMouse = V2(inputFrame.mouse_x, inputFrame.mouse_y);

        if (IsKeyDown(MOUSE_BUTTON_LEFT)) {
            camYaw   += mouseDelta.x * 0.005f;
            camPitch += mouseDelta.y * 0.005f;
            camPitch = clamp(camPitch, 0.2f, 1.2f);
        }

        if (!rotating) {
            if (IsKeyJustPressed('A')) {
                targetPos = (GridPos){cube.grid.x-1, cube.grid.y};
                rotateAxis = Vec3(0,0,1);
                pivotOffset = Vec3(-CELL_SIZE/2, -CELL_SIZE/2, 0);
                rotating = true;
            } else if (IsKeyJustPressed('D')) {
                targetPos = (GridPos){cube.grid.x+1, cube.grid.y};
                rotateAxis = Vec3(0,0,-1);
                pivotOffset = Vec3(CELL_SIZE/2, -CELL_SIZE/2, 0);
                rotating = true;
            } else if (IsKeyJustPressed('W')) {
                targetPos = (GridPos){cube.grid.x, cube.grid.y-1};
                rotateAxis = Vec3(-1,0,0);
                pivotOffset = Vec3(0, -CELL_SIZE/2, -CELL_SIZE/2);
                rotating = true;
            } else if (IsKeyJustPressed('S')) {
                targetPos = (GridPos){cube.grid.x, cube.grid.y+1};
                rotateAxis = Vec3(1,0,0);
                pivotOffset = Vec3(0, -CELL_SIZE/2, CELL_SIZE/2);
                rotating = true;
            }
        }

        if (rotating) {
            rotateT += dt * 3.0f;
            if (rotateT >= 1.0f) {
                cube.grid = targetPos;
                cube.orientation = QuatMul(cube.orientation,
                    QuatFromAxisAngle(rotateAxis, PI32*0.5f));
                rotating = false;
                rotateT = 0.0f;
            }
        }

    float aspect = (float)window.width / (float)window.height;
    drawFrame.projection = M4MakePerspectiveProjection(PI32/4.0f, aspect, 0.1f, 100.f);

    Vec3 camPos = Vec3(sinf(camYaw)*cosf(camPitch)*camDist,
                           sinf(camPitch)*camDist,
                           cosf(camYaw)*cosf(camPitch)*camDist);
    drawFrame.cameraXform = M4MakeLookAt(camPos, Vec3(0,0,0), Vec3(0,1,0));

        // draw board
        for(int y=0;y<BOARD_SIZE;y++)
        for(int x=0;x<BOARD_SIZE;x++) {
            if (x==holePos.x && y==holePos.y) continue;
            Matrix4 tile = M4Scalar(1.0f);
            tile = M4RotateX(tile, -PI32*0.5f);
            tile = M4Translate(tile, BoardTileWorld(x,y));
            DrawRectXform(tile, V2(CELL_SIZE, CELL_SIZE), V4(0.2f,0.2f,0.2f,1.0f));
        }

        // draw cube
        Matrix4 cubeTransform = MakeCubeXform();
        DrawCube(cubeTransform, COLOR_GREEN);

        if (!rotating && cube.grid.x==holePos.x && cube.grid.y==holePos.y) {
            print("You Win!\n");
        }

        OsUpdate();
        GfxUpdate();
    }
    return 0;
}
