vector<int> z_function(string s) {
    int n = (int) s.length();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r)
            z[i] = min (r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}

string multiplyStrings(string num1, string num2) {
    if (num1 == "0" || num2 == "0") return "0";

    int n = num1.size(), m = num2.size();
    vector<int> result(n + m, 0);


    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + result[i + j + 1];

            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }


    string product;
    bool leadingZero = true;
    for (int num : result) {
        if (leadingZero && num == 0) continue;
        leadingZero = false;
        product += (num + '0');
    }

    return product.empty() ? "0" : product;
}