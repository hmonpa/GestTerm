#include "terminal.hpp"
// --------------------------- Mètodes privats ---------------------------


// --- ORDENACIÓN -> TRADUCIDA DIRECTAMENTE DE MERGESORT DE ENTEROS, HAY QUE RETOCARLA ---
/*
//
void terminal::merge(list<string> l1, list<string> l2)
{
  // PRE:
  // POST:

  nat cont1 = 0, cont2 = 0;
  list<string> l(l1.size() + l2.size());

  for (int i=0; i<l.size(); i++){
      if (cont1 >= l1.size() and cont2 < l2.size()){
          l(i) = l2(cont2);
          cont2++;
      }
      else if (cont2 >= l2.size() and cont1 < l1.size()){
          l(i) = l1(cont1);
          cont1++;
      }
      else if (l1(cont1) < l2(cont2) and cont1 < l1.size()){
          l(i) = l1(cont1);
          cont1++;
      }
      else if (l1(cont1) > l2(cont2) and cont2 < l2.size()){
          l(i) = l2(cont2);
          cont2++;
      }
  }
  return l;
}

//
void terminal::mergesort(list<string> &l)
{
  // PRE:
  // POST:

    int mida = l.size();
    list<string> l2(mida/2);
    nat m = mida/2;
    if (mida%2 != 0) m++;
    list<string> l1(m);                               // Dos llistes de tamany "mida/2" (o un mida/2 i l'altre (mida/2)+1)

    for (nat i=0; i<m; i++){
        if (i < l2.size()){
            l2(l2.size()-i-1) = l(mida-i-1);
        }
        l1(i) = l(i);
    }
    if (l1.size() > 1) mergesort(l1);            // Partir
    if (l2.size() > 1) mergesort(l2);
    l = merge(l1, l2);
}*/

//
void terminal::inicialitza_am(int n, int m, int h)
{
  // PRE:
  // POST:

//  string buit = "___";
  string buit = "___";
  est_am = new string**[n];
  for (int i=0; i<n; i++)
  {
    est_am[i] = new string*[m]();

    for (int j=0; j<m; j++)
    {
      est_am[i][j] = new string[h]();

      for (int k=0; k<h; k++)
      {
        //est_am[i][j][k] = new string();
        est_am[i][j][k] = buit;
      }
    }
  }
}

void terminal::borra_am()
{
  // PRE:
  // POST:

  for (int i=0; i<_n; i++)
  {
    for (int j=0; j<_m; j++)
    {
      delete[] est_am[i][j];
    }
    delete[] est_am[i];
  }
  delete[] est_am;
  opsgrua = 0;
}

//
void terminal::crea_llista_lliures(int n, int m, int h)
{
  // PRE:
  // POST:
  //std::cout << n << m << h;
  node *act = NULL;
  node *prev = NULL;
  _size = 0;

  std::cout << "\n ---- Lista de ubicaciones libres: ----" << '\n';
  for (int i=0; i<n; i++)
  {
    for (int j=0; j<m; j++)
    {
      for (int k=0; k<h; k++)
      {
        if (prev == NULL){
          _head = new node;
          _head->_u = ubicacio(i,j,k);
          act = _head;
          act->_ant = NULL;
        }
        else{
          act = new node;
          act->_u = ubicacio(i,j,k);
          prev->_seg = act;
          act->_ant = prev;
        }
        act->_lliu = true;            // Se inicializan las posiciones a TRUE = Están libres todas inicialmente
        act->_seg = NULL;
        prev = act;
        _size++;
        std::cout << act->_u.filera() << act->_u.placa() << act->_u.pis();
        std::cout << "- " << act->_lliu << "  ";
      }
    }
  }
  std::cout << "\n Tamano total de la lista: " << _size << "\n\n";
  //prev->_seg = NULL;
  //std::cout << "Elementos en la lista enlazada: " << _size << '\n';
}

//
void terminal::insereix_ff(Cu co_ub) throw(error)
{
  // PRE:
  // POST:

  if (not _ct.existeix(co_ub.c.matricula()))
  {
    nat longi = co_ub.c.longitud() / 10;
    bool trobat = false;    // Se pone a TRUE para indicar que hemos llegado al final del área de almacenaje
    node *p = _head;
    node* inici = p;
    node* p2;
    nat i = 0;

    // El contenedor necesita más ubicaciones que tanaño del área de almacenaje
    if (longi > _size)
    {
      trobat = true;
    }

    // RECORRIDO EN BUSCA DE UBICACIONES PARA LOS CONTENEDORES
    while (p != NULL and i < longi and not trobat)
    {
      // EL CONTENEDOR ES DE 10 PIES
      if (longi == 1)
      {
        while (p != NULL and p->_lliu != true)      // Busca una libre
        {
          p=p->_seg;
        }
        if (p == NULL)
        {
          trobat = true;
        }
        else                                        // Si no, es que ha encontrado una libre como es de 10 pies, se asigna directamente
        {
          co_ub.u = p->_u;
          _ct.assig(co_ub.c.matricula(), co_ub);      // Asigna al catálogo el contenedor
          p->_lliu = false;                           // Marca la ubicación como ocupada

          // Se inserta contenedor en su ubicación del área de almacenaje (matriz)
          int i = p->_u.filera();
          int j = p->_u.placa();
          int k = p->_u.pis();
          est_am[i][j][k] = co_ub.c.matricula();
          //std::cout << "10 PIES: Ub del mapa almacenaje:   " << est_am[i][j][k] << '\n';
          std::cout << "Colocado en " << p->_u.filera() << " " << p->_u.placa() << " " << p->_u.pis() << "\n";
          opsgrua++;

          // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
          // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
          trobat = true;
        }
      }
      // EL CONTENEDOR ES DE 20 o 30 PIES
      else
      {
        // BUSCA NODO PARA EMPEZAR QUE ESTE LIBRE
        while (p != NULL and p->_lliu != true)
        {
          p = p->_seg;
        }
        if (p == NULL) trobat = true;

        inici = p;
        i++;
        int filera_act = inici->_u.filera();
        int pis_act = inici->_u.pis();
        while (i < longi and not trobat)
        {
            p = p->_seg;
            while (p != NULL and p->_u.pis() != pis_act and p->_u.filera() == filera_act) // Fem el salt cap a la següent plaça
            {
                p = p->_seg;
            }

            if (p == NULL)
            {
              trobat = true; // ES NULL
            }
            else                                    // Se ha encontrado una posible ubicación
            {
              //std::cout << "Primera lliure: " << p->_u.filera() << " " << p->_u.placa() << " " << p->_u.pis() << "\n";
            // El principio del contenedor NO está en la base
              if (inici->_u.pis() != 0)
              {
              //  std::cout << "INICI Fuera de la base " << inici->_u.pis() << " esta libre? " << inici->_lliu << '\n';
                if (p->_ant->_lliu == true)       // El de debajo nuestro no está ocupado, por tanto, no podemos colocar en el aire
                {
                  inici = inici->_seg;
                  filera_act = inici->_u.filera();
                  pis_act = inici->_u.pis();
                  p = inici;
                  p2 = p;
                  i = 0;
                }
                else
                {
                  if ((inici->_u.filera() == p->_u.filera()) and (inici->_lliu == true and p->_lliu == true))
                  {
                    i++;
                    if(i == 2) p2 = p;
                  }
                }
              }
              // El principio del contenedor SI está en la base
              else
              {
                //std::cout << "Inici EN LA BASE: " << inici->_u.pis() << " esta libre? " << inici->_lliu  << '\n';
                //std::cout << "Inici ocupa" << inici->_u.filera() << " " << inici->_u.placa() << " " << inici->_u.pis() << "\n";
                if ((inici->_u.filera() == p->_u.filera()) and (inici->_lliu == true and p->_lliu == true))
                {
                  //std::cout << "P ocupa: " << p->_u.filera() << " " << p->_u.placa() << " " << p->_u.pis() << "\n";
                  i++;
                  if(i == 2) p2 = p;
                  //std::cout << "Contador i :   " << i << '\n';
                }
                else
                {
                  inici = inici->_seg;
                  inici->_u.filera();
                  inici->_u.pis();
                  p = inici;
                  p2 = p;
                  i = 0;
                }
              }
            }
          }
      }
    }
    // FIN DEL BUCLE -- INSERCIONES

      // TROBAT = FALSE -> SE AÑADE AL CATALEG Y AL AREA DE ALMACENAJE
      if (not trobat)
      {
        // 1- Insercions al catàleg de contenidors:
        // Contenidor de 20 peus -> Insercions al catàleg d'ubicacions
        if (longi == 2){
          int i = inici->_u.filera();
          int j = inici->_u.placa();
          int k = inici->_u.pis();
          est_am[i][j][k] = co_ub.c.matricula();
          std::cout << "20 PIES: Ub del mapa almacenaje:1 " << est_am[i][j][k] << '\n';
                    std::cout << "Ocupando: " << inici->_u.filera() << " " << inici->_u.placa() << " " << inici->_u.pis() << "\n";
          co_ub.u = inici->_u;
          _ct.assig(co_ub.c.matricula(), co_ub);

          i = p2->_u.filera();
          j = p2->_u.placa();
          k = p2->_u.pis();
          est_am[i][j][k] = co_ub.c.matricula();
          std::cout << "20 PIES: Ub del mapa almacenaje:2 " << est_am[i][j][k] << '\n';
                    std::cout << "Ocupando: " << p2->_u.filera() << " " << p2->_u.placa() << " " << p2->_u.pis() << "\n";

          co_ub.u = p2->_u;
          _ct.assig(co_ub.c.matricula(), co_ub);

          opsgrua++;

          inici->_lliu = false;
          p2->_lliu = false;

          // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
          // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
        }
        // Contenidor 30 peus -> Insercions al catàleg de contenidors
        else if (longi == 3){
          co_ub.u = inici->_u;
          _ct.assig(co_ub.c.matricula(), co_ub);
          co_ub.u = p2->_u;
          _ct.assig(co_ub.c.matricula(), co_ub);
          co_ub.u = p->_u;
          _ct.assig(co_ub.c.matricula(), co_ub);

          int i = inici->_u.filera();
          int j = inici->_u.placa();
          int k = inici->_u.pis();

          est_am[i][j][k] = co_ub.c.matricula();
          std::cout << "30 PIES: Ub del mapa almacenaje:1 " << est_am[i][j][k] << '\n';
          std::cout << "Ocupando: " << inici->_u.filera() << " " << inici->_u.placa() << " " << inici->_u.pis() << "\n";

          i = p2->_u.filera();
          j = p2->_u.placa();
          k = p2->_u.pis();
          est_am[i][j][k] = co_ub.c.matricula();
          std::cout << "30 PIES: Ub del mapa almacenaje:2 " << est_am[i][j][k] << '\n';
                    std::cout << "Ocupando: " << p2->_u.filera() << " " << p2->_u.placa() << " " << p2->_u.pis() << "\n";

          i = p->_u.filera();
          j = p->_u.placa();
          k = p->_u.pis();
          est_am[i][j][k] = co_ub.c.matricula();
          std::cout << "30 PIES: Ub del mapa almacenaje:3 " << est_am[i][j][k] << '\n';

                    std::cout << "Ocupando: " << p->_u.filera() << " " << p->_u.placa() << " " << p->_u.pis() << "\n";
          opsgrua++;

          inici->_lliu = false;
          p2->_lliu = false;
          p->_lliu = false;

          // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
          // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
        }
      }
      // TROBAT = TRUE -> DIRECTO AL CATALEG Y AL ÁREA DE ESPERA
      else
      {
          if (not _ct.existeix(co_ub.c.matricula()))
          {
            ubicacio u_ae(-1,0,0);
            co_ub.u = u_ae;
            _ct.assig(co_ub.c.matricula(), co_ub);
            _area_espera.push_front(co_ub.c.matricula());
            std::cout << "Colocado en el área de espera \n";
          }
      }
    }
  else
  {
    throw error(MatriculaDuplicada);
  }
}

//
void terminal::borra_llista_lliures(node *&n)
{
    // PRE:
    // POST:

    if (n != NULL)
    {
        borra_llista_lliures(n->_seg);
        delete n;
        n = NULL;
    }
}


// --------------------------- Mètodes públics ---------------------------

//
terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error):
        _n(n),
        _m(m),
        _h(h),
        _st(st),
        _ct(n * m * h),
        _head(NULL)
{
  // PRE: True
  // POST: Crea una terminal vàlida, y una llista enllaçada d'ubicacions lliures
  //       Retorna un error en cas contrari

  if ((_n != 0) and (_m != 0) and (_h != 0) and (_h <= HMAX) and (st == FIRST_FIT || st == LLIURE))
  {

    _n = n;
    _m = m;
    _h = h;

    inicialitza_am(_n, _m, _h);
    crea_llista_lliures(_n, _m, _h);

    if (st == FIRST_FIT)    _st = FIRST_FIT;
    else if (st == LLIURE)  _st = LLIURE;

  }
  else if (n == 0)
  {
    throw error(NumFileresIncorr);
  }
  else if (m == 0)
  {
      throw error(NumPlacesIncorr);
  }
  else if (h == 0 or h >= HMAX)
  {
      throw error(AlcadaMaxIncorr);
  }
  else if (st != FIRST_FIT and st != LLIURE)
  {
      throw error(EstrategiaIncorr);
  }
}

//
terminal::terminal(const terminal& b) throw(error):
        _n(b._n),
        _m(b._m),
        _h(b._h),
        _st(b._st),
        _ct(b._ct),
        _head(NULL)
{
  // PRE:
  // POST:

  inicialitza_am(_n, _m, _h);

  crea_llista_lliures(_n, _m, _h);
  _ct = b._ct;
  _area_espera = b._area_espera;
}

//
terminal& terminal::operator=(const terminal& b) throw(error)
{
  // PRE:
  // POST:

  if (this != &b)
  {
    terminal t(b);        // t es una copia de b

    node* _arrel = _head;
    _head = t._head;
    t._head = _arrel;

    _ct = t._ct;
    _area_espera = t._area_espera;
  }

  return *this;
}

//
terminal::~terminal() throw()
{
  // PRE:
  // POST:

  borra_llista_lliures(_head);
  borra_am();
  //_ct.~cataleg();

  _area_espera.clear();

}

void terminal::insereix_contenidor(const contenidor &c) throw(error)
{
  Cu co_ub;
  co_ub.c = c;
  //std::cout << "Cambio de información en el objeto co_ub, campo c: " << co_ub.c.matricula() << '\n';
  if (_st == FIRST_FIT) insereix_ff(co_ub);
  std::cout << '\n' << "Elements ara mateix al catàleg: " << _ct.quants() << '\n';
  //if (_st == LLIURE) insereix_ll(c);

  // TO DO:
  /*if (c existe) Error duplicidad
    else
          1- le busca una ubicacion en la lista de ubicacions lliures
              1.1- la encuentra:
                    1.1.1 añade al catalogo ct <K = Matricula y V= struct Cu = <Contenedor, Ubicación>>
                    1.1.2 Marca las ubicaciones de la lista como FALSE en caso de ser ocupadas por un contenedor
                    1.1.3 Añade a la matriz el contenedor en la ubicación indicada
                    En caso de ser un contenedor de 20 o 30 pies, se harán 2 o 3 addiciones al catalogo ct (el mismo contenedor 2 o 3 veces con sus 2 o 3 ubicaciones),
                    se tendrán que marcar como false 2 o 3 ubicaciones de la lista de lliures y se añadirán a la matriz 2 o 3 ubicaciones y se les añdirá el mismo contenedor

                    1.1.4 recorre área de espera de inicio a fin, e intenta hacer un insereix(c) de todos los contenedores almacenados
                      1.1.4.1 Consigue añadir contenedor: vuelve al paso 1.1.1.
                      1.1.4.2 No consigue añadir contenedor: sale del método insereix y el área de espera se queda intacta.
              1.2- no encuentra sitio:
                    1.2.1 se añade contenedor al área de espera

  */
}

void terminal::retira_contenidor(const string &m) throw(error)
{
  // TO DO:
  string m2 = m;
}

//
ubicacio terminal::on(const string &m) const throw()
{
  // PRE:
  // POST:

  //std::cout << _ct.existeix(m) << '\n';
  ubicacio u = ubicacio(-1,-1,-1);

  if (_ct.existeix(m))
  {
      u = _ct[m].u;
  }

  return u;
}

//
nat terminal::longitud(const string &m) const throw(error)
{
  // PRE:
  // POST:

  if (_ct.existeix(m))
  {
    contenidor c = _ct[m].c;
    nat lon = c.longitud();
    return lon;
  }
  else
  {
    throw error(MatriculaInexistent);
  }
}

//
void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error)
{
  // PRE:
  // POST:

  int i = u.filera();
  int j = u.placa();
  int k = u.pis();
  //std::cout << "dimesiones terminal: " << _n << " X " << _m << " X " << _h << " ";
  //std::cout << "ubicacion actual: " << i << " X " << j << " X " << k << " ";
  if (((i >= 0) and (j >= 0) and (k >= 0)) and ((i < _n) and (j < _m) and (k < _h)))
  {
    m = est_am[i][j][k];
    if (m == "___") m = "";
  }
  else
  {
    throw error(UbicacioNoMagatzem);
  }
}

//
nat terminal::fragmentacio() const throw()
{
  // PRE:
  // POST:

    node *p = _head;
    nat frag = 0;

    node* aux = NULL;
    while (p != NULL)
    {
      aux = p;
      int i = 0;
      if (_m == 1)
      {
        if (p->_lliu == true) frag++;
        while (p != NULL and i < _h) p = p->_seg;
      }
      else
      {
        while (aux->_seg != NULL and i < _h)
        {
          aux = aux->_seg;
        }
        if (aux->_u.placa() == _m)      // Si aux està a la ultima plaça de la filera...
        {
          if (p->_lliu == false and aux->_lliu == true){   // Es compara la penultima plaça i la ultima, si hi ha un forat a la ultima, frag++
            if (p->_u.filera() == aux->_u.filera()){
              frag++;
            }
            while (p != NULL and p->_u.pis() != 0)      // Trobada una fragmentació o sent fileres diferents, passem a la següent plaça
            {
              p = p->_seg;
            }
          }
          else {
            p = p->_seg;
          }
        }
        else {                        // Si aux no està a la ultima plaça de la filera
          if (p->_lliu == true and aux->_lliu == false) // Es comparen dues places consecutives, si hi ha un forat a la primera d'elles, frag++
          {
            if (p->_u.filera() == aux->_u.filera()){
              frag++;
            }
            while (p != NULL and p->_u.pis() != 0)      // Trobada una fragmentació o sent fileres diferents, passem a la següent plaça
            {
              p = p->_seg;
            }
          }
          else {
            p = p->_seg;
          }
        }
      }
    }
    return frag;
}

//
nat terminal::ops_grua() const throw()
{
  // PRE:
  // POST:

  return opsgrua;
}

//
void terminal::area_espera(list<string> &l) const throw()
{
  // PRE:
  // POST:

  //std::cout << l.size() << '\n';
  l = _area_espera;
  // TO DO :
  // Método de ordenación de la lista l (merge preferiblemente)
  // Bucle que recorra con un iterador desde l.begin hasta l.end y printee los elementos de la lista l
  /*for (list<string>::iterator it=l.begin(); it!=l.end(); it++)
  {
    std::cout << *it << '\n';
  }*/
}

// θ(1)
nat terminal::num_fileres() const throw()
{
  // PRE:
  // POST:

  return _n;
}

// θ(1)
nat terminal::num_places() const throw()
{
  // PRE:
  // POST:

  return _m;
}

// θ(1)
nat terminal::num_pisos() const throw()
{
  // PRE:
  // POST:

  return _h;
}

// θ(1)
terminal::estrategia terminal::quina_estrategia() const throw()
{
  // PRE:
  // POST:

  return _st;
}
