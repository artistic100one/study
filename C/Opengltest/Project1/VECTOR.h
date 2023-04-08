#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int zero(double a) { return a == 0; }
int alzero(double a) { return a <= 1.0e-60; }
double EPS = 1.0e-10;
#define INF 1.0e60

class vector {
public :
	double x, y, z;
	vector() { x = y = z = 0.0; }
	vector(double a, double b, double c) { x = a; y = b; z = c; }
	void print() { printf("(%lf, %lf, %lf)", x, y, z); }

	vector operator *(vector b) { return vector(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x); }
	double operator %(vector b) { return x*b.x + y*b.y + z*b.z; }
	vector operator +(vector b) { return vector(x+b.x, y+b.y, z+b.z); }
	vector operator -(vector b) { return vector(x - b.x, y - b.y, z - b.z); }
	vector operator &(vector b) { return vector(x * b.x, y * b.y, z * b.z); }
	int operator |(vector b) { return alzero(((*this)*b).snorm()); }
	vector operator /(vector b) { 
		if (b.x*b.y*b.z != 0) return vector(x / b.x, y / b.y, z / b.z);
		return vector(b.x == 0? 0:x/b.x, b.y == 0 ? 0 : y / b.y, b.z == 0 ? 0 : z / b.z); 
	}

	vector operator -() { return vector(-x, -y, -z); }
	vector operator !() { return vector(x == 0 ? 1 : 0, y == 0 ? 1 : 0, z == 0 ? 1 : 0); }
	vector operator +(double s) { return vector(x + s, y + s, z + s); }
	vector operator -(double s) { return vector(x - s, y - s, z - s); }
	vector operator *(double s) { return vector(x * s, y * s, z * s); }
	vector operator /(double s) { if (s != 0) return vector(x / s, y / s, z / s); return vector(0, 0, 0); }
	
	int operator ==(vector b) { return ((*this)-b).norm2() < EPS; }

	vector operator +=(vector b) { *this = (*this) + b; return (*this); }
	vector operator -=(vector b) { *this = (*this) - b; return (*this); }
	vector operator &=(vector b) { *this = (*this) & b; return (*this); }

	vector operator +=(double s) { *this = (*this) + s; return (*this); }
	vector operator -=(double s) { *this = (*this) - s; return (*this); }
	vector operator *=(double s) { *this = (*this) * s; return (*this); }
	vector operator /=(double s) { *this = (*this) / s; return (*this); }
	
	double snorm() { return x*x + y*y + z*z; }
	double norm2() { return x*x + y*y + z*z; }
	double norm() { return sqrt(this->norm2()); }

	vector unit()  { return (*this) / this->norm(); }

	int zero(double TOL) { return (this->snorm() <= TOL); }

	vector gradient();
};

typedef struct {
	double co[5];
	double al, be, ga;
	
	int n;
} vect5;

vector vmax(vector a, vector b) {
	a.x = a.x > b.x ? a.x : b.x;
	a.y = a.y > b.y ? a.y : b.y;
	a.z = a.z > b.z ? a.z : b.z;
	return a;
}

vector vmin(vector a, vector b) {
	a.x = a.x < b.x ? a.x : b.x;
	a.y = a.y < b.y ? a.y : b.y;
	a.z = a.z < b.z ? a.z : b.z;
	return a;
}

vector vzero = vector(0, 0, 0);
vector BLACK = vector(0, 0, 0);
vector BLUE = vector(0, 0, 1);
vector GREEN = vector(0, 1, 0);
vector RED = vector(1, 0, 0);
vector YELLOW = vector(1, 1, 0);
vector MAGENTA = vector(1, 0, 1);
vector CYAN = vector(0, 1, 1);
vector WHITE = vector(1, 1, 1);

vector GRAY = vector(0.5, 0.5, 0.5);
vector SKY = vector(0.1, 0.1, 0.4);
vector BRUGUN = vector(0.4, 0.0, 0.0);

vector VMAX = vector(INF, INF, INF);
vector VMIN = vector(-INF, -INF, -INF);