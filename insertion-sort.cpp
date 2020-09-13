#include<iostream>
#include<string.h>
#include<string>
#include<vector>
#include<map>
#include<queue>
#include<deque>
#include<set>
#include<list>
#include<stack>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<cassert>
#define CLRM(x) memset(x,-1,sizeof(x))
#define CLR(x) memset(x,0,sizeof(x))
#define ALL(x) x.begin(),x.end()
#define GI(x) scanf("%d", &x);
#define FORN(i, n) for(int i = 0; i < n; i++)
#define FOR(i, start, end) for(int i = start; i < end; i++)
#define PB push_back
#define MP make_pair
#define VI vector<int> 
#define VVI vector<vector<int> >
#define PII pair<int,int>
#define SZ(x) (int)x.size()
#define LL long long
#define MIN(a,b) (a)<(b)?(a):(b)
#define MAX(a,b) (a)>(b)?(a):(b)
#define LMAX 1000000000000000000LL
#define IMAX 1000000000
using namespace std;

#define MAXN 110000
int N;
int d[MAXN];

LL mergesort(int low, int high)
{
	if(low >= high)
		return 0;
	
	int mid = (low+high)/2;

	LL ret = 0;
	ret = mergesort(low, mid) + mergesort(mid+1, high);
	vector<int> v;
	int i, j, k;
	i = low; j = mid + 1;
	while(i <= mid && j <= high)
	{
		if(d[i] > d[j])
		{
			v.PB(d[j]);
			ret+=(LL)(mid-i+1);
			j++;
		}
		else
		{
			v.PB(d[i]);
			i++;
		}
	}
	while(i <= mid)
	{
		v.PB(d[i]);
		i++;
	}
	while(j <= high)
	{
		v.PB(d[j]);
		j++;
	}
	for(i = 0; i < v.size(); i++)
	{
		d[i + low] = v[i];
	}
	return ret;
}
LL solve()
{
	LL ret = mergesort(0, N-1);
	return ret;
}
int main()
{
	int tes;
	GI(tes);
	while(tes--)
	{
		int i;
		GI(N);
		FORN(i, N)
		{
			GI(d[i]);
		}
		LL ans = solve();
		printf("%lld\n", ans);
	}
	return 0;
}
