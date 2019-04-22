#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include<GL/glut.h>
#include <vector>
#include <utility>
#include <time.h>
#define KEY_ESC 27
#include <list>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;
static char label[100];

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
	
	void change(){
		Node* temp = m_nodes[0];
		m_nodes[0] = m_nodes[1];
		m_nodes[1] = temp;
	}
	
	bool equal(Edge *act){
		if((act->m_nodes[0] == m_nodes[0] && act->m_nodes[1] == m_nodes[1]) ||
			(act->m_nodes[0] == m_nodes[1]  && act->m_nodes[1] == m_nodes[0]))
			return true;
		return false;
	}
	
	
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
	
	Graph(){
		;
	}
	Graph(int tam){
		for(int i=1;i<=tam;i++){
			int a = randNum(-150+tam*2,150-tam*2);
			int b = randNum(-150+tam*2,150-tam*2);
			insertNode(i,a-i*8,b+i*4);
		}
		int num=0;
		for(int i=0;i<tam-1;++i){
			for(int j =1+i;j<tam;++j){
				//insertEdge(1,m_nodes[i],m_nodes[j]);
				//cout<<"Desde: "<<m_nodes[i]->m_data<<"  Hasta : "<<m_nodes[j]->m_data<<endl;
				insertEdge(m_nodes[i]->dis(m_nodes[j]),m_nodes[i],m_nodes[j]);
				
			}
		}
		/*
		cout<<"Tam: "<<m_nodes.size()<<endl;
		for(int i=0;i<=m_nodes.size();++i){
		cout<<"Tam de : "<<i+1<<" "<<m_nodes[i]->m_edges.size()<<endl;
		for(int j =0;j<m_nodes[i]->m_edges.size();++j){
		m_nodes[i]->m_edges[j]->print();
		
		}
		}*/
		
		
	}
	
	
	Edge* find(Node* a, Node *b){
		for(int i=0;i<a->m_edges.size();++i){
			if((a->m_edges[i]->m_nodes[0] == a && a->m_edges[i]->m_nodes[1] == b ) ||
				(a->m_edges[i]->m_nodes[0] == b && a->m_edges[i]->m_nodes[1] == a )){
				return a->m_edges[i];
			}
		}
	}
	
};




class TSP{
public:
	Graph graph;
	int population;
	int generation;
	int mutation_rate;
	vector<Edge*> iPopulation;
	
	TSP(){
		;
	}
	void initial_population(){
		int size = graph.m_nodes.size();
		for(int i=0;i<size-1;++i){
			Edge* e = graph.find(graph.m_nodes[i],graph.m_nodes[i+1]);
			iPopulation.push_back(e);
		}
		Edge* e = graph.find(graph.m_nodes[size-1],graph.m_nodes[0]);
		iPopulation.push_back(e);
		
		for(int i=0;i<iPopulation.size();++i){
			iPopulation[i]->print();
		}
	}
	
	TSP(Graph g,int sizep, int numg, int mut){
		this->graph = g;
		this->population = sizep;
		this->generation = numg;
		this->mutation_rate = mut;
		initial_population();
	}
	
	
	
	
};


Graph g1(10);
TSP g2(g1,10,10,2);


void displayPoint(){
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_POINTS);
	for(int i=0;i<g1.m_nodes.size();++i){
		glVertex2i(g1.m_nodes[i]->m_x,g1.m_nodes[i]->m_y);
	}
	glEnd();
}
void displayEdges(){
	
	glPushMatrix();
	glLineWidth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
	
	for(int i=0;i<g1.m_nodes.size();++i){
		for(int j=0;j<g1.m_nodes[i]->m_edges.size();++j){
			glVertex2i(g1.m_nodes[i]->m_x,g1.m_nodes[i]->m_y);
			glVertex2i(g1.m_nodes[i]->m_edges[j]->m_nodes[1]->m_x,g1.m_nodes[i]->m_edges[j]->m_nodes[1]->m_y);
		}
	}
	glEnd();
	glPopMatrix();
	
	
	glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	
	for(int i=0;i<g2.iPopulation.size();++i){
		
		glVertex2i(g2.iPopulation[i]->m_nodes[0]->m_x,g2.iPopulation[i]->m_nodes[0]->m_y);
		glVertex2i(g2.iPopulation[i]->m_nodes[1]->m_x,g2.iPopulation[i]->m_nodes[1]->m_y);
		//glVertex2i(g2.m_nodes[i]->m_edges[j]->m_nodes[1]->m_x,g2.m_nodes[i]->m_edges[j]->m_nodes[1]->m_y);
		
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
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

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


///Ejemplos de Vectores
vector<float> mejores =  {15.6, 14.3, 12.5, 11.6,  9.7, 8.2, 5.6, 4.2};
vector<float> promedio = {17.2, 15.9, 13.4, 12.8,  11.1, 9.5, 7.3, 6.4};


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
		//glVertex2i(g2.m_nodes[i]->m_edges[j]->m_nodes[1]->m_x,g2.m_nodes[i]->m_edges[j]->m_nodes[1]->m_y);
	/// 
	
	
	glEnd();
	///mejores 
	glPointSize(5.0f);
	
	glBegin(GL_POINTS);
	/*cout<<mejores.size()<<endl;
	cout<<promedio.size()<<endl;*/
	for(int i=0;i<mejores.size();++i){
		glColor3f(1.0f,0.0f,1.0f);
		glVertex2f(-250+(i+1)*30,-250+mejores[i]*30);
		
	}
	
	for(int i=0;i<promedio.size();++i){
		glColor3f(0.0f,0.0f,1.0f);
		glVertex2f(-250+(i+1)*30,-250+promedio[i]*30);
	}
	
	glEnd();
	
	
	//puntos();
	glutSwapBuffers();
	// clear the draw buffer .
	//glClear(GL_COLOR_BUFFER_BIT);   // Erase everything
	
	// set the color to use in draw
	// create a polygon ( define the vertexs)
	
	
	//glFlush();
}

int main(int argc, char** argv) {
	srand(time(NULL));
	//creargrafo(10);
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(50, 50); //posicion de la ventana
	glutCreateWindow("Grafo"); //titulo de la ventana

	
	init_GL(); //funcion de inicializacion de OpenGL
	
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
	
	glutCreateWindow("Grafico");   // Create a window 2
	//init_GL(); //funcion de inicializacion de OpenGL
	
	glutDisplayFunc(display);  
	//glutReshapeFunc(&window_redraw);
	// Callback del teclado
	//glutIdleFunc(&idle);
	 // Register display callback
	
	//g2.printInit();
	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
