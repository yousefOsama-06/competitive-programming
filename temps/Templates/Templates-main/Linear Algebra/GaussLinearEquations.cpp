const int mod = 998244353;

int power(long long n, long long k) {
    int ans = 1 % mod;
    n %= mod;
    if (n < 0) n += mod;
    while (k) {
        if (k & 1) ans = 1ll * ans * n % mod;
        n = 1ll * n * n % mod;
        k >>= 1;
    }
    return ans;
}

vector<int> gaussian_elimination(vector<vector<int> > matrix) {
    int n = matrix.size();
    if (n == 0) return {};
    int vars = matrix[0].size() - 1;

    int row = 0;
    for (int col = 0 ; col < vars ; col++) {
        if (row >= n) break;

        int pivot = -1;
        for (int i = row ; i < n ; i++) {
            if (matrix[i][col] != 0) {
                pivot = i;
                break;
            }
        }
        if (pivot == -1) continue;

        swap(matrix[row], matrix[pivot]);

        int inv = power(matrix[row][col], mod - 2);
        for (int j = col ; j <= vars ; j++) {
            matrix[row][j] = 1ll * matrix[row][j] * inv % mod;
        }

        for (int i = 0 ; i < n ; i++) {
            if (i != row && matrix[i][col] != 0) {
                int factor = matrix[i][col];
                for (int j = col ; j <= vars ; j++) {
                    matrix[i][j] -= 1ll * factor * matrix[row][j] % mod;
                    if (matrix[i][j] < 0) matrix[i][j] += mod;
                }
            }
        }
        row++;
    }

    for (int i = 0 ; i < n ; i++) {
        bool all_zero = true;
        for (int j = 0 ; j < vars ; j++) {
            if (matrix[i][j] != 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero && matrix[i][vars] != 0)
            return {};
    }

    // Check for multiple solutions
    if (row < vars)
        return {};

    vector<int> solution(vars);
    for (int i = 0 ; i < vars ; i++) {
        if (i < matrix.size())
            solution[i] = matrix[i][vars];
        else
            solution[i] = 0;
    }
    return solution;
}


const double EPS = 1e-6;
const int INF = 2; // it doesn't actually have to be infinity or a big number

int gauss(vector<vector<double> > a, vector<double> &ans) {
    int n = (int) a.size();
    int m = (int) a[0].size() - 1;

    vector<int> where(m, -1);
    for (int col = 0, row = 0 ; col < m && row < n ; ++col) {
        int sel = row;
        for (int i = row ; i < n ; ++i)
            if (abs(a[i][col]) > abs(a[sel][col]))
                sel = i;
        if (abs(a[sel][col]) < EPS)
            continue;
        for (int i = col ; i <= m ; ++i)
            swap(a[sel][i], a[row][i]);
        where[col] = row;

        for (int i = 0 ; i < n ; ++i)
            if (i != row) {
                double c = a[i][col] / a[row][col];
                for (int j = col ; j <= m ; ++j)
                    a[i][j] -= a[row][j] * c;
            }
        ++row;
    }

    ans.assign(m, 0);
    for (int i = 0 ; i < m ; ++i)
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];
    for (int i = 0 ; i < n ; ++i) {
        double sum = 0;
        for (int j = 0 ; j < m ; ++j)
            sum += ans[j] * a[i][j];
        if (abs(sum - a[i][m]) > EPS)
            return 0;
    }

    for (int i = 0 ; i < m ; ++i)
        if (where[i] == -1)
            return INF;
    return 1;
}
