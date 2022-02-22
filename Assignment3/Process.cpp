#include<bits/stdc++.h>
using namespace std;

class LRU
{
    unordered_map<int,std::pair<int,int>> link;
    set<int> available;
    int size = 0;
    int head = -1;
    int tail = -2;
    void erase(int num)  //erase element from link
    {
        link[link[num].first].second = link[num].second;
        link[link[num].second].first = link[num].first;
        available.erase(num);
    }
    void push_front(int num)
    {
        link[num].second = link[head].second;
        link[num].first = head;
        link[link[num].second].first = num;
        link[head].second = num;
        available.insert(num);
    }
    void pop_back( )
    {
        int bef = link[link[tail].first].first;
        int drop = link[tail].first;
        link[bef].second = tail;
        link[tail].first = bef;
        available.erase(drop);
    }
    void add_element(int num)
    {
        push_front(num);
        if(available.size()>size) pop_back();
    }
public:
    LRU(int size = 3):size(size)
    {
        link[head].second = tail;
        link[tail].first = head;
    }
    int in(int id) //return 1 if hit else 0
    {
        if(available.count(id)) //hit
        {
            erase(id);
            push_front(id);
            return 1;
        }
        add_element(id);
        return 0;
    }
    void show()
    {
        int i = link[head].second;
        while(i!=tail)
        {
            cout<<i<<" ";
            i = link[i].second;
        }
        cout<<'\n';
    }
    int check(const vector<int>& request )
    {
        int hit = 0;
        for(int i=0;i<request.size();i++)
        {
            if(in(request[i])) ++hit;
        }
        return hit;
    }
};


class FIFO
{
    queue<int> link;
    set<int> available;
    int size = 3;
    void add_element(int num)
    {
        link.push(num);
        available.insert(num);
        if(link.size()>size)
        {
            int drop = link.front();
            link.pop();
            available.erase(drop);
        }
    }
public:
    FIFO(int size=3):size(size){}
    int in(int num)
    {
        if(available.count(num)) return 1;
        add_element(num);
        return 0;
    }
    void show()
    {
        queue<int> tem;
        while(!tem.empty()) tem.pop();
        while(!link.empty())
        {
            auto a = link.front();
            link.pop();
            tem.push(a);
            cout<<a<<" ";
        }
        cout<<'\n';
        while(!tem.empty())
        {
            auto a = tem.front();
            tem.pop();
            link.push(a);
        }
    }
    int check(const vector<int>& request )
    {
        int hit = 0;
        for(int i=0;i<request.size();i++)
        {
            if(in(request[i])) ++hit;
        }
        return hit;
    }
};


class OPT
{
    set<std::pair<int,int>> available;
    vector<int> request;
    vector<int> nex;
    int size;
public:
    OPT(vector<int> re,int size=8):request(re),size(size){}
    void init()
    {
        nex.resize(request.size());
        vector<int> tem;
        tem.resize((int)1e6);
        for(auto &ele : tem) ele = 1e9;
        for(int i = (int)nex.size()-1;i>=0;i--)
        {
            nex[i] = tem[request[i]];
            tem[request[i]] = i;
        }
    }
    int check(const vector<int>& re)
    {
        init();
        int hit = 0;
        for(int i=0;i<request.size();i++)
        {
            //dbg(i);
            //show();
            bool ok = false;
            int pos = -1;
            for(auto ele : available) 
                if(ele.second==request[i]) 
                    {
                        pos = ele.first;
                        ok = true;
                    }
            if(ok)
            {
                //dbg(request[i]);
                available.erase({pos,request[i]});
                available.insert({nex[i],request[i]});
                ++hit;
                continue;
            }
            if(available.size()==size) available.erase(*available.rbegin());
            available.insert({nex[i],request[i]});
        }
         return hit;
    }
    void show()
    {
        for(auto ele : available)
            cout<<ele.second<<" "<<ele.first<<'\n';
        cout<<'\n';
    }
};


void C(int size = 10)
{
    //vector<int> gen = generator(100000);
    int num ; cin>>num;
    cout<<"total_num is "<<num<<'\n';
    vector<int> gen ; gen.clear();
    for(int i=0;i<num;i++)
    {
        int tem ; cin>>tem;
        gen.push_back(tem);
    }
    auto a = OPT(gen,size);
    auto b = FIFO(size);
    auto c = LRU(size);
    
    cout<<"OPT: "<<a.check(gen)<<'\n';
    cout<<"FIFO: "<<b.check(gen)<<'\n';
    cout<<"LRU: "<<c.check(gen)<<'\n';
}

void checko()
{
    vector<int> gen = {1,2,3,2,1,2,3,3,1,1};
    for(auto c : gen) cout<<c<<" ";
    cout<<'\n';
    auto c = LRU(2);
    int hit = c.check(gen);
    cout<<hit<<'\n';
}

int main()
{
    int size ; cin>>size;
    cout<<"size is now "<<size<<'\n';
    C(size);
    return 0;
}