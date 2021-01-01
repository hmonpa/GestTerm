#include "terminal.hpp"

// --------------------------- Mètodes privats ---------------------------

//
void terminal::crea_llista(nat n, nat m, nat h):
        _head(NULL);
{
  //
  //

  _head = NULL;
  node *act;
  node *prev = NULL;

  for (nat i=0; i<n; i++)
  {
    for (nat j=0; j<m; j++)
    {
      for (nat k=0; k<h; k++)
      {
        act = new node;
        act->_u = ubicacio(i,j,k);
        act->_seg = NULL;

        if (prev == NULL)   _head = act;
        else                prev->_seg = act;

        prev = act;
      }
    }
  }
  prev->seg = NULL;
}

//
void terminal::insereix_ff(const contenidor &c, nat h) throw(error)
{
  //
  //
  if (not ct.existeix(c._mat))
  {
    ct.assig(c._mat, c);

    node *p = _head;
    bool trobat = false;
    nat long = c._lon/10;

    while (p != NULL and not trobat)
    {
      if (long == 1)
      {
        ut.assig(c._mat, p->u);
        // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
        // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
        node *aux = p;
        p = p->_seg;
        delete aux;
        p->_ant = NULL;
        head = p;
        trobat = true;
      }
      else {
        nat i = 1;
        node* inici = p;
        node* p2;
        while (i < long and not trobat){
          // Caso 1: estamos en la base
          if (inici->u._k == 0)
          {
            nat z = 0;
            while (p!=NULL and z < h) // Fem el salt cap a la següent plaça
            {
              p = p->seg;
              z++;
            }
            if (z != h) trobat = true;
            if (not trobat)
            {
              if (inici->u._i == p->u._i and inici->u._k == p->u._k){
                p2 = p;
                i++;
              }
              else {
                inici = inici->seg;
                p = inici;
                i = 1;
              }
            }
          }
          // Caso 2: No estamos en la base
          else if (inici->u._k > 0){
            nat z = 0;
            nat estic = inici->u._h;   // Estic = piso en el que nos encontramos
            while (p!=NULL z < h-estic)
            {
              p = p->seg;
            }
            if (z != h) trobat = true;
            if (inici->u._i == p->u._i and inici->u._k == p->u._k){
              p2 = p;
              i++;
            }
            else {
              inici = inici->seg;
              p = inici;
              i = 1;
            }
          }
        }
        if (not trobat){
          // S'han trobat ubicacions per un contenidor de 20 o 30 peus ( i == long )
          // per tant, tenim que:
          //          1- enviar el contenidor en qüestió i la seva ubicació al catàleg d'ubicacions
          //            sent K = contenidor.matricula i V = objecte ubicació.
          //          2- reordenar encadenaments a la llista d'ubicacions lliures

          // 1- Insercions al catàleg d'ubicacions:
          // Contenidor de 20 peus -> Insercions al catàleg d'ubicacions
          if (long == 2){
            ut.assig(c._mat, inici->u);
            ut.assig(c._mat, p2->u);
            // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
            // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
          }
          // Contenidor 30 peus -> Insercions al catàleg d'ubicacions
          else if (long == 3){
            ut.assig(c._mat, inici->u);
            ut.assig(c._mat, p2->u);
            ut.assig(c._mat, p->u);
            // PENDIENTE: Por cada vez que se inserta en el área de almacenaje, las ubicaciones libres cambin,
            // y hay que revisar el área de espera para ver si podemos insertar algun contenedor aqui.
          }
          // 2. Reordenació encadenaments
          // Inici es el primer node de la llista d'ubicacions lliures (Retirem el primer node)
          if (inici = _head)
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
        }
        else {
          // El bool trobat es true, per tant, hi han dos possilibitats:
          //    1- S'ha afegit un contenidor de 10 peus (això es fa directament dins del bucle i es força la sortida amb el booleà)
          //    2- Buscnt ubicacions adjacents per a un contenidor de 20 o 30 peus, ens hem sortit
          //    de les dimensions de l'àrea d'emmagatzematge

          // Long != 1, per tant, ens em sortit de les dimensions
          if (long != 1){
            _area_espera.push_front(c);
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

// --------------------------- Mètodes públics ---------------------------

//
terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error):
        _n(n),
        _m(m),
        _h(h),
        _st(st),
        ct(n*m*h),
        ut(n*m*h),
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

    crea_llista(_n, _m, _h);

    if (st == FIRST_FIT)    _st = FIRST_FIT;
    else if (st == LLIURE)  _st = LLIURE;
  }
  else if (n == 0)
  {
    throw error(NumFileresIncorr);
  )
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

}

terminal& terminal::operator=(const terminal& b) throw(error)
{

}

terminal::~terminal() throw()
{

}

void terminal::insereix_contenidor(const contenidor &c) throw(error)
{
  if (_st == FIRST_FIT) insereix_ff(c);
  if (_st == LLIURE) insereix_ll(c);

  // TO DO:
  /* Hay que añadir el contenedor c a la terminal usando la estrategia indicada.
     --- EST FIRST_FIT ---
      1. ¿El contenedor existe dentro de la terminal (AM o AEM)?
        BuscarEnAEM(c._mat)
        BuscarEnAM(c._mat)

        - En caso afirmativo, genera un error de Duplicidad, ya existe
        - En caso negativo, inserta el contenedor en el primer lugar libre que encuentre del AM
                - En caso de no encontrar ningún lugar libre, inserta el contenedor en el AEM


        En caso de ser un BST:
        Búsqueda en AEM -> Recorrer la lista con un iterador de begin a end
        Búsqueda en AM -> Recorrer el BST desde la raíz recursivamente hasta que llegue a NULL, comparando cada clave ubicación
        con el campo "mat" de su valor contenedor (c.mat)

        Tras la búsqueda:
        1. si ha encontrado la matricula, devuelve el error de duplicidad.
        2. si no ha encontrado la matricula:
        Vuelve a la raíz del BST, revisa el primer hueco libre que encuentra y compara si la clave (contenedor) es de lon = 10 (c.lon)
          - En caso afirmativo, puede colocar el contenedor.
          - En caso negativo, mira si el siguiente hueco (plaza+1) está vacio.
                                            - En caso afirmativo, mira el contenedor es de lon = 20
                                                            - En caso afirmativo lo coloca.
                                                            - En caso negativo, mira si el siguiente hueco (plaza +1) está vacio
                                                                              - En caso afirmativo coloca el contenedor (será de lon = 30) asociandolo
                                                                                a los tres nodos visitados. (Es decir, las claves 000, 010, 020 -> valor A30)
                                                                              - En caso negativo, busca el siguiente hueco libre y repite el preoceso
                                            - En caso negativo, busca el siguiente hueco libre y repite el proceso.


  */
}

void terminal::retira_contenidor(const string &m) throw(error)
{
  // TO DO:
  /* Se retira el contenedor con matricula m de la terminal (es decir, desaparece de AM y de AEM)
      --- EST FIRST_FIT ---
      1. ¿El contenedor con matricula m existe en la terminal?
      BuscarEnAEM(c._mat)
      BuscarEnAM(c._mat)

      - En caso negativo, genera un error, la matricula no existe
      - En caso afirmativo, se tendrán que enviar a la AEM todos los contenedores que hayan encima (Misma N, misma M, H superior)
      al contenedor con matricula m, para poder retirar el contenedor con matricula m.
      Una vez retirado, se intenta traer de vuelta de la AEM a la AM los contenedores retirados anteriormente,
      si las dimensiones actuales (agujero que ha dejado el contenedor con matricula m) lo permiten
  */
}

ubicacio terminal::on(const string &m) const throw()
{

}

nat terminal::longitud(const string &m) const throw(error)
{

}

void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error)
{

}

nat terminal::fragmentacio() const throw()
{

}

nat terminal::ops_grua() const throw()
{

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
