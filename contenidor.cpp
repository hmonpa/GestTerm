#include "contenidor.hpp"

// --------------------------- Mètodes privats ---------------------------

// θ(mat.size)
bool contenidor::matricula_valida(const string &mat)
{
  // PRE: True
  // POST: Retorna true si la matricula es vàlida, es a dir, si es una seqüència d'un o més caràcters i està formada per lletres majuscules i/o digits
  // exceptuant les majuscules Ñ, Ç i vocals accentuades

  bool mat_valida = true;

  if (mat.size() == 0) mat_valida = false;
  else {
    for (nat i = 0; i < mat.size(); ++i)
    {
      if (((mat[i] < 'A') or (mat[i] > 'Z')) and ((mat[i] < '0') or (mat[i] > '9'))) mat_valida = false;
    }
  }
  return mat_valida;
}

// --------------------------- Mètodes públics ---------------------------

// θ(m.size)
contenidor::contenidor(const string &m, nat l) throw(error)
{
  // PRE: True
  // POST: Crea un contenidor vàlid, retorna un error en cas contrari

  if (matricula_valida(m))
  {
    if (l != 10 and l != 20 and l != 30)
    {
      throw error(LongitudIncorrecta);
    }
    else
    {
      _mat = m;
      _lon = l;
    }
  }
  else
  {
    throw error(MatriculaIncorrecta);
  }
}

// θ(1)
contenidor::contenidor(const contenidor &u) throw(error)
{
  // PRE: True
  // POST: El p.i es una copia exacta de u
  _mat = u._mat;
  _lon = u._lon;
}

// θ(1)
contenidor& contenidor::operator=(const contenidor &u) throw(error)
{
  // PRE: True
  // POST: Els valors d'u s'assignen al p.i.
  if (this != &u)
  {
      _mat = u._mat;
      _lon = u._lon;
  }
  return *this;
}

// θ(1)
contenidor::~contenidor() throw()
{
  // PRE: True
  // POST: S'elimina el p.i. de memòria
}

// θ(1)
nat contenidor::longitud() const throw()
{
  // PRE: True
  // POST: Retorna la longitud del p.i
  return _lon;
}

// θ(1)
string contenidor::matricula() const throw()
{
  // PRE: True
  // POST: Retorna la matricula del p.i
  return _mat;
}

// θ(1)
bool contenidor::operator==(const contenidor &c) const throw()
{
  // PRE: True
  // POST: Retorna true si el contenidor del p.i i el contenidor c son iguals,
  //       false en cas contrari
  return (this->_mat==c._mat) and (this->_lon==c._lon);
}

// θ(1)
bool contenidor::operator!=(const contenidor &c) const throw()
{
  // PRE: True
  // POST: Retorna true si el contenidor del p.i i el contenidor c son diferents,
  //       false en cas contrari
  return not(*this==c);
}

// θ(1)
bool contenidor::operator<(const contenidor &c) const throw()
{
  // PRE: True
  // POST: Retorna true si la matricula del p.i és més petita que la matricula del contenidor c.
  // O també retorna true si la matricula del p.i es igual que la matricula del contenidor c i, a més,
  // la longitud del p.i es més petita que la longitud de c. Retorna false en cas contrari
  return ((this->_mat < c._mat) or ((this->_mat == c._mat and this->_lon < c._lon)));
}

// θ(1)
bool contenidor::operator<=(const contenidor &c) const throw()
{
  // PRE: True
  // POST: Retorna true si la matricula del p.i és més petita que la matricula del contenidor c.
  // O també retorna true si la matricula del p.i es igual que la matricula del contenidor c i, a més,
  // la longitud del p.i es més petita que la longitud de c. També retornara true si la matricula i la
  // longitud del p.i son iguals als del contenidor c. Retorna false en cas contrari
  return (*this < c) or (*this == c);
}

// θ(1)
bool contenidor::operator>(const contenidor &c) const throw()
{
  // PRE: El paràmetre c es un contenidor
  // POST: Retorna true si la matricula del p.i és major que la matricula del contenidor c.
  // O també retorna true si la matricula del p.i es igual que la matricula del contenidor c i, a més,
  // la longitud del p.i es més petita que la longitud de c. Retorna false en cas contraric
  return not(*this <= c);
}

// θ(1)
bool contenidor::operator>=(const contenidor &c) const throw()
{
  // PRE: El paràmetre c es un contenidor
  // POST: Retorna true si la matricula del p.i és major que la matricula del contenidor c.
  // O també retorna true si la matricula del p.i es igual que la matricula del contenidor c i, a més,
  // la longitud del p.i es major que la longitud de c. També retornara true si la matricula i la
  // longitud del p.i son iguals als del contenidor c. Retorna false en cas contrari
  return not(*this < c);
}
