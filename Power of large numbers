#include <bits/stdc++.h>
#define ___ ios_base::sync_with_stdio(false);cin.tie(NULL);
#define ll long long
#define ull unsigned long long


using namespace std;

ull cal(ull b,ull p,ull div)
{
    ull k=1,l=b;
    while(p)
    {
        if(p & 1)
        {
            k=(k*l)%div;
        }
        l=(l*l)%div;
        p=p/2;
    }
    return k;
}

int main()
{
    int t;
    ull res,m,n,div=1000000007,div2;
    string a,b;
    cin>>t;
    while(t--)
    {
        cin>>a>>b;
        m=a.size();
        n=b.size();

        ull base=0,power=0;

        for(ull i=0; i<m; i++)
        {
            base=((base*10)+(a[i]-'0'))%div;
        }
        div2=div-1;
        for(ull i=0; i<n; i++)
        {
            power=((power*10)+(b[i]-'0'))%div2;
        }
        if(power==0)
            cout<<1<<endl;
        else
        {
            res=cal(base,power,div);
            cout<<res<<endl;
        }


    }

    return 0;
}
