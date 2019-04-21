#include <iostream>
#include <vector>
#include <algorithm> 
#include <time.h>

#define max_weight 10
using namespace std;

class Grafo{
public:
	vector<vector<short>> adj;
	Grafo(int size): adj( size , vector<short>(size,0) ){
		for(int i = 0 ; i<size ; ++i){
			for(int j = i+1 ; j<size ; ++j){
				adj[i][j] = rand()% max_weight;
				adj[j][i] = adj[i][j];
			}
		}
	}
	
	void print(){
		for(int i = 0 ; i<adj.size() ; ++i){
			for(int j = 0 ; j<adj.size() ; ++j){
				cout << adj[i][j] << " ";
			}cout << endl;
		}cout << endl;
	}
};
Grafo* grafo;


class Individuo{
private:
	void calcular_aptitud(){
		aptitud = 0;
		for ( int i = 1 ; i < genes.size() ; ++i){
			short pos_prev = genes[i-1];
			short pos_cur = genes[i];
			aptitud += grafo->adj[ pos_prev ][ pos_cur  ];
		}
	}
	
public:
	vector<short> genes; //orden de las ciudades que lo componen al individuo(genes)
	int aptitud;
	
	Individuo( vector<short> genes_): genes(genes_.begin(), genes_.end()){ 
		calcular_aptitud();
	}
	
	void print(){
		for(int i = 0 ; i < genes.size() ; ++i){
			cout << genes[i] << " ";
		}cout << endl << " - aptitud: " << aptitud << endl;
	}
	
};

bool comparacion( Individuo* ind1, Individuo* ind2 ){
	return ( ind1->aptitud < ind2->aptitud );
}

class Algoritmo_genetico{
private:
	
	void swap_rand_values(vector<short>& individuo){
		int pos_swap1 = rand()%num_ciudades;
		int pos_swap2 = rand()%num_ciudades;
		while ( pos_swap2 == pos_swap1 ) 
			pos_swap2 = rand()%num_ciudades;
		
		short aux = individuo[pos_swap1];
		individuo[pos_swap1] = individuo[pos_swap2];
		individuo[pos_swap2] = aux;
	}
	
	void mutacion(){
		int i, j;
		for( i = poblacion.size(), j=0 ; i < tam_poblacion ; ++i, ++j){
			if (j >= num_elite) j = 0;//generar nuevos pobladores a parrtir de la elite
			vector<short> individuo(poblacion[j]->genes.begin(), poblacion[j]->genes.end());
			swap_rand_values(individuo);
			poblacion.push_back(new Individuo(individuo));
		}
	}
	
	void seleccion(){
		sort(poblacion.begin(), poblacion.end(), comparacion); 	//ordeno segun aptitud
		poblacion.erase(poblacion.begin()+num_elite, poblacion.end());	//escojo a la elite
	}
	
public:
	vector<Individuo*> poblacion;
	int tam_poblacion;
	int num_generaciones;
	int num_elite; //individuos que permaneces despues de cada seleccion
	int cur_generacion;
	
	int num_ciudades;
	
	Algoritmo_genetico( int tam_poblacion_, int num_generaciones_, int num_elite_, int num_ciudades_): tam_poblacion(tam_poblacion_), 
		num_generaciones(num_generaciones_), num_elite(num_elite_), num_ciudades(num_ciudades_), cur_generacion(0){
		
		vector<short> ciudades_plantilla(num_ciudades);
		for(int i = 0; i < num_ciudades; ++i){
			ciudades_plantilla[i] = i;//ciudades = {0,1,2,3,4}
		}

		for(int i = 0; i < tam_poblacion; ++i){//generacion aleatoria del vector de ciudades para los primeros individuos de la poblaicion
			vector<short> ciudades(ciudades_plantilla.begin(), ciudades_plantilla.end());			
			vector<short> individuo( num_ciudades+1 );

			individuo[0] = individuo[ individuo.size()-1 ] = rand()%num_ciudades;
			ciudades.erase(ciudades.begin() + individuo[0], ciudades.begin() + individuo[0]+1);
			
			for(int j = 1; j < individuo.size() - 1 ; ++j){
				int pos = rand()%ciudades.size();
				individuo[j] = ciudades[pos];
				ciudades.erase(ciudades.begin() + pos, ciudades.begin() + pos+1);
			}
			
			poblacion.push_back(new Individuo(individuo));
			//poblacion[i]->print();
		}
		
	}
	
	void generar_poblacion(){
		cur_generacion += 1;
		seleccion();
		mutacion();
	}

	void print_poblacion(){
		cout << "generacion actual: "  << cur_generacion << endl;
		for(int i = 0 ; i < poblacion.size() ; ++i){
			poblacion[i]->print();
		}cout << endl << endl;
	}
	

};



int main(int argc, char *argv[]) {
	int num_ciudades = 7;
	int tam_poblacion = 10;
	int num_generaciones = 5;
	int num_elite = tam_poblacion/2; //20% de la poblacion sera elite
	
	
	grafo = new Grafo(num_ciudades);
	grafo->print();
	Algoritmo_genetico ag( tam_poblacion , num_generaciones , num_elite , num_ciudades );
	
	
	for(int i = 0; i<20; ++i){//bucle para las iteraciones de generar poblaicon
		ag.generar_poblacion();
	}
	ag.print_poblacion();
	
	return 0;
}



