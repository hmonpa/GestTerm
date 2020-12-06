// --------------------------- Mètodes privats ---------------------------

// θ(k.size)
template <typename Valor>
nat cataleg<Valor>::hash(const string &k)
{
  // PRE: True
  // POST: Retorna la cel·la de la taula on anirà la clau k
  // LINK:https://cp-algorithms.com/string/string-hashing.html#:~:text=For%20the%20conversion%2C%20we%20need,%3Dhash(t)).

  int prime_number = 31;
  nat hash_value = 0;
  for (nat i=0; i<k.size(); ++i)
  {
    // Suma ponderada on b=2 i pow = (2^i)
    hash_value += k[i] * pow(2,i);
  }

  return hash_value % _mida;
}



// --------------------------- Mètodes públics ---------------------------

// θ(n)
template <typename Valor>
explicit cataleg<Valor>::cataleg(nat numelems) throw(error)
{
  // PRE: True
  // POST: Crea un catàleg buit

  _mida = numelems;                     // La mida de la taula es numelems
  _taula = new node_hash *[_mida];
  for (nat i=0;i<_mida;i++)
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

  for (nat i=0;i<_mida;i++)
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
        p->seg = NULL;

        act->_seg = p2;

        act = act->_seg;
        l = l->_seg;
      }
    }
  }
}

// θ(n)
template <typename Valor>
cataleg& cataleg<Valor>::operator=(const cataleg& c) throw(error)
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

/* Mètode modificador. Insereix el parell <clau, valor> indicat.
   En cas que la clau k ja existeixi en el catàleg actualitza el valor
   associat. Genera un error en cas que la clau sigui l'string buit. */
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) throw(error){

}

/* Elimina del catàleg el parell que té com a clau k.
   En cas que la clau k no existeixi en el catàleg genera un error. */
template <typename Valor>
void cataleg<Valor>::elimina(const string &k) throw(error){

}

/* Retorna true si i només si la clau k existeix dins del catàleg; false
   en cas contrari. */
template <typename Valor>
bool cataleg<Valor>::existeix(const string &k) const throw(){

}

/* Retorna el valor associat a la clau k; si no existeix cap parell amb
   clau k llavors genera un error. Exemple:
     cataleg<int> ct;
     ...
     int n = ct["dia"]; */
template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const throw(error){

}

/* Retorna el nombre d'elements que s'han inserit en el catàleg
   fins aquest moment. */
template <typename Valor>
nat cataleg<Valor>::quants() const throw(){

}
