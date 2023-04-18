using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class Vector {
    private double x, y, z;
    public static bool alzero(double a) { return a <= 1.0e-60; }
    public static double EPS = 1.0e-10;
    public static double INF = 1.0e60;

    public Vector()
    {
        x = 0.0; y = 0.0; z = 0.0;
    }

    public Vector(double x, double y, double z)
    {
        this.x = x; this.y = y; this.z = z;
    }

    public static Vector operator +(Vector a, Vector b) { return new Vector(a.x + b.x, a.y + b.y, a.z + b.z); }
    public static Vector operator -(Vector a, Vector b) { return new Vector(a.x - b.x, a.y - b.y, a.z - b.z); }
    public static Vector operator *(Vector a, Vector b) { return new Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
    public static Vector operator &(Vector a, Vector b) { return new Vector(a.x * b.x, a.y * b.y, a.z * b.z); }
    public static Vector operator /(Vector a, Vector b) {
        if(b.x*b.y*b.z != 0) return new Vector(a.x / b.x, a.y / b.y, a.z / b.z);
        return new Vector(b.x == 0? 0:a.x / b.x, b.y == 0 ? 0 : a.y / b.y, b.z == 0 ? 0 : a.z / b.z);
    }

    public static Vector operator -(Vector a) { return new Vector(-a.x, -a.y, -a.z); }
    public static Vector operator !(Vector a) { return new Vector(a.x == 0 ? 1 : 0, a.y == 0 ? 1 : 0, a.z == 0 ? 1 : 0); }

    public static Vector operator +(Vector a, double s) { return new Vector(a.x + s, a.y + s, a.z + s); }
    public static Vector operator -(Vector a, double s) { return new Vector(a.x - s, a.y - s, a.z - s); }
    public static Vector operator *(Vector a, double s) { return new Vector(a.x * s, a.y * s, a.z * s); }
    public static Vector operator /(Vector a, double s) { if (s != 0) return new Vector(a.x / s, a.y / s, a.z / s);return new Vector(0, 0, 0); }

    public static bool operator ==(Vector a, Vector b) { return (a - b).norm2() < EPS; }
    public static bool operator !=(Vector a, Vector b) { return (a - b).norm2() > EPS; }



    public double snorm() { return this.x * this.x + this.y * this.y + this.z * this.z; }
    public double norm2() { return this.x * this.x + this.y * this.y + this.z * this.z; }
    public double norm() { return Math.Sqrt(this.norm2()); }

    public Vector unit() { return this / this.norm(); }

    public bool zero(double TOL) { return this.snorm() <= TOL; }

    public static Vector vmax(Vector a, Vector b)
    {
        a.x = a.x > b.x ? a.x : b.x;
        a.y = a.y > b.y ? a.y : b.y;
        a.z = a.z > b.z ? a.z : b.z;
        return a;
    }

    public static Vector vmin(Vector a, Vector b)
    {
        a.x = a.x < b.x ? a.x : b.x;
        a.y = a.y < b.y ? a.y : b.y;
        a.z = a.z < b.z ? a.z : b.z;
        return a;
    }

    public static Vector vzero = new Vector(0, 0, 0);
    public static Vector BLACK = new Vector(0, 0, 0);
    public static Vector BLUE = new Vector(0, 0, 1);
    public static Vector GREEN = new Vector(0, 1, 0);
    public static Vector RED = new Vector(1, 0, 0);
    public static Vector YELLOW = new Vector(1, 1, 0);
    public static Vector MAGENTA = new Vector(1, 0, 1);
    public static Vector CYAN = new Vector(0, 1, 1);
    public static Vector WHITE = new Vector(1, 1, 1);

    public static Vector GRAY = new Vector(0.5, 0.5, 0.5);
    public static Vector SKY = new Vector(0.1, 0.1, 0.4);
    public static Vector BRUGUN = new Vector(0.4, 0.0, 0.0);

    public static Vector VMAX = new Vector(INF, INF, INF);
    public static Vector VMIN = new Vector(-INF, -INF, -INF);
}

