#include<bits/stdc++.h>
#define pb push_back
#define fi first
#define se second
using ll=long long;
using ld=long double;
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rnd(int l, int r){
	return l+rng()%(r-l+1);
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n; cin>>n;
    vector<int> a(n+1),p(n+1,0);
	for(int i=1;i<=n;++i){
		cin>>a[i];
		p[i]=p[i-1]+a[i];
	}
	int ans=a[1],l=1,r=1,nl,nr,nans;
    auto start_time = chrono::steady_clock::now();
	while(true){
        auto current_time = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(current_time - start_time).count();
        if (elapsed >= 0.95) break;
		nl=rnd(1,n);
		nr=rnd(1,n);
		if(nl>nr){
			swap(nl,nr);
		}
		nans=p[nr]-p[nl-1];
		if(ans<nans){
			l=nl;
			r=nr;
			ans=nans;
		}
        nl=l+rnd(-1000,1000);
		nr=r+rnd(-1000,1000);
		nl=max(nl,1);
		nl=min(nl,n);
		nr=max(nr,1);
		nr=min(nr,n);
		if(nl>nr){
			swap(nl,nr);
		}
		nl=l+rnd(-200,200);
		nr=r+rnd(-200,200);
		nl=max(nl,1);
		nl=min(nl,n);
		nr=max(nr,1);
		nr=min(nr,n);
		if(nl>nr){
			swap(nl,nr);
		}
        nl=l+rnd(-20,20);
		nr=r+rnd(-20,20);
		nl=max(nl,1);
		nl=min(nl,n);
		nr=max(nr,1);
		nr=min(nr,n);
		if(nl>nr){
			swap(nl,nr);
		}
	}
	cout<<ans;
}
