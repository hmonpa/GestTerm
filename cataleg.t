// --------------------------- Mètodes privats ---------------------------

// Nota Héctor: Tengo dudas acerca de si los templates están correctos

template <typename Valor>
typename cataleg<Valor>::node* cataleg<Valor>::crea_arbre(node* n)
{
  // PRE: True
  // POST: Si n es NULL, es retorna NULL. En cas contrari,
  // el resultat apuntarà al primer node d'un arbre binari de nodes,
  // els quals son copia de l'arbre apuntat per n

  node* p;
  if (n == NULL) p = NULL;
  else
  {
    p = new node;
    try
    {
      p->info = n->info;
      p->f_esq = crea_arbre(n->f_esq);
      p->f_dret = crea_arbre(n->f_dret);
    }
    catch(...)
    {
      delete p;
      throw;
    }
  }
  return p;
}


/* Constructora. Crea un catàleg buit on numelems és el nombre
   aproximat d'elements que com a màxim s'inseriran al catàleg. */
template <typename Valor>
explicit cataleg<Valor>::cataleg(nat numelems) throw(error){

}

// --------------------------- Mètodes públics ---------------------------

/* Constructora per còpia, assignació i destructora. */
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) throw(error){
  _arrel = crea_arbre(c._arrel);
  _long = c._long;
}

template <typename Valor>
cataleg<Valor>::cataleg& operator=(const cataleg& c) throw(error){

}

template <typename Valor>
cataleg<Valor>::~cataleg() throw(){

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
