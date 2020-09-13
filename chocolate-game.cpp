#include <string>
#include <vector>
#include <map>
#include <list>
#include <iterator>
#include <set>
#include <queue>
#include <iostream>
#include <sstream>
#include <stack>
#include <deque>
#include <cmath>
#include <memory.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <utility> 
using namespace std;
 
#define FOR(i, a, b) for(int i = (a); i < (b); ++i)
#define RFOR(i, b, a) for(int i = (b) - 1; i >= (a); --i)
#define REP(i, N) FOR(i, 0, N)
#define RREP(i, N) RFOR(i, N, 0)
 
#define ALL(V) V.begin(), V.end()
#define SZ(V) (int)V.size()
#define PB push_back
#define MP make_pair
#define Pi 3.14159265358979

typedef long long Int;
typedef unsigned long long UInt;
typedef vector <int> VI;
typedef pair <int, int> PII;

map<VI, bool> win;
int n;

bool solve(VI v)
{
	if (win.count(v))
		return win[v];

	bool ans = false;
	REP(i,SZ(v))
	{
		if (i == 0 || v[i] > v[i-1])
		{
			int prev = i == 0 ? 0 : v[i-1];
			int pp = v[i];
			FOR(j,prev, v[i])
			{
				v[i] = j;
				if (!solve(v))
					ans = true;
				v[i] = pp;
			}
		}
	}
	
	return win[v] = ans;
}

int a[1<<20];

int main()
{	
//	freopen("in.txt", "r", stdin);
	
	while (scanf("%d", &n) == 1)
	{
		REP(i,n)
		{
			scanf("%d",&a[i]);
		}
		
		sort(a,a+n);
		
		Int res = 0;
		map<int,int> Map;
		int x = 0;
		for (int i = n-1; i - 1 >= 0; i -= 2)
		{
			++Map[x];
			x ^= a[i] - a[i-1];
			
//			if (Map[x] > 0)
//			{
//				cout << "Start: " << i - 1 << " has " << Map[x] << " ends" << endl;
//			}
			res += Map[x];
		}
		
		Map.clear();
		x = 0;
		for (int i = n-1; i >= 0; i -= 2)
		{
			++Map[x];
//			if (Map[x^a[i]] > 0)
//			{
//				cout << "Start: " << i << " has " << Map[x ^ a[i]] << " ends" << endl;
//			}
			res += Map[x ^ a[i]];
			if (i)
				x ^= a[i] - a[i-1];
		}
		
		Map.clear();
		--n;
		x = 0;
		for (int i = n-1; i - 1 >= 0; i -= 2)
		{
			++Map[x];
			x ^= a[i] - a[i-1];
//			if (Map[x] > 0)
//			{
//				cout << "Start: " << i - 1 << " has " << Map[x] << " ends" << endl;
//			}
			res += Map[x];
		}
		
		Map.clear();
		x = 0;
		for (int i = n-1; i >= 0; i -= 2)
		{
			++Map[x];
			res += Map[x ^ a[i]];
//			if (Map[x^a[i]] > 0)
//			{
//				cout << "Start: " << i << " has " << Map[x ^ a[i]] << " ends" << endl;
//			}
			if (i)
				x ^= a[i] - a[i-1];
		}
		++n;
		
		cout << 1LL*n*(n-1)/2 - res << endl;
	}
	
	return 0;
}
