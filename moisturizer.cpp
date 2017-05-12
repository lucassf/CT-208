#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
#define MAX_INT 1000000000
typedef long long int ll;

ll MAXPROD;
vector<int> products;
vector<int> dp;
vector<int> nextBest;

void setUp(int n,int m){
    MAXPROD = (1<<n)-1;
    products.assign(n+1,0);
    dp.resize(n+1);
    nextBest.resize(n+1);

    dp.assign(MAXPROD,-1);
    nextBest.assign(MAXPROD,-1);

}

int numBits(int num){
    int ans = 0;
    while (num!=0){
        ans++;
        num -= (num&(-num));
    }return ans;
}

int recurse(ll used,ll bitprod){
    //used = consumidores ja atendidos
    //bitprod = tintas do lado do misturdor utilizadas na ultima iteração
    //cout<<used<<" "<<current<<" "<<bitprod<<endl;

    if (used==MAXPROD)return 0;
    if (dp[used]!=-1)return dp[used];

    ll canuse = MAXPROD&(~used);//Consumidore que podem ser atendidos
    int val;
    ll producer,nextbitprod;

    int returned;
    ll willbeused=0;//produtos que ainda serão utilizados

    while (canuse!=0){
        ll nextp = canuse&(-canuse);
        canuse-=nextp;
        producer = log2(nextp);
        willbeused|=products[producer];
    }

    bitprod = bitprod&willbeused;//Remove os produtos que não serão mais tilizados
    canuse = MAXPROD&(~used);
    val = MAX_INT;

    while (canuse!=0){
        ll nextp = canuse&(-canuse);
        canuse-=nextp;
        producer = log2(nextp);
        nextbitprod = bitprod|products[producer];//Adiciona os produtos usados por este consumidor
        returned = max(numBits(nextbitprod),recurse(used|nextp,nextbitprod));
        if (returned<val){
            val = returned;
            nextBest[used] = producer;
        }
    }

    return dp[used] = val;
}

vector<int> values;
vector<int> bruteAns;
int globalAns;

int compute(){//Computa valor máximo para dada permutação
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
        bitval&=aux;
    }
    return ans;
}

int perm(int pos,int used){//Gera todas as permutações
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

vector<string> get_all_files_names_within_folder(string folder)
{
    vector<string> names;
    string search_path = folder + "/*.*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}

int main(){
    int n,m,prod,a;
    int t,val,filenum=1;

    FILE* input;
    FILE* output;
    string name,oname,name1="C:\\Users\\lucas\\Documents\\ITA\\CT-208\\Instancias\\",
        name2 = "C:\\Users\\lucas\\Documents\\ITA\\CT-208\\Saidas\\";

    vector<string> files = get_all_files_names_within_folder("C:\\Users\\lucas\\Documents\\ITA\\CT-208\\Instancias");

    //cin>>t;
    while (true){

        name = files.back();
        files.pop_back();
        oname = name2+name;
        name = name1+name;

        input = fopen(name.c_str(),"r");
        output = fopen(oname.c_str(),"w");

        fscanf(input,"%d%d",&n,&m);
        if (n>20||m>64){
            fclose(input);
            fclose(output);
            if (files.empty())break;
            continue;
        }
        setUp(n,m);
        for (int i=0;i<n;i++){
            for (int j=0;j<m;j++){
                fscanf(input,"%d",&a);
                products[i]|=(a<<j);
            }
        }

        fprintf(output,"%d\n",recurse(0,0));

        int used = 0,current = n;
        while (used!=MAXPROD){
            current = nextBest[used];
            fprintf(output,"%d ",current);
            used|=(1<<current);
        }
        fprintf(output,"\n");

        /*globalAns = MAX_INT;
        values.assign(n,0);
        bruteAns.assign(n,0);

        cout<<endl<<perm(0,0)<<endl;
        for (int i=0;i<n;i++)cout<<bruteAns[i]<<" ";
        cout<<endl;*/
        //if (--t<0)break;

        fclose(input);
        fclose(output);
        if (files.empty())break;
    }
}
