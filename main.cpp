#include <iostream>
#include <fstream>
#include <list>
#include "terminal.hpp"
#include "cataleg.hpp"
#include "contenidor.hpp"
#include "ubicacio.hpp"

using namespace std;

int main() 
{
  cout << "Introdueix les dimensions del terminal que vols crear:" << endl;

  nat n, m, h;
  list<string> l;
  cin >> n >> m >> h;

  terminal* t = new terminal (n,m,h,terminal::FIRST_FIT);

  cout << "Creación de nueva terminal: " << endl;

  cout << t -> num_fileres() << '\n';
  cout << t -> num_places() << '\n';
  cout << t -> num_pisos() << '\n';
  cout << '\n';

  contenidor* c = new contenidor("A30", 30);
  t -> terminal::insereix_contenidor(*c);

  t->area_espera(l);

  cout << "Nueva terminal creada a través de una copia:" << endl;
  terminal* t2(t);
  cout << t2 -> num_fileres() << '\n';
  cout << t2 -> num_places() << '\n';
  cout << t2 -> num_pisos() << '\n';
  cout << '\n';
  
  cout << "Nueva terminal a partir del operador de asignación: " << endl;
  terminal* t3 = t;
  cout << t3 -> num_fileres() << '\n';
  cout << t3 -> num_places() << '\n';
  cout << t3 -> num_pisos() << '\n';

}
