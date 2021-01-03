#include "terminal.hpp"
// --------------------------- Mètodes privats ---------------------------


/*terminal::terminal::node (ubicacio u, bool lliu, node* seg, node* ant):
 _u(u), _lliu(lliu), _seg(seg), _ant(ant)
{

}*/

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

  for (int i=0; i<n; i++)
  {
    for (int j=0; j<m; j++)
    {
      for (int k=0; k<h; k++)
      {
        est_am[i][j][k].matricula() = ' ';
      }
    }
  }
}


//
void terminal::crea_llista_lliures(int n, int m, int h)
{
  // PRE:
  // POST:

  node *act = NULL;
  node *prev = NULL;

  for (int i=0; i<n; i++)
  {
    for (int j=0; j<m; j++)
    {
      for (int k=0; k<h; k++)
      {
        if (prev == NULL){
          _head = new node;
          act->_ant = NULL;
        }
        act = _head;
        act->_u = ubicacio(i,j,k);
        act->_lliu = true;            // Se inicializan las posiciones a TRUE = Están libres todas inicialmente
        act->_seg = NULL;
        
        if (prev != NULL){
          prev->_seg = act;
          act->_ant = prev;
        }
        prev = act;
      }
    }
  }
  prev->_seg = NULL;
}

//
void terminal::insereix_ff(contenidor c, nat h) throw(error)
{
  // PRE:
  // POST:

  if (not _ct.existeix(c.matricula()))
  {
    node *p = _head;
    bool trobat = false;

    nat longi = c.longitud() / 10;

    while (p != NULL and not trobat)
    {
      while (p->_lliu != true)
      {
        p = p->_seg;
      }
      if (longi == 1)
      {
        _ct.assig(c.matricula(), std::make_pair(c, p->_u));
        // Se inserta contenedor en su ubicación del área de almacenaje (matriz)
        int i = p->_u.filera();
        int j = p->_u.placa();
        int k = p->_u.pis();
        est_am[i][j][k] = c;

        opsgrua++;

        // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
        // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
        p->_lliu = false;
        //node *aux = p;
        //p = p->_seg;
        //delete aux;
        //p->_ant = NULL;
        //_head = p;
        trobat = true;
      }
      else {
        nat i = 1;
        node* inici = p;
        node* p2;
        while (i < longi and not trobat){
          // Caso 1: estamos en la base
          if (inici->_u.pis() == 0)
          {
            nat z = 0;
            while (p!=NULL and z < h) // Fem el salt cap a la següent plaça
            {
              p = p->_seg;
              z++;
            }
            if (z != h) trobat = true;
            if (not trobat)
            {
              if ((inici->_u.filera() == p->_u.filera()) and (inici->_lliu == true and p->_lliu == true)){
                p2 = p;
                i++;
              }
              else {
                inici = inici->_seg;
                p = inici;
                i = 1;
              }
            }
          }
        }
        if (not trobat){
          // S'han trobat ubicacions per un contenidor de 20 o 30 peus ( i == longi )
          // per tant, tenim que:
          //          1- enviar el contenidor en qüestió i la seva ubicació al catàleg d'ubicacions
          //            sent K = contenidor.matricula i V = objecte ubicació.
          //          2- reordenar encadenaments a la llista d'ubicacions lliures

          // 1- Insercions al catàleg de contenidors:
          // Contenidor de 20 peus -> Insercions al catàleg d'ubicacions
          if (longi == 2){

            _ct.assig(c.matricula(), std::make_pair(c, inici->_u));
            _ct.assig(c.matricula(), std::make_pair(c, p2->_u));
            // Se inserta contenedor en su ubicación del área de almacenaje (matriz)
            int i = inici->_u.filera();
            int j = inici->_u.placa();
            int k = inici->_u.pis();
            est_am[i][j][k] = c;

            i = p2->_u.filera();
            j = p2->_u.placa();
            k = p2->_u.pis();
            est_am[i][j][k] = c;

            opsgrua++;

            inici->_lliu = false;
            p2->_lliu = false;

            // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
            // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
          }
          // Contenidor 30 peus -> Insercions al catàleg de contenidors
          else if (longi == 3){
            //_ct.assig(c.matricula(), {c, inici->_u});
            _ct.assig(c.matricula(), std::make_pair(c, inici->_u));
            _ct.assig(c.matricula(), std::make_pair(c, p2->_u));
            _ct.assig(c.matricula(), std::make_pair(c, p->_u));
            // Se inserta contenedor en su ubicación del área de almacenaje (matriz)
            int i = inici->_u.filera();
            int j = inici->_u.placa();
            int k = inici->_u.pis();
            est_am[i][j][k] = c;

            i = p2->_u.filera();
            j = p2->_u.placa();
            k = p2->_u.pis();
            est_am[i][j][k] = c;

            i = p->_u.filera();
            j = p->_u.placa();
            k = p->_u.pis();
            est_am[i][j][k] = c;

            opsgrua++;

            inici->_lliu = false;
            p2->_lliu = false;
            p->_lliu = false;

            // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
            // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.

          }
        }
        else {
          // El bool trobat es true, per tant, hi han dos possilibitats:
          //    1- S'ha afegit un contenidor de 10 peus (això es fa directament dins del bucle i es força la sortida amb el booleà)
          //    2- Buscnt ubicacions adjacents per a un contenidor de 20 o 30 peus, ens hem sortit
          //    de les dimensions de l'àrea d'emmagatzematge

          // longi != 1, per tant, ens em sortit de les dimensions
          if (longi != 1){
            ubicacio u_ae(-1,0,0);          // Ubicacio especial àrea espera
            _ct.assig(c.matricula(), std::make_pair(c, u_ae));   // S'afegeix al catàleg de contenidors la matricula amb la ubicació d'àrea espera
            _area_espera.push_front(c.matricula());       // S'afegeix objecte contenidor a l'àrea d'espera
          }
        }
      }
    }
  }
  else
  {
    throw error(MatriculaInexistent);
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


  if ((_n != 0) and (_m != 0) and (_h != 0) and _h <= HMAX and (st == FIRST_FIT || st == LLIURE))
  {
    _n = n;
    _m = m;
    _h = h;

    est_am = new contenidor [_n][_m][_h];

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
  else if (h == 0)
  {
      throw error(AlcadaMaxIncorr);
  }
  else if (st != FIRST_FIT and st != LLIURE)
  {
      throw error(EstrategiaIncorr);
  }
}

terminal::terminal(const terminal& b) throw(error)
{
  // PRE:
  // POST:

  /*_n = b._n;
  _m = b._m;
  _h = b._h;

  crea_llista_lliures(_n, _m, _h);
  _ct = b._ct;*/

}

terminal& terminal::operator=(const terminal& b) throw(error)
{

}

// 
terminal::~terminal() throw()
{
  // PRE:
  // POST:

  _ct.~cataleg();
  borra_llista_lliures(_head);
}

void terminal::insereix_contenidor(const contenidor &c) throw(error)
{
  if (_st == FIRST_FIT) insereix_ff(c, _h);
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
}

//
ubicacio terminal::on(const string &m) const throw()
{
  // PRE:
  // POST:

  ubicacio u = ubicacio(-1,-1,-1);

  if (_ct.existeix(m))
  {
    ubicacio u = _ct[m].second;
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
    contenidor c = _ct[m].first;
    nat lon = c.longitud();
    return lon;
  }
  else
  {
    throw error(MatriculaInexistent);
  }
}

void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error)
{

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
      while (aux != NULL and i < _h)
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

  // TO DO :
  // Método de ordenación de la lista l (merge preferiblemente)
  // Bucle que recorra con un iterador desde l.begin hasta l.end y printee los elementos de la lista l
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
