#include "terminal.hpp"
// --------------------------- Mètodes privats ---------------------------

// θ(n³)
void terminal::inicialitza_am(int n, int m, int h)
{
  	// PRE: n = _n, m = _m, h = _h
  	// POST: Crea l'AM de la terminal buida

  	string buit = "";
  	string buit_a_baix = "___";
  	_am = new string**[n];

  	for (int i=0; i<n; i++)
  	{
    	_am[i] = new string*[m];

    	for (int j=0; j<m; j++)
    	{
      		_am[i][j] = new string[h];

      		for (int k=0; k<h; k++)
      		{
        		if (k==0) 	_am[i][j][k] = buit_a_baix;
        		else 		_am[i][j][k] = buit;
      		}
    	}
  	}
}

// θ(n³)
void terminal::crea_llista_lliures(int n, int m, int h)
{
  	// PRE: n = _n, m = _m, h = _h
  	// POST: Crea la llista d'ubicacions lliures de l'AM

	node *act = NULL;
	node *prev = NULL;
  	_size = 0;

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
        
		        act->_lliu = true;
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
  	// PRE: True
  	// POST: Recorre la llista d'ubicacions lliures i actualitza el seu estat (Lliure / Ocupada)

  	node *n = _head;

  	int i = n->_u.filera();
  	int j = n->_u.placa();
  	int k = n->_u.pis();

  	bool acaba = false;

  	while (n != NULL and not acaba)
  	{
      	if (_am[i][j][k] == "___" or _am[i][j][k] == "") n->_lliu = true;
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
void terminal::reinserta_ff()
{
	// PRE: True
	// POST: Recorre l'AEM tractant de insertar contenidors a l'AM (Per l'estrategia FIRST_FIT)

  	Cu co_ub;
 
  	if (not _aem.empty())
  	{
      	for (list<string>::iterator it=_aem.begin(); it!=_aem.end(); it++)
      	{
        	co_ub.c = _ct[*it].c;
        	co_ub.u = _ct[*it].u;

        	if (_st == FIRST_FIT) insereix_ff(co_ub);

        	if (_ct[co_ub.c.matricula()].u != ubicacio(-1,0,0))
        	{
        		list<string>::iterator it2 = it;
        		_aem.erase(it2);
        		it=_aem.begin();
        		it--;
        	}
      	}
  	}
}

// 0(n)
void terminal::reinserta_ll()
{
	// PRE: True
	// POST: Recorre l'AEM tractant de insertar contenidors a l'AM (Per l'estrategia LLIURE)

  	Cu co_ub;
 
  	if (not _aem.empty())
  	{
  		list<string> petits;
  		list<string> mitjans;
  		list<string> grans;

      	for (list<string>::iterator it=_aem.begin(); it!=_aem.end(); it++)
      	{   		
      		if (_ct[*it].c.longitud() == 10)		petits.push_back(*it);
      		else if (_ct[*it].c.longitud() == 20)	mitjans.push_back(*it);
      		else if (_ct[*it].c.longitud() == 30)	grans.push_back(*it);
      	}

      	_aem.clear();
        
        for (list<string>::iterator it=grans.begin(); it!=grans.end(); it++)		_aem.push_back(*it);
      	grans.clear();

		for (list<string>::iterator it=mitjans.begin(); it!=mitjans.end(); it++)	_aem.push_back(*it);
      	mitjans.clear();

		for (list<string>::iterator it=petits.begin(); it!=petits.end(); it++) 		_aem.push_back(*it);
      	petits.clear();

		for (list<string>::iterator it=_aem.begin(); it!=_aem.end(); it++)
      	{
        	co_ub.c = _ct[*it].c;
        	co_ub.u = _ct[*it].u;

        	insereix_ff(co_ub);

        	if (_ct[co_ub.c.matricula()].u != ubicacio(-1,0,0))
        	{
        		list<string>::iterator it2 = it;
        		_aem.erase(it2);
        		it=_aem.begin();
        		it--;
        	}
      	}
    }
}

// θ(1)
void terminal::recorre_dreta(int i, int j, int k, int jinici, int kinici)
{
	// PRE: i es la filera que ocupa el contenidor a retirar
	//		j es el nombre de places, cap a la dreta, que ocupa el contenidor de sobre del que estem retirant
	// 		k es el pis de sobre al contenidor a retirar
	// 		jinici es la plaça que ocupa el contenidor a retirar
	//		kinici es el pis que ocupa el contenidor a retirar
	// POST: Envia a l'AEM els contenidors de la part superior dreta d'un contenidor a retirar

	if (k < _h and j >= jinici)
	{
		if (_ct.existeix(_am[i][j][k]) and _ct[_am[i][j][k]].u != ubicacio(-1,0,0))
    	{
	    	Cu co_aux;
	      	co_aux.c = _ct[_am[i][j][k]].c;
	      	co_aux.u = ubicacio(-1,0,0);
	      	_ct.elimina(_am[i][j][k]);
	      	_aem.push_back(_am[i][j][k]);
	      	_opsgrua++;
	      	_ct.assig(_am[i][j][k], co_aux);
    	}
    	_am[i][j][k]="";
		recorre_dreta(i, j, ++k, jinici, kinici);
		recorre_dreta(i, --j, kinici+1, jinici, kinici);
	}
}

// θ(1)
void terminal::recorre_esq(int i, int j, int k, int jinici, int kinici)
{
	// PRE: i es la filera que ocupa el contenidor a retirar
	//		j es la plaça que ocupa el contenidor a retirar
	// 		k es el pis de sobre al contenidor a retirar
	// 		jinici es el nombre de places, cap a la esquerra, que ocupa el contenidor de sobre del que estem retirant
	//		kinici es el pis que ocupa el contenidor a retirar
	// POST: Envia a l'AEM els contenidors de la part superior esquerra d'un contenidor a retirar

	if (k < _h and j >= jinici)
  	{
  		if (_ct.existeix(_am[i][j][k]) and _ct[_am[i][j][k]].u != ubicacio(-1,0,0))
        {
        	Cu co_aux;
          	co_aux.c = _ct[_am[i][j][k]].c;
          	co_aux.u = ubicacio(-1,0,0);
          	_ct.elimina(_am[i][j][k]);
          	_aem.push_back(_am[i][j][k]);
          	_opsgrua++;
          	_ct.assig(_am[i][j][k], co_aux);
        }
        _am[i][j][k]="";
  		recorre_esq(i, j, ++k, jinici, kinici);
  		recorre_esq(i, --j, kinici+1, jinici, kinici); 		
  	}
}

// θ(n): sent n == actuals
void terminal::reorganitza_aem(nat actuals)
{
	// PRE: actuals es el nombre de contenidors que havien a l'AEM abans de fer una retirada
	// POST: Els contenidors que havien previament, s'envien al final de l'AEM, 
	//		 per prioritzar la reinserció dels últims que han entrat

	int i = 0;
	for (list<string>::iterator it=_aem.begin(); it!=_aem.end() and i < actuals; it++, i++)
	{
		list<string>::iterator it2 = it;
		_aem.push_back(*it);
		_aem.erase(it2);
	}
}

// θ(1)
void terminal::allibera_places(int i, int j, int k, nat longi)
{
	// PRE: i, j i k son filera, placa i pis, respectivament, d'un contenidor retirat
	//		longi es el nombre de places que ocupava el contenidor a l'AM
	// POST: S'alliberen les posicions a l'AM, quedant lliures

	if (longi == 1)
	{
		if (k == 0) _am[i][j][k] = "___";
		else 		_am[i][j][k] = "";
	}
	else if (longi == 2)
	{
		if (k == 0)
        {
          	_am[i][j][k] = "___";
          	_am[i][j+1][k] = "___";
        }
        else
        {
          	_am[i][j][k] = "";
          	_am[i][j+1][k] = "";
        }
    }
    else if (longi == 3)
    {
    	if (k == 0)
      	{
        	_am[i][j][k] = "___";
        	_am[i][j+1][k] = "___";
        	_am[i][j+2][k] = "___";
      	}	
      	else
      	{
        	_am[i][j][k] = "";
        	_am[i][j+1][k] = "";
        	_am[i][j+2][k] = "";
      	}
    }
}

// 0(n)
void terminal::retira_ff(string m) throw(error)
{
  	// PRE: m es la matricula d'un contenidor
  	// POST: Si el contenidor al que fa referència m, està present al catàleg, es retira de la ubicació que ocupava (AEM o AM) seguint l'estrategia FIRST_FIT.
  	//		 En cas de que estigués ubicat a l'AM, si el contenidor en qüestió tenia contenidors a sobre, s'envien a l'AEM temporalment
  	//		 En cas de no existir al catàleg, es retorna un error.

  	if (_ct.existeix(m))
  	{	
	    Cu co_ub;
	    co_ub.c = _ct[m].c;
	    co_ub.u = _ct[m].u;
	    ubicacio ub_aem = ubicacio(-1,0,0);

	    // CD: El contenidor es troba a l'AEM, es retira directament i s'elimina del catàleg
	    if (co_ub.u == ub_aem)
	    {
	      _ct.elimina(co_ub.c.matricula());
	      _aem.remove(co_ub.c.matricula());
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
        		if (k+1 == _h or (k+1 != _h and _am[i][j][k+1] == ""))
        		{
		        	_ct.elimina(co_ub.c.matricula());
		        	_opsgrua++;

          			allibera_places(i, j, k, longi);								// Les places ocupades pel contenidor retirat, es queden lliures a l'AM
          			actualitza_lliures();											// Les places de l'AM que no contenen cap contenidor, estàn lliures per fer noves insercions
        		}	
        		// CR: El contenidor té contenidors a sobre
	        	else
	        	{
		          	int jder = j;
		          	int jesq = j;

		          	// Recorre la part superior dreta del contenidor a retirar mentre el contenidor de sobre sigui el mateix (com a molt 3 iteracions)
		          	while (jder+1 <= _m-1 and _am[i][jder][k+1] == _am[i][jder+1][k+1])		jder++;

		          	// Recorre la part superior esquerra del contenidor a retirar mentre el contenidor de sobre sigui el mateix (com a molt 3 iteracions)
		          	while (jesq-1 >= 0 and _am[i][jesq][k+1] == _am[i][jesq-1][k+1])		jesq--;		


		          	nat actuals = _aem.size();										// Nombre de contenidors que hi ha actualment a l'AEM

		          	recorre_dreta(i, jder, k+1, j, k);								// Es retiren els contenidors de sobre del contenidor a retirar i es van afegint a l'AEM 
		          	recorre_esq(i, j, k+1, jesq, k);								// i s'alliberen les places que ocupaven a l'AM

	          		if (actuals != 0) reorganitza_aem(actuals);						// Si n'hi havien contenidors a l'AEM abans d'enviar-ne de nous en el moment de la retirada
	          																		// aquests passen al final de l'AEM, ja que els prioritaris de reinsertar son els moguts recentment
	          		_ct.elimina(co_ub.c.matricula());
	          		_opsgrua++;

	          		allibera_places(i, j, k, longi);
	          		actualitza_lliures();
	        	}
      		}
      		// C2: El contenidor a retirar ocupa dues places a l'AM
      		else if (longi == 2)
	  		{
	      		int j2 = 0;
	      		if (_am[i][j+1][k] == co_ub.c.matricula())			j2 = j+1;		// Fixem les dues places adjacents 
	      		else if (_am[i][j-1][k] == co_ub.c.matricula())		j2 = j-1;

	      		if (j2 < j)															// j2 serà, de les dues places, la situada més a la dreta
	      		{
	      			int aux = j2;
	      			j2 = j;
	      			j2 = aux;
	      		}

	      		// CD: El contenidor està al pis més alt, o no te res a sobre
	      		if (k+1 == _h or (k+1 != _h and (_am[i][j][k+1] == "" and _am[i][j2][k+1] == "")))       
	      		{
			    	_ct.elimina(co_ub.c.matricula());
			        _opsgrua++;

	        		allibera_places(i, j, k, longi);
					actualitza_lliures();
	      		}
	     	 	// CR: El contenidor té contenidors a sobre
	      		else
	      		{
	      			int jesq = j;
	      			int jder = j2;
	      	
	      			while (jder+1 <= _m-1 and _am[i][jder][k+1] == _am[i][jder+1][k+1])		jder++;

	      			while (jesq-1 >= 0 and _am[i][jesq][k+1] == _am[i][jesq-1][k+1])		jesq--;	

			      	nat actuals = _aem.size();

			      	recorre_dreta(i, jder, k+1, j2, k);
			      	recorre_esq(i, j, k+1, jesq, k);

	      			if (actuals != 0) reorganitza_aem(actuals);

			      	_ct.elimina(co_ub.c.matricula());
			      	_opsgrua++;

			      	allibera_places(i, j, k, longi);
			      	actualitza_lliures();
	 	 		}

	  		}
      		// C3: El contenidor a retirar ocupa tres places a l'AM
      		else 
      		{
		        int j3 = 0;
		        int j2 = 0;

		        if (j+2 != _m and _am[i][j][k] == _am[i][j+2][k]) j3 = j+2;
		        else if (j+1 != _m and _am[i][j][k] == _am[i][j+1][k]) j3 = j+1;
		        else if (j+1 != _m) j3 = j;

		        j2 = j3-1;															// j serà la placa situada més a l'esquerra, j2 la del mig i j3 la del costat dret
		        j = j2-1;

				// CD: El contenidor està al pis més alt, o no te res a sobre
		        if (k+1 == _h or (k+1 != _h and (_am[i][j][k+1] == "" and _am[i][j2][k+1] == "" and _am[i][j3][k+1] == "")))
		        {
		          	_ct.elimina(co_ub.c.matricula());
		          	_opsgrua++;

		          	allibera_places(i, j, k, longi);
		          	actualitza_lliures();
		        }
		        // CR: El contenidor té contenidors a sobre
		        else
		        {
		        	int jesq = j;
		          	int jder = j3;
		          	
		          	while (jder+1 <= _m-1 and _am[i][jder][k+1] == _am[i][jder+1][k+1])			jder++;
		         
		          	while (jesq-1 >= 0 and _am[i][jesq][k+1] == _am[i][jesq-1][k+1])			jesq--;

		         	nat actuals = _aem.size();

		          	recorre_dreta(i, jder, k+1, j2, k);
		          	recorre_esq(i, j, k+1, jesq, k);   

		          	if (actuals != 0) reorganitza_aem(actuals);

		          	_ct.elimina(co_ub.c.matricula());
		          	_opsgrua++;

          			allibera_places(i, j, k, longi);
          			actualitza_lliures();
        		}
      		}
    	}
    	if (_st == FIRST_FIT) 	reinserta_ff();			// Després d'una retirada, es reinserta a l'AM d'una manera o d'altra en funció de l'estrategia utilitzada
    	else 					reinserta_ll();
  }
  else
  {
    throw error(MatriculaInexistent);
  }
}


// 0(n)
void terminal::insereix_ff(Cu co_ub) throw(error)
{
  	// PRE: co_ub es un objecte del tipus Cu, que conté l'objecte contenidor a inserir seguint l'estrategia FIRST_FIT.
  	// POST: Si el contenidor no està present al catàleg, o està present però ubicat a l'AEM, s'intenta inserir a l'AM.
  	//		 ... Si no fos possible d'insertar, s'enviaria directament a l'AEM, i s'afegiria al catàleg. En cas de que ja fos a l'AEM, es mantindria.
  	//		 En cas contrari, es retorna un error.

  	bool existeix_aem = false;
  	bool reinsercio = false;							// Si un contenidor que està intentant ser reinsertat després d'una retirada d'un contenidor d'un pis inferior, ho es finalment
  														// no es cridarà al mètode reinserta() de nou, ja que ja està recorrent l'AEM en aquests moments

  	ubicacio ub_aem = ubicacio(-1,0,0);

  	if (_ct.existeix(co_ub.c.matricula()))
  	{
    	if (_ct[co_ub.c.matricula()].u == ub_aem) existeix_aem = true;
  	}
  	else existeix_aem = false;

  	if (not _ct.existeix(co_ub.c.matricula()) or existeix_aem)
  	{
	    nat longi = co_ub.c.longitud() / 10;
	    bool fi = false;    							// El booleà fi es posarà a true quan insereixi un contenidor de 10 peus, o quan es surti dels límits de l'AM

	    node *p = _head;
	    node *inici = NULL;
	    node *p2 = NULL;

	    // CD: El contenidor es major que la mida de l'AM
	    if (longi > _size)
	    {
	      fi = true;
	    }

	    // C1: El contenidor a insertar només ocupará una ubicació a l'AM,
	    // agafarà la primera ubicació lliure que trobi amb el recorregut actual
	    if (longi == 1)
	    {
	      	while (not fi and p->_lliu != true)      
	      	{
	        	if (p->_seg == NULL) fi = true;
	        	else  p = p->_seg;
	      	}
	      	if (not fi)
	      	{
		        if (_ct.existeix(co_ub.c.matricula()))					// Si el contenidor ja existia al catàleg, significa que es a l'AEM
		        {														// per tant, esborrem el contenidor del catàleg i marquem la reinserció
		          _ct.elimina(co_ub.c.matricula());						// per, a continuació, reinsertar el contenidor a l'AM i al catàleg amb la nova ubicació
		          reinsercio = true;
		        }

		        co_ub.u = p->_u;
		        _ct.assig(co_ub.c.matricula(), co_ub);      			

		        _opsgrua++;	
		        p->_lliu = false;                           			// Marca la ubicació com a ocupada
	        
		        int i = p->_u.filera();
		        int j = p->_u.placa();
		        int k = p->_u.pis();

		        _am[i][j][k] = co_ub.c.matricula();
	        
		        fi = true;

		        if (not reinsercio)
		        {
		        	if (_st == FIRST_FIT) 	reinserta_ff();
		        	else 					reinserta_ll();
		        }
      		}	
    	}	
    	// C2: El contenidor a insertar ocuparà dues o tres places a l'AM
	    else
	    {
	      	while (p != NULL and not fi and p->_lliu != true)     
	      	{
		        if (p->_seg == NULL) fi = true;
		        p = p->_seg;
	      	}
	      	if (not fi)
	      	{
		        inici = p;
		        int filera_act = inici->_u.filera();
		        int pis_act = inici->_u.pis();

		        nat i = 1;      							// Es troba la primera plaça lliure de dos o tres necessàries

	        	while (p != NULL and i < longi and not fi)
	        	{
	        		// Si la inserció a les primeres places lliures ha fracasat, es torna a intentar a les places següents (i == 0)
		            if (i == 0)           					
		            {
		              if (p->_seg != NULL)	p = inici->_seg;

		              while (p!=NULL and not fi and p->_lliu != true)
		              {
		                if (p->_seg == NULL) fi = true;
		                else p = p->_seg;
		              }

		              i = 1;
		              inici = p;
		              p2 = NULL;
		              filera_act = inici->_u.filera();
		              pis_act = inici->_u.pis();
		            }

		            if (p == NULL) fi = true;
		            else
		            {
		              	if (p->_seg != NULL) p = p->_seg;
		              	else fi = true;

		              	while (p->_seg!=NULL and (p->_u.pis() != pis_act and p->_u.filera() == filera_act) and not fi) 
		              	{
		              		// Es fa un salt cap a la següent plaça del mateix pis
		                	p = p->_seg;
		              	}

		              	if (inici->_u.pis() != 0 or p->_u.pis() != 0)
		              	{
		              		if (p2 != NULL)
			                {
			                	if (p->_ant->_lliu == true or inici->_ant->_lliu == true or p2->_ant->_lliu == true) i = 0;			// Si la placa següent no es vàlida per posar el contenidor
			                }																										// es torna a buscar una ubicació vàlida des de la placa següent
			                if ((p->_ant->_lliu == true or inici->_ant->_lliu == true) and p->_u.filera() == filera_act) i = 0;		// a la primera que s'havia trobat
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
	    // Si !fi, no ens hem sortit dels limits de l'AM, per tant, s'han trobat les places necessàries vàlides per inserir un contenidor 
	    if (not fi)
	    {
	    	// El contenidor a afegir ocupa dues places
	      	if (longi == 2)
	      	{
	        	if (_ct.existeix(co_ub.c.matricula()))
	        	{	
	          		_ct.elimina(co_ub.c.matricula());
	          		reinsercio = true;
	        	}

		        int i = inici->_u.filera();
		        int j = inici->_u.placa();
		        int k = inici->_u.pis();

		        _am[i][j][k] = co_ub.c.matricula();			// Ocupa l'AM

		        co_ub.u = inici->_u;						// S'inserta al catàleg amb la plaça més petita de les places que ocupa
		        _ct.assig(co_ub.c.matricula(), co_ub);		// Es reinserta_ff al catàleg
		        _opsgrua++;

		        i = p2->_u.filera();
		        j = p2->_u.placa();
		        k = p2->_u.pis();
		        _am[i][j][k] = co_ub.c.matricula();			// Ocupa l'AM

		        inici->_lliu = false;						// Les places que ocupa ja no estàn lliures
		        p2->_lliu = false;

		        if (not reinsercio)
		        {
		        	if (_st == FIRST_FIT) 	reinserta_ff();
		        	else 					reinserta_ll();
		        }
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

		        _am[i][j][k] = co_ub.c.matricula();

		        co_ub.u = inici->_u;
		        _ct.assig(co_ub.c.matricula(), co_ub);
		        _opsgrua++;

		        i = p2->_u.filera();
		        j = p2->_u.placa();
		        k = p2->_u.pis();
		        _am[i][j][k] = co_ub.c.matricula();

		        i = p->_u.filera();
		        j = p->_u.placa();
		        k = p->_u.pis();
		        _am[i][j][k] = co_ub.c.matricula();

		        inici->_lliu = false;
		        p2->_lliu = false;
		        p->_lliu = false;
		        
		        if (not reinsercio)
		        {
		        	if (_st == FIRST_FIT) 	reinserta_ff();
		        	else 					reinserta_ll();
		        }
	  		}
    	}
	    // Si fi, ens hem sortit dels limits de l'AM sense inserir, o bé, s'ha insertat un contenidor de 10 peus
	    else
	    {
	        if (not _ct.existeix(co_ub.c.matricula()))		// Si no està present al catàleg, el contenidor s'ubica directament a l'AEM
	        {
	          	co_ub.u = ub_aem;
	          	_ct.assig(co_ub.c.matricula(), co_ub);
	          	_aem.push_front(co_ub.c.matricula());
	        }
	    }
  	}
  	else
  	{
    	throw error(MatriculaDuplicada);
  	}
}


// θ(n)
void terminal::borra_llista(node *&n)
{
    // PRE: n es un punter al primer element de la llista (_head)
    // POST: S'allibera memòria dinàmica

    if (n != NULL)
    {
        borra_llista(n->_seg);
        delete n;
        n = NULL;
    }
}


// --------------------------- Mètodes públics ---------------------------

// θ(n³)
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
	    _opsgrua = 0;

	    if (st == FIRST_FIT) 	_st = FIRST_FIT;
	    else if (st == LLIURE) 	_st = LLIURE;

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

// θ(n³)
terminal::terminal(const terminal& b) throw(error):
        _n(b._n),
        _m(b._m),
        _h(b._h),
        _st(b._st),
        _ct(b._ct),
        _head(NULL)
{
  	// PRE: True
  	// POST: El p.i. es una terminal idèntica a b

 	inicialitza_am(b._n, b._m, b._h);

 	crea_llista_lliures(b._n, b._m, b._h);

  	_ct = b._ct;
  	_aem = b._aem;
}

// θ(n)
terminal& terminal::operator=(const terminal& b) throw(error)
{
  	// PRE: True
  	// POST: El p.i. es una terminal idèntica a b

  	if (this != &b)
  	{
    	terminal t(b);        

    	_head = t._head;

    	_ct = t._ct;
    	_aem = t._aem;
  	}

  	return *this;
}

// θ(n)
terminal::~terminal() throw()
{
  	// PRE: True
  	// POST: Memòria dinàmica alliberada de la llista d'ubicacions lliures
  	//	   	 àrea d'emmagatzematge esborrada i àrea d'espera buidada

  	borra_llista(_head);

  	delete[] _am;

  	_aem.clear();								// Mètode del tipus list de la STD que buida el contenidor (llista) sencer, 
  												// eliminant tots els elements i deixant-li una mida de 0 (_aem.size() == 0)
}

// θ(n)
void terminal::insereix_contenidor(const contenidor &c) throw(error)
{
  	// PRE: c es un contenidor
  	// POST: S'afegeix un contenidor a la terminal (AEM o AM)

  	Cu co_ub;				// Creació objecte tipus Cu
  	co_ub.c = c;			// El camp contenidor de l'objecte, obté l'objecte rebut per paràmetre

  	insereix_ff(co_ub);

}

// θ(n)
void terminal::retira_contenidor(const string &m) throw(error)
{
  	// PRE: m es la matricula d'un contenidor
  	// POST: Es retira el contenidor associat a la matricula m de la terminal (AEM o AM)

  	retira_ff(m);
}

// θ(1)
ubicacio terminal::on(const string &m) const throw()
{
  	// PRE: True
  	// POST: Retorna la ubicació on es troba un contenidor.
  	//		 Si es troba a l'AM i el contenidor ocupa diverses places, retorna el nombre de plaça més baix.
  	//		 Si es troba a l'AEM retorna la ubicació <-1,0,0>.
  	//		 Si no es troba al catàleg de contenidors i ubicacions (no existeix) retorna la ubicació <-1,0,0>

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
  	// PRE: True
  	// POST: Retorna la longitud d'un contenidor.
  	//		 Retorna un error si el contenidor no existeix

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
  	// PRE: True
  	// POST: L'string m es la matricula del contenidor que ocupa la ubicació u.
  	//		 Retorna un error si la ubicació u no existeix a l'AM

  	int i = u.filera();
  	int j = u.placa();
  	int k = u.pis();

  	if (((i >= 0) and (j >= 0) and (k >= 0)) and ((i < _n) and (j < _m) and (k < _h)))
  	{
    	m = _am[i][j][k];
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
  	// PRE: True
  	// POST: Retorna el nombre de places on hi cap exactament un contenidor de 10 peus, i no un de més llarg

    node *p = _head;

    int i = p->_u.filera();
    int j = p->_u.placa();
    int k = p->_u.pis();

    nat frag = 0;

    while (p != NULL)
    {
        if (k == 0 or (_am[i][j][k-1] != "" and _am[i][j][k-1] != "___"))
        {
			// C1: Només n'hi ha una plaça
          	if (j == 0 and j == _m-1)
          	{
            	if (_am[i][j][k] == "___" or _am[i][j][k] == "") frag++;
          	}
          	// C2: Cantonada esquerra
          	else if (j == 0 and j != _m-1)
          	{
            	if (_am[i][j][k] == "___" or _am[i][j][k] == "")
            	{
              		if (_am[i][j+1][k] != "___" and _am[i][j+1][k] != "") frag++;
              		if (k != 0)
              		{
              			if (_am[i][j][k-1] != "___" and _am[i][j][k-1] != "")
              			{
              				if (_am[i][j+1][k] == "" and (_am[i][j+1][k-1] == "___" or _am[i][j+1][k-1] == "")) frag++;
              			}
              		}
            	}
          	}
			// C3: Cantonada dreta
          	else if (j != 0 and j == _m-1)
          	{
            	if (_am[i][j][k] == "___" or _am[i][j][k] == "")
            	{
              		if (k!=0)
              		{
                		if (_am[i][j-1][k-1] == "___" or _am[i][j-1][k-1] == "")
                		{
                  			if (_am[i][j-1][k] == "___" or _am[i][j-1][k] == "") frag++;
                		}
                		else if (_am[i][j-1][k-1] != "___" and _am[i][j-1][k-1] != "")
                		{
                  			if (_am[i][j-1][k] != "___" and _am[i][j-1][k] != "") frag++;
                		}
              		}
              		else
              		{
                		if (_am[i][j-1][k] != "___" and _am[i][j-1][k] != "") frag++;
              		}
            	}
          	}
          	// C4: Al mig de l'AM
          	else
          	{
            	if (j >= 0 and j <= _m-1)
            	{
              		if (_am[i][j][k] == "___" or _am[i][j][k] == "")
              		{
                		if ((_am[i][j-1][k] != "" and _am[i][j-1][k] != "___") and (_am[i][j+1][k] != "" and _am[i][j+1][k] != "___")) frag++;
                		if ((_am[i][j-1][k] == "" or _am[i][j-1][k] == "___") and (_am[i][j+1][k] != "" and _am[i][j+1][k] != "___"))
                		{
                  			if (k!=0)
                  			{
                    			if (_am[i][j-1][k-1] == "___" or _am[i][j-1][k-1] == "") frag++;
                  			}
                		}
                		if ((_am[i][j+1][k] == "" or _am[i][j+1][k] == "___") and (_am[i][j-1][k] != "" and _am[i][j-1][k] != "___"))
                		{
                  			if (k!=0)
                  			{
                    			if (_am[i][j+1][k-1] == "___" or _am[i][j+1][k-1] == "") frag++;
                  			}
                		}
                		if ((_am[i][j-1][k] == "" or _am[i][j-1][k] == "___") and (_am[i][j+1][k] == "" or _am[i][j+1][k] == "___"))
                		{
                  			if (k!=0)
                  			{
                    			if ((_am[i][j-1][k-1] == "___" or _am[i][j-1][k-1] == "") and (_am[i][j+1][k-1] == "___" or _am[i][j+1][k-1] == "")) frag++;
                  			}
                		}
              		}
            	}
          	}
        }
        // Iteració de les posicions seguint l'ordre d'augmentar pisos, després places i posterioment fileres
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
  	// PRE: True
  	// POST: Retorna el nombre d'operacions de grua realitzades des de la creació del p.i.

  	return _opsgrua;
}

// θ(n * log n)
void terminal::area_espera(list<string> &l) const throw()
{
  	// PRE: True
  	// POST: l conté les matricules dels contenidors que hi son a l'AEM ordenades ascendentment	 

  	l = _aem;
  	l.sort();								// Mètode del tipus list de la STD que ordena de forma ascendent els elements
  											// el seu cost al best case, average case i worst case es θ(n * log n), el que ho fa molt interessant
  											// equiparant-se a Heap-sort i millorant a Merge-sort i Quick-sort als pitjors casos
}

// θ(1)
nat terminal::num_fileres() const throw()
{
  	// PRE: True
  	// POST: Retorna el nombre de fileres de l'AM

  	return _n;
}

// θ(1)
nat terminal::num_places() const throw()
{
  	// PRE: True
  	// POST: Retorna el nombre de places de l'AM

  	return _m;
}

// θ(1)
nat terminal::num_pisos() const throw()
{
  	// PRE: True
  	// POST: Retorna el nombre de pisos de l'AM

  return _h;
}

// θ(1)
terminal::estrategia terminal::quina_estrategia() const throw()
{
  	// PRE: True
  	// POST: Retorna el tipus d'estrategia utilitzada

  	return _st;
}
