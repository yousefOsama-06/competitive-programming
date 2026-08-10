// Angle Operations in 2D - [-pi, pi] and [0, 2*pi]
template<typename T>
ld angleBetween(T a, T b) {
    ld res = atan2(a.cross(b), a.dot(b));
    return res < 0 ? res + 2 * PI : res;
}

template<typename T>
ld angleO(T a, T O, T b) {
    return angleBetween(a - O, b - O);
}

ld formatAngle(ld angle) {
    while (angle > PI) angle -= 2 * PI;
    while (angle <= -PI) angle += 2 * PI;
    return angle;
}