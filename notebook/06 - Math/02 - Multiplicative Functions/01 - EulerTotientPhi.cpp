int phi[N];

void calculatePhi() {
    for (int i = 0; i < N; ++i) phi[i] = i & 1 ? i : i / 2;
    for (int i = 3; i < N; i += 2)
        if (phi[i] == i)
            for (int j = i; j < N; j += i) phi[j] -= phi[j] / i;
}