#pragma GCC optimize("O3,unroll-loops,inline")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
using ll=long long;
using ld=long double;
#define pb push_back
#define fi first
#define se second

const int N=2e5+5;
const int M=1e9+7;
const ll INF=4e18;

////////////////////////////////////////////////////////
//////////////////// FAST IO ///////////////////////////
////////////////////////////////////////////////////////
/*
Input cực lớn.
read(x);
write(x);
write(x,' ');
flush();
*/

const int S=1<<20;
char ib[S],ob[S];
int ip=S,iq=S,op=0;

inline char gc(){
    if(ip==iq){
        iq=fread(ib,1,S,stdin);
        ip=0;
        if(!iq)return EOF;
    }
    return ib[ip++];
}
inline void pc(char c){
    if(op==S){
        fwrite(ob,1,S,stdout);
        op=0;
    }
    ob[op++]=c;
}
inline void flush(){
    fwrite(ob,1,op,stdout);
    op=0;
}
template<class T>
inline void read(T &x){
    x=0;
    bool neg=0;
    char c=gc();
    while(c<'0'||c>'9'){
        if(c=='-')neg=1;
        c=gc();
    }
    while(c>='0'&&c<='9'){
        x=x*10+c-'0';
        c=gc();
    }
    if(neg)x=-x;
}
template<class T>
inline void write(T x,char c='\n'){
    if(x<0){
        pc('-');
        x=-x;
    }
    char s[25];
    int n=0;
    do{
        s[n++]=x%10+'0';
        x/=10;
    }while(x);
    while(n--)pc(s[n]);
    if(c)pc(c);
}

////////////////////////////////////////////////////////
//////////////////// RANDOM ////////////////////////////
////////////////////////////////////////////////////////
/*
rnd(l,r)
*/

static uint64_t seed=chrono::steady_clock::now().time_since_epoch().count();
inline uint64_t splitmix64(){
    uint64_t z=(seed+=0xdeadbeefc0ffee67);
    z=(z^(z>>30))*0x1770136769420bad;
    z=(z^(z>>27))*0xb16b00ba55331994;
    return z^(z>>31);
}
inline ll rnd(ll l,ll r){
    return l+splitmix64()%(r-l+1);
}

////////////////////////////////////////////////////////
//////////////////// BIT ///////////////////////////////
////////////////////////////////////////////////////////
/*
pc(x)      : số bit 1
lg(x)      : bit cao nhất
ctz(x)     : bit 1 thấp nhất
bt(x,i)    : lấy bit i
on(x,i)    : bật bit i
off(x,i)   : tắt bit i
flip(x,i)  : đảo bit i

for(int s=mask;s;s=(s-1)&mask) // duyệt mọi submask

bitset<1000> bs;
bs.set(i);
bs.reset(i);
bs.flip(i);
bs[i];
bs.count();
bs.any();
bs.none();
bs.all();
*/

inline int pc(ll x){return __builtin_popcountll(x);}
inline int lg(ll x){return 63-__builtin_clzll(x);}
inline int ctz(ll x){return __builtin_ctzll(x);}
inline bool bt(ll x,int i){return(x>>i)&1;}
inline ll on(ll x,int i){return x|(1LL<<i);}
inline ll off(ll x,int i){return x&(~(1LL<<i));}
inline ll flip(ll x,int i){return x^(1LL<<i);}

////////////////////////////////////////////////////////
//////////////// DATA STRUCTURES ///////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
/////////////////////// DSU ////////////////////////////
////////////////////////////////////////////////////////
/*
Quản lí các tập hợp rời nhau.

Init:
for(int i=1;i<=n;i++){
    p[i]=i;
    sz[i]=1;
}

fd(x) tìm gốc của x
un(a,b) gộp 2 tập chứa a,b
*/

int p[N],sz[N];

int fd(int x){
    return p[x]==x?x:p[x]=fd(p[x]);
}

void un(int a,int b){
    a=fd(a);
    b=fd(b);
    if(a==b){
    	return;
    }
    if(sz[a]<sz[b]){
    	swap(a,b);
    }
    p[b]=a;
    sz[a]+=sz[b];
}

////////////////////////////////////////////////////////
//////////////////// FENWICK ///////////////////////////
////////////////////////////////////////////////////////
/*
Point Update
Prefix Sum

1-index

add(i,v) cộng v vào vị trí i

sum(i) tổng đoạn [1..i]

sum(r)-sum(l-1) tổng đoạn [l..r]
*/

ll bit[N];

void add(int i,ll v){
    for(;i<N;i+=i&-i)bit[i]+=v;
}

ll sum(int i){
    ll s=0;
    for(;i;i-=i&-i)s+=bit[i];
    return s;
}

ll query(int l,int r){
    return sum(r)-sum(l-1);
}

////////////////////////////////////////////////////////
//////////////////// FENWICK 2D ///////////////////////////
////////////////////////////////////////////////////////

ll bit2d[1005][1005];
void add2d(int x, int y, ll v) {
    for (int i = x; i < 1005; i += i & -i)
        for (int j = y; j < 1005; j += j & -j) bit2d[i][j] += v;
}
ll sum2d(int x, int y) {
    ll s = 0;
    for (int i = x; i > 0; i -= i & -i)
        for (int j = y; j > 0; j -= j & -j) s += bit2d[i][j];
    return s;
}

////////////////////////////////////////////////////////
//////////////// SEGMENT TREE //////////////////////////
////////////////////////////////////////////////////////
/*
Point Update
Range Query

build(1,1,n) xây cây

upd(1,1,n,pos,val)
gán a[pos]=val

get(1,1,n,l,r)
lấy kết quả đoạn [l..r]

Đổi merge() để dùng:
Sum
Min
Max
GCD
...
*/

ll a[N];
ll st[4*N];

inline ll merge(ll a,ll b){
    return a+b;
}

void build(int id,int l,int r){
    if(l==r){
        st[id]=a[l];
        return;
    }
    int mid=(l+r)>>1;
    build(id<<1,l,mid);
    build(id<<1|1,mid+1,r);
    st[id]=merge(st[id<<1],st[id<<1|1]);
}

void upd(int id,int l,int r,int p,ll v){
    if(l==r){
        st[id]=v;
        return;
    }
    int mid=(l+r)>>1;
    if(p<=mid)upd(id<<1,l,mid,p,v);
    else upd(id<<1|1,mid+1,r,p,v);
    st[id]=merge(st[id<<1],st[id<<1|1]);
}

ll get(int id,int l,int r,int u,int v){
    if(v<l||r<u)return 0;
    if(u<=l&&r<=v)return st[id];
    int mid=(l+r)>>1;
    return merge(get(id<<1,l,mid,u,v),get(id<<1|1,mid+1,r,u,v));
}

////////////////////////////////////////////////////////
////////////////// LAZY SEG TREE ///////////////////////
////////////////////////////////////////////////////////
/*
Range Update
Range Query

buildlazy(1,1,n)

upd_add(1,1,n,l,r,val)
cộng val cho đoạn [l..r]

upd_set(1,1,n,l,r,val)
gán val cho đoạn [l..r]

getlazy(1,1,n,l,r)
lấy tổng đoạn [l..r]

Đổi merge2()
apply_add()
apply_set()

để dùng cho:
Sum
Min
Max
...
*/

ll seg[4*N];
ll lazya[4*N];
ll lazys[4*N];
bool hasset[4*N];

inline ll merge2(ll a,ll b){
    return a+b;
}

void buildlazy(int id,int l,int r){
    lazya[id]=0;
    lazys[id]=0;
    hasset[id]=0;
    if(l==r){
        seg[id]=a[l];
        return;
    }
    int mid=(l+r)>>1;
    buildlazy(id<<1,l,mid);
    buildlazy(id<<1|1,mid+1,r);
    seg[id]=merge2(seg[id<<1],seg[id<<1|1]);
}

inline void apply_set(int id,int l,int r,ll v){
    seg[id]=(r-l+1)*v;
    lazys[id]=v;
    lazya[id]=0;
    hasset[id]=1;
}

inline void apply_add(int id,int l,int r,ll v){
    seg[id]+=(r-l+1)*v;
    if(hasset[id])lazys[id]+=v;
    else lazya[id]+=v;
}

void push(int id,int l,int r){
    if(l==r)return;
    int mid=(l+r)>>1;
    if(hasset[id]){
        apply_set(id<<1,l,mid,lazys[id]);
        apply_set(id<<1|1,mid+1,r,lazys[id]);
        hasset[id]=0;
    }
    if(lazya[id]){
        apply_add(id<<1,l,mid,lazya[id]);
        apply_add(id<<1|1,mid+1,r,lazya[id]);
        lazya[id]=0;
    }
}

void upd_add(int id,int l,int r,int u,int v,ll val){
    if(v<l||r<u)return;
    if(u<=l&&r<=v){
        apply_add(id,l,r,val);
        return;
    }
    push(id,l,r);
    int mid=(l+r)>>1;
    upd_add(id<<1,l,mid,u,v,val);
    upd_add(id<<1|1,mid+1,r,u,v,val);
    seg[id]=merge2(seg[id<<1],seg[id<<1|1]);
}

void upd_set(int id,int l,int r,int u,int v,ll val){
    if(v<l||r<u)return;
    if(u<=l&&r<=v){
        apply_set(id,l,r,val);
        return;
    }
    push(id,l,r);
    int mid=(l+r)>>1;
    upd_set(id<<1,l,mid,u,v,val);
    upd_set(id<<1|1,mid+1,r,u,v,val);
    seg[id]=merge2(seg[id<<1],seg[id<<1|1]);
}

ll getlazy(int id,int l,int r,int u,int v){
    if(v<l||r<u)return 0;
    if(u<=l&&r<=v)return seg[id];
    push(id,l,r);
    int mid=(l+r)>>1;
    return merge2(getlazy(id<<1,l,mid,u,v),getlazy(id<<1|1,mid+1,r,u,v));
}

////////////////////////////////////////////////////////
////////////////// SPARSE TABLE ////////////////////////
////////////////////////////////////////////////////////
/*
Static Range Query

build_sp(n)

rmq(l,r)
lấy min đoạn [l..r]

O(1) mỗi truy vấn

Chỉ dùng khi mảng KHÔNG update.

Đổi min thành max/gcd nếu cần.
*/

ll sp[20][N];

inline ll mergetable(ll a,ll b){
    return min(a,b);
}

void buildsp(int n){
    for(int i=1;i<=n;i++){
        sp[0][i]=a[i];
    }
    for(int j=1;j<20;j++){
        for(int i=1;i+(1<<j)-1<=n;i++){
            sp[j][i]=mergetable(sp[j-1][i],sp[j-1][i+(1<<(j-1))]);
        }
    }
}

ll rmq(int l,int r){
    int k = 31 - __builtin_clz(r - l + 1);
    return mergetable(sp[k][l],sp[k][r-(1<<k)+1]);
}

////////////////////////////////////////////////////////
/////////////////////// TRIE ///////////////////////////
////////////////////////////////////////////////////////
/*
Lưu nhiều xâu. Lowercase only

ins(s)
thêm xâu s

Có thể sửa thêm:
đếm số lần xuất hiện
đếm prefix
xoá xâu
*/

int tr[N][26];
int en[N];
int cnt[N];
int tt=1;

void ins(string s){
    int u=1;
    for(char c:s){
        int v=c-'a';
        if(!tr[u][v]){
            tr[u][v]=++tt;
        }
        u=tr[u][v];
        cnt[u]++;
    }
    en[u]++;
}

bool find(string s){
    int u=1;
    for(char c:s){
        int v=c-'a';
        if(!tr[u][v]){
            return 0;
        }
        u=tr[u][v];
    }
    return en[u];
}

void erase(string s){
    if(!find(s)){
        return;
    }
    int u=1;
    for(char c:s){
        int v=c-'a';
        u=tr[u][v];
        cnt[u]--;
    }
    en[u]--;
}

////////////////////////////////////////////////////////
//////////// COORDINATE COMPRESSION ////////////////////
////////////////////////////////////////////////////////
/*
compress(v)

0-index compressed.

Add +1 if you want 1-index.
*/

template<class T>
void compress(vector<T>&v){
    vector<T>b=v;
    sort(b.begin(),b.end());
    b.erase(unique(b.begin(),b.end()),b.end());
    for(auto &x:v){
        x=lower_bound(b.begin(),b.end(),x)-b.begin();
    }
}

////////////////////////////////////////////////////////
/////////////////// ORDERED SET ////////////////////////
////////////////////////////////////////////////////////
/*
Set có index.

find_by_order(k)
phần tử nhỏ thứ k (0-index)

order_of_key(x)
đếm số phần tử < x

erase(x)
xoá giá trị x
*/

template<class T>
using oset=tree<T,null_type,less<T>,rb_tree_tag,tree_order_statistics_node_update>;

////////////////////////////////////////////////////////
/////////////////// GP_HASH_TABLE //////////////////////
////////////////////////////////////////////////////////
/*
fmap<int,int> mp;

mp[x]=value

mp[x]

mp.find(x)

mp.erase(x)

mp.count(x)
*/

template<class K,class V>
using fmap=gp_hash_table<K,V>;

////////////////////////////////////////////////////////
//////////////////// GP_HASH_SET ///////////////////////
////////////////////////////////////////////////////////
/*
fset<int> s;

s.insert(x)

s.erase(x)

s.find(x)

s.count(x)
*/

template<class T>
using fset=gp_hash_table<T,null_type>;

////////////////////////////////////////////////////////
/////////////////////// GRAPH //////////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//////////////////// DFS + BFS /////////////////////////
////////////////////////////////////////////////////////
/*
Đồ thị không trọng số.

g[u].pb(v)

dfs(u)
duyệt thành phần liên thông.

bfs(s)
duyệt BFS từ s.
Có thể sửa để lưu khoảng cách.
*/

vector<int> g[N];
bool vis[N];

void dfs(int u){
    vis[u]=1;
    for(int v:g[u]){
        if(!vis[v]){
            dfs(v);
        }
    }
}

void bfs(int s){
    queue<int> q;
    q.push(s);
    vis[s]=1;
    while(q.size()){
        int u=q.front();
        q.pop();
        for(int v:g[u]){
            if(!vis[v]){
                vis[v]=1;
                q.push(v);
            }
        }
    }
}

////////////////////////////////////////////////////////
/////////////////////// GRID ///////////////////////////
////////////////////////////////////////////////////////
/*
Grid BFS / DFS.

'.' là ô đi được.

ok(x,y)

dfs_grid(x,y)

bfs_grid(sx,sy)

bfs_multi()
BFS từ nhiều nguồn cùng lúc.

Reset:
memset(vs,0,sizeof(vs));
*/

const int GX=1005;

char gr[GX][GX];
bool vs[GX][GX];
int dista[GX][GX];

int dx[4]={-1,1,0,0};
int dy[4]={0,0,-1,1};

// 8 hướng:
// int dx[8]={-1,-1,-1,0,0,1,1,1};
// int dy[8]={-1,0,1,-1,1,-1,0,1};

bool ok(int x,int y,int n,int m){
    return x>=1&&x<=n&&y>=1&&y<=m&&!vs[x][y];
}

void dfs_grid(int x,int y,int n,int m){
    vs[x][y]=1;
    for(int i=0;i<4;i++){
        int nx=x+dx[i];
        int ny=y+dy[i];
        if(ok(nx,ny,n,m)&&gr[nx][ny]=='.'){
            dfs_grid(nx,ny,n,m);
        }
    }
}

void bfs_grid(int sx,int sy,int n,int m){
    queue<pair<int,int>> q;
    q.push({sx,sy});
    vs[sx][sy]=1;
    dista[sx][sy]=0;

    while(q.size()){
        auto cur=q.front();
        q.pop();

        int x=cur.fi;
        int y=cur.se;

        for(int i=0;i<4;i++){
            int nx=x+dx[i];
            int ny=y+dy[i];

            if(ok(nx,ny,n,m)&&gr[nx][ny]=='.'){
                vs[nx][ny]=1;
                dista[nx][ny]=dista[x][y]+1;
                q.push({nx,ny});
            }
        }
    }
}

void bfs_multi(int n,int m){
    queue<pair<int,int>> q;

    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            if(gr[i][j]=='S'){
                q.push({i,j});
                vs[i][j]=1;
                dista[i][j]=0;
            }
        }
    }

    while(q.size()){
        auto cur=q.front();
        q.pop();

        int x=cur.fi;
        int y=cur.se;

        for(int i=0;i<4;i++){
            int nx=x+dx[i];
            int ny=y+dy[i];

            if(ok(nx,ny,n,m)&&gr[nx][ny]=='.'){
                vs[nx][ny]=1;
                dista[nx][ny]=dista[x][y]+1;
                q.push({nx,ny});
            }
        }
    }
}

////////////////////////////////////////////////////////
//////////////////// TOPO SORT /////////////////////////
////////////////////////////////////////////////////////
/*
Đồ thị có hướng DAG.

indeg[v]++
khi thêm cạnh.

topo(n)

Trả về thứ tự topo.

Nếu size<n
=> có chu trình.
*/

int indeg[N];

vector<int> topo(int n){
    queue<int> q;
    vector<int> res;

    for(int i=1;i<=n;i++){
        if(indeg[i]==0){
            q.push(i);
        }
    }

    while(q.size()){
        int u=q.front();
        q.pop();

        res.pb(u);

        for(int v:g[u]){
            indeg[v]--;
            if(indeg[v]==0){
                q.push(v);
            }
        }
    }

    return res;
}

////////////////////////////////////////////////////////
///////////////// SHORTEST PATH ////////////////////////
////////////////////////////////////////////////////////
/*
Đồ thị có trọng số.

adj[u].pb({v,w})

w là trọng số cạnh.
*/

vector<pair<int,ll>> adj[N];

////////////////////////////////////////////////////////
//////////////////// DIJKSTRA //////////////////////////
////////////////////////////////////////////////////////
/*
Đường đi ngắn nhất.

dijkstra(s,n)

Từ đỉnh s tới mọi đỉnh.

Khoảng cách lưu trong d[].

Không dùng khi có cạnh âm.

O((n+m)logn)
*/

ll d[N];

void dijkstra(int s,int n){
    for(int i=1;i<=n;i++){
        d[i]=4e18;
    }

    priority_queue<
        pair<ll,int>,
        vector<pair<ll,int>>,
        greater<pair<ll,int>>
    > q;

    d[s]=0;
    q.push({0,s});

    while(q.size()){
        auto cur=q.top();
        q.pop();

        ll du=cur.fi;
        int u=cur.se;

        if(du!=d[u]){
            continue;
        }

        for(auto e:adj[u]){
            int v=e.fi;
            ll w=e.se;

            if(d[v]>d[u]+w){
                d[v]=d[u]+w;
                q.push({d[v],v});
            }
        }
    }
}

////////////////////////////////////////////////////////
///////////////////// 0-1 BFS //////////////////////////
////////////////////////////////////////////////////////
/*
Đường đi ngắn nhất.

Cạnh chỉ có trọng số 0 hoặc 1.

bfs01(s,n)

Khoảng cách lưu trong d01[].

O(n+m)
*/

ll d01[N];

void bfs01(int s,int n){
    for(int i=1;i<=n;i++){
        d01[i]=4e18;
    }

    deque<int> q;

    q.push_front(s);
    d01[s]=0;

    while(q.size()){
        int u=q.front();
        q.pop_front();

        for(auto e:adj[u]){
            int v=e.fi;
            ll w=e.se;

            if(d01[v]>d01[u]+w){
                d01[v]=d01[u]+w;

                if(w==0){
                    q.push_front(v);
                }
                else{
                    q.push_back(v);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////
/////////////////////// LCA ////////////////////////////
////////////////////////////////////////////////////////
/*
Lowest Common Ancestor.

dfs_lca(root,0,n)

lca(u,v)

Trả về tổ tiên chung gần nhất.

Độ sâu lưu trong dep[].

O(logn)
*/

int up[N][20];
int dep[N];

void dfs_lca(int u,int p,int n){
    up[u][0]=p;

    for(int i=1;i<20;i++){
        up[u][i]=up[up[u][i-1]][i-1];
    }

    for(int v:g[u]){
        if(v==p){
            continue;
        }

        dep[v]=dep[u]+1;
        dfs_lca(v,u,n);
    }
}

int lca(int a,int b){
    if(dep[a]<dep[b]){
        swap(a,b);
    }

    int k=dep[a]-dep[b];

    for(int i=0;i<20;i++){
        if(k&(1<<i)){
            a=up[a][i];
        }
    }

    if(a==b){
        return a;
    }

    for(int i=19;i>=0;i--){
        if(up[a][i]!=up[b][i]){
            a=up[a][i];
            b=up[b][i];
        }
    }

    return up[a][0];
}

////////////////////////////////////////////////////////
//////////////////// EULER TOUR ////////////////////////
////////////////////////////////////////////////////////
/*
Euler Tour.

dfs_euler(root,0)

tin[u]
thời điểm vào.

tout[u]
thời điểm ra.

Node u nằm trong subtree v
<=> tin[v]<=tin[u]<=tout[v]
*/

int tin[N];
int tout[N];
int timer=0;

void dfs_euler(int u,int p){
    tin[u]=++timer;

    for(int v:g[u]){
        if(v!=p){
            dfs_euler(v,u);
        }
    }

    tout[u]=timer;
}

////////////////////////////////////////////////////////
//////////////// STRING ALGORITHMS /////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
/////////////////// ROLLING HASH ///////////////////////
////////////////////////////////////////////////////////
/*
Double Hash

build_hash(s)

get_hash(l,r)

Chuỗi 1-index.

Ví dụ:
string s;
cin>>s;
build_hash(s);

if(get_hash(2,5)==get_hash(7,10)){
    // hai đoạn bằng nhau
}

Độ phức tạp:
Build O(n)
Query O(1)
*/

const ll M1=1000000007;
const ll M2=1000000009;
const ll BASE=311;

ll h1[N],h2[N];
ll pw1[N],pw2[N];

void build_hash(string s){
    int n=s.size();
    s=" "+s;

    pw1[0]=1;
    pw2[0]=1;

    for(int i=1;i<=n;i++){
        pw1[i]=pw1[i-1]*BASE%M1;
        pw2[i]=pw2[i-1]*BASE%M2;

        h1[i]=(h1[i-1]*BASE+s[i])%M1;
        h2[i]=(h2[i-1]*BASE+s[i])%M2;
    }
}

pair<ll,ll> get_hash(int l,int r){
    ll x1=(h1[r]-h1[l-1]*pw1[r-l+1])%M1;
    if(x1<0)x1+=M1;

    ll x2=(h2[r]-h2[l-1]*pw2[r-l+1])%M2;
    if(x2<0)x2+=M2;

    return {x1,x2};
}

////////////////////////////////////////////////////////
//////////////////////// KMP ///////////////////////////
////////////////////////////////////////////////////////
/*
KMP.

build_kmp(s)

lps[i]
độ dài prefix=suffix lớn nhất.

Tìm pattern O(n).

Border.

Period.
*/

vector<int> lps;

void build_kmp(string s){
    int n=s.size();

    lps.assign(n,0);

    for(int i=1,j=0;i<n;i++){

        while(j&&s[i]!=s[j]){
            j=lps[j-1];
        }

        if(s[i]==s[j]){
            j++;
        }

        lps[i]=j;
    }
}

////////////////////////////////////////////////////////
/////////////////////// Z FUNCTION /////////////////////
////////////////////////////////////////////////////////
/*
Tìm độ dài tiền tố dài nhất bắt đầu tại mỗi vị trí.

O(n)

z=số kí tự đầu giống với s[i...]

build_z(s)

Ví dụ:
aabcaabxaaaz

z[0]=0
z[1]=1
z[4]=3

Ứng dụng:
- Pattern Matching
- Đếm prefix xuất hiện
- String Period
*/

vector<int> z;

void build_z(string s){
    int n=s.size();
    z.assign(n,0);
    int l=0,r=0;
    for(int i=1;i<n;i++){
        if(i<=r){
            z[i]=min(r-i+1,z[i-l]);
        }
        while(i+z[i]<n&&s[z[i]]==s[i+z[i]]){
            z[i]++;
        }
        if(i+z[i]-1>r){
            l=i;
            r=i+z[i]-1;
        }
    }
}

////////////////////////////////////////////////////////
//////////////////// MANACHER //////////////////////////
////////////////////////////////////////////////////////
/*
Tìm palindrome trong O(n)

build_manacher(s)

d1[i]=bán kính palindrome lẻ tại i
độ dài=là 2*d1[i]-1

d2[i]=bán kính palindrome chẵn
giữa i-1 và i
độ dài=2*d2[i]

Ví dụ:
"abacaba"

d1:
1 2 1 4 1 2 1

Ứng dụng:
- Longest Palindrome
- Đếm palindrome
- Check palindrome rất nhanh
*/

vector<int>d1,d2;

void build_manacher(string s){
    int n=s.size();

    d1.assign(n,0);
    int l=0,r=-1;

    for(int i=0;i<n;i++){
        int k=1;
        if(i<=r){
            k=min(d1[l+r-i],r-i+1);
        }
        while(i-k>=0&&i+k<n&&s[i-k]==s[i+k]){
            k++;
        }
        d1[i]=k;
        if(i+k-1>r){
            l=i-k+1;
            r=i+k-1;
        }
    }

    d2.assign(n,0);
    l=0;
    r=-1;

    for(int i=0;i<n;i++){
        int k=0;
        if(i<=r){
            k=min(d2[l+r-i+1],r-i+1);
        }
        while(i-k-1>=0&&i+k<n&&s[i-k-1]==s[i+k]){
            k++;
        }
        d2[i]=k;
        if(i+k-1>r){
            l=i-k;
            r=i+k-1;
        }
    }
}

////////////////////////////////////////////////////////
//////////////////////// MATH //////////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//////////////////// FAST POWER ////////////////////////
////////////////////////////////////////////////////////
/*
pw(a,k,mod)

Tính a^k mod mod

O(logk)
*/

ll mul_mod(ll a,ll b,ll mod){
    return (__int128)a*b%mod;
}

ll pw(ll a,ll k,ll mod){
    ll r=1;
    while(k){
        if(k&1)r=mul_mod(r,a,mod);
        a=mul_mod(a,a,mod);
        k>>=1;
    }
    return r;
}

////////////////////////////////////////////////////////
////////////////// MOD INVERSE /////////////////////////
////////////////////////////////////////////////////////
/*
inv(x)

Nghịch đảo modulo.

mod phải là số nguyên tố.

Ví dụ:
inv(5,M)
*/

ll inv(ll x,ll mod){
    return pw(x,mod-2,mod);
}

////////////////////////////////////////////////////////
//////////////// FACTORIAL + COMB //////////////////////
////////////////////////////////////////////////////////
/*
build_fact(n)

C(n,k)

Build O(n)

Query O(1)

Đổi M nếu cần.
*/

const int F=2000005;

ll fac[F];
ll ifac[F];

void build_fact(int n){
    fac[0]=1;

    for(int i=1;i<=n;i++){
        fac[i]=fac[i-1]*i%M;
    }

    ifac[n]=inv(fac[n],M);

    for(int i=n;i>=1;i--){
        ifac[i-1]=ifac[i]*i%M;
    }
}

ll C(int n,int k){
    if(k<0||k>n){
        return 0;
    }
    return fac[n]*ifac[k]%M*ifac[n-k]%M;
}

////////////////////////////////////////////////////////
/////////////////// LINEAR SIEVE ///////////////////////
////////////////////////////////////////////////////////
/*
build_sieve(n)

pr : danh sách số nguyên tố

lp[x] : ước nguyên tố nhỏ nhất của x

O(n)
*/

vector<int>pr;
int lp[N];

void build_sieve(int n){
    for(int i=2;i<=n;i++){
        if(!lp[i]){
            lp[i]=i;
            pr.pb(i);
        }
        for(int p:pr){
            if(p>lp[i]||1LL*i*p>n)break;
            lp[i*p]=p;
        }
    }
}

////////////////////////////////////////////////////////
//////////////// SEGMENTED SIEVE ///////////////////////
////////////////////////////////////////////////////////
/*
seg_sieve(L,R)

Trả về các số nguyên tố trong đoạn [L,R]

Cần build_sieve(sqrt(R)) trước.
*/

vector<ll>seg_sieve(ll L,ll R){
    vector<bool>mark(R-L+1);

    for(int p:pr){
        if(1LL*p*p>R)break;

        ll st=max(1LL*p*p,(L+p-1)/p*p);

        for(ll j=st;j<=R;j+=p){
            mark[j-L]=1;
        }
    }

    vector<ll>res;

    for(ll i=L;i<=R;i++){
        if(i>1&&!mark[i-L]){
            res.pb(i);
        }
    }

    return res;
}

////////////////////////////////////////////////////////
//////////////////// EULER PHI /////////////////////////
////////////////////////////////////////////////////////
/*
phi(n)

Số lượng số nguyên dương <=n
nguyên tố cùng nhau với n.

O(sqrt(n))
*/

ll phi(ll n){
    ll res=n;

    for(ll i=2;i*i<=n;i++){
        if(n%i==0){
            while(n%i==0)n/=i;
            res=res/i*(i-1);
        }
    }

    if(n>1){
        res=res/n*(n-1);
    }

    return res;
}

////////////////////////////////////////////////////////
//////////////////// DIVISORS //////////////////////////
////////////////////////////////////////////////////////
/*
get_div(n)

Trả về vector các ước
đã sắp xếp tăng dần.

O(sqrt(n))
*/

vector<ll>get_div(ll n){
    vector<ll>res;

    for(ll i=1;i*i<=n;i++){
        if(n%i==0){
            res.pb(i);
            if(i*i!=n){
                res.pb(n/i);
            }
        }
    }

    sort(res.begin(),res.end());

    return res;
}

////////////////////////////////////////////////////////
//////////// PRIME FACTORIZATION ///////////////////////
////////////////////////////////////////////////////////
/*
factor(n)

Trả về map<prime,exponent>

O(sqrt(n))
*/

map<ll,int>factor(ll n){
    map<ll,int>mp;

    for(ll i=2;i*i<=n;i++){
        while(n%i==0){
            mp[i]++;
            n/=i;
        }
    }

    if(n>1){
        mp[n]++;
    }

    return mp;
}

////////////////////////////////////////////////////////
//////////////// MATRIX EXPONENT ///////////////////////
////////////////////////////////////////////////////////
/*
mat_mul(a,b)

mat_pow(a,k)

Ma trận vuông.

Đổi M nếu cần.

matrix T = {{1, 1}, 
            {1, 0}};
            
matrix T_pow = mat_pow(T, n - 1);

ll Fn = T_pow[0][0]; // Kết quả F[n] % M

*/

using matrix=vector<vector<ll>>;

matrix mat_mul(matrix a,matrix b){
    int n=a.size();

    matrix c(n,vector<ll>(n));

    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            for(int j=0;j<n;j++){
                c[i][j]=(c[i][j]+a[i][k]*b[k][j])%M;
            }
        }
    }

    return c;
}

matrix mat_pow(matrix a,ll k){
    int n=a.size();

    matrix r(n,vector<ll>(n));

    for(int i=0;i<n;i++){
        r[i][i]=1;
    }

    while(k){
        if(k&1){
            r=mat_mul(r,a);
        }
        a=mat_mul(a,a);
        k>>=1;
    }

    return r;
}

////////////////////////////////////////////////////////
///////////////////// LAGRANGE /////////////////////////
////////////////////////////////////////////////////////
/*
lagrange(x,y,n)

Cho các điểm (x,y)

Tính f(n)

O(k²)
*/

ll lagrange(vector<ll>x,vector<ll>y,ll n){
    int k=x.size();

    for(int i=0;i<k;i++){
        if(x[i]==n){
            return y[i];
        }
    }

    ll ans=0;

    for(int i=0;i<k;i++){
        ll num=1;
        ll den=1;

        for(int j=0;j<k;j++){
            if(i==j)continue;

            num=num*(n-x[j])%M;
            den=den*(x[i]-x[j])%M;
        }

        ans=(ans+y[i]*num%M*inv(den,M))%M;
    }

    return (ans+M)%M;
}

////////////////////////////////////////////////////////
/////////////////// MILLER RABIN ///////////////////////
////////////////////////////////////////////////////////
/*
Check nguyen to toi 1e18

mr(n)

true -> prime
false -> composite

O(logn)
*/

bool mr(ll n){
    if(n<2)return 0;

    for(ll p:{2,3,5,7,11,13,17,19,23,29,31,37}){
        if(n%p==0)return n==p;
    }

    ll d=n-1,s=0;

    while((d&1)==0){
        d>>=1;
        s++;
    }

    for(ll a:{2,325,9375,28178,450775,9780504,1795265022}){

        if(a%n==0)continue;

        ll x=pw(a,d,n);

        if(x==1||x==n-1)continue;

        bool ok=0;

        for(int i=1;i<s;i++){
            x=mul_mod(x,x,n);
            if(x==n-1){
                ok=1;
                break;
            }
        }

        if(!ok)return 0;
    }

    return 1;
}

////////////////////////////////////////////////////////
//////////// PRECOMPUTE DIVISOR COUNT //////////////////
////////////////////////////////////////////////////////
/*
divcnt[i]=so uoc cua i

build_divcnt(maxn)

O(nlogn)
*/

int divcnt[N];

void build_divcnt(int n){
    for(int i=1;i<=n;i++){
        for(int j=i;j<=n;j+=i){
            divcnt[j]++;
        }
    }
}

////////////////////////////////////////////////////////
///////////////////// GEOMETRY /////////////////////////
////////////////////////////////////////////////////////

/*
Point = pair<ll,ll>

{x,y}

fi=x
se=y
*/

using pt=pair<ll,ll>;

////////////////////////////////////////////////////////
//////////////////// BASIC /////////////////////////////
////////////////////////////////////////////////////////
/*
dot(a,b)

cross(a,b)

cross(a,b,c)

dist2(a,b)

orientation(a,b,c)

0  : thang hang
1  : quay trai
-1 : quay phai
*/

ll dot(pt a,pt b){
    return a.fi*b.fi+a.se*b.se;
}

ll cross(pt a,pt b){
    return a.fi*b.se-a.se*b.fi;
}

ll cross(pt a,pt b,pt c){
    return (b.fi-a.fi)*(c.se-a.se)-(b.se-a.se)*(c.fi-a.fi);
}

ll dist2(pt a,pt b){
    ll dx=a.fi-b.fi;
    ll dy=a.se-b.se;
    return dx*dx+dy*dy;
}

int orientation(pt a,pt b,pt c){
    ll t=cross(a,b,c);
    if(t==0)return 0;
    if(t>0)return 1;
    return -1;
}

////////////////////////////////////////////////////////
/////////////////// ON SEGMENT /////////////////////////
////////////////////////////////////////////////////////
/*
onseg(a,b,p)

p nam tren doan ab
*/

bool onseg(pt a,pt b,pt p){
    if(cross(a,b,p)!=0)return 0;
    return min(a.fi,b.fi)<=p.fi&&p.fi<=max(a.fi,b.fi)
        &&min(a.se,b.se)<=p.se&&p.se<=max(a.se,b.se);
}

////////////////////////////////////////////////////////
//////////////// LINE INTERSECTION /////////////////////
////////////////////////////////////////////////////////
/*
intersect(a,b,c,d)

2 doan giao nhau

true
false
*/

bool intersect(pt a,pt b,pt c,pt d){

    int o1=orientation(a,b,c);
    int o2=orientation(a,b,d);
    int o3=orientation(c,d,a);
    int o4=orientation(c,d,b);

    if(o1!=o2&&o3!=o4)return 1;

    if(o1==0&&onseg(a,b,c))return 1;
    if(o2==0&&onseg(a,b,d))return 1;
    if(o3==0&&onseg(c,d,a))return 1;
    if(o4==0&&onseg(c,d,b))return 1;

    return 0;
}

////////////////////////////////////////////////////////
//////////////////// POLYGON AREA //////////////////////
////////////////////////////////////////////////////////
/*
polyarea(p)

dien tich da giac

return = area*2

Neu can area thuc:

abs(polyarea(p))/2.0
*/

ll polyarea(vector<pt>&p){
    int n=p.size();
    __int128 s=0;
    for(int i=0;i<n;i++){
        int j=(i+1)%n;
        s+=cross(p[i],p[j]);
    }
    return abs((ll)s);
}

using ptd=pair<ld,ld>;

////////////////////////////////////////////////////////
//////////////////// ROTATE POINT //////////////////////
////////////////////////////////////////////////////////
/*
rotate(p,rad)

Quay quanh goc O

rad = goc (radian)

deg*pi/180
*/

ptd rotate(ptd p,ld rad){

    ld c=cos(rad);
    ld s=sin(rad);

    return {
        p.fi*c-p.se*s,
        p.fi*s+p.se*c
    };
}

////////////////////////////////////////////////////////
//////////// DISTANCE POINT TO LINE ////////////////////
////////////////////////////////////////////////////////
/*
dist_line(p,a,b)

Khoang cach tu p den duong thang AB
*/

ld dist_line(ptd p,ptd a,ptd b){

    ld A=b.se-a.se;
    ld B=a.fi-b.fi;
    ld C=-(A*a.fi+B*a.se);

    return fabsl(A*p.fi+B*p.se+C)/sqrt(A*A+B*B);
}

////////////////////////////////////////////////////////
/////////////// PROJECTION TO LINE /////////////////////
////////////////////////////////////////////////////////
/*
projection(p,a,b)

Hinh chieu cua p len duong thang AB
*/

ptd projection(ptd p,ptd a,ptd b){

    ld dx=b.fi-a.fi;
    ld dy=b.se-a.se;

    ld t=((p.fi-a.fi)*dx+(p.se-a.se)*dy)/(dx*dx+dy*dy);

    return {
        a.fi+t*dx,
        a.se+t*dy
    };
}

////////////////////////////////////////////////////////
//////////////// REFLECT THROUGH LINE //////////////////
////////////////////////////////////////////////////////
/*
reflect(p,a,b)

Diem doi xung cua p qua duong thang AB
*/

ptd reflect(ptd p,ptd a,ptd b){

    ptd q=projection(p,a,b);

    return {
        2*q.fi-p.fi,
        2*q.se-p.se
    };
}

////////////////////////////////////////////////////////
//////////////////// SWEEP LINE ////////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
/////////////////// CLOSEST PAIR ///////////////////////
////////////////////////////////////////////////////////
/*
Tìm khoảng cách nhỏ nhất giữa 2 điểm.

O(nlogn)

closest(v)
v={ {x,y},... }
*/

struct P{
    ld x,y;
    bool operator<(const P&o)const{
        if(x!=o.x)return x<o.x;
        return y<o.y;
    }
};

ld closest(vector<P> v){
    sort(v.begin(),v.end());
    set<pair<ld,ld>> s;
    ld ans=1e100;
    int j=0;
    for(auto p:v){
        while(j<(int)v.size()&&v[j].x<p.x-ans){
            s.erase({v[j].y,v[j].x});
            j++;
        }
        auto it=s.lower_bound({p.y-ans,-1e100});
        while(it!=s.end()&&it->fi<=p.y+ans){
            ld dx=p.x-it->se;
            ld dy=p.y-it->fi;
            ans=min(ans,sqrt(dx*dx+dy*dy));
            ++it;
        }
        s.insert({p.y,p.x});
    }
    return ans;
}

////////////////////////////////////////////////////////
////////////////// BINARY SEARCH ///////////////////////
////////////////////////////////////////////////////////
/*
Tìm đáp án nhỏ nhất/lớn nhất thỏa điều kiện.

check(x)=true/false phải đơn điệu.

Lower Bound:
    tìm giá trị nhỏ nhất đúng.

Upper Bound:
    tìm giá trị lớn nhất đúng.

Double:
    lặp khoảng 70~100 lần.
*/

////////////////////////////////////////////////////////
/////////////////// LOWER BOUND ////////////////////////
////////////////////////////////////////////////////////
/*
first true

ans=-1

while(l<=r){
    mid=(l+r)>>1;
    if(check(mid)){
        ans=mid;
        r=mid-1;
    }else{
        l=mid+1;
    }
}
*/

////////////////////////////////////////////////////////
/////////////////// UPPER BOUND ////////////////////////
////////////////////////////////////////////////////////
/*
last true

ans=-1

while(l<=r){
    mid=(l+r)>>1;
    if(check(mid)){
        ans=mid;
        l=mid+1;
    }else{
        r=mid-1;
    }
}
*/

////////////////////////////////////////////////////////
/////////////////// LONGEST WINDOW /////////////////////
////////////////////////////////////////////////////////
/*
Đoạn dài nhất.

int l=0;

for(int r=0;r<n;r++){

    add(a[r]);

    while(!check()){
        del(a[l]);
        l++;
    }

    ans=max(ans,r-l+1);
}
*/

////////////////////////////////////////////////////////
////////////////// SHORTEST WINDOW /////////////////////
////////////////////////////////////////////////////////
/*
Đoạn ngắn nhất.

int l=0;

for(int r=0;r<n;r++){

    add(a[r]);

    while(check()){
        ans=min(ans,r-l+1);
        del(a[l]);
        l++;
    }
}
*/

////////////////////////////////////////////////////////
/////////////////// COUNT SUBARRAY /////////////////////
////////////////////////////////////////////////////////
/*
Đếm mọi đoạn thỏa điều kiện.

int l=0;

for(int r=0;r<n;r++){

    add(a[r]);

    while(!check()){
        del(a[l]);
        l++;
    }

    ans+=r-l+1;
}
*/

////////////////////////////////////////////////////////
//////////////////////// LIS ///////////////////////////
////////////////////////////////////////////////////////
/*
Longest Increasing Subsequence.

O(nlogn)

lis(a)

Trả về độ dài LIS.
*/

int lis(vector<int>&a){
    vector<int>v;
    for(int x:a){
        auto it=lower_bound(v.begin(),v.end(),x);
        if(it==v.end()){
            v.pb(x);
        }else{
            *it=x;
        }
    }
    return v.size();
}

////////////////////////////////////////////////////////
/////////////////// LIS SEG TREE ///////////////////////
////////////////////////////////////////////////////////
/*
LIS có update / giá trị lớn.

Compress trước.

build(1,1,m)

upd(1,1,m,pos,val)

get(1,1,m,1,pos-1)

dp[i]=get(...)+1
upd(...,dp[i])

O(nlogn)
*/

ll stlis[4*N];

inline ll merge3(ll a,ll b){
    return max(a,b);
}

void upd3(int id,int l,int r,int p,ll v){
    if(l==r){
        stlis[id]=max(stlis[id],v);
        return;
    }
    int mid=(l+r)>>1;
    if(p<=mid){
        upd3(id<<1,l,mid,p,v);
    }else{
        upd3(id<<1|1,mid+1,r,p,v);
    }
    stlis[id]=merge3(stlis[id<<1],stlis[id<<1|1]);
}

ll get3(int id,int l,int r,int u,int v){
    if(v<l||r<u){
        return 0;
    }
    if(u<=l&&r<=v){
        return stlis[id];
    }
    int mid=(l+r)>>1;
    return merge3(get3(id<<1,l,mid,u,v),get3(id<<1|1,mid+1,r,u,v));
}

////////////////////////////////////////////////////////
///////////////// INVERSION COUNT //////////////////////
////////////////////////////////////////////////////////
/*
Đếm số cặp nghịch thế.

Compress trước.

ans=inversion(a)

O(nlogn)
*/

ll inversion(vector<int>a){
    compress(a);
    memset(bit,0,sizeof(bit));
    ll ans=0;
    for(int i=(int)a.size()-1;i>=0;i--){
        ans+=sum(a[i]-1);
        add(a[i],1);
    }
    return ans;
}

////////////////////////////////////////////////////////
//////////////////// RADIX SORT ////////////////////////
////////////////////////////////////////////////////////
/*
Sắp xếp số nguyên.

O(k*n)

radix(a)

Hỗ trợ:
int
long long

Đổi sang unsigned nếu có số âm.
*/

void radix(vector<int>&a){
    int n=a.size();
    vector<int>b(n);
    const int B=1<<16;
    static int cnt[B];

    for(int sh=0;sh<32;sh+=16){
        memset(cnt,0,sizeof(cnt));

        for(int x:a){
            cnt[(x>>sh)&65535]++;
        }

        for(int i=1;i<B;i++){
            cnt[i]+=cnt[i-1];
        }

        for(int i=n-1;i>=0;i--){
            b[--cnt[(a[i]>>sh)&65535]]=a[i];
        }

        swap(a,b);
    }
}

////////////////////////////////////////////////////////
/////////////////////// TIMER //////////////////////////
////////////////////////////////////////////////////////
/*
Auto timer.

stt()

tim()

Usage:

stt();

while(tim()<1.95){
    ...
}
*/

chrono::steady_clock::time_point start;

inline void stt(){
    start=chrono::steady_clock::now();
}

inline double tim(){
    return chrono::duration<double>(chrono::steady_clock::now()-start).count();
}

void solve(){
	int n;read(n);
	write(n,'\n');write(n,'\n');
	flush();
}
signed main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	solve();
}
