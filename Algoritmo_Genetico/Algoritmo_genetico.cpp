#include <iostream>
#include <vector>

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
			}
			cout << endl;
		}
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

class Algoritmo_genetico{
public:
	vector<Individuo*> poblacion;
	int tam_poblacion;
	int num_generaciones;
	int num_elite; //individuos que permaneces despues de cada seleccion
	
	int num_ciudades;
	
	Algoritmo_genetico( int tam_poblacion_, int num_generaciones_, int num_elite_, int num_ciudades_): tam_poblacion(tam_poblacion_), 
		num_generaciones(num_generaciones_), poblacion(tam_poblacion), num_elite(num_elite_), num_ciudades(num_ciudades_){
		
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
			
			poblacion[i] = new Individuo(individuo);
			//poblacion[i]->print();
		}
		
	}
	
	void seleccion(){
		
	}

	void mutacion(){
		
	}
};



int main(int argc, char *argv[]) {
	int num_ciudades = 5;
	int tam_poblacion = num_ciudades*2;
	int num_generaciones = 5;
	int num_elite = tam_poblacion/5; //20% de la poblacion
	
	grafo = new Grafo(num_ciudades);
	//grafo->print();
	Algoritmo_genetico ag( tam_poblacion , num_generaciones , num_elite , num_ciudades );
	
	return 0;
}



