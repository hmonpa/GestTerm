# GestTerm
Projecte d'ESIN tardor de 2020

## Enunciat del projecte
El propòsit d’aquest projecte és desenvolupar un sistema de gestió automàtica d’una
terminal de contenidors de càrrega.
Típicament, una terminal de contenidors de càrrega és una gran esplanada organitzada
en N fileres de contenidors, cadascuna de les quals té M places. En el problema
que ens ocupa les places són rectangulars, de 10 peus de llarg per 6 d’amplada i es
situen una darrera l’altra formant una filera. Entre les fileres hi ha passadissos per permetre
la circulació de les grues encarregades de col·locar i retirar els contenidors. Les
N fileres constitueixen el que s’anomena àrea d’emmagatzematge. A més hi sol haver una
àrea d’espera i maniobres on es poden emmagatzemar temporalment un petit nombre de
contenidors (en aquest projecte, i per simplificar, no es limitarà la capacitat d’aquesta
àrea).
Tots els contenidors tenen la mateixa amplada i alçada, però la seva longitud és diferent:
n’hi ha de 10, 20 i 30 peus. Això significa que un contenidor ocuparà l’equivalent
a una, dues o tres places en funció de la seva longitud. Per altra banda, els contenidors
es poden apilar uns sobre els altres, sempre i quan la totalitat de la seva base toqui el
terra o contenidors en el “pis” inferior. Ateses les limitacions de pes que poden suportar
els contenidors i a les limitacions de les grues de la terminal, només es poden apilar fins
un màxim de H pisos. Habitualment, H = 3 i mai serà més gran que una certa constant
petita HMAX.
Les fileres de la terminal es numeren de 0 a N-1, i les places dins d’una filera es
numeren de 0 a M-1. Els nivells o pisos es numeren de 0 a H-1. Una ubicació és una
terna d’enters <i, j, k> que designen, respectivament, un número de filera, un número de
plaça i un pis. La ubicació d’un contenidor és la terna corresponent a la plaça ocupada
pel contenidor amb menor j (cal recordar que els contenidors de 20 i 30 peus ocupen 2
i 3 places, respectivament). S’usa el mateix conveni per la ubicació d’un forat o grup de
places consecutives lliures.
Quan un contenidor nou arriba a la terminal, el sistema ha de trobar-li un lloc apropiat
en l’àrea d’emmagatzematge, automàticament. Si existeixen diversos espais lliures
en els que es pot posar el nou contenidor, necessitarem una estratègia per decidir quin
d’ells triar, intentant que:
1. Sigui senzilla d’implementar i l’algorisme corresponent sigui eficient en temps d’execució
i en espai de memòria;
2. Aprofiti de la millor manera possible l’espai de la terminal, minimitzant la fragmentació

<img src="https://github.com/hmonpa/Gestterm/blob/master/Filera.PNG" width="500">

## Estratègia FIRST_FIT
L’estratègia FIRST_FIT estableix:
1. Quin forat triar per un contenidor si hi ha diverses possibilitats
2. Com gestionar l’àrea d’espera.

Durant una inserció, si existeixen diversos llocs lliures possibles pel nou contenidor,
FIRST_FIT tria el “primer” d’ells en un hipotètic recorregut per la terminal de filera en
filera, plaça a plaça, és a dir, preferentment s’usarà el forat amb el número de filera més
baix i en cas d’empat, el forat amb el número de plaça més baix (no pot haver dos forats
amb igual nombre de filera i de plaça). Aquesta característica concreta de l’algorisme és
la que dóna nom a l’estratègia.
Si no hi hagués cap forat capaç d’albergar al contenidor, FIRST_FIT no pretén realitzar
cap reorganització dels contenidors de l’àrea d’emmagatzematge, i es limita a posar
el nou contenidor en l’àrea d’espera.
FIRST_FIT gestiona l’àrea d’espera com una llista de contenidors on els últims contenidors
inserits a la llista són els que s’intenten recol·locar en primer lloc. Cada vegada
que l’operació insereix_contenidor té èxit aconseguint col·locar el nou contenidor
en l’àrea d’emmagatzematge es recorrerà l’àrea d’espera, començant pel que es va inserir
en últim lloc en l’àrea d’espera, en busca d’un contenidor que pugui ser mogut a
l’àrea d’emmagatzematge. Si la cerca té èxit s’utilitza la regla FIRST_FIT per moure
el contenidor en qüestió des de l’àrea d’espera a la d’emmagatzematge. Aquest procés
es repeteix fins que l’àrea d’espera s’ha examinat completament sense que hi hagi cap
contenidor susceptible de ser mogut a l’àrea d’emmagatzematge.
En quant a l’operació de retira_contenidor, l’estratègia FIRST_FIT es limita
a treure els contenidors que hi hagi per damunt del contenidor a retirar (si n’hi ha),
posant-los a l’àrea d’espera. Després es recorre la llista intentant reubicar contenidors
de l’àrea d’espera a la d’emmagatzematge.

## Disseny modular
<img src="https://github.com/hmonpa/Gestterm/blob/master/DissenyModular.PNG" width="500">

GestTerm consta d’una classe anomenada terminal que és l’encarregada de la gestió
del terminal. Per tal de realitzar aquesta tasca, aquesta classe usa aquestes altres classes:
ubicacio, contenidor i cataleg.

## Classe ubicació

Una ubicació consisteix en una terna de números <i, j, k> corresponents a un número
de filera, número de plaça i pis. Hi ha valors del tipus ubicacio que tenen significats
especials: <-1, 0, 0> s’usa per indicar que el contenidor corresponent
està a l’àrea d’espera i <-1, -1, -1> per indicar que un contenidor no està a la terminal.

## Classe contenidor

Tot objecte de la classe contenidor té una matrícula o identificador format per una seqüència
de lletres majúscula i dígits (p.e. AX02B7Z8) i una longitud (10, 20 o 30 peus). En
una aplicació real es disposaria de més informació per cada contenidor: data d’entrada a
la terminal, data prevista de sortida, descripció de la càrrega, pes, dades d’identificació
del propietari, dades de procedència, model del contenidor, etc., però en aquest projecte
prescindirem d’ella.
Decisions sobre les dades:
  - Les matrícules es representaran mitjançant strings. La longitud de l’string
no està limitada, però una matrícula no serà vàlida si l’string que la representa
conté caràcters que no siguin lletres majúscules o dígits, o és l’string buit. Totes
les lletres que formen la matrícula són lletres majúscules de l’A a la Z, excloses la
Ñ, la Ç i les vocals accentuades.
  - La longitud del contenidor es representa mitjançant un enter positiu (unsigned int).
  
## Classe catàleg

Un objecte de la clase cataleg representa una col·lecció de parells <Clau, Valor> en la
que no hi ha dos parells amb igual clau.
En el mètode constructor d’aquesta classe s’indica el nombre d’elements que com a
màxim s’inseriran en el catàleg. En qualsevol cas el catàleg ha de permetre emmagatzemar
elements més enllà del màxim indicat.
Decisions sobre les dades:
  - Les claus són strings no buits.
  - El tipus Valor ha de disposar del constructor per còpia i de l’operador assignació.
  
## Classe terminal
 
Aquesta classe ofereix dues operacions “principalment”: insereix_contenidor i
retira_contenidor, a més d’un cert nombre de mètodes auxiliars i el mètode constructor.
La primera d’aquestes operacions s’utilitza quan un nou contenidor arriba a la
terminal i la segona quan un contenidor abandona la terminal.
Per retirar un contenidor c (que està a l’àrea d’emmagatzematge) s’han de moure a
l’àrea d’espera tots els contenidors que estiguin sobre el contenidor en qüestió, eliminar
el contenidor c de la terminal i intentar recol·locar contenidors de l’àrea d’espera a l’àrea
d’emmagatzematge. Si el contenidor que es vol retirar està a l’àrea d’espera, s’elimina
de manera immediata.
Decisions sobre les dades:
  - HMAX és una constant.
  - Les operacions on i longitud reben un string qualsevol com a paràmetre.
  
Atès que les operacions insereix_contenidor i retira_contenidor poden
involucrar el moviment de varis contenidors des de l’àrea d’espera a l’àrea d’emmagatzematge,
resultaria ineficient o molt complicat garantir que la o les estructures de
dades que representen la terminal quedin en el seu estat original si es produís un error
de memòria dinàmica durant un pas intermedi de l’execució. Per aquesta raó, i
excepcionalment, la vostra implementació d’aquests dos mètodes no tindrà que deixar
necessàriament inalterada l’estructura de dades en el cas que es produeixi un error de
manca de memòria dinàmica. Si es produeix algun altre tipus d’error, la regla sí que
s’aplica, ja que aquests altres errors poden ser fàcilment verificats abans de procedir a la
modificació de l’estructura.

- Observacions: Es pot emprar la classe list de la biblioteca estàndard de C++, però únicament
per representar i gestionar l’àrea d’espera del terminal i implementar el mètode
area_espera.
