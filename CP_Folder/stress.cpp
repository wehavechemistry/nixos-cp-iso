#include<bits/stdc++.h>
using namespace std;
#pragma GCC optimize("O3,unroll-loops")

const double TL=1.0;
const int T=100;
const bool REBUILD=1;
const string O="-O3 -march=native -mtune=native -pipe";

string run(const string &s){
    FILE *f=popen(s.c_str(),"r");
    if(!f)return "";
    string r;
    r.reserve(1<<16);
    char b[8192];
    while(fgets(b,sizeof(b),f))r+=b;
    pclose(f);
    return r;
}

inline void trim(string &s){
    while(!s.empty()&&isspace(s.back()))s.pop_back();
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);

    if(REBUILD){
        cerr<<"Compiling...\n";
        if(system(("g++ "+O+" gen.cpp -o gen").c_str()))return cerr<<"gen.cpp CE\n",0;
        if(system(("g++ "+O+" sol.cpp -o sol").c_str()))return cerr<<"sol.cpp CE\n",0;
        if(system(("g++ "+O+" brute.cpp -o brute").c_str()))return cerr<<"brute.cpp CE\n",0;
    }

    for(int t=1;t<=T;t++){
        string in=run("./gen");
        ofstream("in")<<in;

        auto st=chrono::steady_clock::now();
        string a=run("./sol < in");
        double tm=chrono::duration<double>(chrono::steady_clock::now()-st).count();

        if(tm>TL){
            cerr<<"\nTLE "<<t<<" ("<<fixed<<setprecision(3)<<tm<<"s)\n\n";
            cerr<<in;
            return 0;
        }

        string b=run("./brute < in");

        trim(a);
        trim(b);

        if(a!=b){
            cerr<<"\nWA "<<t<<"\n\n";
            cerr<<"Input:\n"<<in;
            cerr<<"\nSol:\n"<<a;
            cerr<<"\nBrute:\n"<<b<<"\n";
            return 0;
        }

        if(!(t&31))cerr<<"\rPassed "<<t<<" tests"<<flush;
    }

    remove("in");
    cerr<<"\n\nAC "<<T<<" tests\n";
}
