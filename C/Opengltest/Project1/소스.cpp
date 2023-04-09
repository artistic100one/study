#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "VECTOR.h"

/***
	static value setting
*/

#define PHI 3.141592
#define EPS 1.0e-10
#define BIG 1.0e10
#define INF 1.0e60
#define SHADING 0.3
#define SHADOW 0.95
#define HIGHRIGHT 0.582

#define bigger(a, b) ((a) > (b) ? (a) : (b))
#define smaller(a, b) ((a) < (b) ? (a) : (b))

/*
	ray struct
	o is one's position
	v is a vector of course
	t is a distance how far from once
	p is a vector of target's position
	obj_id is a object id
*/

typedef struct {
	vector o;
	vector v;
	double t;
	vector p; // = o + v*t;
	int obj_id;
} ray;

vector UP;
vector E;
vector AT;

int nx, ny;
double fovy;
double sdist;

vector **FB;
int FBready = 0;
vector light;
vector   background;

/*
	object ball's struct
	c is center of ball
	r is ball's radius
	refl is reflection coefficient value
	alpha is 
	eta is refraction's value
	color is RGB value

	hitt is ray's t hit position
	hitt is ray's p hit position
*/

typedef struct {
	vector c;
	double r;

	double refl;
	double alpha;
	double eta;
	vector color;

	double   hitt;
	vector   hitp;

} object_ball;


//typedef struct {
//   vector c;
//   vector r_x;
//
//} object_table;

int nobj;
object_ball OBJ_BALL[100];
//object_table OBJ_TABLE[2];

vector U, V, W;
vector M1, M2, M3;

/*
	Viewing Transformation is rescale and transformation
*/

void Viewing_Transformation() {
	vector u, v, w;
	w = E - AT;
	u = UP * w;
	v = w * u;

	U = u.unit();
	V = v.unit();
	W = w.unit();

	M1 = vector(U.x, V.x, W.x);
	M2 = vector(U.y, V.y, W.y);
	M3 = vector(U.z, V.z, W.z);

}

/*
	change dimension Eye to window
*/

vector EtoW(vector eye) {
	return vector(M1 % eye, M2 % eye, M3 % eye);
}

void Screen_Distance() {
	sdist = ny / (2 * tan(fovy * PHI / 360));
}

/*
	reflect law
*/

vector reflected(vector I, vector N)
{
	return I - N*(2 * (I%N) / (N%N));
}

/*
	refract law
*/

vector refracted(vector I, vector N, double eta1, double eta2) {
	double NI = I%N;
	double eta = eta1 / eta2;
	return (I - N * NI) * eta - N * (sqrt(1 - eta * eta * (1 - NI * NI)));
}

/*
	make shade from light
*/

vector shade(int id) {
	vector color, p, N, L;
	double   alpha;

	p = OBJ_BALL[id].hitp;
	L = light - p;
	N = p - OBJ_BALL[id].c;
	double co = L % N / (L.norm() * N.norm());
	alpha = (1 - co) / 2 * SHADING + (co + 1) / 2;

	return   OBJ_BALL[id].color * alpha;
}

vector background_color() {
	return WHITE;
}

/*
	find first hit object's point t
*/

double first_hit_of_t(object_ball ob, ray r) {
	double a, b, c, d, t;
	vector oc;

	oc = r.o - ob.c;

	a = r.v % r.v;
	b = r.v % oc;
	c = oc % oc - (ob.r*ob.r);

	d = b*b - a*c;

	if (d < 0) return -1;
	d = sqrt(d);
	t = (-b - d) / a;
	return t;
}

/*
	find first hit object id
*/

int first_hit_id(ray r) {

	object_ball ob;
	vector   oc;
	double t_loc = INF;
	double a, b, c, d, t;
	int id = -1;

	for (int i = 0; i < nobj; i++) {
		ob = OBJ_BALL[i];
		t = first_hit_of_t(ob, r);

		if (t > 0 && t < t_loc) {
			t_loc = t;
			id = i;
		}
	}
	if (id<0) return -1;
	OBJ_BALL[id].hitt = t_loc;
	OBJ_BALL[id].hitp = r.o + r.v * t_loc;

	return id;
}

/*
	check hit object from pixel
*/

double ray_hit_obj(ray sr, int id) {
	double a, b, c, d;
	object_ball	ob;
	vector oc;

	ob = OBJ_BALL[id];
	oc = sr.o - ob.c;

	a = sr.v % sr.v;
	b = sr.v % oc;
	c = oc % oc - (ob.r*ob.r);
	d = b*b - a*c;

	if (d < 0) return -1;
	d = sqrt(d);
	return	(-b - d) / a;

}

/*
	shadow is checking block the others
*/

int shadow(int id)// check if an obstacle id<->light
{
	ray	sr;
	double t;

	sr.o = light;
	sr.v = OBJ_BALL[id].hitp - light;

	for (int j = 0; j < nobj; j++) {
		t = ray_hit_obj(sr, j);
		if (t > 0 && t < 1 - EPS) return 1;
	}
	return 0;
}
 
/*
	highlight is object's most lighting point
*/

double	highlight(int id)// hl at P of obj
{
	vector I, N, R, P, PE;
	vector	color;
	object_ball ob = OBJ_BALL[id];
	double co;

	P = ob.hitp;
	I = P - light;
	N = P - ob.c;
	PE = E - P;
	R = reflected(I, N);
	co = (R % PE) / (R.norm() * PE.norm());

	co = pow(co, 10);

	return  co<HIGHRIGHT ? 1 : (1 - co) / (1 - HIGHRIGHT);
}

/*
	get color from the ray
	color, shade, shadow, reflect, refract, highlight
*/

vector getColor_of_the_Ray(ray r) {
	vector color;
	int   id;
	double hl;

	id = first_hit_id(r);
	if (id < 0) return background_color();

	color = shade(id);
	if (shadow(id)) return color*SHADOW;
	//reflected();      
	//refracted();
	hl = highlight(id);
	return (color*hl) + (WHITE * (1 - hl));
}

/*
	main raycasting
*/

void RayCast() {
	vector pixel;
	vector color;
	ray ray;
	printf("Ray casting ...\n");

	pixel.z = -sdist;
	pixel.y = ny / 2 - 0.5;
	ray.o = E;

	for (int i = ny; i > 0; i--)
	{
		glBegin(GL_POINTS);

		pixel.x = -nx / 2 + 0.5;
		for (int j = 0; j < nx; j++) {
			ray.v = EtoW(pixel);

			color = getColor_of_the_Ray(ray);
			glColor3d(color.x, color.y, color.z);
			glVertex2i(j, i);
			FB[i][j] = color;
			pixel.x++;
		}
		glEnd();
		glFlush();

		pixel.y--;
	}
	FBready = 1;
}

/*
	scanning data from data text
*/

void myInit() {
	FILE *fd;
	char *fn;
	vector p;
	double r;
	double refl;
	double alpha;
	double eta;

	fn = "DATA.txt";

	fd = fopen(fn, "r");
	if (fd <= 0) {
		printf("\7file open error:[%s]\n", fn);
		exit(0);
	}
	fscanf(fd, "%lf %lf %lf", &p.x, &p.y, &p.z); E = p;
	fscanf(fd, "%lf %lf %lf", &p.x, &p.y, &p.z); AT = p;
	fscanf(fd, "%lf %lf %lf", &p.x, &p.y, &p.z); UP = p;
	fscanf(fd, "%d %d", &nx, &ny);
	fscanf(fd, "%lf", &fovy);
	fscanf(fd, "%lf %lf %lf", &p.x, &p.y, &p.z); light = p;

	fscanf(fd, "%d", &nobj);

	object_ball ball;
	for (int i = 0; i < nobj; i++) {
		fscanf(fd, "%lf %lf %lf", &p.x, &p.y, &p.z); ball.c = p;
		fscanf(fd, "%lf", &ball.r);
		fscanf(fd, "%lf", &ball.refl);
		fscanf(fd, "%lf", &ball.alpha);
		fscanf(fd, "%lf", &ball.eta);
		fscanf(fd, "%lf %lf %lf", &p.x, &p.y, &p.z); ball.color = p;
		OBJ_BALL[i] = ball;
	}
	fclose(fd);

	Viewing_Transformation();
	Screen_Distance();

	FB = new vector*[ny + 1];
	for (int i = 0; i <= ny; i++)
		FB[i] = new vector[nx + 1];

	FBready = 0;

	//   printf("nobj=%d\n", nobj);
	//   for (int i = 0; i < nobj; i++) {
	//      ball = OBJ_BALL[i];
	//      p = ball.c;
	//      printf("c=(%lf %lf %lf) ", p.x, p.y, p.z);
	//      printf("r=%lf ", ball.r);
	//      p = ball.color;
	//      printf("color=(%lf %lf %lf)\n", p.x, p.y, p.z);
	//
	//   }

}

/*
	display uses opengl
*/

void display() {
	vector c;

	if (!FBready) RayCast();

	glBegin(GL_POINTS);
	for (int i = ny; i > 0; i--)
		for (int j = 0; j < nx; j++)
		{
			c = FB[i][j];
			glColor3d(c.x, c.y, c.z);
			glVertex2i(j, i);
		}
	glEnd();
	glFlush();
}

/*
	program main stream
*/

int   main(int ac, char **av)
{
	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	myInit();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(nx + 1, ny + 1);
	glutCreateWindow("Ray Tracing");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, nx, 0, ny, -1, 1);

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;

}