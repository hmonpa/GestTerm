#include "terminal.hpp"
// --------------------------- Mètodes privats ---------------------------


//
void terminal::inicialitza_am(int n, int m, int h)
{
  // PRE:
  // POST:

//  string buit = "___";
  string buit = "___";
  est_am = new string**[n];
  for (int i=0; i<n; i++)
  {
    est_am[i] = new string*[m]();

    for (int j=0; j<m; j++)
    {
      est_am[i][j] = new string[h]();

      for (int k=0; k<h; k++)
      {
        //est_am[i][j][k] = new string();
        est_am[i][j][k] = buit;
      }
    }
  }
}

void terminal::borra_am()
{
  // PRE:
  // POST:

  for (int i=0; i<_n; i++)
  {
    for (int j=0; j<_m; j++)
    {
      delete[] est_am[i][j];
    }
    delete[] est_am[i];
  }
  delete[] est_am;
  opsgrua = 0;
}

//
void terminal::crea_llista_lliures(int n, int m, int h)
{
  // PRE:
  // POST:
  //std::cout << n << m << h;
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
        //std::cout << act->_u.filera() << act->_u.placa() << act->_u.pis();
        //std::cout << "- " << act->_lliu << "  ";
      }
    }
  }
  //std::cout << "\n Tamano total de la lista: " << _size << "\n\n";
  //std::cout << "Elementos en la lista enlazada: " << _size << '\n';
}

// Líneal (Worst case)
void terminal::actualitza_lliures(node* n)
{
  // PRE: n es el _head y js es la plaza más a la derecha (depende de las plazas que ocupe)
  // POST: Recorre la lista hasta el centinela (js = siguiente plaça)

  bool acaba = false;
  int i = n->_u.filera();
  int j = n->_u.placa();
  int k = n->_u.pis();
  while (n != NULL and not acaba)
  {
    //if (j == js) acaba = true;
    //else
    //{
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
    //}
  }
}

//
int terminal::maxim_dos(int a, int b)
{
  //
  //

  return a > b ? a : b;
}

//
int terminal::maxim_tres(int a, int b, int c)
{
  //
  //

  return maxim_dos(maxim_dos(a,b), c);
}


void terminal::recorre(int cont)
{
  // PRE:
  // POST: Recorre el área de espera para intentar insertar contenedores

  Cu co_ub;
  //_area_espera.unique();
  bool acaba = false;
  if (not _area_espera.empty())
  {
    if (cont != 0)
    {
      for (list<string>::iterator it=_area_espera.begin(); it!=_area_espera.end() and not acaba; it++)
      {
        co_ub.c = _ct[*it].c;
        co_ub.u = _ct[*it].u;

        /*std::cout << "Intento de inserción de "<< co_ub.c.matricula() << '\n';
        std::cout << "LISTA ACTUAL:\n";
        for (list<string>::iterator it=_area_espera.begin(); it!=_area_espera.end(); it++) std::cout << (*it) << " ";
		std::cout << '\n';*/
        insereix_ff(co_ub);
        if(_ct[co_ub.c.matricula()].u != ubicacio(-1,0,0))
        {
        	list<string>::iterator it2 = it;
        	_area_espera.erase(it2);
        	it=_area_espera.begin();
        	it--;
        }
      }
    }
  }
}

void terminal::recorrido_der(int i, int jj, int kk, int jinici, int kinici)
{
	// PRE: k es k+1 respecto al contenedor a retirar 
	//

	if (kk < _h and jj >= jinici)
	{
		//std::cout << i << jj << kk << "\n";
		if (_ct.existeix(est_am[i][jj][kk]) and _ct[est_am[i][jj][kk]].u != ubicacio(-1,0,0))
    	{
	    	//std::cout << "Al area de espera " << est_am[i][jj][kk] << '\n';
	    	Cu co_aux;
	      	co_aux.c = _ct[est_am[i][jj][kk]].c;
	      	co_aux.u = ubicacio(-1,0,0);
	      	_ct.elimina(est_am[i][jj][kk]);
	      	//std::cout << mat1 << '\n';
	      	_area_espera.push_back(est_am[i][jj][kk]);
	      	_ct.assig(est_am[i][jj][kk], co_aux);
    	}
    	est_am[i][jj][kk]="";
		recorrido_der(i, jj, ++kk, jinici, kinici);
		recorrido_der(i, --jj, kinici+1, jinici, kinici);
	}
}

void terminal::recorrido_izq(int i, int jj, int kk, int jinici, int kinici)
{
	if (kk < _h and jj >= jinici)
  	{
  		//std::cout << i << jj << kk << "\n";
  		
  		if (_ct.existeix(est_am[i][jj][kk]) and _ct[est_am[i][jj][kk]].u != ubicacio(-1,0,0))
        {
        	//std::cout << "Al area de espera " << est_am[i][jj][kk] << '\n';
        	Cu co_aux;
          	co_aux.c = _ct[est_am[i][jj][kk]].c;
          	co_aux.u = ubicacio(-1,0,0);
          	_ct.elimina(est_am[i][jj][kk]);
          	//std::cout << mat1 << '\n';
          	_area_espera.push_back(est_am[i][jj][kk]);
          	_ct.assig(est_am[i][jj][kk], co_aux);
        }
        est_am[i][jj][kk]="";
        
  		recorrido_izq(i, jj, ++kk, jinici, kinici);
  		recorrido_izq(i, --jj, kinici+1, jinici, kinici);
  		//std::cout << est_am[i][jj][kk] << '\n';
  		
  	}
}
          
void terminal::reorganitza(nat actuals)
{
	//
	//

	int i = 0;
	for (list<string>::iterator it=_area_espera.begin(); it!=_area_espera.end() and i < actuals; it++, i++)
	{
		list<string>::iterator it2 = it;
		_area_espera.push_back(*it);
		_area_espera.erase(it2);
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
    Cu co_aux;
    ubicacio aem = ubicacio(-1,0,0);
    //std::cout << co_ub.c.matricula() << '\n';
    //std::cout << co_ub.u.filera() << co_ub.u.placa() << co_ub.u.pis() << '\n';

    // CD1 : Está en el área de espera
    if (co_ub.u == aem)
    {
      _ct.elimina(co_ub.c.matricula());
      _area_espera.remove(co_ub.c.matricula());
    }
    else
    {
      // Está en el área de almacenaje
      int i = co_ub.u.filera();
      int j = co_ub.u.placa();
      int k = co_ub.u.pis();

      nat longi = co_ub.c.longitud() / 10;
      // CD1: Sólo tiene una ubicación, por tanto, sólo hay que mirar si hay algo debajo de esa
      if (longi == 1)
      {
        if (k == _h-1)  // CD1.1: Estamos en arriba del todo,
        {
            _ct.elimina(co_ub.c.matricula());
            opsgrua++;

            if (_h == 1) est_am[i][j][k] = "___";
            else est_am[i][j][k] = "";

            actualitza_lliures(_head);

        }
        // CD1.2:NO hay nada encima
        else if (k != _h-1 and est_am[i][j][k+1] == "")
        {
          _ct.elimina(co_ub.c.matricula());
          opsgrua++;

          if (_h == 1) est_am[i][j][k] = "___";
          else est_am[i][j][k] = "";

          actualitza_lliures(_head);
        }
        // CD1.2: Hay contenedores encima
        else
        {
          //int cont_der = 0;
          //int cont_izq = 0;
          int jder = j;
          int jizq = j;
          // Traslado hacia la parte más derecha de encima del contenedor
          while (jder+1 <= _m-1 and est_am[i][jder][k+1] == est_am[i][jder+1][k+1])
          {
          		jder++;
          		
          }
          // Traslado hacia la parte más izq de encima del contenedor
          while (jizq-1 >= 0 and est_am[i][jizq][k+1] == est_am[i][jizq-1][k+1])
          {
          		jizq--;
          		
          }
          // PRUEBA RECURSIVIDAD: ii filera actual, jj = posición de encima del contenedor a retirar, k piso actual del contenedor a retirar
          nat actuals = _area_espera.size();
          recorrido_der(i, jder, k+1, j, k);

          recorrido_izq(i, j, k+1, jizq, k);
          reorganitza(actuals);

          if (_h == 1) est_am[i][j][k] = "___";
          else est_am[i][j][k] = "";
          _ct.elimina(co_ub.c.matricula());
          actualitza_lliures(_head);
          // PARTE ITERATIVA QUE FUNCIONA AUNQUE INSERTA EN DESORDEN:
          /*while (kk+1 < _h) // Mientras no llegue arriba del todo
          {

            string mat1 = est_am[i][j][kk+1];
            int ii = 0;
            int jj = j;
            if (mat1 != "")
            {
              // PRIMERA ITERACIÓN (O VARIAS, DEPENDE DE SI SOLO SE RETIRA ESTRICTAMENTE EL DE ENCMA)
              if (cont_izq == 0 and cont_der == 0)
              {
                while(jj+1 <= _m-1 and mat1 == est_am[i][jj+1][kk+1])
                {
                  cont_der++;
                  jj++;
                }
                jj = j;
                while (jj-1 >= 0 and mat1 == est_am[i][jj-1][kk+1])
                {
                  //std::cout << est_am[i][jj-1][kk+1] << "\n\n";
                  cont_izq++;
                  jj--;
                }
                if (_ct.existeix(mat1) and _ct[mat1].u != aem)
                {
                  	co_aux.c = _ct[mat1].c;
                  	co_aux.u = aem;
                  	_ct.elimina(mat1);
                  	//std::cout << mat1 << '\n';
                  	_area_espera.push_back(mat1);
                  	_ct.assig(mat1, co_aux);
                }
                
                opsgrua++;
                jj = j;

                while (ii <= cont_der-1)
                {
                  est_am[i][jj+1][kk+1]="";
                  ii++;
                  jj++;
                }
                ii = 0;
                jj = j;
                while (ii <= cont_izq-1)
                {
                  est_am[i][jj-1][kk+1]="";
                  ii++;
                  jj--;
                }
                jj = j;
                est_am[i][jj][kk+1]="";
              }
              // EN LA PRIMERA ITERACIÓN SE HAN RETIRADO VARIOS DEL LADO
              else
              {
                //std::cout << "Cont izq " << cont_izq << " y cont der: " << cont_der << '\n';
                ii = 0;
                jj = j+1;
                while (ii <= cont_der-1)
                {

		            if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
		            {
		              	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
		              	co_aux.u = aem;
		              	_ct.elimina(est_am[i][jj][kk+1]);
		              	//std::cout << est_am[i][jj][kk+1] << '\n';
		              	_area_espera.push_back(est_am[i][jj][kk+1]);
		              	_ct.assig(est_am[i][jj][kk+1], co_aux);
		            }
                  //_area_espera.push_back(est_am[i][jj][kk+1]);
                  est_am[i][jj][kk+1]="";

                    jj++;
                    ii++;
                }
                jj = j;
                while (jj+1 <= _m-1 and est_am[i][jj][kk+1] == est_am[i][jj+1][kk+1])
                {
                  	if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
		            {
		              	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
		              	co_aux.u = aem;
		              	_ct.elimina(est_am[i][jj][kk+1]);
		              	//std::cout << est_am[i][jj][kk+1] << '\n';
		              	_area_espera.push_back(est_am[i][jj][kk+1]);
		              	_ct.assig(est_am[i][jj][kk+1], co_aux);
		            }
                  est_am[i][jj][kk+1]="";
                  cont_der++;
                }

                ii = 0;
                jj = j-1;
                while (ii <= cont_izq-1)
                {
                  	if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
		            {
		              	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
		              	co_aux.u = aem;
		              	_ct.elimina(est_am[i][jj][kk+1]);
		              	_area_espera.push_back(est_am[i][jj][kk+1]);
		              	//std::cout << est_am[i][jj][kk+1] << '\n';
		              	_ct.assig(est_am[i][jj][kk+1], co_aux);
		            }
                  est_am[i][jj][kk+1]="";

                  jj--;
                  ii++;
                }
                jj = j;

                while (jj-1 >= 0 and est_am[i][jj][kk+1] == est_am[i][jj-1][kk+1])
                {
                  	if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
		            {
		              	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
		              	co_aux.u = aem;
		              	_ct.elimina(est_am[i][jj][kk+1]);
		              	_area_espera.push_back(est_am[i][jj][kk+1]);
		              	//std::cout << est_am[i][jj][kk+1] << '\n';
		              	_ct.assig(est_am[i][jj][kk+1], co_aux);
		            }
                  est_am[i][jj][kk+1]="";
                  cont_izq++;
                }
                jj = j;
                	if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
		            {
		              	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
		              	co_aux.u = aem;
		              	_ct.elimina(est_am[i][jj][kk+1]);
		              	//std::cout << est_am[i][jj][kk+1] << '\n';
		              	_area_espera.push_back(est_am[i][jj][kk+1]);
		              	_ct.assig(est_am[i][jj][kk+1], co_aux);
		            }
                est_am[i][jj][kk+1]="";
              }
              //std::cout << "eyouuuuuu\n";
            }
            kk++;
          }
          //std::cout << _ct.existeix(co_ub.c.matricula()) << '\n';
          //std::cout << co_ub.c.matricula() << '\n';
          _ct.elimina(co_ub.c.matricula());
          //std::cout << _ct.existeix(co_ub.c.matricula()) << '\n';
          opsgrua++;
          if (kk==0) est_am[i][j][k]="___";
          else est_am[i][j][k]="";
          actualitza_lliures(_head);

          //_area_espera.sort();

        */}
      }
      // CD2: El contenedor a retirar tiene varias ubicaciones
      else
      {
        if (longi == 2)
        {
          //std::cout << co_ub.c.matricula() << '\n';
          int j2 = 0;
          if (est_am[i][j+1][k] == co_ub.c.matricula())         // Buscamos posicion adyacentes
          {
            j2 = j+1;
          }
          else if (est_am[i][j-1][k] == co_ub.c.matricula())
          {
            j2 = j-1;
          }
          // j y jj son las placas que ocupa nuestro contenedor a retirar
          if (j2 < j)
          {
          	int aux = j2;
          	j2 = j;
          	j = aux;
          }

          if (k+1 == _h)                                        // CD2.1: Estamos en el piso más alto
          {

            _ct.elimina(co_ub.c.matricula());
            opsgrua++;
              //std::cout << "Se retira " << co_ub.c.matricula() << '\n';
            if (_h == 1)
            {
              est_am[i][j][k] = "___";
              est_am[i][j2][k] = "___";
            }
            else
            {
              est_am[i][j][k] = "";
              est_am[i][j2][k] = "";
            }
            //int maxim = maxim_dos(j, jj);
            actualitza_lliures(_head);
          }
          else if (k+1 != _h and est_am[i][j][k+1] == "" and est_am[i][j2][k+1] == "")       // CD2.2: No hay nada encima
          {
            _ct.elimina(co_ub.c.matricula());
            opsgrua++;
            if (_h == 1)
            {
              est_am[i][j][k] = "___";
              est_am[i][j2][k] = "___";
            }
            else
            {
              est_am[i][j][k] = "";
              est_am[i][j2][k] = "";
            }
            //int maxim = maxim_dos(j, jj);
            actualitza_lliures(_head);

          }
          else
          // CD3: Hay contenedores encima
          {
          	int jizq = j;
          	int jder = j2;
          	
          // Traslado hacia la parte más derecha de encima del contenedor
          while (jder+1 <= _m-1 and est_am[i][jder][k+1] == est_am[i][jder+1][k+1])
          {
          		jder++;
          }
          // Traslado hacia la parte más izq de encima del contenedor
          while (jizq-1 >= 0 and est_am[i][jizq][k+1] == est_am[i][jizq-1][k+1])
          {
          		jizq--;
          		
          }
          nat actuals = _area_espera.size();
          recorrido_der(i, jder, k+1, j2, k);

          recorrido_izq(i, j, k+1, jizq, k);
          reorganitza(actuals);
          if (_h == 1)
          {
          		est_am[i][j][k] = "___";
          		est_am[i][j2][k] = "___";
          }
          else
          {
          	est_am[i][j][k] = "";
          	est_am[i][j2][k] = "";
          }
          _ct.elimina(co_ub.c.matricula());
          actualitza_lliures(_head);
           /* //int cont_der = 0;
            //int cont_izq = 0;
            int kk = k;

            while (kk+1 < _h) // Mientras no llegue arriba del todo
            {
              string mat1 = est_am[i][j][kk+1];
              string mat2 = est_am[i][j2][kk+1];
              //std::cout << "Encima1: " << mat1 << '\n';
              //std::cout << "Encima2: " << mat2 << '\n';
              int ii = 0;
              int jj = j;
              if (mat1 != "" or mat2 != "")
              {
                // PRIMERA ITERACIÓN (O VARIAS, DEPENDE DE SI SOLO SE RETIRA ESTRICTAMENTE EL DE ENCMA)
                if (cont_izq == 0 and cont_der == 0)
                {
                  while(jj+1 <= _m-1 and mat2 == est_am[i][jj+1][kk+1])
                  {
                    //std::cout << est_am[i][jj+1][kk+1] << '\n';
                    cont_der++;
                    jj++;
                  }
                  jj = j;
                  while (jj-1 >= 0 and mat1 == est_am[i][jj-1][kk+1])
                  {
                    //std::cout << est_am[i][jj-1][kk+1] << '\n';
                    cont_izq++;
                    jj--;
                  }
                  // VOLVEMOS A METER EN EL CATALOGO PERO CON LA NUEVA UBICACIÓN
                  // TAMBIÉN EN EL ÁREA DE ESPERA
                  if (_ct.existeix(mat1) and _ct[mat1].u != aem){
                  co_aux.c = _ct[mat1].c;
                  co_aux.u = aem;
                  _ct.elimina(mat1);
                  _area_espera.push_back(mat1);

                  _ct.assig(mat1, co_aux);
              		}
                  if (_ct.existeix(mat2) and _ct[mat2].u != aem){	
                  	co_aux.c = _ct[mat2].c;
                  	co_aux.u = aem;
                  	_ct.elimina(mat2);
                  	_area_espera.push_back(mat2);
                  	_ct.assig(mat2, co_aux);
                  }
                  
                  if (mat1 == mat2) opsgrua++;
                  else
                  {
                    opsgrua+=2;
                  }

                  jj = j;

                  while (ii <= cont_der-1)
                  {
                    est_am[i][jj+1][kk+1]="";
                    ii++;
                    jj++;
                  }
                  ii = 0;
                  jj = j;
                  while (ii <= cont_izq-1)
                  {
                    est_am[i][jj-1][kk+1]="";
                    ii++;
                    jj--;
                  }
                  jj = j;
                  est_am[i][jj][kk+1]="";
                }
                // EN LA PRIMERA ITERACIÓN SE HAN RETIRADO VARIOS DEL LADO
                else
                {
                  ii = 0;
                  jj = j+1;
                  while (ii <= cont_der-1)
                  {
                  	if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    //_area_espera.push_back(est_am[i][jj][kk+1]);
                    est_am[i][jj][kk+1]="";

                      jj++;
                      ii++;
                  }

                  jj = j;
                  while (jj+1 <= _m-1 and est_am[i][jj][kk+1] == est_am[i][jj+1][kk+1] and est_am[i][jj][kk+1] != "")
                  {
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    est_am[i][jj][kk+1]="";
                    cont_der++;
                  }
                  opsgrua++;
                  ii = 0;
                  jj = j-1;
                  while (ii <= cont_izq-1)
                  {
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    est_am[i][jj][kk+1]="";

                    jj--;
                    ii++;
                  }
                  jj = j;

                  while (jj-1 >= 0 and est_am[i][jj][kk+1] == est_am[i][jj-1][kk+1] and est_am[i][jj][kk+1] != "")
                  {
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    est_am[i][jj][kk+1]="";
                    cont_izq++;
                  }
                  jj = j;
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                  est_am[i][jj][kk+1]="";
                }
              }
              kk++;
            }
            _ct.elimina(co_ub.c.matricula());
            opsgrua++;
            if (kk==0){
              est_am[i][j][k]="___";
              est_am[i][j2][k]="___";
            }
            else{
              est_am[i][j][k]="";
              est_am[i][j2][k]="";
            }
            actualitza_lliures(_head);
            //_area_espera.sort();
          */}
      }
      // LONGITUD 3
      if (longi == 3)
      {
        int j3 = 0;
        int j2 = 0;

        if (j+2 != _m and est_am[i][j][k] == est_am[i][j+2][k]) j3 = j+2;
        else if (j+1 != _m and est_am[i][j][k] == est_am[i][j+1][k]) j3 = j+1;
        else if (j+1 != _m) j3 = j;
        // CD3.1: Estamos en el piso más alto
        j2 = j3-1;
        j = j2-1;

        if (k+1 == _h)
        {
          _ct.elimina(co_ub.c.matricula());
          opsgrua++;
          if (_h == 1)
          {
            est_am[i][j3][k] = "___";
            est_am[i][j2][k] = "___";
            est_am[i][j][k] = "___";
          }
          else
          {
            est_am[i][j3][k] = "";
            est_am[i][j2][k] = "";
            est_am[i][j][k] = "";
          }

          // Buscar centinela
          //int maxim = maxim_tres(jj-2, jj-1, jj);
          actualitza_lliures(_head);
        }
         // CD2.2: No hay nada encima
        else if (est_am[i][j][k+1] == "" and est_am[i][j2][k+1] == "" and est_am[i][j3][k+1] == "")
        {

          _ct.elimina(co_ub.c.matricula());
          opsgrua++;
          if (_h == 1)
          {
            est_am[i][j3][k] = "___";
            est_am[i][j2][k] = "___";
            est_am[i][j][k] = "___";
          }
          else
          {
            est_am[i][j3][k] = "";
            est_am[i][j2][k] = "";
            est_am[i][j][k] = "";
          }

          //int maxim = maxim_tres(jj-2, jj-1, jj);
          actualitza_lliures(_head);

        }
        // CD3: Hay contenedores encima
        else
        {
        	int jizq = j;
        	//int jcen = j2;
          	int jder = j3;
          	
          // Traslado hacia la parte más derecha de encima del contenedor
          while (jder+1 <= _m-1 and est_am[i][jder][k+1] == est_am[i][jder+1][k+1])
          {
          		jder++;
          }
          // Traslado hacia la parte más izq de encima del contenedor
          while (jizq-1 >= 0 and est_am[i][jizq][k+1] == est_am[i][jizq-1][k+1])
          {
          		jizq--;
          		
          }
          //jcen=jder-1;
          nat actuals = _area_espera.size();
          recorrido_der(i, jder, k+1, j2, k);

          recorrido_izq(i, j, k+1, jizq, k);       
          reorganitza(actuals);

          if (_h == 1)
          {
          		est_am[i][j][k] = "___";
          		est_am[i][j2][k] = "___";
          		est_am[i][j3][k] = "___";
          }
          else
          {
          	est_am[i][j][k] = "";
          	est_am[i][j2][k] = "";
          	est_am[i][j3][k] = "";
          }
          _ct.elimina(co_ub.c.matricula());
          actualitza_lliures(_head);
        	/*
            int cont_der = 0;
            int cont_izq = 0;
            int kk = k;

            while (kk+1 < _h) // Mientras no llegue arriba del todo
            {
              string mat1 = est_am[i][j][kk+1];
              string mat2 = est_am[i][j2][kk+1];
              string mat3 = est_am[i][j3][kk+1];
              //std::cout << "Encima1: " << mat1 << '\n';
              //std::cout << "Encima2: " << mat2 << '\n';
              //std::cout << "Encima3: " << mat3 << '\n';
              int ii = 0;
              int jj = j;
              int jjj = j3;
              if (mat1 != "" or mat2 != "" or mat3 != "")
              {
                // PRIMERA ITERACIÓN (O VARIAS, DEPENDE DE SI SOLO SE RETIRA ESTRICTAMENTE EL DE ENCMA)
                if (cont_izq == 0 and cont_der == 0)
                {
                  while(jjj+1 <= _m-1 and mat3 == est_am[i][jjj+1][kk+1])
                  {
                    //std::cout << est_am[i][jj+1][kk+1] << '\n';
                    cont_der++;
                    jjj++;
                  }
                  jj = j;
                  while (jj-1 >= 0 and mat1 == est_am[i][jj-1][kk+1])
                  {
                    //std::cout << est_am[i][jj-1][kk+1] << '\n';
                    cont_izq++;
                    jj--;
                  }
	              if (_ct.existeix(mat1) and _ct[mat1].u != aem){
                  co_aux.c = _ct[mat1].c;
                  co_aux.u = aem;
                  _ct.elimina(mat1);
                  _area_espera.push_back(mat1);

                  _ct.assig(mat1, co_aux);
              		}

                  
                   if (_ct.existeix(mat2) and _ct[mat2].u != aem){	
                  	co_aux.c = _ct[mat2].c;
                  	co_aux.u = aem;
                  	_ct.elimina(mat2);
                  	_area_espera.push_back(mat2);
                  	_ct.assig(mat2, co_aux);
                  }

                   if (_ct.existeix(mat3) and _ct[mat3].u != aem){	
                  	co_aux.c = _ct[mat3].c;
                  	co_aux.u = aem;
                  	_ct.elimina(mat3);
                  	_area_espera.push_back(mat3);
                  	_ct.assig(mat3, co_aux);
                  }
                  
                  jj = j;
                  jjj = j3;
                  while (ii <= cont_der-1)
                  {
                    est_am[i][jjj+1][kk+1]="";
                    ii++;
                    jjj++;
                  }
                  ii = 0;
                  jj = j;
                  jjj = j3;
                  while (ii <= cont_izq-1)
                  {
                    est_am[i][jj-1][kk+1]="";
                    ii++;
                    jj--;
                  }
                  jj = j;
                  est_am[i][j][kk+1]="";
                  est_am[i][j2][kk+1]="";
                  est_am[i][j3][kk+1]="";
                }
                // EN LA PRIMERA ITERACIÓN SE HAN RETIRADO VARIOS DEL LADO
                else
                {
                  ii = 0;
                  jj = j3+1;
                  while (ii <= cont_der-1)
                  {
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    est_am[i][jj][kk+1]="";

                      jj++;
                      ii++;
                  }
                  jj = j3;
                  while (jj+1 <= _m-1 and est_am[i][jj][kk+1] == est_am[i][jj+1][kk+1] and est_am[i][jj][kk+1]!="")
                  {
                    //std::cout << "Recorro esquina derecha: " << est_am[i][jj][kk+1] << '\n';
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    est_am[i][jj][kk+1]="";
                    cont_der++;
                  }

                  ii = 0;
                  jj = j-1;
                  while (ii <= cont_izq-1)
                  {
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    est_am[i][jj][kk+1]="";

                    jj--;
                    ii++;
                  }
                  jj = j;

                  while (jj-1 >= 0 and est_am[i][jj][kk+1] == est_am[i][jj-1][kk+1] and est_am[i][jj][kk+1] != "")
                  {
                    //std::cout << "Recorro esquina izqa: " << est_am[i][jj][kk+1] << '\n';
                    if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                    est_am[i][jj][kk+1]="";
                    cont_izq++;
                  }
                  jj = j;
                  if (_ct.existeix(est_am[i][jj][kk+1]) and _ct[est_am[i][jj][kk+1]].u != aem)
                  	{	
	                  	co_aux.c = _ct[est_am[i][jj][kk+1]].c;
	                  	co_aux.u = aem;
	                  	_ct.elimina(est_am[i][jj][kk+1]);
	                  	_area_espera.push_back(est_am[i][jj][kk+1]);
	                  	_ct.assig(est_am[i][jj][kk+1], co_aux);
                  	}
                  opsgrua++;
                  est_am[i][jj][kk+1]="";
                }
              }
              kk++;
            }
            _ct.elimina(co_ub.c.matricula());
            opsgrua++;
            if (kk==0){
              est_am[i][j][k]="___";
              est_am[i][j2][k]="___";
              est_am[i][j3][k]="___";
            }
            else{
              est_am[i][j][k]="";
              est_am[i][j2][k]="";
              est_am[i][j3][k]="";
            }
            actualitza_lliures(_head);
            //_area_espera.unique();

          //_ct.elimina(co_ub.c.matricula());

          if (_h == 1)
          {
            est_am[i][j3][k] = "___";
            est_am[i][j2][k] = "___";
            est_am[i][j][k] = "___";
          }
          else
          {
            est_am[i][j3][k] = "";
            est_am[i][j2][k] = "";
            est_am[i][j][k] = "";
          }*/

          //int maxim = maxim_tres(jj-2, jj-1, jj);
          //actualitza_lliures(_head);

        }
      }
    }
    //_area_espera.remove("");
    //_area_espera.remove("___");
    //_area_espera.unique();
    //std::cout << "ey\n";
    recorre(1);
  }
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
  //std::cout << co_ub.c.matricula() << " " << co_ub.c.longitud() << '\n';
  //std::cout << co_ub.u.filera() << co_ub.u.placa() << co_ub.u.pis() << '\n';

  bool exis_aem = false;
  bool reinsercio = false;
  //std::cout << co_ub.c.matricula() << '\n';

  if (_ct.existeix(co_ub.c.matricula()))
  {
    if (_ct[co_ub.c.matricula()].u == ubicacio(-1,0,0)) exis_aem = true;
    //exis_aem = revisa(co_ub.c.matricula(), exis_aem);
  }
  else {
    exis_aem = false;
  }

  if (not _ct.existeix(co_ub.c.matricula()) or exis_aem)
  {

    nat longi = co_ub.c.longitud() / 10;
    bool trobat = false;    // Se pone a TRUE para indicar que hemos llegado al final del área de almacenaje
    node *p = _head;
    node* inici;
    node* p2 = NULL;

    // El contenedor necesita más ubicaciones que tanaño del área de almacenaje
    if (longi > _size)
    {
      trobat = true;
    }
    // RECORRIDO EN BUSCA DE UBICACIONES PARA LOS CONTENEDORES

    // EL CONTENEDOR ES DE 10 PIES
    if (longi == 1)
    {

      while (not trobat and p->_lliu != true)      // Busca una libre
      {
        if (p->_seg == NULL) trobat = true;
        else  p = p->_seg;
      }
      if (not trobat)
      {
        if (_ct.existeix(co_ub.c.matricula()))
        {
          //std::cout << "Eliminado antes de reinsertarse\n";
          //_area_espera.remove(co_ub.c.matricula());
          _ct.elimina(co_ub.c.matricula());
          reinsercio = true;
        }
        co_ub.u = p->_u;
        _ct.assig(co_ub.c.matricula(), co_ub);      // Asigna al catálogo el contenedor
        //std::cout << "Long 1: Se inserta " << co_ub.c.matricula() << '\n';
        p->_lliu = false;                           // Marca la ubicación como ocupada

        // Se inserta contenedor en su ubicación del área de almacenaje (matriz)
        int i = p->_u.filera();
        int j = p->_u.placa();
        int k = p->_u.pis();
        est_am[i][j][k] = co_ub.c.matricula();
        //std::cout << "10 PIES: Ub del mapa almacenaje:   " << est_am[i][j][k] << '\n';
        //std::cout << "Colocado en " << p->_u.filera() << " " << p->_u.placa() << " " << p->_u.pis() << "\n";
        opsgrua++;
        trobat = true;
        actualitza_lliures(_head);
        //actualitza_lliures(_head, j);
        if (not reinsercio) recorre(1);
      }
    }
    // EL CONTENEDOR ES DE 20 o 30 PIES
    else
    // BÚSQUEDA DEL PRIMER HUECO LIBRE VÁLIDO, FUERA DEL BUCLE
    {
      //std::cout << co_ub.c.matricula() << '\n';
      while (p != NULL and not trobat and p->_lliu != true)      // Busca la primera libre
      {
        if (p->_seg == NULL) trobat = true;
        p = p->_seg;
      }
      if (not trobat)
      {
        inici = p;
        int filera_act = inici->_u.filera();
        int pis_act = inici->_u.pis();

        nat i = 1;      // Ha encontrado el primero (inici)

        while (p != NULL and i < longi and not trobat)
        {

            if (i == 0)           // Buscamos el primer libre de nuevo, en caso de que la primera búsqueda haya sido mala
            {
              if (p->_seg != NULL){

                p = inici->_seg;

              }
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

              //if (p->_seg == NULL) trobat = true;
              //else
              //{
                if (inici->_u.pis() != 0 or p->_u.pis() != 0)
                {

                  if (p2 != NULL)
                  {
                    if (p->_ant->_lliu == true or inici->_ant->_lliu == true or p2->_ant->_lliu == true) i = 0;
                  }
                  if ((p->_ant->_lliu == true or inici->_ant->_lliu == true) and p->_u.filera() == filera_act){

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
              //}
            }
          }
      }
    }
    // FIN DEL BUCLE -- INSERCIONES

    // TROBAT = FALSE -> SE AÑADE AL CATALEG Y AL AREA DE ALMACENAJE
    if (not trobat)
    {
        // 1- Insercions al catàleg de contenidors:
        // Contenidor de 20 peus -> Insercions al catàleg d'ubicacions
      if (longi == 2)
      {
        if (_ct.existeix(co_ub.c.matricula()))
        {
          //std::cout << "Eliminado antes de reinsertarse\n";
          //_area_espera.remove(co_ub.c.matricula());
          _ct.elimina(co_ub.c.matricula());
          reinsercio = true;
        }

        int i = inici->_u.filera();
        int j = inici->_u.placa();
        int k = inici->_u.pis();
        est_am[i][j][k] = co_ub.c.matricula();
        co_ub.u = inici->_u;
        _ct.assig(co_ub.c.matricula(), co_ub);
        //std::cout << "Long 2: Se inserta " << co_ub.c.matricula() << '\n';

        i = p2->_u.filera();
        j = p2->_u.placa();
        k = p2->_u.pis();
        est_am[i][j][k] = co_ub.c.matricula();
        co_ub.u = p2->_u;

        opsgrua++;

        inici->_lliu = false;
        p2->_lliu = false;
        actualitza_lliures(_head);
        if (not reinsercio) recorre(1);
      }
      // Contenidor 30 peus -> Insercions al catàleg de contenidors
      else if (longi == 3)
      {
        while (_ct.existeix(co_ub.c.matricula()))
        {
          //_area_espera.remove(co_ub.c.matricula());
          _ct.elimina(co_ub.c.matricula());
          reinsercio = true;
        }
        co_ub.u = inici->_u;
        _ct.assig(co_ub.c.matricula(), co_ub);
        //std::cout << "Long 3: Se inserta " << co_ub.c.matricula() << '\n';
        co_ub.u = p2->_u;
        //_ct.assig(co_ub.c.matricula(), co_ub);
        co_ub.u = p->_u;
        //_ct.assig(co_ub.c.matricula(), co_ub);

        int i = inici->_u.filera();
        int j = inici->_u.placa();
        int k = inici->_u.pis();
        est_am[i][j][k] = co_ub.c.matricula();

        i = p2->_u.filera();
        j = p2->_u.placa();
        k = p2->_u.pis();
        est_am[i][j][k] = co_ub.c.matricula();


        i = p->_u.filera();
        j = p->_u.placa();
        k = p->_u.pis();
        est_am[i][j][k] = co_ub.c.matricula();

        opsgrua++;

        inici->_lliu = false;
        p2->_lliu = false;
        p->_lliu = false;
        actualitza_lliures(_head);
        if (not reinsercio) recorre(1);
      }
    }
    // TROBAT = TRUE -> DIRECTO AL CATALEG Y AL ÁREA DE ESPERA
    else
    {
        if (not _ct.existeix(co_ub.c.matricula()))
        {
          //std::cout << "meto en aem\n";
          ubicacio u_ae(-1,0,0);
          co_ub.u = u_ae;
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

//
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
  // POST: Crea una terminal vàlida, y una llista enllaçada d'ubicacions lliures
  //       Retorna un error en cas contrari

  if ((_n != 0) and (_m != 0) and (_h != 0) and (_h <= HMAX) and (st == FIRST_FIT || st == LLIURE))
  {

    _n = n;
    _m = m;
    _h = h;

    inicialitza_am(_n, _m, _h);
    crea_llista_lliures(_n, _m, _h);

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

//
terminal& terminal::operator=(const terminal& b) throw(error)
{
  // PRE:
  // POST:

  if (this != &b)
  {
    terminal t(b);        // t es una copia de b

    node* _arrel = _head;
    _head = t._head;
    t._head = _arrel;

    _ct = t._ct;
    _area_espera = t._area_espera;
  }

  return *this;
}

//
terminal::~terminal() throw()
{
  // PRE:
  // POST:

  borra_llista_lliures(_head);
  borra_am();
  //_ct.~cataleg();

  _area_espera.clear();

}

void terminal::insereix_contenidor(const contenidor &c) throw(error)
{
  Cu co_ub;
  co_ub.c = c;
  //std::cout << "Cambio de información en el objeto co_ub, campo c: " << co_ub.c.matricula() << '\n';
  if (_st == FIRST_FIT) insereix_ff(co_ub);
  //std::cout << '\n' << "Elements ara mateix al catàleg: " << _ct.quants() << '\n';
  //if (_st == LLIURE) insereix_ll(c);

}

//
void terminal::retira_contenidor(const string &m) throw(error)
{
  // PRE:
  // POST:
  if (_st == FIRST_FIT) retira_ff(m);

}

//
ubicacio terminal::on(const string &m) const throw()
{
  // PRE:
  // POST:

  ubicacio u = ubicacio(-1,-1,-1);

  if (_ct.existeix(m))
  {
      u = _ct[m].u;
      if (u != ubicacio(-1,0,0))
      {
        int i = u.filera();
        int j = u.placa();
        int k = u.pis();
        string mat = est_am[i][j][k];
        bool trobat = false;
        while (not trobat)
        {
          if (j-1 >= 0)
          {
            if (est_am[i][j-1][k] == mat) j--;
            else trobat = true;
          }
          else trobat = true;
        }
        u = ubicacio(i,j,k);
      }
  }

  return u;
}

//
nat terminal::longitud(const string &m) const throw(error)
{
  // PRE:
  // POST:

  if (_ct.existeix(m))
  {
    contenidor c = _ct[m].c;
    nat lon = c.longitud();
    return lon;
  }
  else
  {
    throw error(MatriculaInexistent);
  }
}

//
void terminal::contenidor_ocupa(const ubicacio &u, string &m) const throw(error)
{
  // PRE:
  // POST:

  int i = u.filera();
  int j = u.placa();
  int k = u.pis();
  //std::cout << "dimesiones terminal: " << _n << " X " << _m << " X " << _h << " ";
  //std::cout << "ubicacion actual: " << i << " X " << j << " X " << k << " ";
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

//
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

//
nat terminal::ops_grua() const throw()
{
  // PRE:
  // POST:

  return opsgrua;
}

//
void terminal::area_espera(list<string> &l) const throw()
{
  // PRE:
  // POST:

  //std::cout << l.size() << '\n';
  l = _area_espera;
  l.sort();
  // TO DO :
  // Método de ordenación de la lista l (merge preferiblemente)
  // Bucle que recorra con un iterador desde l.begin hasta l.end y printee los elementos de la lista l
  /*for (list<string>::iterator it=l.begin(); it!=l.end(); it++)
  {
    std::cout << *it << '\n';
  }*/
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
