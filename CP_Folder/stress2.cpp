#include<bits/stdc++.h>
using namespace std;
#pragma GCC optimize("O3,unroll-loops")

const double TL=1.0;
const int T=100;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cerr<<"Compiling...\n";
    if(system("g++ -O3 -march=native -mtune=native -pipe gen.cpp -o gen"))return 0;
    if(system("g++ -O3 -march=native -mtune=native -pipe sol.cpp -o sol"))return 0;
    if(system("g++ -O3 -march=native -mtune=native -pipe brute.cpp -o brute"))return 0;
    cerr<<"Start\n";
    int ac=0,wa=0,tle=0;

    for(int t=1;t<=T;t++){
        system("./gen > in");

        auto st=chrono::steady_clock::now();
        system("./sol < in > out");
        double tm=chrono::duration<double>(chrono::steady_clock::now()-st).count();

        if(tm>TL){
            if(!tle){
                cerr<<"\nTLE "<<t<<" ("<<fixed<<setprecision(3)<<tm<<"s)\n\n";
                system("cat in");
            }
            tle++;
        }else{
            system("./brute < in > ans");
            if(system("cmp -s out ans")==0){
                ac++;
            }else{
                if(!wa){
                    cerr<<"\nWA "<<t<<"\n\nInput:\n";
                    system("cat in");
                    cerr<<"\nSol:\n";
                    system("cat out");
                    cerr<<"\nBrute:\n";
                    system("cat ans");
                    cerr<<"\n";
                }
                wa++;
            }
        }

        if((t&31)==0)cerr<<"\r"<<t<<"/"<<T<<"  AC:"<<ac<<" WA:"<<wa<<" TLE:"<<tle<<flush;
    }

    system("rm -f in out ans");

    cerr<<"\n\n";
    cerr<<"AC  : "<<ac<<"/"<<T<<"\n";
    cerr<<"WA  : "<<wa<<"\n";
    cerr<<"TLE : "<<tle<<"\n";
}
