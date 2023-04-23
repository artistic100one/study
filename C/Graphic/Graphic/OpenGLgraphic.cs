using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Vector
{
    public class OpenGLgraphic
    {
        Vector UP;
        Vector E;
        Vector AT;

        private static double PHI = 3.141592;
        private static double EPS = 1.0e-10;
        private static double Big = 1.0e10;
        private static double INF = 1.0e60;
        private static double SHADING = 0.3;
        private static double SHADOW = 0.95;
        private static double HIGHRIGHT = 0.582;

        OpenGLgraphic()
        {
            UP = new Vector();
            E = new Vector();
            AT = new Vector();
        }

        OpenGLgraphic(Vector up, Vector e, Vector at)
        {
            UP = up;
            E = e;
            AT = at;
        }

        private int nx, ny;
        private double fovy;
        private double sdist;

        private Vector[][] FBuffer;
        private int FBready = 0;
        public Vector light;
        public Vector background;


        Vector U, V, W;
        Vector M1, M2, M3;

        public Vector EtoW(Vector eye)
        {
            return new Vector(M1 % eye, M2 % eye, M3 % eye);
        }

        public void Screen_Distance()
        {
            sdist = ny / (2 * Math.Tan(fovy * PHI / 360));
        }
    }
}
