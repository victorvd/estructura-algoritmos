//B* TREE

#ifndef BPlusTree_H
#define BPlusTree_H

using namespace std;

template <class Tipo>
class BSTree;

template <class Tipo>
class HistorialNodo{
	friend class BSTree<Tipo>;
public:
	HistorialNodo(bool _operation, Tipo _data): operation(_operation), data(_data){};
	HistorialNodo();
private:
	bool operation;
	Tipo data;
};

template <class Tipo>
class Nodo{
	friend class BSTree<Tipo>;
public:
	Nodo(int _grado, bool _hoja, Nodo<Tipo>*P = NULL): 
		grado(_grado), numeroLlaves(0), hoja(_hoja), Padre(P) 
	{
		Llaves = new Tipo[grado];
		for(int i=0; i<grado; i++){
			Llaves[i] = 0;
		}
		Hijos = new Nodo<Tipo>*[grado+1];
		for(int i=0; i<grado+1; i++){
			Hijos[i] = NULL;
		}
	};
private:
	Tipo *Llaves;
	Nodo **Hijos;
	Nodo *Padre;
	int grado;
	int numeroLlaves;
	bool hoja;
	
};

template <class Tipo>
class Cola {
 
private:
	struct Nodo {
		Tipo elemento;
		Nodo* siguiente;  // coloca el nodo en la segunda posicion
	}* ultimo;
	unsigned int elementos;
 
public:
	Cola() {
		elementos = 0;
	}
 
	~Cola() {
		while (elementos != 0) Pop();
	}
 
	void Push(const Tipo& elem) {
		Nodo* aux = new Nodo;
		aux->elemento = elem;
		aux->siguiente = ultimo;
		ultimo = aux;
		++elementos;
	}
 
	void Pop() {
		Nodo* aux = ultimo;
		ultimo = ultimo->siguiente;
		delete aux;
		--elementos;
	}
 
	Tipo Cima() const {
		return ultimo->elemento;
	}
 
	bool Vacio() const {
		return elementos == 0;
	}
 
	unsigned int Altura() const {
		return elementos;
	}
};

template <class Tipo> 
class InfoPak{
	friend class BSTree<Tipo>;
	friend ostream &operator<< (ostream &COUT, InfoPak &W)
	{
	 COUT << "Indice: "<< W.indice;
	 COUT <<"Valor: "<< W.valor;
	 return COUT;
	}
public:
	InfoPak(int _indice, Tipo _valor, Nodo<Tipo>* _nodo ): indice(_indice), valor(_valor), nodo(_nodo){};
private:
	Nodo<Tipo> *nodo;
	int indice;
	Tipo valor;
};

template <class Tipo> 
class BSTree{
public:
	BSTree(int _grado): Raiz(NULL){	grado = _grado;}
	~BSTree(){ Vaciar(Raiz);}
	BSTree(const BSTree &);
	BSTree & operator=(const BSTree<Tipo> &);
	//Insercion
	void Insertar(Tipo);
	void Promover(InfoPak<Tipo>);
	void PromoverRaiz(Tipo);
	bool CuidameLlave(InfoPak<Tipo>);
	void PrestamoIzquierdaADerecha(InfoPak<Tipo>);
	void PrestamoDerechaAIzquierda(InfoPak<Tipo>);
	//Eliminacion
	bool Eliminar(Tipo _valor);
	void PrestameLlave(InfoPak<Tipo>);
	void Centraliza(InfoPak<Tipo> Pak);
	
	InfoPak<Tipo> MayorDeLosMenores(Nodo<Tipo>*, bool );
	InfoPak<Tipo> MenorDeLosMayores(Nodo<Tipo>*, bool );
	
	void Demotar(InfoPak<Tipo>);
	void DemotarRaiz();
	void RecorreDerecha(Nodo<Tipo>* );
	void RellenaVacio(Nodo<Tipo>* );
 
	//Impresion
	void Imprimir();
	void ImprimirNodo(Nodo<Tipo>*);
	void ImprimirInfoPak(InfoPak<Tipo>);
	void ImprimirHistorialNodo(HistorialNodo<Tipo>);
	//Utileria
	bool Buscar (Tipo);
	InfoPak<Tipo> Buscar(Tipo, int);
	bool EstaVacio(){ return !Raiz; }
	void CopiarNodo(Nodo<Tipo>*, Nodo<Tipo>*, int , int, int);

private:
	void Vaciar(Nodo<Tipo> *&);
	Nodo<Tipo> *Raiz;
	int grado;
	Cola<HistorialNodo<Tipo> > Historial;
};

template <class Tipo> 
BSTree<Tipo> & BSTree<Tipo>::operator=(const BSTree<Tipo> &W) //Sobrecarga Igual
{
	if(this!=&W){ 
		Vaciar(Raiz);
		grado = W.grado;
		Historial = W.Historial;
		HistorialNodo<Tipo> aux(0, W.Raiz->Llaves[0]);
		while(Historial.elementos != 0){
			aux = Historial.Pop();
			if(aux.operation) Eliminar(aux.data);
			else {
				Insertar(aux.data);
			}
			Historial.elementos--;
		}
		Historial = W.Historial;
		
	} 	
 return *this;
}

template <class Tipo> 
BSTree<Tipo>::BSTree(const BSTree<Tipo> &C): Raiz(NULL) //Constructor de Copias
{
	*this = C;
}

template <class Tipo>		
void BSTree<Tipo>::Insertar(Tipo _valor)
{
	
	if(Raiz){
		HistorialNodo<Tipo> HistNodo(0, _valor);
		Historial.Push(HistNodo); //Saving the operation in history
		
		cout<<"Insertando "<<_valor<<"...\n";
		Nodo<Tipo> *aux = Raiz;
		int i=0;
		while(!aux->hoja){
			if(aux->numeroLlaves == i || (aux->Llaves[i] > _valor)){
				aux = aux->Hijos[i];
				i = -1;
			}
			i++;
		} 
		i = 0;
		
		while(aux->numeroLlaves > i && aux->Llaves[i] < _valor )i++;
		int n = aux->numeroLlaves+1;
		while(n != i){
			aux->Llaves[n] = aux->Llaves[n-1];
			n--;
		}
		aux->Llaves[i] = _valor;
		aux->numeroLlaves++;
		
		InfoPak<Tipo> Pak (i, aux->Llaves[i], aux);
		if(aux->numeroLlaves == grado && aux != Raiz){
			if(!CuidameLlave(Pak)) Promover(Pak);
		}
		if(aux->numeroLlaves == aux->grado && aux == Raiz){
			PromoverRaiz(aux->Llaves[aux->grado/2]);
		}	
	}else{
		int gradoRaiz = (grado*2)/3;
		gradoRaiz = (gradoRaiz*2)+1;
		Raiz = new Nodo<Tipo>(gradoRaiz, 1);
		Insertar(_valor);
	}
}


template <class Tipo>	
void BSTree<Tipo>::CopiarNodo(Nodo<Tipo>* nuevo, Nodo<Tipo>* original, int startNew, int startOriginal, int stop)
//Copia de intervalos definibles para la copia y del objeto a copiar
{
	int i=0;
	while(i < stop){
		if(original->Llaves[startOriginal]!=0){
			nuevo->Llaves[startNew] = original->Llaves[startOriginal];
			nuevo->Hijos[startNew] = original->Hijos[startOriginal];
			if(nuevo->Hijos[startNew]) nuevo->Hijos[startNew]->Padre = nuevo;	
			nuevo->numeroLlaves++;
			original->Llaves[startOriginal] = 0;
			startNew++;
			i++;
		}
		startOriginal++;
	}
	nuevo->Hijos[startNew] = original->Hijos[startOriginal];
	if(nuevo->Hijos[startNew]) nuevo->Hijos[startNew]->Padre = nuevo;
}

template <class Tipo>		
void BSTree<Tipo>::Promover(InfoPak<Tipo> Pak)
{

	Nodo<Tipo>* Izq = new Nodo<Tipo>(grado, Pak.nodo->hoja); 
	Nodo<Tipo>* Der = new Nodo<Tipo>(grado, Pak.nodo->hoja); 
	Nodo<Tipo>* Mid = new Nodo<Tipo>(grado, Pak.nodo->hoja); 
	Nodo<Tipo>* papa = Pak.nodo->Padre;
	
	int i=0;
	while(papa->numeroLlaves > i && (papa->Llaves[i] < Pak.valor))i++;	
	if(papa->Llaves[i] == 0) {
		InfoPak<Tipo> PakPadre(i-1, Pak.nodo->Padre->Llaves[i-1], Pak.nodo->Padre);
		PrestamoDerechaAIzquierda(PakPadre);
		i--;
	}
	int n = papa->numeroLlaves; //Recorrido de llaves e hijos para hacer espacio
	while(n != i+1){
		papa->Llaves[n] = papa->Llaves[n-1];
		papa->Hijos[n+1] = papa->Hijos[n];
		n--; 
	}
	
	CopiarNodo(Izq, papa->Hijos[i], 0, 0, (grado*2/3));
	CopiarNodo(Mid, papa->Hijos[i], 0, Izq->numeroLlaves+1, (grado/3));	
	Mid->Llaves[Mid->numeroLlaves] = papa->Llaves[i];
	Mid->numeroLlaves++; 
	CopiarNodo(Mid, papa->Hijos[i+1], Mid->numeroLlaves, 0, (grado/3)-1);
	papa->Llaves[i] = papa->Hijos[i]->Llaves[(grado*2)/3];
	papa->Llaves[i+1] = papa->Hijos[i+1]->Llaves[(grado/3)-1];
	CopiarNodo(Der, papa->Hijos[i+1], 0, (grado/3), ((grado*2)/3));
	papa->Hijos[i]= Izq;
	papa->Hijos[i+1] = Mid;
	papa->Hijos[i+2] = Der;
	
	Izq->Padre = papa;
	Mid->Padre = papa;
	Der->Padre = papa;	
	
	delete Pak.nodo;
	Pak.nodo = NULL;
	papa->numeroLlaves++;
	
	InfoPak<Tipo> PakN (i, papa->Llaves[i], papa);
	if(papa->numeroLlaves == papa->grado && papa != Raiz){
		if(!CuidameLlave(PakN)) Promover(PakN);
	}
	if(papa->numeroLlaves == papa->grado && papa == Raiz){
		PromoverRaiz(papa->Llaves[papa->grado/2 ]);
	}
	
}

template <class Tipo>		
void BSTree<Tipo>::PromoverRaiz(Tipo _valor)
{
	int gradoRaiz = (grado*2)/3;
	gradoRaiz = (gradoRaiz*2)+1;
	Nodo<Tipo>* nuevaRaiz = new Nodo<Tipo>(gradoRaiz, 0); 
	
	Nodo<Tipo>* Izq = new Nodo<Tipo>(grado, Raiz->hoja); 
	Nodo<Tipo>* Der = new Nodo<Tipo>(grado, Raiz->hoja); 	
	
	nuevaRaiz->Llaves[0] = _valor;
	nuevaRaiz->numeroLlaves++;
	
	CopiarNodo(Izq, Raiz, 0, 0, ((grado*2)/3));
	if(grado%2 == 0)CopiarNodo(Der, Raiz, 0, Izq->numeroLlaves+1, ((grado*2)/3)-1);
	else CopiarNodo(Der, Raiz, 0, Izq->numeroLlaves+1, ((grado*2)/3));
	nuevaRaiz->Hijos[0]= Izq;
	nuevaRaiz->Hijos[1] = Der;
	Izq->Padre = nuevaRaiz;
	Der->Padre = nuevaRaiz;
	
	delete Raiz;
	Raiz = nuevaRaiz;
}

template <class Tipo>		
bool BSTree<Tipo>::Eliminar(Tipo _valor)
{
	cout<<"Eliminando a... "<<_valor<<endl; 
	
	HistorialNodo<Tipo> HistNodo(1, _valor);
	Historial.Push(HistNodo); //Saving the operation in history
	
	if(Raiz){
		InfoPak<Tipo> Pak = Buscar(_valor, 0);
		if(Pak.valor == _valor){
			if(Pak.nodo->hoja){
				Pak.nodo->Llaves[Pak.indice]=0;
				Pak.nodo->numeroLlaves--;
				RellenaVacio(Pak.nodo);	
				if(Pak.nodo!= Raiz && Pak.nodo->numeroLlaves < ((grado/3)*2)) PrestameLlave(Pak);
				return true;
			}else{
				InfoPak<Tipo> Mayor = MayorDeLosMenores(Pak.nodo->Hijos[Pak.indice], 1);
				RellenaVacio(Mayor.nodo);
				Pak.nodo->Llaves[Pak.indice] = Mayor.valor;
				if (Mayor.nodo->numeroLlaves < (grado/3)*2) PrestameLlave(Mayor); 
				return true;
			}
		}
	}
	return false;
}

template <class Tipo>	
bool BSTree<Tipo>::Buscar(Tipo _valor) //Buscar que se llama de main
{
	InfoPak<Tipo> Info = Buscar(_valor, 0);
	if(Info.valor == _valor) return true;
	return false; 
}

template <class Tipo>	
InfoPak<Tipo> BSTree<Tipo>::Buscar(Tipo _valor, int Utileria) //Buscar de utileria se llama de main
{
	Nodo<Tipo> *aux = Raiz;
	int i=0;
	while(aux->Llaves[i] != _valor)
	{
		if(aux->numeroLlaves == i || (aux->Llaves[i] > _valor)){
			if(aux->Hijos[i] != 0) aux = aux->Hijos[i];	
			else break;
			i = -1;
		}
		i++;		
	}
	if(aux){
		InfoPak<Tipo> info(i, aux->Llaves[i], aux);
	 	return info;
	}else{
	 	InfoPak<Tipo> info(i, Raiz->Llaves[i], Raiz);
		return info; 
	}
}

template <class Tipo>		
void BSTree<Tipo>::PrestameLlave(InfoPak<Tipo> Pak)
{
	int i=0;
	while(Pak.nodo->Padre->numeroLlaves > i && Pak.nodo->Padre->Llaves[i] < Pak.valor )i++; //Tenemos que saber que hijo de papa es este Pa
	if(Pak.nodo->Padre->numeroLlaves > 1 && (i==0 || Pak.nodo->Padre->Llaves[i]==0)){
		InfoPak<Tipo> PakCentralizar(i, Pak.nodo->Padre->Llaves[i], Pak.nodo->Padre);
		Centraliza(PakCentralizar); //Oops
	}else{
		if(i!= 0 && Pak.nodo->Padre->Hijos[i-1]->numeroLlaves > (grado/3) * 2){
			InfoPak<Tipo> PakPadre(i-1, Pak.nodo->Padre->Llaves[i-1], Pak.nodo->Padre);
			PrestamoIzquierdaADerecha(PakPadre);
		}else
		if(Pak.nodo->Padre->Hijos[i+1]!=0 && Pak.nodo->Padre->Hijos[i+1]->numeroLlaves > (grado/3)*2){
			InfoPak<Tipo> PakPadre(i, Pak.nodo->Padre->Llaves[i], Pak.nodo->Padre);
			PrestamoDerechaAIzquierda(PakPadre);
		}else{
			InfoPak<Tipo> Democion(i, Pak.nodo->Padre->Llaves[i], Pak.nodo->Padre);
			Demotar(Democion);	
		}
	}
}

template <class Tipo>		
bool BSTree<Tipo>::CuidameLlave(InfoPak<Tipo> Pak)
{
	int i=0;
	while(Pak.nodo->Padre->numeroLlaves > i && Pak.nodo->Padre->Llaves[i] < Pak.valor )i++; //Tenemos que saber que hijo de papa es este Pak	
	if(i!= 0 && Pak.nodo->Padre->Hijos[i-1]->numeroLlaves != Pak.nodo->numeroLlaves-1){ //Si no es el elemento mas a la izquierda y el elemento a su izquierda tiene espacio...
		InfoPak<Tipo> PakPadre(i-1, Pak.nodo->Padre->Llaves[i-1], Pak.nodo->Padre);
		PrestamoDerechaAIzquierda(PakPadre);
		return true;
	}
	if(Pak.nodo->Padre->Llaves[i] !=0 && Pak.nodo->Padre->Hijos[i+1]->numeroLlaves != Pak.nodo->numeroLlaves-1){
		InfoPak<Tipo> PakPadre(i, Pak.nodo->Padre->Llaves[i], Pak.nodo->Padre);
		PrestamoIzquierdaADerecha(PakPadre);
		return true;
	}
	return false;
}

template<class Tipo>
void BSTree<Tipo>::PrestamoDerechaAIzquierda(InfoPak<Tipo>Pak)
{
	
	
	Nodo<Tipo>* HijoIzq = Pak.nodo->Hijos[Pak.indice];
	Nodo<Tipo>* HijoDer = Pak.nodo->Hijos[Pak.indice+1];
	
	HijoIzq->Llaves[HijoIzq->numeroLlaves] = Pak.nodo->Llaves[Pak.indice];
	HijoIzq->numeroLlaves++;
	Pak.nodo->Llaves[Pak.indice] = HijoDer->Llaves[0];
	HijoDer->Llaves[0] = 0;
	HijoIzq->Hijos[HijoIzq->numeroLlaves] = HijoDer->Hijos[0];
	if(HijoDer->Hijos[0])HijoDer->Hijos[0]->Padre = HijoIzq;
	HijoDer->Hijos[0] = 0;
	HijoDer->numeroLlaves--;
	RellenaVacio(HijoDer);
}

template<class Tipo>
void BSTree<Tipo>::PrestamoIzquierdaADerecha(InfoPak<Tipo>Pak)
{	
	
	Nodo<Tipo>* HijoIzq = Pak.nodo->Hijos[Pak.indice];
	Nodo<Tipo>* HijoDer = Pak.nodo->Hijos[Pak.indice+1];
	RecorreDerecha(HijoDer);
	HijoDer->Llaves[0] = Pak.nodo->Llaves[Pak.indice];
	HijoDer->numeroLlaves++;
	Pak.nodo->Llaves[Pak.indice] = HijoIzq->Llaves[HijoIzq->numeroLlaves-1];
	HijoIzq->Llaves[HijoIzq->numeroLlaves-1] = 0;
	HijoDer->Hijos[0] = HijoIzq->Hijos[HijoIzq->numeroLlaves];
	if(HijoDer->Hijos[0])HijoDer->Hijos[0] = HijoDer;
	HijoIzq->Hijos[HijoIzq->numeroLlaves] = 0;
	HijoIzq->numeroLlaves--;
}

template<class Tipo>
void BSTree<Tipo>::Centraliza(InfoPak<Tipo> Pak)
{
	if(Pak.indice==0){
		PrestamoDerechaAIzquierda(Pak);
		Pak.indice++;
		InfoPak<Tipo> nuevoPak(Pak.indice, Pak.nodo->Hijos[Pak.indice]->Llaves[0], Pak.nodo->Hijos[Pak.indice]);
		PrestameLlave(nuevoPak);
	} 
	if(Pak.valor ==0){
		Pak.indice--;
		Pak.valor = Pak.nodo->Llaves[Pak.indice];
		PrestamoIzquierdaADerecha(Pak);
		InfoPak<Tipo> nuevoPak(Pak.indice, Pak.nodo->Hijos[Pak.indice]->Llaves[0], Pak.nodo->Hijos[Pak.indice]);
		PrestameLlave(nuevoPak);
	}
}

template<class Tipo>
void BSTree<Tipo>::Demotar(InfoPak<Tipo> Pak)
{
	if(Pak.nodo==Raiz && Raiz->numeroLlaves == 1) DemotarRaiz();
	else{		
		
		Nodo<Tipo>* DemocionIzq = new Nodo<Tipo>(grado, Pak.nodo->Hijos[0]->hoja );
		Nodo<Tipo>* DemocionDer = new Nodo<Tipo>(grado, Pak.nodo->Hijos[0]->hoja );
		DemocionIzq->Padre = Pak.nodo;
		DemocionDer->Padre = Pak.nodo;
		
		CopiarNodo(DemocionIzq, Pak.nodo->Hijos[Pak.indice-1], 0, 0, Pak.nodo->Hijos[Pak.indice-1]->numeroLlaves); 
		DemocionIzq->Llaves[DemocionIzq->numeroLlaves] = Pak.nodo->Llaves[Pak.indice-1];
		DemocionIzq->numeroLlaves++;

		CopiarNodo(DemocionIzq, Pak.nodo->Hijos[Pak.indice], DemocionIzq->numeroLlaves, 0, (grado/3)-1);

		Pak.nodo->Llaves[Pak.indice-1]= Pak.nodo->Hijos[Pak.indice]->Llaves[(grado/3)-1];
		CopiarNodo(DemocionDer, Pak.nodo->Hijos[Pak.indice], 0, (grado/3), (grado/3)-1);
	
		DemocionDer->Llaves[DemocionDer->numeroLlaves]= Pak.nodo->Llaves[Pak.indice];
		Pak.nodo->Llaves[Pak.indice] =0;
		
		CopiarNodo(DemocionDer, Pak.nodo->Hijos[Pak.indice+1], DemocionDer->numeroLlaves, 0, ((grado/3)*2));
		
		Pak.nodo->numeroLlaves--;
		RellenaVacio(Pak.nodo);
		
		Pak.nodo->Hijos[Pak.indice-1] = DemocionIzq;
		Pak.nodo->Hijos[Pak.indice] = DemocionDer;
	
		Pak.valor = Pak.nodo->Llaves[Pak.nodo->numeroLlaves];
		
		if(Pak.nodo->numeroLlaves < (grado/3)*2 && Pak.nodo != Raiz) PrestameLlave(Pak);	
	}
}

template<class Tipo>
void BSTree<Tipo>::DemotarRaiz()
{	
	Nodo<Tipo>* nuevaRaiz = new Nodo<Tipo>(Raiz->grado, 1);
	
	if(Raiz->Hijos[0]->numeroLlaves < Raiz->Hijos[1]->numeroLlaves){
		CopiarNodo(nuevaRaiz, Raiz->Hijos[0], 0, 0, ((grado/3)*2)-1);
		nuevaRaiz->Llaves[nuevaRaiz->numeroLlaves] = Raiz->Llaves[0];
		nuevaRaiz->numeroLlaves++;
		CopiarNodo(nuevaRaiz, Raiz->Hijos[1], nuevaRaiz->numeroLlaves, 0, (grado/3)*2);
	}else{
		CopiarNodo(nuevaRaiz, Raiz->Hijos[0], 0, 0, (grado/3)*2);
		nuevaRaiz->Llaves[nuevaRaiz->numeroLlaves] = Raiz->Llaves[0];
		nuevaRaiz->numeroLlaves++;
		CopiarNodo(nuevaRaiz, Raiz->Hijos[1], nuevaRaiz->numeroLlaves, 0, ((grado/3)*2)-1);

		nuevaRaiz->Hijos[nuevaRaiz->numeroLlaves] = Raiz->Hijos[1]->Hijos[Raiz->Hijos[1]->numeroLlaves];
	}
	delete Raiz->Hijos[0], Raiz->Hijos[1], Raiz;
	Raiz = nuevaRaiz; 	
}

template<class Tipo>
InfoPak<Tipo> BSTree<Tipo>::MayorDeLosMenores(Nodo<Tipo>* aux, bool Hoja)
//bool que me dice si es necesario conseguir el mayor inmediato o recorrer a profundidad.
{
	int i=0;
	if(aux->hoja && Hoja){
		while(aux->Llaves[i] != 0) i++;
		InfoPak<Tipo> Mayor(i-1, aux->Llaves[i-1], aux);
		aux->Llaves[i-1] =0;
		aux->numeroLlaves--;
	//	RellenaVacio(Mayor);
		return Mayor;
	}else{
		if(!Hoja){
			while(aux->Llaves[i] != 0) i++;
			InfoPak<Tipo> Mayor(i-1, aux->Llaves[i-1], aux);
			aux->Llaves[i-1] =0;
			aux->numeroLlaves--;
	//		RellenaVacio(Mayor);
			return Mayor;
		}
		while(aux->Llaves[i]!=0) i++;
		return MayorDeLosMenores(aux->Hijos[i], Hoja);
	}
}

template<class Tipo>
InfoPak<Tipo> BSTree<Tipo>::MenorDeLosMayores(Nodo<Tipo>* aux, bool Hoja)
{
	if(aux->hoja && Hoja){
		InfoPak<Tipo> Menor(0, aux->Llaves[0], aux);
		aux->Llaves[0] =0;
		aux->numeroLlaves--;
		return Menor;
	}else{
		if(!Hoja){
			InfoPak<Tipo> Menor(0, aux->Llaves[0], aux);
			aux->Llaves[0] =0;
			aux->numeroLlaves--;
			return Menor;			
		}
		return MenorDeLosMayores(aux->Hijos[0], Hoja);
	}
}

template<class Tipo>
void BSTree<Tipo>::RecorreDerecha(Nodo<Tipo>* aux){
	
	int i = aux->numeroLlaves;
	aux->Hijos[i+1] = aux->Hijos[i];
	
	while(i != 0){
		aux->Llaves[i] = aux->Llaves[i-1]; 
		aux->Hijos[i] = aux->Hijos[i-1];
		i--;
	}
	aux->Llaves[0] =0;
	aux->Hijos[0] =0;
}

template <class Tipo>
void BSTree<Tipo>::RellenaVacio(Nodo<Tipo>* aux){
	int i=0;
	while(aux->Llaves[i]!=0) i++;
	while(i < aux->numeroLlaves){
		aux->Llaves[i] = aux->Llaves[i+1];
		aux->Hijos[i] = aux->Hijos[i+1];
		i++;
	}
	aux->Llaves[i] = 0;
	aux->Hijos[i] = aux->Hijos[i+1];
	aux->Hijos[i+1] = 0;
}

template <class Tipo>
void BSTree<Tipo>::Imprimir()
{
	cout<<"Impresion por niveles"<<endl;

		Nodo<Tipo> *aux;
		Cola <Nodo<Tipo>*> colaNodos;
		if (Raiz){
			colaNodos.Push(Raiz);
		}
		while( !colaNodos.Vacio() ){			
			colaNodos.Pop();
			aux = colaNodos;
			cout<<"[";
			ImprimirNodo(aux);
			cout<<"] "<<endl;				
			int i=0;
			while(aux->Hijos[i]!=0){
				colaNodos.Push(aux -> Hijos[i]);
				i++;
			}
		} 
}

template <class Tipo>
void BSTree<Tipo>::ImprimirNodo(Nodo<Tipo>* aux)
{
	for(int i=0 ; i<aux->grado; i++){
		if(aux->Llaves[i] != 0){
			cout<<aux->Llaves[i];
			cout<<" ";
		}	
	}
}

template<class Tipo>
void BSTree<Tipo>::ImprimirInfoPak(InfoPak<Tipo> Pak)
{
	cout<<"Indice: "<<Pak.indice<<endl;
	cout<<"Dato: "<<Pak.valor<<endl;
	cout<<"Nodo: ";
	ImprimirNodo(Pak.nodo);
	cout<<endl;
}

template<class Tipo>
void BSTree<Tipo>::ImprimirHistorialNodo(HistorialNodo<Tipo> Nodo)
{
	if(Nodo.operation) cout<<"Una Eliminacion de el valor ";
	else cout<<"Una insercion de el valor ";
	cout<<Nodo.data;
	cout<<endl;
}

template <class Tipo> 
void BSTree<Tipo>::Vaciar(Nodo<Tipo> *&subRaiz)
{
	if(subRaiz){
		int i =0;
		while(i < subRaiz->numeroLlaves){
			Vaciar(subRaiz->Hijos[i]); 
		}
		delete subRaiz;
		subRaiz = NULL; 
	} 
}

#endif
