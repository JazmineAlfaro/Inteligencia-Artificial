#include <iostream>
#include <vector>
	
	using namespace std;

const int deep = 3;

class Damas {
public:
	vector <vector<short>> tablero;
	int puntaje_p1;
	int puntaje_p2;
	
	Damas(vector <vector<short>> tablero_, int puntaje_p1_) : tablero(tablero_), puntaje_p1(puntaje_p1_) {}
	
	Damas() : puntaje_p1(0), puntaje_p2(0) {
		//asi seria el tablero: vacio = 0 , player_1 = 1 y player_2 = 2
		tablero = { {0,2,0,2,0,2,0,2},
					{2,0,2,0,2,0,2,0}, 
					{0,2,0,2,0,2,0,2}, 
					{0,0,0,0,0,0,0,0}, 
					{0,0,0,0,0,0,0,0}, 
					{1,0,1,0,1,0,1,0},
					{0,1,0,1,0,1,0,1},
					{1,0,1,0,1,0,1,0}
					};
	}
	
	void print_tablero() {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j)
				cout << tablero[i][j] << " ";
				cout << endl;
		}
		cout << endl << endl;
	}
};

class min_max {
public:
	vector<min_max*> children;
	int player;	//true for max
	Damas* current;
	
	min_max() : player(1), current(new Damas) {
		//primer constructor    //player 1 empieza
		make_levels(this);
	}
	
	min_max(Damas* tab_, int player_) : player(player_), current(tab_) {}//constructor de los nodos hoja cada vez que avanza una jugada
	
	
	void make_levels(min_max* min_current, int actual_deep = 0 ) {
		if (actual_deep > deep) return;
		
		possible_plays(min_current);
		for (int j = 0; j < min_current->children.size(); ++j) {
			make_levels(min_current->children[j], actual_deep + 1);
		}
	}
	
	void make_levels_leaf(min_max* min_current, int actual_deep) {//crea niveles solo despues de la hoja
		if (actual_deep == deep - 1) {
			possible_plays(min_current);
			return;
		}
		
		for (int j = 0; j < min_current->children.size(); ++j) {
			make_levels_leaf(min_current->children[j], actual_deep + 1);
		}
	}
	
	
	int calculate_min_max(min_max* min_current, int actual_deep) {
		if(!min_current) return 0;
		
		if (actual_deep == deep) return min_current->current->puntaje_p1;
		
		if (min_current->player == 2) { //max
			int maximo = -1000;
			for (int j = 0; j < min_current->children.size(); ++j) {
				int val = min_current->current->puntaje_p1 + calculate_min_max(min_current->children[j], actual_deep + 1);
				if (val > maximo) maximo = val;
			}
			return  maximo;
		}
		else {  // min
			int minimo = 1000;
			for (int j = 0; j < min_current->children.size(); ++j) {
				int val = min_current->current->puntaje_p1 + calculate_min_max(min_current->children[j], actual_deep + 1);
				if (val < minimo) minimo = val;
			}
			return  minimo;
		}
	}
	
	int main_calculate_minmax() {
		//calcular minmax para jugar
		if (children.size() == 0) return -1;
		int indice = 0;//es el indice del hijo del que se hara la siguiente jugada
		
		if (player == 2) {//MAX
			int maximo = -1000;
			for (int j = 0; j < this->children.size(); ++j) {
				int val = calculate_min_max(this->children[j], 1);
				if (val > maximo) {
					indice = j;
					maximo = val;
				}
			}
		}
		else {//MIN
			int minimo = 1000;
			for (int j = 0; j < this->children.size(); ++j) {
				int val = calculate_min_max(this->children[j], 1);
				if (val < minimo) {
					indice = j;
					minimo = val;
				}
			}
		}
		return indice;
	}
	
	void possible_plays(min_max* cur_min_max) {
		// retorna un vector con jugadas de la posicion x, y a la posicion posible x, y
		
		cur_min_max->children.clear();
		
		//se usa num para saber si se suma o se resta porque hay dos perspectivas del tablero
		int num;
		(cur_min_max->player == 1) ? num = -1 : num = 1;
		
		int next_player; (cur_min_max->player == 1) ? next_player = 2 : next_player = 1;
		
		Damas* current_ = cur_min_max->current;
		
		for (int i = 0; i < 8; i += 1) {
			for (int j = 0; j < 8; j += 1) {
				if (current_->tablero[i][j] == cur_min_max->player) {
					
					if (i + num < 0 || i + num > 7) continue; // si lo hacemos con coronacion, aqui se deberia programar
					
					if (j - 1 >= 0) {
						///diagonal izquierda
						if (current_->tablero[i + num][j - 1] == 0) {
							//posible porque ese espacio en el tablero está vacío
							
							vector <vector<short>> tablero = current_->tablero;
							tablero[i][j] = 0;
							tablero[i + num][j - 1] = cur_min_max->player;
							
							cur_min_max->children.push_back(new min_max(new Damas(tablero, current_->puntaje_p1), next_player));
						}
						else if (j - 2 >= 0 && current_->tablero[i + num][j - 1] != cur_min_max->player && current_->tablero[i + 2 * num][j - 2] == 0) {
							// (caso comer) en el que la posicion a avanzar hay la ficha del otro jugador y espacio vacio la siguiente
							
							vector <vector<short>> tablero = current_->tablero;
							tablero[i][j] = 0;
							tablero[i + num][j - 1] = 0;//desaparece ficha oponente(se la come)
							tablero[i + 2 * num][j - 2] = cur_min_max->player;
							
							int puntaje1_aux = current_->puntaje_p1;
							if (cur_min_max->player == 2) {
								puntaje1_aux += 2;
							}
							else {
								puntaje1_aux -= 1;
							}
							
							cur_min_max->children.push_back(new min_max(new Damas(tablero, puntaje1_aux), next_player));
						}
					}
					
					if (j + 1 < 8) {
						///diagonal derecha
						if (current_->tablero[i + num][j + 1] == 0) {
							//posible porque ese espacio en el tablero está vacío
							
							vector <vector<short>> tablero = current_->tablero;
							tablero[i][j] = 0;
							tablero[i + num][j + 1] = cur_min_max->player;
							
							cur_min_max->children.push_back(new min_max(new Damas(tablero, current_->puntaje_p1), next_player));
						}
						else if (j + 2 < 8 && current_->tablero[i + num][j + 1] != cur_min_max->player && current_->tablero[i + 2 * num][j + 2] == 0) {
							// (caso  comer) en el que la posicion a avanzar hay la ficha del otro jugador y espacio vacio la siguiente
							
							vector <vector<short>> tablero = current_->tablero;
							tablero[i][j] = 0;
							tablero[i + num][j + 1] = 0;//desaparece ficha oponente(se la come)
							tablero[i + 2 * num][j + 2] = cur_min_max->player;
							
							int puntaje1_aux = current_->puntaje_p1, puntaje2_aux = current_->puntaje_p2;
							if (cur_min_max->player == 2) {
								puntaje1_aux += 2;
							}
							else {
								puntaje1_aux -= 1;
							}
							
							cur_min_max->children.push_back(new min_max(new Damas(tablero, puntaje1_aux), next_player));
						}
					}
				}
			}
		}
	}
	
	int comparar_tablero( vector<vector<short>>& tab, int index_child=0 ){
		if(index_child > this->children.size()) return -1;
		
		for (int i=0; i<tab.size(); ++i){
			for(int j =0; j< tab[i].size(); ++ j){
				if(tab[i][j]!= this->children[index_child]->current->tablero[i][j]){ 
					return comparar_tablero(tab,index_child+1);
				}
			}
		}	
		return index_child;
	}
	
	vector<vector<short>> play( vector<vector<short>>& tab) {
		if (player == 1 ){
			*this = *new min_max( new Damas(tab,this->current->puntaje_p1 ),2);
			make_levels(this);
		}
		else{
			int indice_next_play;
			indice_next_play = main_calculate_minmax();
			if (indice_next_play < 0 || indice_next_play > 100)return vector<vector<short>>(0);///
			*this = *children[indice_next_play];
		}
		
		return this->current->tablero;
	}
};

/*
int main() {
//player 1 empieza
;
for (int i = 0; i < 50; ++i)
game1.play();

system("pause");
return 0;
}*/
