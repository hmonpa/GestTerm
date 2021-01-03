#include "terminal.hpp"
// --------------------------- Mètodes privats ---------------------------

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
        act = new node;
        act->_u = ubicacio(i,j,k);
        act->_seg = NULL;

        if (prev == NULL){
          act->_ant = NULL;
          _head = act;
        }
        else{
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
terminal::node_tst* terminal::crea_tst(node_tst *n)
{
  // PRE:
  // POST:

  node_tst* aux = NULL;

  if (n != NULL)
  {
    aux = new node_tst;
    aux->_esq = crea_tst(n->_esq);
    aux->_cen = crea_tst(n->_cen);
    aux->_dret = crea_tst(n->_dret);
    aux->_k = n->_k;
    aux->_v = n->_v;
  }
  return aux;
}

//
void terminal::insereix_ff(contenidor c, nat h) throw(error)
{
  // PRE:
  // POST:

  if (not _ct.existeix(c.matricula()))
  {
    //ct.assig(c.matricula(), c);

    node *p = _head;
    bool trobat = false;

    nat longi = c.longitud() / 10;

    while (p != NULL and not trobat)
    {
      if (longi == 1)
      {
        _ct.assig(c.matricula(), std::make_pair(c,p->_u));
        // PENDIENTE: Insertar en el área de almacenaje (TST) el contenedor
        opsgrua++;

        // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
        // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
        node *aux = p;
        p = p->_seg;
        delete aux;
        p->_ant = NULL;
        _head = p;
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
              if (inici->_u.filera() == p->_u.filera() and inici->_u.pis() == p->_u.pis()){
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
          // Caso 2: No estamos en la base
          else if (inici->_u.pis() > 0){
            nat z = 0;
            nat estic = inici->_u.pis();   // Estic = piso en el que nos encontramos
            while (p!=NULL and z < h-estic)
            {
              p = p->_seg;
            }
            if (z != h) trobat = true;
            if (inici->_u.filera() == p->_u.filera() and inici->_u.pis() == p->_u.pis()){
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
        if (not trobat){
          // S'han trobat ubicacions per un contenidor de 20 o 30 peus ( i == longi )
          // per tant, tenim que:
          //          1- enviar el contenidor en qüestió i la seva ubicació al catàleg d'ubicacions
          //            sent K = contenidor.matricula i V = objecte ubicació.
          //          2- reordenar encadenaments a la llista d'ubicacions lliures

          // 1- Insercions al catàleg de contenidors:
          // Contenidor de 20 peus -> Insercions al catàleg d'ubicacions
          if (longi == 2){

            _ct.assig(c.matricula(), std::make_pair(c,inici->_u));
            _ct.assig(c.matricula(), std::make_pair(c,p2->_u));
            // PENDIENTE: Insertar en e área de almacenaje (TST) el contenedor
            opsgrua++;
            // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
            // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
          }
          // Contenidor 30 peus -> Insercions al catàleg de contenidors
          else if (longi == 3){
            _ct.assig(c.matricula(), std::make_pair(c,inici->_u));
            _ct.assig(c.matricula(), std::make_pair(c,p2->_u));
            _ct.assig(c.matricula(), std::make_pair(c,p->_u));
            // PENDIENTE: Insertar en e área de almacenaje (TST) el contenedor
            opsgrua++;

          }
          // 2. Reordenació encadenaments
          // Inici es el primer node de la llista d'ubicacions lliures (Retirem el primer node)
          if (p->_ant = NULL)
          {
            _head = inici->_seg;
            _head->_ant = NULL;
            _head->_seg = inici->_seg->_seg;
          }
          // Inici no es el primer node de la llista d'ubicacións lliures (Retirem el primer node)
          else
          {
            inici->_ant->_seg = inici->_seg;
            inici->_seg->_ant = inici->_ant;
            delete inici;
          }
          // Contenidor de 30 peus, retirem una ubicació que ocupa 3 nodes
          if (p2 != p){
            p2->_ant->_seg = p2->_seg;
            p2->_seg->_ant = p2->_ant;
            p->_ant->_seg = p->_seg;
            p->_seg->_ant = p->_ant;
            delete p2;
          }
          // Contenidor de 20 peus, retirem una ubicació que ocupa 2 nodes
          else {
            p->_ant->_seg = p->_seg;
            p->_seg->_ant = p->_ant;
            delete p;
          }
          // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambian,
          // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
        }
        else {
          // El bool trobat es true, per tant, hi han dos possilibitats:
          //    1- S'ha afegit un contenidor de 10 peus (això es fa directament dins del bucle i es força la sortida amb el booleà)
          //    2- Buscnt ubicacions adjacents per a un contenidor de 20 o 30 peus, ens hem sortit
          //    de les dimensions de l'àrea d'emmagatzematge

          // longi != 1, per tant, ens em sortit de les dimensions
          if (longi != 1){
            ubicacio u_ae(-1,0,0);          // Ubicacio especial àrea espera
            _ct.assig(c.matricula(), std::make_pair(c,u_ae));   // S'afegeix al catàleg de contenidors la matricula amb la ubicació d'àrea espera
            _area_espera.push_front(c);       // S'afegeix objecte contenidor a l'àrea d'espera
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

//
void terminal::borra_tst(node_tst* n)
{
  // PRE:
  // POST: 

  if (n != NULL)
  {
    borra_tst(n->_esq);
    borra_tst(n->_cen);
    borra_tst(n->_dret);

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
        _ct(n*m*h),
        _arrel(NULL),
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

  _n = b._n;
  _m = b._m;
  _h = b._h;

  crea_llista_lliures(_n, _m, _h);

  _ct = b._ct;

  _arrel = crea_tst(b._arrel);

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
  borra_tst(_arrel);
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
                    1.1.1 añade al catalogo ct <K = Matricula y V= Ubicación encontrada>
                    1.1.2 arregla encadenamientos en la lista de ubicaciones libres, elimina los que se han ocupado
                    1.1.3 Añade al tst una clave ubicacion = i, j, k -> TO INT -> se le asocia el contenedor como valor
                    En caso de ser un contenedor de 20 o 30 pies, se harán 2 o 3 addiciones al catalogo ct (el mismo contenedor 2 o 3 veces con sus 2 o 3 ubicaciones),
                    se tendrán que arreglar 2 o 3 encadenamientos de la lista de lliures y se añadirán al TST 2 o 3 ubicaciones y se les asociará el mismo contenedor como valor.

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

ubicacio terminal::on(const string &m) const throw()
{

}

nat terminal::longitud(const string &m) const throw(error)
{
  // PRE:
  // POST:

  if (_ct.existeix(m))
  {

  }
  else
  {

  }
}

void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error)
{

}

nat terminal::fragmentacio() const throw()
{

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
estrategia terminal::quina_estrategia() const throw()
{
  // PRE:
  // POST:

  return st;
}
