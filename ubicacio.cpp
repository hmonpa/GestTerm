#include "ubicacio.hpp"

// θ(1)
ubicacio::ubicacio(int i, int j, int k) throw(error)
{
    // PRE: True
    // POST: Crea una ubicació vàlida amb valors i,j,k. És a dir que: (i >=0 and j >=0 and k >=0) or (i >=-1 and j >=0 and k >=0)  and (i >=-1 and j >=-1 and k >=-1)
    if (((_i >= 0) and (_j >= 0) and (_k >= 0) ) || ((_i == -1) and (_j >= 0) and (_k >= 0)) || ((_i == -1) and (_j == -1) and (_k == -1 )))
    {
      _i = i;
      _j = j;
      _k = k;
    }
    else
    {
        throw error(UbicacioIncorrecta);
    }
}

// θ(1)
ubicacio::ubicacio(const ubicacio& u) throw(error)
{
    // PRE: True
    // POST: El p.i es una copia exacta de u
    _i = u._i;
    _j = u._j;
    _k = u._k;
}

// θ(1)
ubicacio& ubicacio::operator=(const ubicacio& u) throw(error)
{
    // PRE: True
    // POST: Els valors d'u s'assignen al p.i.
    if (this != &u)
    {
        _i = u._i;
        _j = u._j;
        _k = u._k;
    }
    return *this;
}

// θ(1)
ubicacio::~ubicacio() throw()
{
    // PRE: True
    // POST: S'elimina el p.i. de memòria
}

// θ(1)
int ubicacio::filera() const throw()
{
    // PRE: True
    // POST: Retorna la filera corresponent a la ubicació
    return _i;
}

// θ(1)
int ubicacio::placa() const throw()
{
    // PRE: True
    // POST: Retorna la plaça corresponent a la ubicació
    return _j;
}

// θ(1)
int ubicacio::pis() const throw()
{
    // PRE: True
    // POST: Retorna el pis corresponent a la ubicació
    return _k;
}

// θ(1)
bool ubicacio::operator==(const ubicacio &u) const throw()
{

    // PRE: El paràmetre u es una ubicació
    // POST: Es retorna true si la ubicació del p.i. i la ubicació u son idèntiques, false en cas contrari
    if (_i == u._i and _j == u._j and _k == u._k){
        return true;
    }
    else
    {
        return false;
    }
}

// θ(1)
bool ubicacio::operator!=(const ubicacio &u) const throw()
{
    // PRE: El paràmetre u es una ubicació
    // POST: Es retorna true si la ubicació del p.i. i la ubicació u son idèntiques, false en cas contrari
    return not(*this == u);
}

// θ(1)
bool ubicacio::operator<(const ubicacio &u) const throw()
{
    // PRE: El paràmetre u es una ubicació
    // POST: Es retorna true si la filera del p.i es menor a la filera de la ubicació u,
    // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es menor a la plaça de la ubicació u,
    // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es igual a la plaça de la ubicació u i el pis del p.i es menor al pis de la ubicació u
    if (( _i < u._i ) or ( _i == u._i and _j < u._j ) or ( _i == u._i and _j == u._j and _k < u._k ))
    {
      return true;
    }
    else
    {
      return false;
    }
}

// θ(1)
bool ubicacio::operator<=(const ubicacio &u) const throw()
{
  // PRE: El paràmetre u es una ubicació
  // POST: Es retorna true si la filera del p.i es menor o igual a la filera de la ubicació u,
  // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es menor o igual a la plaça de la ubicació u,
  // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es igual a la plaça de la ubicació u i el pis del p.i es menor o igual al pis de la ubicació u
    return (*this < u) or (*this == u);
}

// θ(1)
bool ubicacio::operator>(const ubicacio &u) const throw()
{
  // PRE: El paràmetre u es una ubicació
  // POST: Es retorna true si la filera del p.i es major a la filera de la ubicació u,
  // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es major a la plaça de la ubicació u,
  // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es igual a la plaça de la ubicació u i el pis del p.i es major al pis de la ubicació u
    return not (*this < u);
}

// θ(1)
bool ubicacio::operator>=(const ubicacio &u) const throw()
{
  // PRE: El paràmetre u es una ubicació
  // POST: Es retorna true si la filera del p.i es major o igual a la filera de la ubicació u,
  // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es major o igual a la plaça de la ubicació u,
  // o, si la filera del p.i es igual a la filera de la ubicació u i la plaça del p.i es igual a la plaça de la ubicació u i el pis del p.i es major o igual al pis de la ubicació u
    return not (*this <= u);
}
