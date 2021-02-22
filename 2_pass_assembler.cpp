#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include<map>
using namespace std;
#include "symbol_table.h"
#include "data_storage.h"

m_opcodeop m; //MOT
p_opcodeop p; //POT
opcode l1; //literals
branchcode b1;
registerr r;
SymbolTable sy;//symboltable
int pooltab[10];

string extnum(string h)
{
  for (int i = 0; i < h.size(); i++)
  {
    if (h[i]< 48 || h[i]>57)
    {
        h.erase(i,1);
        i--;
    }
  }
    return h;
}

bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
    return true;
}
string* splitString(string line,int n)
{
  string *s=new string[n];
  string word= "";
  int v=0;
  for (char x:line)
  {
    if(x==' ' && word!="")
    {

      s[v]=word;
      word="";
      if(v<n-1)
      {
      v=v+1;
      }
    }
    if(x!=' ')
    {
    word=word+x;
    }
  }
  s[v]=word;
  return s;
}
string numprint(string h)
{
  for (int i = 0; i < h.size(); i++) {
  if (h[i]< 48 || h[i]>57)
  {
      h.erase(i, 1);
      i--;
  }
}
return h;
}
string literalprint1(string y)
{
  for (int i = 0; i < y.length(); i++) {
  if ((y[i] < 'A' || y[i] > 'Z') && (y[i] < 'a' || y[i] > 'z') && ( y[i]< 48 || y[i]>57))
  {
      y.erase(i, 1);
      i--;
  }
}
return y;
}
string literalprint(string y)
{
  for (int i = 0; i < y.length(); i++)
  {
    if ((y[i] < 'A' || y[i] > 'Z') && (y[i] < 'a' || y[i] > 'z') && ( y[i]< 48 || y[i]>57) && y[i]!='\'')
    {
        y.erase(i, 1);
        i--;
    }
  }
  return y;
}
//pass1
void pass1()
{
  int v=0;
  ifstream f;
  f.open("/home/GanapriyaS/Documents/SPOS/2 pass assembler/Input1.txt");
  ofstream out;
  out.open("Intermediate code.txt");
  int n;
  int loc,k=0,o=0;
  string start;
  string line;
  while(getline(f,line))
  {
    n=(line.find("DS")==string::npos && line.find("DC")==string::npos && line.find(":")==string::npos)?2:3;
    string *s=new string[n];
    string word= "";
    s=splitString(line,n);

    int a,b;
    a=(n==3)?1:0;
    b=(n==3)?2:1;
    //label
    if(n==3)
    {
      if(s[0].find(":")!=string::npos)
      {
        if(sy.find(s[0])==-1)
          sy.insert(s[0],to_string(loc));
        else if(sy.find(s[0])!=-1)
          sy.modify(s[0],to_string(loc));
      }
      //DS
      else if(s[1].find("DS")!=string::npos)
      {
        if(sy.find(s[0])==-1)
          sy.insert(s[0],to_string(loc));
        else if(sy.find(s[0])!=-1)
          sy.modify(s[0],to_string(loc));
        string y=s[2];
        y=literalprint(y);
      out<<loc<<") (DL,01) - ";

      if(isNumber(y))
      {
        out<<'0' + stoi(y);
        loc=loc+stoi(y)-1;
      }
      else
      {
        y=literalprint1(y);
      loc=loc+int(y[0])-1;
        if(isNumber(y))
          out<<y;
        else
          out<<int(y[0]);
      }
      }
      //DC
      else if(s[1].find("DC")!=string::npos)
      {
        string y=s[2];
        y=literalprint(y);
        if(sy.find(s[0])==-1)
          sy.insert(s[0],to_string(loc));
        else if(sy.find(s[0])!=-1)
          sy.modify(s[0],to_string(loc));
        out<<loc<<") (DL,02) - ";
        if(isNumber(y))
        {
          out<<'0' + stoi(y);
        }
        else
        {

          y=literalprint1(y);
            if(isNumber(y))
              out<<y;
            else
              out<<int(y[0]);
        }
      }
    }
    //start of program
    if(s[a]=="START")
    {
      start=s[b];
      out<<"(AD,01)"<<" - "<<"(C,"<<start<<")";
      loc=stoi(start)-1;
    }
    //end or ltorg
    else if(s[a]=="END" || s[a]=="LTORG")
    {
      out<<loc<<") (AD,"<<p.op1.code[s[a]]<<") - ";
      int h=0;
      for(auto const& [key,val]:l1.code)
      {
        if(h<k)
        {
          h++;
          continue;
        }
        if(h!=k)
          out<<"\n"<<loc<<") (AD,"<<p.op1.code[s[a]]<<") - ";
        l1.code[key]=to_string(loc);
        string s=key;
        s=literalprint(s);
        if(isNumber(s))
        {
          out<<'0' + stoi(s);
        }
        else
        {
          s=literalprint1(s);
          if(isNumber(s))
            out<<s;
          else
            out<<stoi(s);
        }
        if(h!=o)
        {
        loc++;
      }
      if(h==o)
        break;
        h++;
      }
      k=o;
      if(s[a]=="END")
        break;
    }
    //invalid nmenomics
    else if(p.op1.code.find(s[a])==p.op1.code.end() && m.op.code.find(s[a])==m.op.code.end() && (line.find("DS")==string::npos && line.find("DC")==string::npos))
      cout<<"INVALID NMEMONICS";
    else if(line.find("DS")==string::npos && line.find("DC")==string::npos)
    {
      if(p.op1.code.find(s[a])!=p.op1.code.end()){out<<loc<<") (AD,"<<p.op1.code[s[a]]<<") ";}
      else if(m.op.code.find(s[a])!=m.op.code.end()){out<<loc<<") (IS,"<<m.op.code[s[a]]<<") ";}

      if(s[b]!="" && s[b].find(',')!=string::npos)
      {
        vector<string>v1;
        word="";
        for (auto x:s[b])
        {
          if(x==',' && word!="")
          {
            v1.push_back(word);
            word="";
          }
          if(x!=',')
          word=word+x;
        }
        v1.push_back(word);
        int cc=0;
        for(int i=0;i<v1.size();i++)
        {
    if(r.op2.code.find(v1[0])== r.op2.code.end() && cc==0)
    {
      out<<"- ";
      cc++;
    }
    if(r.op2.code.find(v1[i])== r.op2.code.end())
    {
        if(b1.op3.code.find(v1[i])== b1.op3.code.end())
        {
          if(s[b].find("=")!=string::npos)
          {
            string h=to_string(loc);
            if(l1.code.find(v1[i])==l1.code.end() )
                {
                  l1.code[v1[i]]=h;
                  o++;
                 out<<"(L,"<<v1[i]<<") ";
                  }
            else if(l1.code.find(v1[i])!=l1.code.end() )
            {
              out<<"(L,"<<v1[i]<<") ";
            }
          }
          else
          {
            string h=to_string(loc);
            int len=v1[i].length();
            char a[len+1];
            strcpy(a,v1[i].c_str());
            int zz=0,yy=0;
            for(auto x : a)
            {
              if(isalpha(x)!=0)
                zz++;
              if(isdigit(x)!=0)
                yy++;
            }
            if(zz==v1[i].length())
            {
                if(sy.find(v1[i])==-1)
                {
                  sy.insert(v1[i],"");
                  out<<"(S,"<<v1[i]<<") ";
                }
                else if(sy.find(v1[i])!=-1)
                  out<<"(S,"<<v1[i]<<") ";
            }
            else
            {
              out<<"(C,"<<int(v1[i][0])<<")"<<" ";
            }
        }
      }
        else if(b1.op3.code.find(v1[i])!= b1.op3.code.end())
        {
          out<<b1.op3.code[v1[i]]<<" ";
        }
    }
    else if(r.op2.code.find(v1[i])!= r.op2.code.end())
    {
      out<<r.op2.code[v1[i]]<<" ";
    }
  }
        }
      if(s[b]!="" && s[b].find(',')==string::npos)
      {
        if(r.op2.code.find(s[b])== r.op2.code.end())
        {
          out<<"- ";
        }
        if(r.op2.code.find(s[b])== r.op2.code.end())
        {
            if(b1.op3.code.find(s[b])== b1.op3.code.end())
            {
              if(s[b].find("=")!=string::npos)
              {
                string h=to_string(loc);
                if(l1.code.find(s[b])==l1.code.end() )
                    {
                      l1.code[s[b]]=h;
                      o++;
                     out<<"(L,"<<s[b]<<") ";
                      }
                else if(l1.code.find(s[b])!=l1.code.end() )
                  out<<"(L,"<<s[b]<<") ";
              }
              else
              {
                string h=to_string(loc);
                int len=s[b].length();
                char a[len+1];
                strcpy(a,s[b].c_str());
                int zz=0,yy=0;
                for(auto x : a)
                {
                  if(isalpha(x)!=0)
                    zz++;
                  if(isdigit(x)!=0)
                    yy++;
                }
                if(zz==s[b].length())
                {
                    if(sy.find(s[b])==-1)
                    {
                      sy.insert(s[b],"");
                      out<<"(S,"<<s[b]<<") ";
                    }
                    else if(sy.find(s[b])!=-1)
                      out<<"(S,"<<s[b]<<") ";
                }
                else
                {
                  out<<"(C,"<<int(s[b][0])<<")"<<" ";
                }
            }

          }
            else if(b1.op3.code.find(s[b])!= b1.op3.code.end())
            {
              out<<b1.op3.code[s[b]]<<" ";
            }
        }
        else if(r.op2.code.find(s[b])!= r.op2.code.end())
        {
          out<<r.op2.code[s[b]]<<" ";
        }
      }
    }
    //
    loc++;
    out<<endl;
  }
  out.close();
  f.close();
}
//pass2
void pass2()
{
  ifstream f;
  f.open("/home/GanapriyaS/Documents/SPOS/2 pass assembler/Intermediate code.txt");
  ofstream out;
  out.open("Machine code.txt");
  int j=0;
  string line;
  int v=0;
  while(getline(f,line))
  {
    string *s=new string[4];
    string word= "";
    v=0;
    string ll="";
    for (char x:line)
    {
      if(x==' ' && word!="")
      {

        s[v]=word;
        if(v<3)
        {
        v=v+1;
        }
        else
        {
          break;
        }
        word="";
      }
      if(x!=' ')
      {
      word=word+x;
      if(line.find("(IS,00)")==string::npos)
      ll=word;
    }
    }
    s[v]=ll;
    if(j==0)
    {
      string h=s[0];
      h=numprint(h);
      out<<h<<" "<<s[1]<<" ";
      h=s[2];
      h=numprint(h);
      out<<h;
      j=1;
  }
  else
  {
    out<<s[0]<<" ";
    if(s[1].find("IS")!=string::npos)
    {
      string h=s[1];
      h=extnum(h);
      out<<h<<" "<<s[2]<<" ";
      if(s[3].find("S")!=string::npos)
      {

        int u=s[3].length()-4;
        string f=s[3].substr(3,u);
        out<<sy.head[sy.find(f)]->address<<" ";
      }
      else if(s[3].find("L")!=string::npos)
      {
        int u=s[3].length()-4;
        string f=s[3].substr(3,u);
        out<<l1.code[f]<<" ";
      }
    }
    else if(s[1].find("DL")!=string::npos && s[1].find("01")!=string::npos)
      out<<"- - -";
    else if(s[1].find("DL")!=string::npos || s[1].find("AD")!=string::npos)
      out<<"- - "<<s[3];
  }
out<<endl;
}
out.close();
f.close();
}

int main()
{
  p.open();
  m.open();
  r.open();
  b1.open();
  pass1();
  pass2();
  ifstream f;
  f.open("Input1.txt");
  string str="";
  if(f)
  {
    ostringstream ss;
    ss << f.rdbuf();
    str = ss.str();
  }
  cout<<"\nINPUT FILE\n"<<endl;
  cout<<str;
  f.close();
  cout<<"\nINTERMEDIATE CODE FILE\n"<<endl;
  f.open("Intermediate code.txt");
  str="";
  if(f)
  {
    ostringstream ss;
    ss << f.rdbuf();
    str = ss.str();
  }
  cout<<str;
  f.close();
  cout<<"\n\nMACHINE CODE FILE\n"<<endl;
  f.open("Machine code.txt");
  str="";
  if(f)
  {
    ostringstream ss;
    ss << f.rdbuf();
    str = ss.str();
  }
  cout<<str;
  f.close();
  cout<<endl;
  return 0;
}
