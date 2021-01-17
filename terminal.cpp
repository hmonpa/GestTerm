#include "terminal.hpp"
// --------------------------- Mètodes privats ---------------------------


// θ(n³)
void terminal::inicialitza_am(int n, int m, int h)
{
  // PRE:
  // POST:

  string buit = "";
  string buit_a_baix = "___";
  est_am = new string**[n];

  for (int i=0; i<n; i++)
  {
    est_am[i] = new string*[m];

    for (int j=0; j<m; j++)
    {
      est_am[i][j] = new string[h];

      for (int k=0; k<h; k++)
      {
        if (k==0) est_am[i][j][k] = buit_a_baix;
        else est_am[i][j][k] = buit;
      }
    }
  }
}

// θ(n³)
void terminal::crea_llista_lliures(int n, int m, int h)
{
  // PRE:
  // POST:

  node *act = NULL;
  node *prev = NULL;
  _size = 0;

  //std::cout << "\n ---- Lista de ubicaciones libres: ----" << '\n';
  for (int i=0; i<n; i++)
  {
    for (int j=0; j<m; j++)
    {
      for (int k=0; k<h; k++)
      {
        if (prev == NULL)
        {
          _head = new node;
          _head->_u = ubicacio(i,j,k);
          act = _head;
          act->_ant = NULL;
        }
        else
        {
          act = new node;
          act->_u = ubicacio(i,j,k);
          prev->_seg = act;
          act->_ant = prev;
        }
        
        act->_lliu = true;            // Se inicializan las posiciones a TRUE = Están libres todas inicialmente
        act->_seg = NULL;
        prev = act;
        _size++;
      }
    }
  }
}

// θ(n)
void terminal::actualitza_lliures()
{
  // PRE: n es el _head y js es la plaza más a la derecha (depende de las plazas que ocupe)
  // POST: Recorre la lista hasta el centinela (js = siguiente plaça)

  node* n = _head;

  int i = n->_u.filera();
  int j = n->_u.placa();
  int k = n->_u.pis();

  bool acaba = false;

  while (n != NULL and not acaba)
  {
      if (est_am[i][j][k] == "___" or est_am[i][j][k] == "") n->_lliu = true;
      else n->_lliu = false;

      if (i == _n-1 and j == _m-1 and k ==_h-1) acaba = true;
      else if (j == _m-1 and k == _h-1)
      {
        i++;
        j=0;
        k=0;
      }
      else if (k == _h-1)
      {
        j++;
        k=0;
      }
      else k++;
      n = n->_seg;
  }
}


// θ(n)
void terminal::reinserta()
{
  // PRE:
  // POST: Recorre el área de espera para intentar insertar contenedores

  Cu co_ub;

  if (not _area_espera.empty())
  {
      for (list<string>::iterator it=_area_espera.begin(); it!=_area_espera.end(); it++)
      {
        co_ub.c = _ct[*it].c;
        co_ub.u = _ct[*it].u;
        insereix_ff(co_ub);

        if (_ct[co_ub.c.matricula()].u != ubicacio(-1,0,0))
        {
        	list<string>::iterator it2 = it;
        	_area_espera.erase(it2);
        	it=_area_espera.begin();
        	it--;
        }
      }
  }
}

// θ(1)
void terminal::recorrido_der(int i, int j, int k, int jinici, int kinici)
{
	// PRE: k es k+1 respecto al contenedor a retirar 
	//

	if (k < _h and j >= jinici)
	{
		if (_ct.existeix(est_am[i][j][k]) and _ct[est_am[i][j][k]].u != ubicacio(-1,0,0))
    	{
	    	Cu co_aux;
	      	co_aux.c = _ct[est_am[i][j][k]].c;
	      	co_aux.u = ubicacio(-1,0,0);
	      	_ct.elimina(est_am[i][j][k]);
	      	_area_espera.push_back(est_am[i][j][k]);
	      	opsgrua++;
	      	_ct.assig(est_am[i][j][k], co_aux);
    	}
    	est_am[i][j][k]="";
		recorrido_der(i, j, ++k, jinici, kinici);
		recorrido_der(i, --j, kinici+1, jinici, kinici);
	}
}

// θ(1)
void terminal::recorrido_izq(int i, int j, int k, int jinici, int kinici)
{
	//
	//

	if (k < _h and j >= jinici)
  	{
  		if (_ct.existeix(est_am[i][j][k]) and _ct[est_am[i][j][k]].u != ubicacio(-1,0,0))
        {
        	Cu co_aux;
          	co_aux.c = _ct[est_am[i][j][k]].c;
          	co_aux.u = ubicacio(-1,0,0);
          	_ct.elimina(est_am[i][j][k]);
          	_area_espera.push_back(est_am[i][j][k]);
          	opsgrua++;
          	_ct.assig(est_am[i][j][k], co_aux);
        }
        est_am[i][j][k]="";
  		recorrido_izq(i, j, ++k, jinici, kinici);
  		recorrido_izq(i, --j, kinici+1, jinici, kinici); 		
  	}
}

// θ(1)
void terminal::reorganitza(nat actuals)
{
	// PRE:
	// POST

	// Reorganiza el área de espera después de una retirada de contenedor.
	// Los contenedores que ya estaban en el AEM antes de que un contenedor fuese retirado y todos los de encima suyo pasasen al AEM,
	// se ponen al final, para que al reinsertar, se pueda empezar desde el inicio

	int i = 0;
	for (list<string>::iterator it=_area_espera.begin(); it!=_area_espera.end() and i < actuals; it++, i++)
	{
		list<string>::iterator it2 = it;
		_area_espera.push_back(*it);
		_area_espera.erase(it2);
	}
}

// θ(1)
void terminal::allibera_ubi(int i, int j, int k, nat longi)
{
	if (longi == 1)
	{
		if (_h == 1) est_am[i][j][k] = "___";
		else est_am[i][j][k] = "";
	}
	else if (longi == 2)
	{
		if (_h == 1)
        {
          	est_am[i][j][k] = "___";
          	est_am[i][j+1][k] = "___";
        }
        else
        {
          	est_am[i][j][k] = "";
          	est_am[i][j+1][k] = "";
        }
    }
    else if (longi == 3)
    {
    	if (_h == 1)
      	{
        	est_am[i][j][k] = "___";
        	est_am[i][j+1][k] = "___";
        	est_am[i][j+2][k] = "___";
      }
      else
      {
        	est_am[i][j][k] = "";
        	est_am[i][j+1][k] = "";
        	est_am[i][j+2][k] = "";
      }
    }
}

//
void terminal::retira_ff(string m) throw(error)
{
  // PRE:
  // POST:

  if (_ct.existeix(m))
  {
    Cu co_ub;
    co_ub.c = _ct[m].c;
    co_ub.u = _ct[m].u;
    ubicacio ub_aem = ubicacio(-1,0,0);

    // CD: El contenidor es troba a l'AEM
    if (co_ub.u == ub_aem)
    {
      _ct.elimina(co_ub.c.matricula());
      _area_espera.remove(co_ub.c.matricula());
    }
    // Es troba a l'AM
    else
    {
      int i = co_ub.u.filera();
      int j = co_ub.u.placa();
      int k = co_ub.u.pis();

      nat longi = co_ub.c.longitud() / 10;									// Es fixen les places que ocupa un contenidor a l'AM, en funció de la seva longitud

      // C1: El contenidor a retirar només ocupa una ubicació a l'AM
      if (longi == 1)
      {
      	// CD: El contenidor està al pis més alt, o no te res a sobre
        if (k+1 == _h or (k+1 != _h and est_am[i][j][k+1] == ""))
        {
          _ct.elimina(co_ub.c.matricula());
          opsgrua++;

          allibera_ubi(i, j, k, longi);
          actualitza_lliures();
        }
        // CR: El contenidor té contenidors a sobre
        else
        {
          int jder = j;
          int jizq = j;

          while (jder+1 <= _m-1 and est_am[i][jder][k+1] == est_am[i][jder+1][k+1])	jder++;

          while (jizq-1 >= 0 and est_am[i][jizq][k+1] == est_am[i][jizq-1][k+1])	jizq--;		

          nat actuals = _area_espera.size();

          recorrido_der(i, jder, k+1, j, k);
          recorrido_izq(i, j, k+1, jizq, k);

          if (actuals != 0) reorganitza(actuals);

          _ct.elimina(co_ub.c.matricula());
          opsgrua++;

          allibera_ubi(i, j, k, longi);
          actualitza_lliures();
        }
      }
      // C2: El contenidor a retirar ocupa dues places a l'AM
      else if (longi == 2)
	  {
	      int j2 = 0;
	      if (est_am[i][j+1][k] == co_ub.c.matricula())			j2 = j+1;	// Fixem les dues places adjacents 
	      else if (est_am[i][j-1][k] == co_ub.c.matricula())	j2 = j-1;
	      if (j2 < j)														// j2 serà, de les dues places, la situada més a la dreta
	      {
	      	int aux = j2;
	      	j2 = j;
	      	j = aux;
	      }

	      // CD: El contenidor està al pis més alt, o no te res a sobre
	      if (k+1 == _h or (k+1 != _h and (est_am[i][j][k+1] == "" and est_am[i][j2][k+1] == "")))       
	      {
	        _ct.elimina(co_ub.c.matricula());
	        opsgrua++;

	        allibera_ubi(i, j, k, longi);
	        actualitza_lliures();

	      }
	      // CR: El contenidor té contenidors a sobre
	      else
	      {
	      	int jizq = j;
	      	int jder = j2;
	      	
	      	while (jder+1 <= _m-1 and est_am[i][jder][k+1] == est_am[i][jder+1][k+1])	jder++;
	      	while (jizq-1 >= 0 and est_am[i][jizq][k+1] == est_am[i][jizq-1][k+1])		jizq--;	

	      	nat actuals = _area_espera.size();

	      	recorrido_der(i, jder, k+1, j2, k);
	      	recorrido_izq(i, j, k+1, jizq, k);

	      	if (actuals != 0) reorganitza(actuals);

	      	_ct.elimina(co_ub.c.matricula());
	      	opsgrua++;

	      	allibera_ubi(i, j, k, longi);
	      	actualitza_lliures();
	 	 }

	  }
      // C3: El contenidor a retirar ocupa tres places a l'AM
      else 
      {
        int j3 = 0;
        int j2 = 0;

        if (j+2 != _m and est_am[i][j][k] == est_am[i][j+2][k]) j3 = j+2;
        else if (j+1 != _m and est_am[i][j][k] == est_am[i][j+1][k]) j3 = j+1;
        else if (j+1 != _m) j3 = j;

        j2 = j3-1;
        j = j2-1;

		// CD: El contenidor està al pis més alt, o no te res a sobre
        if (k+1 == _h or (k+1 != _h and (est_am[i][j][k+1] == "" and est_am[i][j2][k+1] == "" and est_am[i][j3][k+1] == "")))
        {
          _ct.elimina(co_ub.c.matricula());
          opsgrua++;

          allibera_ubi(i, j, k, longi);
          actualitza_lliures();
        }
        // CR: El contenidor té contenidors a sobre
        else
        {
        	int jizq = j;
          	int jder = j3;
          	
          while (jder+1 <= _m-1 and est_am[i][jder][k+1] == est_am[i][jder+1][k+1])		jder++;
          // Traslado hacia la parte más izq de encima del contenedor
          while (jizq-1 >= 0 and est_am[i][jizq][k+1] == est_am[i][jizq-1][k+1])		jizq--;

          nat actuals = _area_espera.size();

          recorrido_der(i, jder, k+1, j2, k);
          recorrido_izq(i, j, k+1, jizq, k);   

          if (actuals != 0) reorganitza(actuals);

          _ct.elimina(co_ub.c.matricula());
          opsgrua++;

          allibera_ubi(i, j, k, longi);
          actualitza_lliures();
        }
      }
    }
    reinserta();
  }
  else
  {
    throw error(MatriculaInexistent);
  }
}


//
void terminal::insereix_ff(Cu co_ub) throw(error)
{
  // PRE:
  // POST:

  bool existeix_aem = false;
  bool reinsercio = false;						// Si un contenidor que està intentant sent insertat, ho es finalment
  												// no truquem al mètode reinserta() de nou, ja que ja està recorrent l'AEM en aquests moments

  ubicacio ub_aem = ubicacio(-1,0,0);

  if (_ct.existeix(co_ub.c.matricula()))
  {
    if (_ct[co_ub.c.matricula()].u == ub_aem) existeix_aem = true;
  }
  else existeix_aem = false;

  if (not _ct.existeix(co_ub.c.matricula()) or existeix_aem)
  {
    nat longi = co_ub.c.longitud() / 10;
    bool trobat = false;    					// El booleà trobat es posarà a true quan insereixi un contenidor de 10 peus, o quan es surti dels límits de l'AM
    node *p = _head;
    node* inici;
    node* p2 = NULL;

    // CD: El contenidor es major que la mida de l'AM
    if (longi > _size)
    {
      trobat = true;
    }

    // C1: El contenidor a insertar només ocupará una ubicació a l'AM,
    // agafarà la primera ubicació lliure que trobi amb el recorregut actual
    if (longi == 1)
    {
      while (not trobat and p->_lliu != true)      
      {
        if (p->_seg == NULL) trobat = true;
        else  p = p->_seg;
      }
      if (not trobat)
      {
        if (_ct.existeix(co_ub.c.matricula()))
        {
          _ct.elimina(co_ub.c.matricula());
          reinsercio = true;
        }

        co_ub.u = p->_u;
        _ct.assig(co_ub.c.matricula(), co_ub);      			

        opsgrua++;	
        p->_lliu = false;                           			// Marca la ubicació com a ocupada

        
        int i = p->_u.filera();
        int j = p->_u.placa();
        int k = p->_u.pis();
        est_am[i][j][k] = co_ub.c.matricula();
        
        trobat = true;

        if (not reinsercio) reinserta();
      }
    }
    // C2: El contenidor a insertar ocuparà dues o tres places a l'AM
    else
    {
      	while (p != NULL and not trobat and p->_lliu != true)     
      	{
	        if (p->_seg == NULL) trobat = true;
	        p = p->_seg;
      	}
      	if (not trobat)
      	{
	        inici = p;
	        int filera_act = inici->_u.filera();
	        int pis_act = inici->_u.pis();

	        nat i = 1;      							// Es troba la primera ubicació lliure de dos o tres necessaries

        	while (p != NULL and i < longi and not trobat)
        	{
	            if (i == 0)           					// Si la inserció a les primeres places lliures ha fracasat, tornem a intentar-ho
	            {
	              if (p->_seg != NULL)	p = inici->_seg;

	              while (p!=NULL and not trobat and p->_lliu != true)
	              {
	                if (p->_seg == NULL) trobat = true;
	                else p = p->_seg;
	              }

	              i = 1;
	              inici = p;
	              p2 = NULL;
	              filera_act = inici->_u.filera();
	              pis_act = inici->_u.pis();

	            }
	            if (p == NULL) trobat = true;
	            else
	            {
	              	if (p->_seg != NULL) p = p->_seg;
	              	else trobat = true;

	              	while (p->_seg!=NULL and (p->_u.pis() != pis_act and p->_u.filera() == filera_act) and not trobat) // Fem el salt cap a la següent plaça
	              	{
	                	p = p->_seg;
	              	}

	              	if (inici->_u.pis() != 0 or p->_u.pis() != 0)
	              	{
	              		if (p2 != NULL)
		                {
		                	if (p->_ant->_lliu == true or inici->_ant->_lliu == true or p2->_ant->_lliu == true) i = 0;
		                }
		                if ((p->_ant->_lliu == true or inici->_ant->_lliu == true) and p->_u.filera() == filera_act)
		                {
		                    i = 0;    // Comprobamos si el de debajo nuestro está libre, si es así, no ponemos en el aire
		                }
	              	}
	              	if (i != 0)
	              	{
	                	if ((filera_act == p->_u.filera()) and (inici->_lliu == true and p->_lliu == true))
	                	{
	                		i++;
	                    	if (i == 2) p2 = p;
	                	}
	                	else i = 0;
	               }
	            }
        	}
      	}
    }
    // Si !trobat, no ens hem sortit dels limits de l'AM 
    if (not trobat)
    {
    	// El contenidor a afegir ocupa dues places
      	if (longi == 2)
      	{
      		// Si ja estiguès al catàleg, significaria que està a l'AEM
        	if (_ct.existeix(co_ub.c.matricula()))
        	{	
          		_ct.elimina(co_ub.c.matricula());		// S'elimina del catàleg per tornar a ser inserit
          		reinsercio = true;						// Es marca la reinserció per no tornar a revisar l'AEM des de l'inici
        	}

	        int i = inici->_u.filera();
	        int j = inici->_u.placa();
	        int k = inici->_u.pis();

	        est_am[i][j][k] = co_ub.c.matricula();		// Ocupa l'AM

	        co_ub.u = inici->_u;						// S'inserta al catàleg amb la plaça més petita de les places que ocupa
	        _ct.assig(co_ub.c.matricula(), co_ub);		// Es reinserta al catàleg
	        opsgrua++;

	        i = p2->_u.filera();
	        j = p2->_u.placa();
	        k = p2->_u.pis();
	        est_am[i][j][k] = co_ub.c.matricula();		// Ocupa l'AM

	        inici->_lliu = false;						// Les places que ocupa ja no estàn lliures
	        p2->_lliu = false;

	        if (not reinsercio) reinserta();
      	}
      	// El contenidor a afegir ocupa 3 places
  		else if (longi == 3)
  		{
        	if (_ct.existeix(co_ub.c.matricula()))
        	{
          		_ct.elimina(co_ub.c.matricula());
          		reinsercio = true;
        	}

        	int i = inici->_u.filera();
        	int j = inici->_u.placa();
        	int k = inici->_u.pis();

	        est_am[i][j][k] = co_ub.c.matricula();

	        co_ub.u = inici->_u;
	        _ct.assig(co_ub.c.matricula(), co_ub);
	        opsgrua++;

	        i = p2->_u.filera();
	        j = p2->_u.placa();
	        k = p2->_u.pis();
	        est_am[i][j][k] = co_ub.c.matricula();

	        i = p->_u.filera();
	        j = p->_u.placa();
	        k = p->_u.pis();
	        est_am[i][j][k] = co_ub.c.matricula();

	        inici->_lliu = false;
	        p2->_lliu = false;
	        p->_lliu = false;
	        
	        if (not reinsercio) reinserta();
  		}
    }
    // Si trobat, ens hem sortit dels limits de l'AM 
    else
    {
        if (not _ct.existeix(co_ub.c.matricula()))
        {
          	co_ub.u = ub_aem;
          	_ct.assig(co_ub.c.matricula(), co_ub);
          	_area_espera.push_front(co_ub.c.matricula());
        }
    }
  }
  else
  {
    throw error(MatriculaDuplicada);
  }
}

// θ(n)
void terminal::borra_llista_lliures(node *&n)
{
    // PRE:
    // POST:

    if (n != NULL)
    {
        borra_llista_lliures(n->_seg);
        delete n;
        n = NULL;
    }
}


// --------------------------- Mètodes públics ---------------------------

//
terminal::terminal(nat n, nat m, nat h, estrategia st) throw(error):
        _n(n),
        _m(m),
        _h(h),
        _st(st),
        _ct(n * m * h),
        _head(NULL)
{
  // PRE: True
  // POST: Crea una terminal vàlida, y una llista enllaçada de places lliures
  //       Retorna un error en cas contrari

  if ((_n != 0) and (_m != 0) and (_h != 0) and (_h <= HMAX) and (st == FIRST_FIT || st == LLIURE))
  {

    _n = n;
    _m = m;
    _h = h;

    inicialitza_am(_n, _m, _h);
    crea_llista_lliures(_n, _m, _h);

    opsgrua = 0;

    if (st == FIRST_FIT)    _st = FIRST_FIT;
    else if (st == LLIURE)  _st = LLIURE;

  }
  else if (n == 0)
  {
    throw error(NumFileresIncorr);
  }
  else if (m == 0)
  {
      throw error(NumPlacesIncorr);
  }
  else if (h == 0 or h >= HMAX)
  {
      throw error(AlcadaMaxIncorr);
  }
  else if (st != FIRST_FIT and st != LLIURE)
  {
      throw error(EstrategiaIncorr);
  }
}

// 
terminal::terminal(const terminal& b) throw(error):
        _n(b._n),
        _m(b._m),
        _h(b._h),
        _st(b._st),
        _ct(b._ct),
        _head(NULL)
{
  // PRE:
  // POST:

  inicialitza_am(_n, _m, _h);

  crea_llista_lliures(_n, _m, _h);

  _ct = b._ct;
  _area_espera = b._area_espera;
}

// θ(n)
terminal& terminal::operator=(const terminal& b) throw(error)
{
  // PRE:
  // POST:

  if (this != &b)
  {
    terminal t(b);        // t es una copia de b

    _head = t._head;

    _ct = t._ct;
    _area_espera = t._area_espera;
  }

  return *this;
}

// θ(n)
terminal::~terminal() throw()
{
  	// PRE: True
  	// POST: Memòria dinàmica alliberada de les ubicacions lliures
  	//	   àrea d'emmagatzematge esborrada i àrea d'espera buida

  borra_llista_lliures(_head);

  delete[] est_am;

  _area_espera.clear();

}

//
void terminal::insereix_contenidor(const contenidor &c) throw(error)
{
  	// PRE:
  	// POST:

  	Cu co_ub;
  	co_ub.c = c;
  	if (_st == FIRST_FIT) insereix_ff(co_ub);

  	//if (_st == LLIURE) insereix_ll(c);

}

//
void terminal::retira_contenidor(const string &m) throw(error)
{
  	// PRE:
  	// POST:

  	if (_st == FIRST_FIT) retira_ff(m);
}

// θ(1)
ubicacio terminal::on(const string &m) const throw()
{
  	// PRE:
  	// POST:

  	ubicacio u = ubicacio(-1,-1,-1);

  	if (_ct.existeix(m))
  	{
      u = _ct[m].u;
  	}

  	return u;
}

// θ(1)
nat terminal::longitud(const string &m) const throw(error)
{
  	// PRE:
  	// POST:

  	if (_ct.existeix(m))
  	{	
   		contenidor c = _ct[m].c;
    	return c.longitud();
  	}
  	else
  	{
    	throw error(MatriculaInexistent);
  	}
}

// θ(1)
void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error)
{
  	// PRE:
  	// POST:

  	int i = u.filera();
  	int j = u.placa();
  	int k = u.pis();

  	if (((i >= 0) and (j >= 0) and (k >= 0)) and ((i < _n) and (j < _m) and (k < _h)))
  	{
    	m = est_am[i][j][k];
    	if (m == "___") m = "";
  	}
  	else
  	{
    	throw error(UbicacioNoMagatzem);
  	}
}

// θ(n)
nat terminal::fragmentacio() const throw()
{
  	// PRE:
  	// POST:

    node *p = _head;

    int i = p->_u.filera();
    int j = p->_u.placa();
    int k = p->_u.pis();

    nat frag = 0;

    while (p != NULL)
    {
        //std::cout << i << j << k << '\n';

        // Sólo se comprueba si se esta en la base o hay algo debajo
        if (k == 0 or (est_am[i][j][k-1] != "" and est_am[i][j][k-1] != "___"))
        {
          // Caso 1: Sólo hay una plaza
          if (j == 0 and j == _m-1)
          {
            if (est_am[i][j][k] == "___" or est_am[i][j][k] == "") frag++;
          }
          // Caso 2: Esquina izquierda
          else if (j == 0 and j != _m-1)
          {
            if (est_am[i][j][k] == "___" or est_am[i][j][k] == "")
            {
              if (est_am[i][j+1][k] != "___" and est_am[i][j+1][k] != "") frag++;
              if (k != 0)
              {
              	if (est_am[i][j][k-1] != "___" and est_am[i][j][k-1] != "")
              	{
              		if (est_am[i][j+1][k] == "" and (est_am[i][j+1][k-1] == "___" or est_am[i][j+1][k-1] == "")) frag++;
              	}
              }
            }
          }
          // Caso 3: Esquina derecha:
          else if (j != 0 and j == _m-1)
          {
            if (est_am[i][j][k] == "___" or est_am[i][j][k] == "")
            {
              if (k!=0)
              {
                if (est_am[i][j-1][k-1] == "___" or est_am[i][j-1][k-1] == "")
                {
                  if (est_am[i][j-1][k] == "___" or est_am[i][j-1][k] == "") frag++;
                }
                else if (est_am[i][j-1][k-1] != "___" and est_am[i][j-1][k-1] != "")
                {
                  if (est_am[i][j-1][k] != "___" and est_am[i][j-1][k] != "") frag++;
                }
              }
              else
              {
                if (est_am[i][j-1][k] != "___" and est_am[i][j-1][k] != "") frag++;
              }
            }
          }
          // Por el medio de la matriz
          else
          {
            if (j >= 0 and j <= _m-1)
            {
              if (est_am[i][j][k] == "___" or est_am[i][j][k] == "")
              {
                if ((est_am[i][j-1][k] != "" and est_am[i][j-1][k] != "___") and (est_am[i][j+1][k] != "" and est_am[i][j+1][k] != "___")) frag++;
                if ((est_am[i][j-1][k] == "" or est_am[i][j-1][k] == "___") and (est_am[i][j+1][k] != "" and est_am[i][j+1][k] != "___"))
                {
                  if (k!=0)
                  {
                    if (est_am[i][j-1][k-1] == "___" or est_am[i][j-1][k-1] == "") frag++;
                  }
                }
                if ((est_am[i][j+1][k] == "" or est_am[i][j+1][k] == "___") and (est_am[i][j-1][k] != "" and est_am[i][j-1][k] != "___"))
                {
                  if (k!=0)
                  {
                    if (est_am[i][j+1][k-1] == "___" or est_am[i][j+1][k-1] == "") frag++;
                  }
                }
                if ((est_am[i][j-1][k] == "" or est_am[i][j-1][k] == "___") and (est_am[i][j+1][k] == "" or est_am[i][j+1][k] == "___"))
                {
                  if (k!=0)
                  {
                    if ((est_am[i][j-1][k-1] == "___" or est_am[i][j-1][k-1] == "") and (est_am[i][j+1][k-1] == "___" or est_am[i][j+1][k-1] == "")) frag++;
                  }
                }
              }
            }
          }
        }
        // Iteracion de las posiciones
        if (j == _m-1 and k == _h-1)
        {
          i++;
          j=0;
          k=0;
        }
        else if (k == _h-1)
        {
          j++;
          k=0;
        }
        else k++;

        p = p->_seg;
    }
    return frag;
}

// θ(1)
nat terminal::ops_grua() const throw()
{
  	// PRE:
  	// POST:

  	return opsgrua;
}

// θ(n * log n)
void terminal::area_espera(list<string> &l) const throw()
{
  	// PRE:
  	// POST:

  	l = _area_espera;
  	l.sort();								// Mètode del tipus list de la STD que ordena de forma ascendent els elements
  											// el seu cost al best case, average case i worst case es θ(n * log n), el que ho fa molt interessant
  											// equiparant-se a Heap-sort i millorant a Merge-sort i Quick-sort als pitjors casos
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

// θ(1)
terminal::estrategia terminal::quina_estrategia() const throw()
{
  	// PRE:
  	// POST:

  	return _st;
}
