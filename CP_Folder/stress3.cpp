#include<bits/stdc++.h>
using namespace std;
#pragma GCC optimize("O3,unroll-loops")

const double TL=1.0;
const int T=100;
const bool REBUILD=1;
const string O="-O3 -march=native -pipe";

string run(const string &s){
    FILE *f=popen(s.c_str(),"r");
    if(!f)return "";
    string r; r.reserve(1<<16);
    char b[8192];
    while(fgets(b,sizeof(b),f))r+=b;
    pclose(f);
    return r;
}

inline void trim(string &s){
    while(!s.empty()&&isspace(s.back()))s.pop_back();
}

int main(){
    ios::sync_with_stdio(false); cin.tie(0);

    if(REBUILD){
        cerr<<"Compiling...\n";
        if(system(("g++ "+O+" gen.cpp -o gen").c_str())) return cerr<<"gen.cpp CE\n",0;
        if(system(("g++ "+O+" sol.cpp -o sol").c_str())) return cerr<<"sol.cpp CE\n",0;
        if(system(("g++ "+O+" brute.cpp -o brute").c_str())) return cerr<<"brute.cpp CE\n",0;
    }

    int ac=0, wa=0, tle=0;

    for(int t=1;t<=T;t++){
        string in=run("./gen");
        ofstream("in")<<in;

        auto st=chrono::steady_clock::now();
        int exit_code = system(("timeout " + to_string(TL) + "s ./sol < in > out").c_str());
        double tm=chrono::duration<double>(chrono::steady_clock::now()-st).count();

        if(exit_code != 0 || tm > TL){
            if(!tle){
                cerr<<"\nTLE "<<t<<" ("<<fixed<<setprecision(3)<<tm<<"s)\nInput:\n"<<in;
            }
            tle++;
            continue;
        }

        string b = run("./brute < in");
        
        
        ifstream sol_file("out");
        string a((istreambuf_iterator<char>(sol_file)), istreambuf_iterator<char>());
        
        trim(a); trim(b);

        if(a!=b){
            if(!wa){
                cerr<<"\nWA "<<t<<"\nInput:\n"<<in<<"\nSol:\n"<<a<<"\nBrute:\n"<<b<<"\n";
            }
            wa++;
        } else {
            ac++;
        }

        if((t&31)==0 || t==T) {
            cerr<<"\rTest "<<t<<"/"<<T<<" | AC: "<<ac<<" WA: "<<wa<<" TLE: "<<tle<<flush;
        }
    }

    system("rm -f in out");
    cerr<<"\n\n=== FINAL SCORE: "<<ac<<"/"<<T<<" AC ===\n";
}
