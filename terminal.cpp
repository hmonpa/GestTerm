#include "terminal.hpp"

// --------------------------- Mètodes públics ---------------------------
terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error):
        _n(n),
        _m(m),
        _h(h)
{
  // PRE:
  // POST:
  
  if ((_n != 0) and (_m != 0) and (_h != 0) and _h <= HMAX and (st == FIRST_FIT || st == LLIURE))
  {
    _n = n;
    _m = m;
    _h = h;

    if (st == FIRST_FIT)    // TO DO
    else if (st == LLIURE)  // TO DO
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

}

void terminal::retira_contenidor(const string &m) throw(error)
{

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

void terminal::area_espera(list<string> &l) const throw()
{

}

nat terminal::num_fileres() const throw()
{

}

nat terminal::num_places() const throw()
{

}

nat terminal::num_pisos() const throw()
{

}

estrategia terminal::quina_estrategia() const throw()
{

}
