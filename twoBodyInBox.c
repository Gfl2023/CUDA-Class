// gcc twoBodyInBox.c -o temp2 -lglut -lm -lGLU -lGL
//To stop hit "control c" in the window you launched it from.
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define XWindowSize 1000
#define YWindowSize 1000

#define STOP_TIME 10000.0
#define DT        0.0001

#define GRAVITY 2.1 

#define MASS 10.0  	
#define DIAMETER 1.0

#define SPRING_STRENGTH 5000.0
#define SPRING_REDUCTION 0.1

#define DAMP 0.0

#define DRAW 100

#define LENGTH_OF_BOX 6.0
#define MAX_VELOCITY 5.0

#define N_O_S 3

const float XMax = (LENGTH_OF_BOX/2.0);
const float YMax = (LENGTH_OF_BOX/2.0);
const float ZMax = (LENGTH_OF_BOX/2.0);
const float XMin = -(LENGTH_OF_BOX/2.0);
const float YMin = -(LENGTH_OF_BOX/2.0);
const float ZMin = -(LENGTH_OF_BOX/2.0);

// Globals


typedef struct sphereStruct 
{
	float px,py,pz; 
	float vx,vy,vz;
	float gx,gy,gz;
	float mass;
}sphereStruct;

sphereStruct *sphereCPU;

void set_initail_conditions()
{ 
	time_t t;
	srand((unsigned) time(&t));
	int yeahBuddy;
	int i = 0;
	int j = 0;
	float dx, dy, dz, seperation;
	
	if(i = 0)
	{
		sphereCPU[i].px = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
		sphereCPU[i].py = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
		sphereCPU[i].pz = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
	}

	i++;
	
		while(i < N_O_S) 
		{
			sphereCPU[i].px = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
			sphereCPU[i].py = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
			sphereCPU[i].pz = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
	
			j = i;
		
			while(j>=0)
			{
				j--;
				yeahBuddy = 0;
					while(yeahBuddy == 0)
					{
						sphereCPU[j].px = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
						sphereCPU[j].py = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
						sphereCPU[j].pz = (LENGTH_OF_BOX - DIAMETER)*rand()/RAND_MAX - (LENGTH_OF_BOX - DIAMETER)/2.0;
		
						dx = sphereCPU[j].px - sphereCPU[i].px;
						dy = sphereCPU[j].py - sphereCPU[i].py;
						dz = sphereCPU[j].pz - sphereCPU[i].pz;
						seperation = sqrt(dx*dx + dy*dy + dz*dz);
						yeahBuddy = 1;
						if(seperation < DIAMETER) yeahBuddy = 0;
					}
			}	
			i++;
		}	

						i = 0;
							while(i<N_O_S)
								{
								sphereCPU[i].vx = 2.0*MAX_VELOCITY*rand()/RAND_MAX - MAX_VELOCITY;
								sphereCPU[i].vy = 2.0*MAX_VELOCITY*rand()/RAND_MAX - MAX_VELOCITY;
								sphereCPU[i].vz = 2.0*MAX_VELOCITY*rand()/RAND_MAX - MAX_VELOCITY;
								sphereCPU[i].mass = 1.0;
								i++;
								}
}
void Drawwirebox()
{		
	glColor3f (5.0,1.0,1.0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(XMax,YMax,ZMax);
		glVertex3f(XMax,YMax,ZMin);	
		glVertex3f(XMax,YMin,ZMin);
		glVertex3f(XMax,YMin,ZMax);
		glVertex3f(XMax,YMax,ZMax);
		
		glVertex3f(XMin,YMax,ZMax);
		
		glVertex3f(XMin,YMax,ZMax);
		glVertex3f(XMin,YMax,ZMin);	
		glVertex3f(XMin,YMin,ZMin);
		glVertex3f(XMin,YMin,ZMax);
		glVertex3f(XMin,YMax,ZMax);	
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(XMin,YMin,ZMax);
		glVertex3f(XMax,YMin,ZMax);		
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(XMin,YMin,ZMin);
		glVertex3f(XMax,YMin,ZMin);		
	glEnd();
	
	glBegin(GL_LINES);
		glVertex3f(XMin,YMax,ZMin);
		glVertex3f(XMax,YMax,ZMin);		
	glEnd();
	
}

void draw_picture()
{
	float radius = DIAMETER/2.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	Drawwirebox();
	for(int i = 0; i < N_O_S; i++)
	{
		glColor3d(.15,0.65,.30);
		glPushMatrix();
		glTranslatef(sphereCPU[i].px, sphereCPU[i].py, sphereCPU[i].pz);
		glutSolidSphere(radius,20,20);
		glPopMatrix();
	}
	
	
	glutSwapBuffers();
}

void keep_in_box()
{
	float halfBoxLength = (LENGTH_OF_BOX - DIAMETER)/2.0;
	
	
	for(int i = 0; i < N_O_S; i++)
	{
		if(sphereCPU[i].px > halfBoxLength)
		{
			sphereCPU[i].px = 2.0*halfBoxLength - sphereCPU[i].px;
			sphereCPU[i].vx = - sphereCPU[i].vx;
		}
		else if(sphereCPU[i].px < -halfBoxLength)
		{
			sphereCPU[i].px = -2.0*halfBoxLength - sphereCPU[i].px;
			sphereCPU[i].vx = - sphereCPU[i].vx;
		}
		
		if(sphereCPU[i].py > halfBoxLength)
		{
			sphereCPU[i].py = 2.0*halfBoxLength - sphereCPU[i].py;
			sphereCPU[i].vy = - sphereCPU[i].vy;
		}
		else if(sphereCPU[i].py < -halfBoxLength)
		{
			sphereCPU[i].py = -2.0*halfBoxLength - sphereCPU[i].py;
			sphereCPU[i].vy = - sphereCPU[i].vy;
		}
				
		if(sphereCPU[i].pz > halfBoxLength)
		{
			sphereCPU[i].pz = 2.0*halfBoxLength - sphereCPU[i].pz;
			sphereCPU[i].vz = - sphereCPU[i].vz;
		}
		else if(sphereCPU[i].pz < -halfBoxLength)
		{
			sphereCPU[i].pz = -2.0*halfBoxLength - sphereCPU[i].pz;
			sphereCPU[i].vz = - sphereCPU[i].vz;
		}
	
	}
	
		
}

void get_forces()
{
	float dx,dy,dz,r,r2,dvx,dvy,dvz,forceMag,inout;
	for(int i = 0; i < N_O_S-1 ; i++)
	{
		for(int j = 0; j < N_O_S; j++)
	{
	

	dx = sphereCPU[j].px - sphereCPU[i].px;
	dy = sphereCPU[j].py - sphereCPU[i].py;
	dz = sphereCPU[j].pz - sphereCPU[i].pz;
				
	r2 = dx*dx + dy*dy + dz*dz;
	r = sqrt(r2);
		
			forceMag =  sphereCPU[i].mass*sphereCPU[j].mass*GRAVITY/r2;
	

	
	if (r < DIAMETER)
	{
		dvx = sphereCPU[j].vx - sphereCPU[i].vx;
		dvy = sphereCPU[j].vy - sphereCPU[i].vy;
		dvz = sphereCPU[j].vz - sphereCPU[i].vz;
		inout = dx*dvx + dy*dvy + dz*dvz;
		if(inout <= 0.0)
		{
			forceMag +=  SPRING_STRENGTH*(r - DIAMETER);
		}
		else
		{
			forceMag +=  SPRING_REDUCTION*SPRING_STRENGTH*(r - DIAMETER);
		}
	}

	sphereCPU[i].gx = forceMag*dx/r;
	sphereCPU[i].gy = forceMag*dy/r;
	sphereCPU[i].gz = forceMag*dz/r;
	sphereCPU[j].gx = -forceMag*dx/r;
	sphereCPU[j].gy = -forceMag*dy/r;
	sphereCPU[j].gz = -forceMag*dz/r;
	}
	}
}

void move_bodies(float time)
{
	if(time == 0.0)
	{
		for(int i = 0; i < N_O_S; i++)
		{
			sphereCPU[i].vx += 0.5*DT*(sphereCPU[i].gx - DAMP*sphereCPU[i].vx)/sphereCPU[i].mass;
			sphereCPU[i].vy += 0.5*DT*(sphereCPU[i].gy - DAMP*sphereCPU[i].vy)/sphereCPU[i].mass;
			sphereCPU[i].vz += 0.5*DT*(sphereCPU[i].gz - DAMP*sphereCPU[i].vz)/sphereCPU[i].mass;
		}
	}
	else
	{
		for(int i = 0; i < N_O_S; i++)
		{
			sphereCPU[i].vx += DT*(sphereCPU[i].gx - DAMP*sphereCPU[i].vx)/sphereCPU[i].mass;
			sphereCPU[i].vy += DT*(sphereCPU[i].gy - DAMP*sphereCPU[i].vy)/sphereCPU[i].mass;
			sphereCPU[i].vz += DT*(sphereCPU[i].gz - DAMP*sphereCPU[i].vz)/sphereCPU[i].mass;
		}
		
	}
		for(int i = 0; i < N_O_S; i++)
		{
			sphereCPU[i].px += DT*sphereCPU[i].vx;
			sphereCPU[i].py += DT*sphereCPU[i].vy;
			sphereCPU[i].pz += DT*sphereCPU[i].vz;
		}
			keep_in_box();
}

void nbody()
{	
	int    tdraw = 0;
	float  time = 0.0;

	set_initail_conditions();
	
	draw_picture();
	
	while(time < STOP_TIME)
	{
		get_forces();
	
		move_bodies(time);
	
		tdraw++;
		if(tdraw == DRAW) 
		{
			draw_picture(); 
			tdraw = 0;
		}
		
		time += DT;
	}
	printf("\n DONE \n");
	while(1);
}

void Display(void)
{
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutSwapBuffers();
	glFlush();
	nbody();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glFrustum(-0.2, 0.2, -0.2, 0.2, 0.2, 50.0);

	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{

	sphereCPU= (sphereStruct*)malloc(N_O_S*sizeof(sphereStruct));
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(XWindowSize,YWindowSize);
	glutInitWindowPosition(0,0);
	glutCreateWindow("2 Body 3D");
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	GLfloat light_ambient[]  = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat mat_specular[]   = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[]  = {10.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


