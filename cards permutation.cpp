#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> II;
typedef vector<II> VII;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long int LL;

#define PB push_back
#define MP make_pair
#define F first
#define S second
#define SZ(a) (int)(a.size())
#define ALL(a) a.begin(), a.end()
#define SET(a, b) memset(a, b, sizeof(a))

#define si(n) scanf("%d", &n)
#define dout(n) printf("%d\n", n)
#define sll(n) scanf("%lld", &n)
#define lldout(n) printf("%lld\n", n)
#define fast_io                                                                \
  ios_base::sync_with_stdio(false);                                            \
  cin.tie(NULL)

#define TRACE

#ifdef TRACE
#define trace(...) __f(#__VA_ARGS__, __VA_ARGS__)
template <typename Arg1> void __f(const char *name, Arg1 &&arg1) {
  cerr << name << " : " << arg1 << std::endl;
}
template <typename Arg1, typename... Args>
void __f(const char *names, Arg1 &&arg1, Args &&... args) {
  const char *comma = strchr(names + 1, ',');
  cerr.write(names, comma - names) << " : " << arg1 << " | ";
  __f(comma + 1, args...);
}
#else
#define trace(...)
#endif

// FILE *fin = freopen("in","r",stdin);
// FILE *fout = freopen("out","w",stdout);

const int N = 300005;
const LL mod = 1000000007;

LL fact[N];
LL power(LL a, LL b) {
  if (b == 0)
    return 1;
  LL tmp = power(a, b / 2);
  tmp = (tmp * tmp) % mod;
  if (b % 2 == 1)
    tmp = (tmp * a) % mod;
  return tmp;
}
void pre() {
  fact[0] = 1;
  for (int i = 1; i < N; i++) {
    fact[i] = (fact[i - 1] * i) % mod;
  }
}
LL A[N];         // input
LL missing[N];   // no. of missing in postions i to n
LL missarray[N]; // no. of missing from i to n from the array globally.
LL smallerMissing[N];
LL postmissarray[N];
LL filledahead[N]; // no. fillead ahead in array and smaller. use some data
                    // structure to find
int vis[N];
int BIT[N];
int query(int x) {
  int ret = 0;
  for (; x; x = x & (x - 1))
    ret += BIT[x];
  return ret;
}
void update(int x) {
  for (; x < N; x += x ^ (x & (x - 1)))
    BIT[x]++;
}

int main() {
  pre();
  int n, m = 0;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> A[i];
    if (A[i])
      vis[A[i]] = 1;
    else
      m++;
  }
  for (int i = n; i >= 1; i--) {
    filledahead[i] = query(A[i]);
    if (A[i])
      update(A[i]);
  }
  for (int i = n; i >= 1; i--) {
    if (A[i] == 0)
      missing[i] = missing[i + 1] + 1;
    else
      missing[i] = missing[i + 1];
  }
  for (int i = n; i >= 1; i--) {
    if (vis[i] == 0)
      missarray[i] = missarray[i + 1] + 1;
    else
      missarray[i] = missarray[i + 1];
    smallerMissing[i] = m - missarray[i];
    if (!vis[i])
      smallerMissing[i]++;
  }
  for (int i = n; i >= 1; i--) {
    postmissarray[i] = (postmissarray[i + 1] + missarray[A[i]]) % mod;
  }
  /*
     compute filled ahead using some data structure
     */
  LL ans = 0;
  // part 1
  ans = (ans + fact[m]) % mod;
  // part 2
  for (int i = 1; i <= n; i++) {
    if (A[i] == 0) {
      LL mm = missing[i];
      LL tmp = mm * (mm - 1);
      tmp /= 2;
      tmp %= mod;
      tmp = (tmp * fact[n - i]) % mod;
      tmp = (tmp * fact[m]) % mod;
      tmp = (tmp * power(mm, mod - 2)) % mod;
      ans = (ans + tmp) % mod;
    }
  }

  // part 3
  for (int i = 1; i <= n; i++) {
    if (A[i] == 0) {
      LL tmp = fact[m - 1];
      tmp = (tmp * fact[n - i]) % mod;
      tmp = (tmp * postmissarray[i]) % mod;
      ans = (ans + tmp) % mod;
    }
  }
  // part 4
  for (int i = 1; i <= n; i++) {
    if (A[i]) {
      LL temp = fact[m];
      temp = (temp * filledahead[i]) % mod;
      temp = (temp * fact[n - i]) % mod;
      ans = (ans + temp) % mod;
    }
  }
  // part 5
  for (int i = 1; i <= n; i++) {
    if (A[i]) {
      LL temp = (smallerMissing[A[i]] * missing[i]) % mod;
      temp = (temp * fact[n - i]) % mod;
      if (m)
        temp = (temp * fact[m - 1]) % mod;
      ans = (ans + temp) % mod;
    }
  }
  lldout(ans);
  return 0;
}
