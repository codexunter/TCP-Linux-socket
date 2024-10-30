#include <iostream>
#include <string>
using namespace std;
class Input{
  public:
    string name;
    int port;
    int period;
    Input(string data){
      name.clear();
      port=period=-1;
      for(int i=0, n1=0, n2=0; data[i]!='\0'; ++i){
        if(n1==0){
          if((data[i]==' ' || data[i]=='\t') && data[i+1]>47 && data[i+1]<58)
            n1=1;
          else name.push_back(data[i]);
        }
        else if (n2==0){
          if (data[i]<48 || data[i]>57)
            n2=1;
          else if(port==-1)
            port=data[i]-48;
          else port=port*10+data[i]-48;
        }
        else {
          if (data[i]<48 || data[i]>57)
            break;
          if(period==-1)
            period=data[i]-48;
          else period=period*10+data[i]-48;
        }
      }
    }
    ~Input(){
      name.clear();
    }
};