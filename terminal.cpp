#include "terminal.hpp"

// --------------------------- Mètodes públics ---------------------------

//
terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error):
        _n(n),
        _m(m),
        _h(h)
{
  // PRE: True
  // POST: Crea una terminal vàlida, retorna un error en cas contrari

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

estrategia terminal::quina_estrategia() const throw()
{

}
