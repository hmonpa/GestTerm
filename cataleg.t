// --------------------------- Mètodes privats ---------------------------



// --------------------------- Mètodes públics ---------------------------

/* Constructora. Crea un catàleg buit on numelems és el nombre
   aproximat d'elements que com a màxim s'inseriran al catàleg. */
// θ(n)
template <typename Valor>
explicit cataleg<Valor>::cataleg(nat numelems) throw(error):
                      _mida(numelems)
{
  // PRE: True
  // POST: Crea un catàleg buit

  _taula = new node_hash *[_mida];
  for (nat i=0;i<_mida;i++)
  {
    _taula[i] = NULL;
  }
}


/* Constructora per còpia, assignació i destructora. */
// θ
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error)
{
  // PRE: True
  // POST: El p.i es una copia exacta de c

}

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
