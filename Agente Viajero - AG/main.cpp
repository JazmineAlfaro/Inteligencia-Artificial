#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <utility>
#include <time.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <vector>
#include<GL/glut.h>

#define KEY_ESC 27
#define KEY_IZQ 97
#define KEY_DER 100
#define KEY_UP 119
#define KEY_DWN 115
#define KEY_ADD 9 ///tab
#define KEY_REMOV 8 ///retro
#define KEY_INTRO 13

using namespace std;

vector<int> num_iteraciones = {10,100,1000,2000,5000,10000,20000,50000};

int rdtsc(){ 
	__asm__ __volatile__("rdtsc"); 
} 


float randNum(float LO, float HI){
	srand(rdtsc());
	float r = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(HI-LO)));
	return r;
}



class Node;
class Edge;
class Graph;

struct Node {
	float m_data;
	int m_x,m_y;
	vector<Edge*> m_edges; 
	
	float dis(Node *aux){
		return sqrt(pow(aux->m_x-m_x,2)+pow(aux->m_y-m_y,2));
	}
	
};


struct Edge {
	Node* m_nodes[2];
	float m_data;
	
	void print(){
		cout<<m_nodes[0]->m_data<<" "<<m_data<<" "<<m_nodes[1]->m_data<<endl;
	}
};

class Graph {
public:
	vector<Node*> m_nodes;
	void insertNode(float node, int x0, int y0) {
		Node *t = new Node;
		t->m_data = node;
		t->m_x = x0;
		t->m_y = y0;
		m_nodes.push_back(t);
	}
	
	void insertEdge(float edge, Node* a, Node* b) {
		Edge *t = new Edge;
		t->m_data = edge;
		t->m_nodes[0] = a;
		t->m_nodes[1] = b;
		a->m_edges.push_back(t);
		
		Edge *p = new Edge;
		p->m_data = edge;
		p->m_nodes[0] = b;
		p->m_nodes[1] = a;
		b->m_edges.push_back(p);
	}
	
	Graph(){;}
	
	Graph(int tam){
		for(int i=1;i<=tam;i++){
			int a = randNum(-150+tam*2,150-tam*2);
			int b = randNum(-150+tam*2,150-tam*2);
			insertNode(i-1,a-i*8,b+i*4);
		}
		int num=0;
		for(int i=0;i<tam;++i){
			for(int j =1+i;j<tam;++j){
				insertEdge(m_nodes[i]->dis(m_nodes[j]),m_nodes[i],m_nodes[j]);
			}
		}		
	}
	
	float find_weight(int index_node_a, int index_node_b){
		for(int i=0;i< (m_nodes[index_node_a]->m_edges).size();++i){
			if( (m_nodes[index_node_a]->m_edges[i]->m_nodes[1])->m_data == index_node_b ){
				return (m_nodes[index_node_a]->m_edges[i])->m_data;
			}
		}
	}
	
};
Graph* grafo;

class Individuo{
private:
	void calcular_aptitud(){
		aptitud = 0;
		for ( int i = 1 ; i < genes.size() ; ++i){
			short pos_prev = genes[i-1];
			short pos_cur = genes[i];
			aptitud += grafo->find_weight( pos_prev , pos_cur  );
		}
	}
	
public:
	vector<short> genes; //orden de las ciudades que lo componen al individuo(genes)
	float aptitud;
	
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
		int pos_swap1 = rand()%(num_ciudades-2)+1;
		int pos_swap2 = rand()%(num_ciudades-2)+1;
		while ( pos_swap2 == pos_swap1 ) 
			pos_swap2 = rand()%(num_ciudades-2)+1;
		
		short aux = individuo[pos_swap1];
		individuo[pos_swap1] = individuo[pos_swap2];
		individuo[pos_swap2] = aux;
	}
	
	float mutacion(){
		int i, j;
		
		for( i = poblacion.size(), j=0 ; i < tam_poblacion ; ++i, ++j){
			if (j >= num_elite) j = 0;//generar nuevos pobladores a parrtir de la elite
			vector<short> individuo(poblacion[j]->genes.begin(), poblacion[j]->genes.end());
			swap_rand_values(individuo);
			poblacion.push_back(new Individuo(individuo));
			
		}
		
		float average = 0;
		for( i = 0 ; i < poblacion.size() ; ++i){
			average += poblacion[i]->aptitud;
		}
		return average/tam_poblacion;
	}
	
	
	float seleccion(){
		sort(poblacion.begin(), poblacion.end(), comparacion); 	//ordeno segun aptitud
		poblacion.erase(poblacion.begin()+num_elite, poblacion.end());	//escojo a la elite
		return poblacion[0]->aptitud;
	}
	
public:
	vector<Individuo*> poblacion;
	int tam_poblacion;
	int num_generaciones;
	int num_elite; //individuos que permaneces despues de cada seleccion
	int cur_generacion;
	
	int num_ciudades;
	
	vector<float> best_path;
	vector<float> average_path;
	
	
	
	Algoritmo_genetico( int tam_poblacion_, int num_generaciones_, int num_elite_, int num_ciudades_): tam_poblacion(tam_poblacion_), 
		num_generaciones(num_generaciones_), num_elite(num_elite_), num_ciudades(num_ciudades_), cur_generacion(0){
		
		vector<short> ciudades_plantilla(num_ciudades);
		for(int i = 0; i < num_ciudades; ++i){
			ciudades_plantilla[i] = i;//ciudades = {0,1,2,3,4}
		}
		
		float average=0;
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
			average+=poblacion[poblacion.size()-1]->aptitud;//para el promedio
		}
		//para añadir el mejor y el promedio
		sort(poblacion.begin(), poblacion.end(), comparacion); 	
		best_path.push_back(poblacion[0]->aptitud);
		average_path.push_back(average/tam_poblacion);
	}
	
	
	void generar_poblacion( int iteraciones = 1 ){
		cur_generacion += 1;
		for ( int i=0; i < iteraciones - 1 ; ++i){
			seleccion();
			mutacion();
		}
		best_path.push_back( seleccion() );
		average_path.push_back( mutacion() );
		//print_poblacion();
	}
	
	void print_poblacion(){
		cout << "generacion actual: "  << cur_generacion << endl;
		for(int i = 0 ; i < poblacion.size() ; ++i){
			poblacion[i]->print();
		}cout << endl << endl;
		
	}
	
};

Algoritmo_genetico* ag;



void displayPoint(){
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<grafo->m_nodes.size();++i){
		glVertex2i(grafo->m_nodes[i]->m_x,grafo->m_nodes[i]->m_y);
	}
	glEnd();
}

void displayEdges(){
	
	glPushMatrix();
	glLineWidth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
	
	
	for(int i=0;i<grafo->m_nodes.size();++i){
		for(int j=0;j<grafo->m_nodes[i]->m_edges.size();++j){
			glVertex2i(grafo->m_nodes[i]->m_x, grafo->m_nodes[i]->m_y);
			glVertex2i(grafo->m_nodes[i]->m_edges[j]->m_nodes[1]->m_x, grafo->m_nodes[i]->m_edges[j]->m_nodes[1]->m_y);
			
		}
	}
	glEnd();
	glPopMatrix();
	
	
	glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	
	for(int i=1;i< (ag->poblacion[0]->genes).size() ;++i){
		short pos = ag->poblacion[0]->genes[i-1];
		short pos2 = ag->poblacion[0]->genes[i];
		glVertex2i( grafo->m_nodes[pos]->m_x , grafo->m_nodes[pos]->m_y );
		glVertex2i( grafo->m_nodes[pos2]->m_x , grafo->m_nodes[pos2]->m_y );
	}
	
	glEnd();
	glPopMatrix();
	
}


void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
	}
}

void OnMouseMotion(int x, int y)
{
	//opcional
	//hacer algo x,z cuando se mueve el mouse
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {
	
	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f,  300.0f,-300.0f, 300.0f, -1.0f, 1.0f);
	
	displayEdges();
	displayPoint();
	
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//


//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
}

void load_path();
GLvoid window_key(unsigned char key, int x, int y) {
	;
}


void display()
{
	
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f,  300.0f, -300.0f, 300.0f, -1.0f, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glutInitWindowPosition(350, 50); 
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLineWidth(2.0f);
	
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(-250,-250);
	glVertex2i(250,-250);
	glVertex2i(-250,-250);
	glVertex2i(-250,250);
	///dibujar los ejes 
	for(int i=1;i<=16;++i){
		glVertex2i(-250+i*30,-245);
		glVertex2i(-250+i*30,-255);
	}
	
	for(int i=1;i<=16;++i){
		glVertex2i(-255,-250+i*30);
		glVertex2i(-245,-250+i*30);
	}
	glEnd();
	
	///mejores 
	glPointSize(5.0f);
	
	glBegin(GL_POINTS);
	for(int i=0; i < ag->best_path.size();++i){
		glColor3f(1.0f,0.0f,1.0f);
		glVertex2f(-250+(i+1)*30,-310+ ag->best_path[i]/10);	
	}
	
	for(int i=0;i<ag->average_path.size();++i){
		glColor3f(0.0f,0.0f,1.0f);
		glVertex2f(-250+(i+1)*30,-310+ag->average_path[i]/10);
	}
	glEnd();
	
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH);
	glColor3f(1.0f, 0.0f, 1.0f);
	
	for(int i=0; i < ag->best_path.size()-1 ;++i){
		glVertex2f(-250+(i+1)*30,-310+ag->best_path[i]/10);
		glVertex2f(-250+(i+2)*30,-310+ag->best_path[i+1]/10);
	}
	
	glColor3f(0.0f, 0.0f, 1.0f);
	
	for(int i=0;i<ag->average_path.size()-1;++i){
		glVertex2f(-250+(i+1)*30,-310+ag->average_path[i]/10);
		glVertex2f(-250+(i+2)*30,-310+ag->average_path[i+1]/10);
	}	
	glEnd();
	
	glutSwapBuffers();
}


//inicializacion de OpenGL
//
void init_GL(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(50, 50); //posicion de la ventana
	glutCreateWindow("Grafo"); //titulo de la ventana
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo de la escena negro 
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	glutIdleFunc(&idle);
	
	//init_GL(); //funcion de inicializacion de OpenGL
	
	glutInitWindowPosition(650, 50); //posicion de la ventana
	glutCreateWindow("Grafico");   // Create a window 2
	glutDisplayFunc(display);  
	
}



int main(int argc, char** argv) {
	srand(time(NULL));
	
	int num_ciudades = 10;
	int tam_poblacion = 20;
	int num_generaciones = 20;
	int num_elite = tam_poblacion/2; //20% de la poblacion sera elite
	
	
	grafo = new Graph(num_ciudades);
	
	ag = new Algoritmo_genetico( tam_poblacion , num_generaciones , num_elite , num_ciudades );
	
	///*****************bucle para las iteraciones de generar poblaicon*****************
	
	for(int i = 0; i<num_iteraciones.size(); ++i){
		ag->generar_poblacion(num_iteraciones[i]);
	}
	
	for(int i = 0; i< ag->best_path.size(); ++i){
		cout << ag->best_path[i] << " " << ag->average_path[i] << endl;
	}
	
	
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	init_GL(); //funcion de inicializacion de OpenGL
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
}
