#include "../oogabooga/oogabooga.h"
#include <assert.h>

int Entry(int argc, char **argv) {
    (void)argc; (void)argv;
    Vec3 a = Vec3(1.0f, 2.0f, 3.0f);
    Vec3 b = Vec3(4.0f, 5.0f, 6.0f);
    Vec3 c = V3Add(a, b);
    assert(c.x == 5.0f && c.y == 7.0f && c.z == 9.0f);

    Quaternion qx = QuatFromAxisAngle(Vec3(1,0,0), PI32 * 0.5f);
    Quaternion qy = QuatFromAxisAngle(Vec3(0,1,0), PI32 * 0.25f);
    Quaternion q = QuatMul(qx, qy);
    (void)q;

    print("All simple tests passed\n");
    return 0;
}
