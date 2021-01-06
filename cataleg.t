// --------------------------- Mètodes privats ---------------------------
#include <cmath>

// θ(k.size)
template <typename Valor>
nat cataleg<Valor>::hash(const string &k) const
{
  // PRE: True
  // POST: Retorna la cel·la de la taula on anirà la clau k

  nat hash_value = 0;
  for (nat i=0; i<k.size(); ++i)
  {
    // Suma ponderada on b=2 i pow = (2^i)
    hash_value += k[i] * pow(2,i);
  }
  return hash_value % _mida;
}

// θ(n)
template <typename Valor>
void cataleg<Valor>::redispersio(bool alpha_alt)
{
  // PRE: True = _mida*=2; False = _mida/=2
  // POST: Es dobla o redueix a la meitat la mida de la taula, segons si alpha_alt es T o F, i es reinserten els elements

  nat _mida_orig = _mida;
  if (alpha_alt) _mida*=2;                  // Factor càrrega supera llindar
  else           _mida/=2;                  // Factor càrrega excessivament baix

  _mida = primer(_mida);                    // La nova taula també ha de tenir una mida que sigui un nombre primer

  node_hash **_t = new node_hash *[_mida];
  _quants = 0;

  for (nat i=0; i<_mida; i++)               // Preparació de la nova taula
  {
    _t[i] = NULL;
  }

  for (nat i=0; i<_mida_orig; i++)          // Reinserció elements originals
  {
    node_hash *p = _taula[i];
    while (p != NULL)
    {
      node_hash *aux = p;
      p = p->_seg;
      nat nova_cell = hash(aux->_k);
      aux->_seg = _t[nova_cell];
      _t[nova_cell] = aux;

      _quants++;

    }
  }
  delete[] _taula;
  _taula = _t;
}

// θ(n)
template <typename Valor>
nat cataleg<Valor>::primer(nat numelems)
{
  // PRE: True
  // POST: Retorna un nombre primer. Si numelems originariament era un nombre primer, ho retorna.
  // En cas contrari, si no era un nombre primer, retorna el següent nombre primer que trobi

  bool es_primer = true;
  if (numelems > 1)
  {
    int i = 2;
    while (i < numelems and es_primer)
    {
      if (numelems % i == 0)                  // Parell
      {
         numelems += 1;                       // Provem amb el segúent
         i = 2;
      }
      else                                    // Senar
      {
          int k = 2;
          int arrel = sqrt(numelems)+1;

          while (k < arrel and es_primer)     // Comprova des de 2 fins a sqrt(numelems)+1
          {                                   // ... el +1 es per arrodonir a l'alça el nombre decimal resultant
            if (numelems % k != 0) k++;
            else es_primer = false;
          }

          if (es_primer)                      // Surt del bucle amb el booleà true -> Hem trobat un primer
          {
            i=numelems;
          }
          else                                // Continuem buscant...
          {
            es_primer = true;
            numelems+=1;
          }
        }
        i++;
      }
    }
  else
  {
    numelems = 2;
  }

  return numelems;
}

// --------------------------- Mètodes públics ---------------------------

// θ(n)
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) throw(error)
{
  // PRE: True
  // POST: Crea un catàleg buit

  numelems = primer(numelems);
  _mida = numelems;                         // La mida de la taula es numelems

  _taula = new node_hash *[_mida];
  for (nat i=0; i<_mida; i++)
  {
    _taula[i] = NULL;
  }
}

// θ(n)
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error)
{
  // PRE: True
  // POST: El p.i es una copia exacta de c

  _mida = c._mida;
  _taula = new node_hash *[_mida];
  _quants = c._quants;

  for (nat i=0; i<_mida; i++)
  {
    if (c._taula[i] != NULL)
    {
      // Copia de claus, valors i punters de la taula
      _taula[i] = new node_hash;
      _taula[i]->_k = c._taula[i]->_k;
      _taula[i]->_v = c._taula[i]->_v;
      _taula[i]->_seg = NULL;             // Punter a linked-list

      node_hash *l = c._taula[i]->_seg;   // Punter a linked-list de c
      node_hash *act = _taula[i];

      while (l != NULL)                   // Mentre hi hagin sinònims apuntats per la cel·la i
      {
        node_hash *p = new node_hash;
        p->_k = l->_k;
        p->_v = l->_v;
        p->_seg = NULL;

        act->_seg = l;

        act = act->_seg;
        l = l->_seg;
      }
    }
  }
}

// θ(n)
template <typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c) throw(error)
{
  // PRE: True
  // POST: El p.i es una copia exacta de c

  if (this != &c)
  {
    cataleg<Valor> cat(c);                  // cat es una còpia de c
    node_hash **_aux = _taula;
    _taula = cat._taula;
    cat._taula = _aux;

    _mida = c._mida;
    _quants = c._quants;
  }
  return *this;
}

// θ(n)
template <typename Valor>
cataleg<Valor>::~cataleg() throw()
{
  // PRE: True
  // POST: S'han eliminat els elements del p.i.

  node_hash *act;                         // Punter a l'actual
  node_hash *ant;                         // Punter a l'anterior
  act = ant = NULL;

  for (nat i=0;i<_mida;i++)
  {
    act = _taula[i];
    while (act != NULL)
    {
      ant = act;
      act = act->_seg;
      delete ant;
    }
  }
  delete[] _taula;
}

// θ(1)
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error)
{
  // PRE: True
  // POST: Insereix al catàleg un node amb clau k i valor v
  //       Si k es un string buit, retorna un error

  if (k.size() != 0)
  {
    nat pos = hash(k);
    node_hash *p = _taula[pos];
    bool existeix = false;

    while (p != NULL and not existeix)
    {
      if (p->_k == k)
      {
        existeix = true;
      }
      else
      {
        p = p->_seg;
      }
    }

    if (existeix)
    {
      p->_v = v;                                        // Només canviem valor associat
    }
    else
    {
      node_hash *n = new node_hash;
    	n->_k = k;
    	n->_v = v;
      n->_seg = _taula[pos];
      _taula[pos] = n;

      ++_quants;                                        // Nou element a la taula
    }

  /*  float fc_actual = (float)_quants / (float)_mida;    // Fàctor de càrrega actual = fc_actual
    if (fc_actual > alpha)                              // ... es comprova si es major a 0.75
    {
      redispersio(true);                                // ... en cas afirmatiu, fem redispersió doblant la mida de la taula
    }*/
  }
  else
  {
    throw error(ClauStringBuit);
  }
}

// θ(1)
template <typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error)
{
  // PRE: True
  // POST: Elimina del catàleg la posició que conté la clau k
  //       Si k no es una clau existent al catàleg, retorna un error

  nat pos = hash(k);
  node_hash *p = _taula[pos], *p_ant = NULL;
  bool existeix = false;

  while (p != NULL and not existeix)
  {
    if (p->_k == k)
    {
      existeix = true;
    }
    else
    {
      p_ant = p;
      p = p->_seg;
    }
  }
  if (existeix)
  {
    if (p_ant == NULL)                    // Era el primer element de la llista de sinònims
    {
      _taula[pos] = p->_seg;              // NULL
    }
    else                                  // Era qualsevol element següent de la llista
    {
      p_ant->_seg = p->_seg;
    }

    delete p;
    _quants--;

  /*  float fc_actual = (float)_quants / (float)_mida;    // Fàctor de càrrega actual = fc_actual
    if (fc_actual < (alpha/3))                          // ... es comprova si es 3 cops mes petit que 0.75
    {
      redispersio(false);                               // ... en cas afirmatiu, fem redispersió partint la mida de la taula
    }*/
  }
  else
  {
    throw error(ClauInexistent);
  }
}

// θ(1)
template <typename Valor>
bool cataleg<Valor>::existeix(const string &k) const throw()
{
  // PRE: True
  // POST: Es retorna True si k existeix al catàleg, False en cas contrari

  nat pos = hash(k);
  node_hash *p = _taula[pos];
  bool existeix = false;

  while (p != NULL and not existeix)
  {
    if (p->_k == k)
    {
      existeix = true;
    }
    else
    {
      p = p->_seg;
    }
  }
  return existeix;
}

// θ(1)
template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error)
{
  // PRE: True
  // POST: Retorna el valor associat a la clau k.
  // 	     Retorna un error en cas de que la clau k no existeixi

  nat pos = hash(k);
  node_hash *p = _taula[pos];
  bool existeix = false;
  Valor v;

  while (p != NULL and not existeix)
  {
    if (p->_k == k)
    {
      v = p->_v;
      existeix = true;
    }
    else
    {
      p = p->_seg;
    }
  }
  if (not existeix) throw error(ClauInexistent);
  else return v;
}

// θ(1)
template <typename Valor>
nat cataleg<Valor>::quants() const throw()
{
  // PRE: True
  // POST: Retorna el nombre d'elements que hi ha al catàleg

  return _quants;
}
