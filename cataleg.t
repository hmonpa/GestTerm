// --------------------------- Mètodes privats ---------------------------



// --------------------------- Mètodes públics ---------------------------

/* Constructora. Crea un catàleg buit on numelems és el nombre
   aproximat d'elements que com a màxim s'inseriran al catàleg. */
// θ(n)
template <typename Valor>
explicit cataleg<Valor>::cataleg(nat numelems) throw(error)
{
  // PRE: True
  // POST: Crea un catàleg buit

  _mida = numelems;                 // Cambio
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

// θ
template <typename Valor>
cataleg<Valor>::cataleg& operator=(const cataleg& c) throw(error)
{

}

// θ
template <typename Valor>
cataleg<Valor>::~cataleg() throw()
{
  // PRE: True
  // POST:

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
