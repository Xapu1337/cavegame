#include "../oogabooga/oogabooga.h"

int Entry(int argc, char **argv) {
    Quaternion qx = QuatFromAxisAngle(Vec3(1,0,0), PI32 * 0.5f);
    Quaternion qy = QuatFromAxisAngle(Vec3(0,1,0), PI32 * 0.25f);
    Quaternion q = QuatMul(qx, qy);
    Matrix4 m = M4FromQuatPos(q, Vec3(0,0,0));
    (void)m;
    print("Quaternion: %.3f %.3f %.3f %.3f\n", q.x, q.y, q.z, q.w);
    return 0;
}
