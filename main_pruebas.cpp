#include <iostream>
#include <fstream>
#include <list>
#include "terminal.hpp"
#include "cataleg.hpp"
#include "contenidor.hpp"
#include "ubicacio.hpp"
using util::byte;
using namespace std;
/*void printa_lista(list<string> &l)
{
  list<string>::iterator it;
  for (it = l.begin(); it != l.end(); it++)
      cout << " " << *it;
  cout << endl;
}*/
//void estado_terminal(terminal &t) {
  //list<string> l;
  //t.area_espera(l);
  //cout<<"Area d'espera: "<<endl;
  //printa_lista(l);
  //cout<<endl;
  /*
  cout<<"Operacions de grua: "<<t.ops_grua()<<endl;
  cout<<"Fragmentacion"<<t.fragmentacio()<<endl;


  for (nat i=0; i < t.num_fileres(); ++i) {
    cout<< endl << "Filera " << i << endl;
    cout << "---------" << endl;
    for (int k=(int)t.num_pisos()-1; k >= 0; --k) {
      cout << "Pis " << k << ' ';
      for (nat j=0; j < t.num_places(); ++j) {
        ubicacio u(i, j, k);
        string m = "";
        t.contenidor_ocupa(u, m);
        string out(3, ' ');
        if (m == "" and k == 0) {
          out = string(3, '_');
        }
        else {
          for (nat x=0; x<m.length() and x<3; ++x) {
            out[x] = m[x];
          }
        }
        cout << " " << out;
      }
      cout << endl;
    }
    cout << "       ";
    for (nat i=0; i < t.num_places(); ++i) {
      string s = util::tostring((int) i);
      if (s.length() > 3) {
        s = s.substr(0, 3);
      }
      else {
        for (int x=s.length(); x < 3; ++x) {
          s = s + ' ';
        }
      }
      cout << ' ' << s;
    }
    cout << endl;
  }
  */
//}

int main() {
  cout<<"Introdueix les dimensions del terminal que vols crear:"<<endl;
  nat n, m, h;
  cin>>n>>m>>h;
  terminal t (n,m,h, terminal::FIRST_FIT);

  return 0;
}
