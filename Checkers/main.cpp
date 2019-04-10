#include <iostream>
#include <GL/glut.h>
#include<math.h> 
#define pi 3.142857 
#include "damas.cpp" 
using namespace std;

#define ROWS 8
#define COLS 8
#define EMPTY 0
#define RED 1
#define BLACK 2
#define REDKING 3
#define BLACKKING 4 

min_max* game1;
void whiteBox(int x, int y){
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.9f, 0.8f);
	glVertex2i(x, y);
	glVertex2i(x, y + 80);
	glVertex2i(x + 80, y + 80);
	glVertex2i(x + 80, y);
	glEnd();
}

void blackBox(int x, int y){
	glBegin(GL_POLYGON);
	glColor3f(0.6f, 0.4f, 0.3f);
	glVertex2i(x, y);
	glVertex2i(x, y + 80);
	glVertex2i(x + 80, y + 80);
	glVertex2i(x + 80, y);
	glEnd();
}

void drawCircle(int i , int j , int c){
	glBegin(GL_POLYGON);
	if (c==1) glColor3f(1.0f, 1.0f, 1.0f);
	if (c==2) glColor3f(0.0f, 0.0f, 0.0f);
	if (c==3) glColor3f(0.0f, 1.0f, 1.0f);
	if (c==4) glColor3f(0.0f, 1.0f, 1.0f);
	
	float x, y, i1; 
	
	for ( i1 = 0; i1 < (2 * pi); i1 += 0.001){ 
		x = 40+80*j +30 * -1*cos(i1); 
		y = 40+80*i+ 30* -1*sin(i1); 
		glVertex2i(x, y); 
	} 
	
	glEnd(); 
	
	
}
vector < vector<short> > d ={{0,2,0,2,0,2,0,2},
							{2,0,2,0,2,0,2,0}, 
							{0,2,0,2,0,2,0,2}, 
							{0,0,0,0,0,0,0,0}, 
							{0,0,0,0,0,0,0,0}, 
							{1,0,1,0,1,0,1,0},
							{0,1,0,1,0,1,0,1},
							{1,0,1,0,1,0,1,0}};

int player = 1;
bool valida = 0;
void move(int x0, int y0, int x1, int y1){//verificia si el movimiento que se quiere realizar es posible
	int num;
	(player == 1) ? num = -1 : num = 1;
	int i = y0/80, j = x0/80, p = x1/80, q = y1/80;
	if (d[i][j] == player) {
		//cout<<"Turno: Jugador "<<player<<endl;
		if ((d[i + num][j-1] == 0 && (i + num)== q && (j - 1)==p )||(d[i + num][j+1] == 0 && 
			(i + num)== q && (j + 1)==p)){//diagonal negra o blanca
			int c = d[i][j] ;
			d[i][j] = 0;
			d[q][p] = c;
			(player == 1) ? player = 2 : player = 1;
			valida = 1;
		}
		else if ( j + 2 < 8 && d[i + num][j + 1] != player && d[i + num][j + 1] != 0 && 
			d[i + 2*num][j + 2] == 0 && (i+2*num) == q && (j+2) == p){//comer negra
			int c = d[i][j] ;
			d[i][j] = 0;
			d[q][p] = c;
			d[i + num][j + 1] = 0;
			(player == 1) ? player = 2 : player = 1;
			valida = 1;
			
		}
		else if(j - 2 > 0 && d[i + num][j - 1] != player && d[i + num][j - 1] != 0 && 
			d[i + 2*num][j - 2] == 0 && (i + 2*num) == q && (j-2) == p){//comer blanca
				int c = d[i][j] ;
				d[i][j] = 0;
				d[q][p] = c;
				d[i + num][j -1] = 0;
				(player == 1) ? player = 2 : player = 1;
				valida = 1;
			}
			
		else{
				cout<<"Jugada no valida : Vuelva a intentarlo"<<endl;
				cout<<"Turno del jugador : "<<player<<endl;
				valida = 0;
			}
	}
	
	else cout<<"No puede mover esta ficha no es su turno"<<endl;
	
}

void displayCircle(){
	glPushMatrix(); 
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if(d[i][j] != 0){
				drawCircle(i,j,d[i][j]);
			}
		}
	}
	glPopMatrix();
}

void myDisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
	
	int x1 = 0, y1=0;
	
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			x1 = 80*i;
			y1 = 80*j;
			if((i+j) % 2 == 0) whiteBox(x1, y1);
			else blackBox(x1, y1);
		}
	}
	displayCircle();
	
	glFlush();
	
}

int count = 0;
int x0,y0;
int jugador = 1;
void OnMouseClick(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		count++;
		cout<<count<<endl;
		if(count == 2){
			
			move(x0,y0,x,y);
			count =0;
			//cout<<"Moviendo "<<endl;
			//cout<<x<<"  "<<y<<endl;
			if(valida){
			d = game1->play(d);
			(jugador == 1) ? jugador = 2 : jugador = 1;
			cout<<"Turno del jugador :"<<jugador<<endl;
			}
			
		}
		if(count == 1){
			x0 = x;
			y0 = y;
			//cout<<x<<"  "<<y<<endl;
		}
		
	}
}

void myInit(){
	cout<<"Turno del jugador :"<<player<<endl;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,640,640,0);
	
}

int main(int argc, char** argv){
	game1 = new min_max;
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Checkers");
	
	glutMouseFunc(&OnMouseClick);
	glutDisplayFunc(myDisplay);
	
	myInit();
	glutMainLoop();
	return 0;
}
