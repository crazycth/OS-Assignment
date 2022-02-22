#include<bits/stdc++.h>
using namespace std;

//尽量不使用高级数据结构 从底层指针维护
//做好空间管理 注意申请与回收
//当前只支持相对路径访问
//pwd ls touch cat > >> mkdir 

struct file {
    string name;
    string content;
    int len;
    string safe_code;
    file()
    {
        name = content = safe_code = "";
        len = 0;
    }
    ~file()
    {
        ;
    }
    file( string init_name , string init_content , string init_safe_code = "000")
    {
        name = init_name;
        content = init_content;
        len = init_content.size();
        safe_code = init_safe_code;
    }
    friend ostream& operator<<(ostream& out, file fileNode) {
        out << fileNode.name << "\t\t\t" << fileNode.safe_code << "\t\t\t" << fileNode.len;
        return out;
    }
};

struct struct_pwd{
    vector<string> p;
    struct_pwd()
    {
        p.clear();
        p.push_back("root");
    }
    void push_back(string name)
    {
        p.push_back(name);
    }
    void pop_back()
    {
        p.pop_back();
    }
    void clear()
    {
        p.clear();
    }
    friend ostream& operator<<(ostream& out, struct_pwd fileNode) {
        for(int i=0;i<fileNode.p.size();i++)
        {
            cout<<fileNode.p[i]<<"/"[i==fileNode.p.size()-1];
        }
        return out;
    }
};

struct file_catalogue {
    struct_pwd pwd;
    vector<pair<string,file>> file_list;
    vector<pair<string,file_catalogue*>> catalogue_list;

    string name;
    int len;
    string safe_code;

    file_catalogue(string init_name="FAULT")
    {
        file_list.clear();
        catalogue_list.clear();
        name = init_name;
        len = 0;
        safe_code = "-1";
    }

    ~file_catalogue()
    {
        //cout<<name<<'\n';
        for(auto &[name,address]:catalogue_list) delete address;
    }

    void insert_file(string name , file this_file)
    {
        file_list.push_back({name,this_file});
    }

    void insert_catalogue(string file_catalogue_name , file_catalogue * add_file_catalogue)
    {
        catalogue_list.push_back({file_catalogue_name,add_file_catalogue});
    }

    void file_catalogue_ls()
    {
        for(auto [name , address] : catalogue_list)
        {
            cout<<name<<"\t\t\t"<<(*address).safe_code<<"\t\t\t"<<(*address).len<<'\n';
        }
        //cout<<"file:\n";
        for(auto [name , file] : file_list)
        {
            cout<<file<<'\n';
        }
        cout<<'\n';
    }
};

vector<string> split(string s)
{
    string tem = "";
    int i = 0;
    vector<string> res;
    while(s[i]==' '&&i!=s.size()) i = i + 1;
    for(;i<s.size();i++)
    {
        if(s[i]==' '&&tem.size()!=0)
        {
            res.push_back(tem);
            tem.clear();
        }
        else if(s[i]!=' ')
        {
            tem += s[i];
        }
    }
    if(tem.size()!=0) res.push_back(tem);
    return res;
}


vector<string> rootsplit(string a)
{
    vector<string> res;
    string tem = "";
    for(int i=0;i<a.size();i++)
    {
        if(a[i]=='/'&&tem.size()!=0)
        {
            res.push_back(tem);
            tem="";
        }
        else if(a[i]!='/') tem += a[i];
    }
    if(tem.size()!=0) res.push_back(tem);
    return res;
}

struct user {
    struct_pwd PWD;
    vector<file_catalogue*> pwd_catalogue;

    void dbg()
    {
        for(auto ele : pwd_catalogue)
        {
            cout<<ele<<" ";
        }
        cout<<'\n';
    }

    user()
    {
        pwd_catalogue.clear();
        file_catalogue* now = new file_catalogue("root");
        pwd_catalogue.push_back(now);
    }

    void init()
    {
        file_catalogue* now = pwd_catalogue.back();
        mkdir("DeepL");
        cd("DeepL");
        touch("transformer");
        cd("..");
        mkdir("OS");
        cd("OS");
        touch("LFS");
        cd("..");
    }

    void pwd()
    {
        cout<<PWD<<'\n';
    }

    void ls()
    {
        file_catalogue* now = pwd_catalogue.back();
        (*now).file_catalogue_ls();
    }

    void mkdir(string name)
    {
        file_catalogue* now = pwd_catalogue.back();
        file_catalogue* tem = new file_catalogue(name);
        (*now).insert_catalogue(name,tem);
    }

    void cd(string next_pos)
    {
        if(next_pos=="..")
        {
            if(pwd_catalogue.size()>1)
            {
                PWD.pop_back();
                pwd_catalogue.pop_back();
            }
            return;
        }
        bool find = false;
        file_catalogue* target = NULL;
        file_catalogue* local = pwd_catalogue.back();
        for(auto &[name,address]:(*local).catalogue_list)
        {
            if(name==next_pos)
            {
                find = true;
                target = address;
            }
        }

        if(find==false)
        {
            cout<<"cd "<<next_pos<<" : No such directory!\n";
            return;
        }

        PWD.push_back(next_pos);
        pwd_catalogue.push_back(target);
    }

    void cat(string target_file)
    {
        file_catalogue* now = pwd_catalogue.back();
        for(auto &[name,file_struct] : (*now).file_list)
        {
            if(name==target_file)
            {
                cout<<file_struct.content<<'\n';
                return;
            }
        }
        cout<<"cat "<<target_file<<": No such file or directory\n";
    }

    void touch(string target_name)
    {
        file_catalogue* now = pwd_catalogue.back();
        for(auto &[name,file_struct] : (*now).file_list)
        {
            if(name==target_name) return;
        }
        (*now).file_list.push_back({target_name,file(target_name,"")});
    }

    void append(file & modified_file , string s )
    {
        cout<<"s is now "<<s<<'\n';
        modified_file.content += s;
        modified_file.len = modified_file.content.size();
    }

    void correct_file(file & modified_file , string s)
    {
        modified_file.content = s;
        modified_file.len = modified_file.content.size();
    }

    void rm(string name)
    {
        auto & file_list = pwd_catalogue.back()->file_list;
        bool del = false;
        //cout<<file_list.size()<<'\n';
        for(auto it = file_list.begin();it != file_list.end() ; )
        {
            cout<<(*it).first<<'\n';
            if((*it).first==name) 
            {
                it = file_list.erase(it);
                del = true;
                // cout<<"DEL!\n";
            }
            else ++it;
        }
        if(!del) cout<<"rm "<<name<<" No such file or directory\n";
    }

    void rmr(string name)
    {
        auto & file_list = pwd_catalogue.back()->catalogue_list;
        bool del = false;
        cout<<"find name : "<<name<<'\n';
        for(auto it = file_list.begin();it != file_list.end() ; )
        {
            cout<<"now "<<(*it).first<<'\n';
            if((*it).first==name) 
            {
                it = file_list.erase(it);
                del = true;
            }
            else ++it;
        }
        if(!del) cout<<"rm "<<name<<" No such file or directory\n";
    }
    

    void run()
    {
        init();
        while(1)
        {
            cout<<"PWD:"<<PWD<<'\n'<<":";
            string s ;
            getline(cin,s);
            vector<string> tem = split(s);
            if(tem[0]=="ls")
            {
                ls();
            }
            else if(tem[0]=="pwd")
            {
                pwd();
            }
            else if(tem[0]=="clear")
            {
                system("clear");
            }
            else if(tem[0]=="mkdir")
            {
                mkdir(tem[1]);
            }
            else if(tem[0]=="cd")
            {
                bool absolute = false;
                for(int i=0;i<tem[1].size();i++)
                {
                    if(tem[1][i]=='/') absolute = true;
                }
                if(absolute)
                {
                    auto root_list = rootsplit(tem[1]);
                    bool ok = true;
                    if(root_list[0]!="root") ok = false;
                    vector<file_catalogue*> new_pwd_catalogue;
                    struct_pwd new_pwd;

                    new_pwd_catalogue.clear();
                    new_pwd.clear();

                    new_pwd.push_back("root");
                    new_pwd_catalogue.push_back(pwd_catalogue[0]);

                    for(int i=1;i<root_list.size();i++)
                    {
                        file_catalogue* now = new_pwd_catalogue.back();
                        bool find = false;
                        for(auto &[name,address]:now->catalogue_list)
                        {
                            if(name==root_list[i])
                            {
                                find = true;
                                new_pwd_catalogue.push_back(address);
                                new_pwd.push_back(name);
                                break;
                            }
                        }
                        if(find==false)
                        {
                            ok = false;
                            break;
                        }
                    }

                    if(ok)
                    {
                        PWD = new_pwd;
                        pwd_catalogue = new_pwd_catalogue;
                    }
                    else
                    {
                        cout<<"No such file or directory";
                    }
                }
                else cd(tem[1]);
            }
            else if(tem[0]=="dbg")
            {
                dbg();
            }
            else if(tem[0]=="cat"&&tem.size()==2)
            {
                cat(tem[1]);
            }
            else if(tem[0]=="echo")
            {
                bool swi1 = false; //>
                bool swi2 = false; //>>
                int pos1 = 0;
                int pos2 = 0;
                for(int i=1;i<tem.size();i++)
                {
                    if(tem[i]==">") swi1 = true,pos1=i;
                    if(tem[i]==">>") swi2 = true,pos2=i;
                }
                if(swi1)
                {
                    string content = "";
                    for(int i=1;i<pos1;i++)
                    {
                        if(content.size()==0) content+=tem[i];
                        else
                        {
                            content += " ";
                            content += tem[i];
                        }
                    }
                    string path = tem.back();
                    touch(path);
                    file_catalogue* now = pwd_catalogue.back();
                    for(auto &[name,file_struct] : (*now).file_list)
                    {
                        if(name==path) correct_file(file_struct,content);
                    }
                }
                else if(swi2)
                {
                    string content = "";
                    
                    for(int i=1;i<pos1;i++)
                    {
                        if(content.size()==0) content+=tem[i];
                        else
                        {
                            content += " ";
                            content += tem[i];
                        }
                    }

                    string path = tem.back();
                    touch(path);
                    file_catalogue* now = pwd_catalogue.back();
                    for(auto &[name,file_struct] : (*now).file_list)
                    {
                        if(name==path) append(file_struct,content);
                    }
                }
                else{
                    for(int i=1;i<tem.size();i++) cout<<tem[i]<<" ";
                    cout<<'\n';
                }
            }
            else if(tem[0]=="touch")
            {
                touch(tem[1]);
            }
            else if(tem[0]=="rm")
            {
                if(tem[1]!="-r") rm(tem[1]);
                else if(tem.size()>=3) rmr(tem[2]);
            }
        }
    }
};

file_catalogue* user_pwd;
user USER;

int main()
{
    USER.run();
}