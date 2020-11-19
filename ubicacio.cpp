#include "ubicacio.hpp"

ubicacio::ubicacio(int i, int j, int k) throw(error)
{
    // PRE:
    // POST:
}

// θ(1)
ubicacio::ubicacio(const ubicacio& u) throw(error)
{
    // PRE: True
    // POST: El p.i es una copia exacta de u

    i = u.i;
    j = u.j;
    k = u.k;
}

// θ(1)
ubicacio& ubicacio::operator=(const ubicacio& u) throw(error)
{
    // PRE: TRUE
    // POST: Els valors d'u s'assignen al p.i.
    if (this != &u)
    {
        i = u.i;
        j = u.j;
        k = u.k;
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
    // PRE: 
    // POST: 

    //return i;
}

// θ(1)
int ubicacio::placa() const throw()
{
    // PRE: 
    // POST: 

    //return j;
}

// θ(1)
int ubicacio::pis() const throw()
{
    // PRE: 
    // POST: 

    // return k;
}

// θ(1)
bool ubicacio::operator==(const ubicacio &u) const throw()
{
        
    // PRE: El paràmetre u es una ubicació
    // POST: Es retorna true si la ubicació del p.i. i la ubicació u son idèntiques, false en cas contrari
    if (i == u.i and j == u.j and k == u.k){
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


bool ubicacio::operator<(const ubicacio &u) const throw()
{
    // PRE: 
    // POST: 
}

bool ubicacio::operator<=(const ubicacio &u) const throw()
{
    // PRE: 
    // POST: 
}

bool ubicacio::operator>(const ubicacio &u) const throw()
{
    // PRE: 
    // POST: 
}

bool ubicacio::operator>=(const ubicacio &u) const throw()
{
    // PRE: 
    // POST: 
}