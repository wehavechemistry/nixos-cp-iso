#include<bits/stdc++.h>
using namespace std;
// #pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt")

uint64_t seed=chrono::steady_clock::now().time_since_epoch().count();
mt19937 mt(seed);
inline uint64_t rng(){
    uint64_t z=(seed+=0x9e3779b97f4a7c15);
    z=(z^(z>>30))*0xbf58476d1ce4e5b9;
    z=(z^(z>>27))*0x94d049bb133111eb;
    return z^(z>>31);
}

inline int rnd(int l,int r){
    return l+rng()%(r-l+1);
}

void sh(vector<int>&a){
    for(int i=a.size()-1;i;i--){
        swap(a[i],a[rng()%(i+1)]);
    }
}

void sh(vector<pair<int,int>>&a){
    for(int i=a.size()-1;i;i--){
        swap(a[i],a[rng()%(i+1)]);
    }
}

////////////////////////////////////////////////////////
///////////////////// GENERATOR ////////////////////////
////////////////////////////////////////////////////////
/*
rnd(l,r)
random integer

arr(n,l,r)
print random integer array

perm(n)
print random permutation

tree(n,t)
print tree
t=0 random
t=1 line
t=2 star

graph(n,m,t)
print graph
t=0 random
t=1 connected
t=2 DAG

str(n,t)
print random string
t=0 a-z
t=1 A-Z
t=2 0-9
t=3 a-zA-Z
t=4 a-zA-Z0-9
t=5 01
t=6 #.*
t=7 printable ASCII

grid(n,m,t)
print random character matrix

mat(n,m,l,r)
print random integer matrix
*/

void arr(int n,int l,int r){
    for(int i=1;i<=n;i++){
        cout<<rnd(l,r)<<" ";
    }
    cout<<"\n";
}

void perm(int n){
    vector<int>p(n);
    iota(p.begin(),p.end(),1);
    sh(p);
    for(int x:p)cout<<x<<" ";
    cout<<"\n";
}

void tree(int n,int t=0){
    vector<pair<int,int>>e;

    if(t==1){
        for(int i=2;i<=n;i++)e.push_back({i-1,i});
    }
    else if(t==2){
        for(int i=2;i<=n;i++)e.push_back({1,i});
    }
    else{
        for(int i=2;i<=n;i++){
            e.push_back({i,rnd(1,i-1)});
        }
        sh(e);
    }

    for(auto [u,v]:e){
        cout<<u<<" "<<v<<"\n";
    }
}

void graph(int n,int m,int t=0){
    vector<pair<int,int>>e;
    set<pair<int,int>>s;

    auto add=[&](int u,int v){
        if(u==v)return;
        if(t!=2&&u>v)swap(u,v);
        if(s.count({u,v}))return;
        s.insert({u,v});
        e.push_back({u,v});
    };

    if(t==1){
        for(int i=2;i<=n;i++){
            add(i,rnd(1,i-1));
        }
    }

    while((int)e.size()<m){
        int u=rnd(1,n);
        int v=rnd(1,n);

        if(t==2){
            if(u==v)continue;
            if(u>v)swap(u,v);
        }

        add(u,v);
    }

    for(auto [u,v]:e){
        cout<<u<<" "<<v<<"\n";
    }
}

void str(int n,int t=0){
    string c;

    if(t==0)c="abcdefghijklmnopqrstuvwxyz";
    else if(t==1)c="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    else if(t==2)c="0123456789";
    else if(t==3)c="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    else if(t==4)c="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    else if(t==5)c="01";
    else if(t==6)c="#.*";
    else{
        for(int i=32;i<=126;i++)c+=char(i);
    }

    for(int i=1;i<=n;i++){
        cout<<c[rng()%c.size()];
    }
    cout<<"\n";
}

void grid(int n,int m,int t=0){
    for(int i=1;i<=n;i++){
        str(m,t);
    }
}

void mat(int n,int m,int l,int r){
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            cout<<rnd(l,r)<<" ";
        }
        cout<<"\n";
    }
}

signed main(){
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n=rnd(1,20);
	cout<<n<<"\n";
	arr(n,1,200);
}
