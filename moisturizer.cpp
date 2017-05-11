#include <bits/stdc++.h>
using namespace std;
#define MAX_INT 1000000000
int MAXPROD;

vector<int> products;
vector<int> dp;
vector<int> nextBest;

int numBits(int num){
    int ans = 0;
    while (num!=0){
        ans++;
        num -= (num&(-num));
    }return ans;
}

int recurse(int used,int bitprod){
    //cout<<used<<" "<<current<<" "<<bitprod<<endl;

    if (used==MAXPROD)return 0;
    if (dp[used]!=-1)return dp[used];

    int canuse = MAXPROD&(~used);
    int val,producer,nextbitprod;

    int returned;
    int willbeused=0;

    while (canuse!=0){
        int nextp = canuse&(-canuse);
        canuse-=nextp;
        producer = log2(nextp);
        willbeused|=products[producer];
    }

    bitprod = bitprod&willbeused;
    canuse = MAXPROD&(~used);
    val = MAX_INT;

    while (canuse!=0){
        int nextp = canuse&(-canuse);
        canuse-=nextp;
        producer = log2(nextp);
        nextbitprod = bitprod|products[producer];
        returned = max(numBits(nextbitprod),recurse(used|nextp,nextbitprod));
        if (returned<val){
            val = returned;
            nextBest[used] = producer;
        }
    }

    return dp[used] = val;
}

void setUp(int n,int m){
    MAXPROD = (1<<n)-1;
    products.assign(n+1,0);
    dp.resize(n+1);
    nextBest.resize(n+1);

    dp.assign(MAXPROD,-1);
    nextBest.assign(MAXPROD,-1);

}

vector<int> values;
vector<int> bruteAns;
int globalAns;

int compute(){
    int ans = -1;
    int bitval = 0;
    int aux;

    for (int i=0;i<values.size();i++){
        bitval|=products[values[i]];
        ans = max(ans,numBits(bitval));
        aux = 0;
        for (int j=i+1;j<values.size();j++){
            aux|=products[values[j]];
        }
        //cout<<bitval<<" ";
        bitval&=aux;
    }//cout<<endl;
    return ans;
}

int perm(int pos,int used){
    int poss = MAXPROD&(~used);
    if (poss==0){
        poss = compute();
        if (poss<globalAns){
            globalAns=poss;
            for (int i=0;i<values.size();i++)bruteAns[i]=values[i];
        }
        return poss;
    }

    int val,ans=MAX_INT,returned;

    while (poss){
        val = poss&(-poss);
        poss-=val;
        values[pos] = log2(val);
        returned = perm(pos+1,used|val);
        if (returned<ans)ans = returned;
    }
    return ans;
}

int main(){
    int n,m,prod,a;
    int t;

    cin>>t;
    while (t-->0){
        cin>>n>>m;
        setUp(n,m);
        srand (time(NULL));
        for (int i=0;i<n;i++){
            for (int j=0;j<m;j++){
                //cin>>a;

                a = rand()%2;
                cout<<a<<" ";
                products[i]|=(a<<j);
            }
            cout<<endl;
        }
        products[n]=0;

        cout<<recurse(0,0)<<endl;

        int used = 0,current = n;
        while (used!=MAXPROD){
            current = nextBest[used];
            cout<< current << " ";
            used|=(1<<current);
        }

        globalAns = MAX_INT;
        values.assign(n,0);
        bruteAns.assign(n,0);

        cout<<endl<<perm(0,0)<<endl;
        for (int i=0;i<n;i++)cout<<bruteAns[i]<<" ";
        cout<<endl;
    }
}
