template<class T>
vector<T> circleLine(T o, double r, T a, T b) {
    double h2 = r * r - lineDist(o, a, b) * lineDist(o, a, b);
    if (dcmp(h2, 0) != -1) { // the line touches the circle
        T p = o.projectOnLine(a, b); // point P
        T h = (a - b).unit() * sqrt(h2); // vector parallel to l, of length h
        if (p - h == p + h)
            return {p - h};
        else
            return {p - h, p + h};
    }
    return {};
}

template<class T>
vector<T> circleSegment(T o, double r, T a, T b) {
    vector<T> v = circleLine(o, r, a, b);
    vector<T> out;
    for (auto x: v)
        if (onSegment(x, a, b))
            out.pb(x);
    return out;
}