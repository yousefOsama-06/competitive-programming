#include <iostream>
#include <sstream>
#include <set>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <vector>
#include <map>
#include <queue>
#include <numeric>
#include <string>
#include <cmath>
#include <climits>
#include <stack>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <bitset>
#include <cassert>
#include <tuple>
#include <iterator>
#include <random>
#include <chrono>
#include <list>

using namespace std;
typedef long long ll;
typedef long double ld;
const int N = 2e6+5;
//const int mod=998244353;
const int mod=1e9+7;

//given n numbers, k posititins, for each position u choose a number randomly .... expected number of different numbers

//m=1 to m=n, m*nCm*(m^k-(m-1)^k)/n^k ...
//m=1 to m=n nCm*(m^k-(m-1)^k) = n^k

int fac[N],inv[N];

int mul(const int &a, const int &b) {
	return 1ll * a * b % mod;
}

int add(int a, int b) {
	a += b;
	if (a >= mod)a -= mod;
	if (a < 0)a += mod;
	return a;
}

int pw(int b, int p) {
	if (p < 0)return 0;
	if (!p) return 1;
	int ret = pw(b, p >> 1);
	ret = mul(ret, ret);
	if (p & 1)
		ret = mul(ret, b);
	return ret;
}

void build(){
	fac[0] = 1;
	for (int i = 1; i < N; ++i) {
		fac[i] = mul(fac[i - 1],i);
	}
	inv[N - 1] = pw(fac[N - 1],mod - 2);
	for (int i = N - 2; ~i ; --i) {
		inv[i] = mul(inv[i + 1],i + 1);
	}
}

int nCr(int n,int r){
	if(r > n)return 0;
	return mul(mul(fac[n],inv[n - r]),inv[r]);
}

//given n numbers, k posititins, for each position u choose a number randomly .... expected number of different numbers
//dp[i]=i^k- j {dp[j]*iCj}
//m=1 to m=n nCm*(m^k-(m-1)^k) = n^k

void solve() {
	int n,k;
	for (n=1;n<=100;n++) {
		for (k=1;k<=100;k++) {
			vector <int> dp(n+1);
			for (int i=1;i<=n;i++) {
				dp[i]=pw(i,k);
				for (int j=1;j<i;j++) {
					dp[i]=add(dp[i],-mul(nCr(i,j),dp[j]));
				}
			}
			int sum=0;
			for (int m=1;m<=n;m++) {
				sum=add(sum,mul(m,mul(nCr(n,m),dp[m])));
			}
			cout<<sum<<'\n';
		}
	}
	/*vector <int> dp(n+1);
	for (int i=1;i<=n;i++) {
		dp[i]=pw(i,k);
		for (int j=1;j<i;j++) {
			dp[i]=add(dp[i],-mul(nCr(i,j),dp[j]));
		}
	}
	int sum=0;
	for (int m=1;m<=n;m++) {
		sum=add(sum,mul(m,mul(nCr(n,m),dp[m])));
	}
	cout<<sum<<'\n';*/
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int t=1;
	//cin>>t;
	build();
	while (t--) solve();
	return 0;
}