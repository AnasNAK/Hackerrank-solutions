#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <map>
using namespace std;

#define MAXN 100010
#define mod  1000000007
typedef long long LL;
typedef pair<LL,LL> PLL;

struct Mat
{
    int a[3][3];///last row is always 0 0 1
    void zero() {memset(a,0,sizeof(a));}
    void one()  {zero();for(int i=0;i<3;i++) a[i][i]=1;}
};
Mat operator*(const Mat &A,const Mat &B)
{
    Mat C;C.zero();
    for(int k=0;k<3;k++)
    {
        for(int i=0;i<2;i++)
        for(int j=0;j<3;j++)
        C.a[i][j] = ((LL)C.a[i][j]+(LL)A.a[i][k]*(LL)B.a[k][j])%mod;
    }
    C.a[2][2]=1;
    return C;
}
Mat Pow(Mat Mul,int pw)
{
    Mat ret;ret.one();
    while(pw)
    {
        if(pw&1) ret=ret*Mul;
        Mul=Mul*Mul;
        pw>>=1;
    }
    return ret;
}


struct segtree
{
    int l,r;
    LL lazy[2];
    LL SUMLToR[2],SUMRToL[2];
    ///ADD SUMLRA,SUMLRB: Left->Right
    ///ADD SUMRLA,SUMRLB: Right->Left
}t[MAXN*4+200];


int siz[MAXN],top[MAXN],fa[MAXN],son[MAXN],id[MAXN],dep[MAXN],pnum;//son ???????id???,pnum ????
vector<int> link1[MAXN];

int initvalue[MAXN][2],M,N,value[MAXN][2];

int STACK[MAXN],TOP,F[MAXN];

void dfs1(int x,int f)
{
    memset(F,-1,sizeof(int)*N);
    TOP=0;
    STACK[++TOP]=0;

    while(TOP)
    {
        x=STACK[TOP];
        f=F[x];
        if(f>=-1)
        {
            fa[x]=f;

            dep[x]=(f==-1?0:dep[f]+1);


            int len=link1[x].size(),v;
            for(int i=0;i<len;i++)
            {
                v=link1[x][i];
                if(f==v) continue;
                STACK[++TOP]=v;
                F[v]=x;
            }
            F[x]=-100;//visit 1
        }
        else
        {
             siz[x]=1;
             son[x]=-1;
             f=fa[x];
             int bestsiz=0;
             int len=link1[x].size(),v;
             for(int i=0;i<len;i++)
             {
                 v=link1[x][i];
                 if(f==v) continue;
                 siz[x]+=siz[v];
                 if(siz[v]>bestsiz)
                 {
                    son[x]=v;
                    bestsiz=siz[v];
                    swap(link1[x][0],link1[x][i]);//??dfs2???????
                 }
             }
             --TOP;
        }
    }
}
void dfs2(int x,int f)
{
    memset(F,-1,sizeof(int)*N);
    TOP=0;
    STACK[++TOP]=0;

    while(TOP)
    {
        x=STACK[TOP];
        f=F[x];
        if(f>=-1)
        {
            if(f==-1||son[fa[x]]!=x) top[x]=x;//??
            else top[x]=top[fa[x]];//????
            id[x]=pnum++;//????????
            value[id[x]][0]=initvalue[x][0];//??????
            value[id[x]][1]=initvalue[x][1];
            int len=link1[x].size(),v;
            for(int i=len-1;i>=0;i--)
            {
                v=link1[x][i];
                if(f==v) continue;
                STACK[++TOP]=v;
                F[v]=x;
            }

            F[x]=-100;
        }
        else TOP--;
    }
}

void build(int l,int r,int p)
{
    t[p].l=l;
    t[p].r=r;
    t[p].lazy[0]=-1;
    t[p].lazy[1]=-1;
    if(l==r)
    {
        t[p].SUMRToL[0] = t[p].SUMLToR[0] = value[l][0];
        t[p].SUMRToL[1] = t[p].SUMLToR[1] = value[l][1];
        return;
    }
    int mid=(l+r)>>1;
    build(l,mid,p<<1);
    build(mid+1,r,(p<<1)+1);
    ///L->R:fR(fL)
    t[p].SUMLToR[0] =  t[p<<1].SUMLToR[0]*t[1+(p<<1)].SUMLToR[0]%mod;
    t[p].SUMLToR[1] = (t[p<<1].SUMLToR[1]*t[1+(p<<1)].SUMLToR[0] + t[1+(p<<1)].SUMLToR[1])%mod;
    ///R->L:fL(fR)
    t[p].SUMRToL[0] =  t[p<<1].SUMRToL[0]*t[1+(p<<1)].SUMRToL[0]%mod;
    t[p].SUMRToL[1] = (t[p<<1].SUMRToL[0]*t[1+(p<<1)].SUMRToL[1] + t[p<<1].SUMRToL[1])%mod;
}
void dealcal(int p)
{
    Mat Mul,Ret;
    Mul.zero();
    Mul.a[1][1] = Mul.a[0][0] = t[p].lazy[0];
    Mul.a[1][2] = t[p].lazy[1];
    Mul.a[2][2] = 1;
    Ret = Pow(Mul,t[p].r-t[p].l+1);
    t[p].lazy[0] = t[p].lazy[1] = -1;
    t[p].SUMRToL[0] = t[p].SUMLToR[0] = (Ret.a[0][0] + Ret.a[0][2])%mod;
    t[p].SUMRToL[1] = t[p].SUMLToR[1] = (Ret.a[1][0] + Ret.a[1][2])%mod;
}
void deal(int p)
{
    if(t[p].l!=t[p].r)
    {
        t[p<<1].lazy[0]     = t[p].lazy[0];
        t[p<<1].lazy[1]     = t[p].lazy[1];
        t[1+(p<<1)].lazy[0] = t[p].lazy[0];
        t[1+(p<<1)].lazy[1] = t[p].lazy[1];
    }
    dealcal(p);
}
PLL query(int l,int r,int p,int LabLR)///LabLR=0:L->R;LabLR=1:R->L
{
    ///cout<<l<<" "<<r<<" "<<p<<" "<<LabLR<<endl;
    if(t[p].lazy[0]!=-1)
    {
        ///cout<<"QJ: "<< t[p].lazy[0]<<" "<<t[p].lazy[1]<<endl;
        deal(p);
    }
    if(t[p].l==l&&t[p].r==r)
    {
        if(LabLR==0) return PLL(t[p].SUMLToR[0],t[p].SUMLToR[1]);
        else         return PLL(t[p].SUMRToL[0],t[p].SUMRToL[1]);
    }
    int mid = (t[p].l+t[p].r)>>1;
    if(r<=mid) return query(l,r,p<<1,LabLR);
    if(l>mid ) return query(l,r,1+(p<<1),LabLR);
    PLL LAB = query(l,mid,p<<1,LabLR);
    PLL RAB = query(mid+1,r,1+(p<<1),LabLR);
    if(LabLR==0)
    {
        ///L->R:fR(fL)
        return PLL(LAB.first*RAB.first%mod , (LAB.second*RAB.first+RAB.second)%mod);
    }
    else
    {
        ///R->L:fL(fR)
        return PLL(RAB.first*LAB.first%mod , (RAB.second*LAB.first+LAB.second)%mod);
    }
}

void changeFunc(int l,int r,int p,int A,int B)
{
    ///cout<<"change: "<<l<<" "<<r<<" "<<p<<" "<<A<<" "<<B<<endl;
    if(t[p].l==l&&t[p].r==r)
    {
        t[p].lazy[0] = A;
        t[p].lazy[1] = B;
        deal(p);
        ///cout<<"end: "<<l<<" "<<r<<" "<<p<<" "<<A<<" "<<B<<" => ";
        ///cout<<t[p].SUMLToR[0]<<" "<<t[p].SUMLToR[1]<<" : "<<t[p].SUMRToL[0]<<" "<<t[p].SUMRToL[1]<<endl;
        return;
    }
    if(t[p].lazy[0]!=-1) deal(p);
    int mid = (t[p].l+t[p].r)>>1;
    if(r<=mid) changeFunc(l,r,p<<1,A,B);
    else if(l>mid) changeFunc(l,r,1+(p<<1),A,B);
    else
    {
        changeFunc(l,mid,p<<1,A,B);
        changeFunc(mid+1,r,1+(p<<1),A,B);
    }
    if(t[p<<1].lazy[0]!=-1) deal(p<<1);
    if(t[1+(p<<1)].lazy[0]!=-1) deal(1+(p<<1));
    ///L->R:
    t[p].SUMLToR[0] =  t[p<<1].SUMLToR[0]*t[1+(p<<1)].SUMLToR[0]%mod;
    t[p].SUMLToR[1] = (t[p<<1].SUMLToR[1]*t[1+(p<<1)].SUMLToR[0] + t[1+(p<<1)].SUMLToR[1])%mod;
    ///R->L:
    t[p].SUMRToL[0] =  t[p<<1].SUMRToL[0]*t[1+(p<<1)].SUMRToL[0]%mod;
    t[p].SUMRToL[1] = (t[p<<1].SUMRToL[0]*t[1+(p<<1)].SUMRToL[1] + t[p<<1].SUMRToL[1])%mod;
    ///cout<<"end: "<<l<<" "<<r<<" , "<<t[p].l<<" "<<t[p].r<<" , "<<A<<" "<<B<<" => ";
    ///cout<<t[p].SUMLToR[0]<<" "<<t[p].SUMLToR[1]<<" : "<<t[p].SUMRToL[0]<<" "<<t[p].SUMRToL[1]<<endl;
}

void updata(int u,int v,int A,int B)
{
    ///cout<<"-********-------"<<endl;
    int fu=top[u],fv=top[v];
    while(fu!=fv)
    {
        ///cout<<"up : " <<fu<<" "<<fv<<endl;
        if(dep[fu]>=dep[fv])
        {
            changeFunc(id[fu],id[u],1,A,B);
            u=fa[fu];fu=top[u];
        }
        else
        {
            changeFunc(id[fv],id[v],1,A,B);
            v=fa[fv];fv=top[v];
        }
    }
    u=id[u];v=id[v];
    if(u>v) swap(u,v);
    changeFunc(u,v,1,A,B);
}

LL QueryFunctions(int u,int v,int X)///u->v:fv(...fu(X)...)
{
    ///to u: always u->fu (Lab:R->L)
    ///to v: always fv->v (Lab:L->R)
    PLL Lu=PLL(1,0);
    PLL Lv=PLL(1,0);
    PLL tmp;
    int fu=top[u],fv=top[v];
    while(fu!=fv)
    {
        ///cout<<fu<<" "<<fv<<endl;
        if(dep[fu]>=dep[fv])
        {
            tmp = query(id[fu],id[u],1,1);
            Lu = PLL(Lu.first*tmp.first%mod , (Lu.second*tmp.first+tmp.second)%mod );
            u=fa[fu];fu=top[u];
        }
        else
        {
            tmp = query(id[fv],id[v],1,0);
            Lv = PLL(Lv.first*tmp.first%mod , (tmp.second*Lv.first+Lv.second)%mod );
            v=fa[fv];fv=top[v];
        }
    }
    u=id[u];v=id[v];
    if(v<=u)
    {
        tmp = query(v,u,1,1);
        Lu = PLL(Lu.first*tmp.first%mod , (Lu.second*tmp.first+tmp.second)%mod );
    }
    else
    {
        tmp = query(u,v,1,0);
        ///cout<<"u<v : "<<tmp.first<<" "<<tmp.second<<endl;
        Lv  = PLL(Lv.first*tmp.first%mod , (tmp.second*Lv.first+Lv.second)%mod );
    }
    PLL AB = PLL(Lv.first*Lu.first%mod , (Lu.second*Lv.first+Lv.second)%mod );
    return (AB.first*X+AB.second)%mod;
}

void init()
{
    dfs1(0,-1);
    pnum=0;
    dfs2(0,-1);
    ///cout<<"pnum: "<<pnum<<endl;
    ///for(int i=0;i<N;i++) cout<<id[i]<<" is id of "<<i<<endl;
    ///for(int i=0;i<N;i++) cout<<i<<" son:"<<son[i]<<" top:"<<top[i]<<" dep:"<<dep[i]<<" fa:"<<fa[i]<<endl;
    build(0,N-1,1);
}

int main()
{
    ///freopen("in.txt","r",stdin);
    int u,v,a,b,type,X;
    while(scanf("%d",&N)!=EOF)
    {
        for(int i=0;i<N;i++)
        {
            scanf("%d%d",&initvalue[i][0],&initvalue[i][1]);
            link1[i].clear();
        }
        M=N-1;
        for(int i=0;i<M;i++)
        {
            scanf("%d%d",&u,&v);
            u--;v--;
            link1[u].push_back(v);
            link1[v].push_back(u);
        }
        init();

        scanf("%d",&M);
        while(M--)
        {
            scanf("%d",&type);
            if(type==1)
            {

                scanf("%d%d%d%d",&u,&v,&a,&b);
                u--;v--;
                updata(u,v,a,b);
            }
            else
            {
                scanf("%d%d%d",&u,&v,&X);
                u--;v--;
                int ans = QueryFunctions(u,v,X);
                ans = (ans+mod)%mod;
                printf("%d\n",ans);
            }
        }
        ///cout<<"========="<<endl;
    }
    return 0;
}
