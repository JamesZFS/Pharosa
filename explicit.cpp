#include <cmath>   // smallpt, a Path Tracer by Kevin Beason, 2009
#include <cstdlib> // Make : g++ -O3 -fopenmp explicit.cpp -o explicit
#include <cstdio>  //        Remove "-fopenmp" for g++ version < 4.2

struct Vec
{        // Usage: time ./explicit 16 && xv image.ppm
	double x, y, z;                  // position, also color (r,g,b)
	Vec(double x_ = 0, double y_ = 0, double z_ = 0)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	Vec operator+(const Vec &b) const
	{ return Vec(x + b.x, y + b.y, z + b.z); }

	Vec operator-(const Vec &b) const
	{ return Vec(x - b.x, y - b.y, z - b.z); }

	Vec operator*(double b) const
	{ return Vec(x * b, y * b, z * b); }

	Vec mult(const Vec &b) const
	{ return Vec(x * b.x, y * b.y, z * b.z); }

	Vec &norm()
	{ return *this = *this * (1 / sqrt(x * x + y * y + z * z)); }

	double dot(const Vec &b) const
	{ return x * b.x + y * b.y + z * b.z; } // cross:
	Vec operator%(Vec &b)
	{ return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x); }
};

struct Ray
{
	Vec o, d;

	Ray(Vec o_, Vec d_) : o(o_), d(d_)
	{}
};

enum Refl_t
{
	DIFF, SPEC, REFR
};  // material types, used in radiance()
struct Sphere
{
	double rad;       // radius
	Vec p, e, c;      // position, emission, color
	Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive)
	Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_) :
			rad(rad_), p(p_), e(e_), c(c_), refl(refl_)
	{}

	double intersect(const Ray &r) const
	{ // returns distance, 0 if nohit
		Vec op = p - r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
		double t, eps = 1e-4, b = op.dot(r.d), det = b * b - op.dot(op) + rad * rad;
		if (det < 0) return 0; else det = sqrt(det);
		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
	}
};

//Sphere spheres[] = {//Scene: radius, position, emission, color, material
//		Sphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(), Vec(.75, .25, .25), DIFF),//Left
//		Sphere(1e5, Vec(-1e5 + 99, 40.8, 81.6), Vec(), Vec(.25, .25, .75), DIFF),//Rght
//		Sphere(1e5, Vec(50, 40.8, 1e5), Vec(), Vec(.75, .75, .75), DIFF),//Back
//		Sphere(1e5, Vec(50, 40.8, -1e5 + 170), Vec(), Vec(), DIFF),//Frnt
//		Sphere(1e5, Vec(50, 1e5, 81.6), Vec(), Vec(.75, .75, .75), DIFF),//Botm
//		Sphere(1e5, Vec(50, -1e5 + 81.6, 81.6), Vec(), Vec(.75, .75, .75), DIFF),//Top
//		Sphere(16.5, Vec(27, 16.5, 47), Vec(), Vec(1, 1, 1) * .999, SPEC),//Mirr
//		Sphere(16.5, Vec(73, 16.5, 78), Vec(), Vec(1, 1, 1) * .999, REFR),//Glas
//		Sphere(1.5, Vec(50, 81.6 - 16.5, 81.6), Vec(4, 4, 4) * 100, Vec(), DIFF),//Lite
//};

// forest
//Vec tc(0.0588, 0.361, 0.0941);
//Vec sc = Vec(1,1,1)*.7;
//Sphere spheres[] = {//Scene: radius, position, emission, color, material
//		// center 50 40.8 62
//		// floor 0
//		// back  0
////  Sphere(1e5, Vec(50, 1e5+100, 0),  Vec(1,1,1)*1,Vec(),DIFF), //lite
////  Sphere(1e5, Vec(50, -1e5, 0),  Vec(),Vec(.3,.3,.1),DIFF), //grnd
////  Sphere(1e5, Vec(50, 1e5+100, 0),  Vec(0.761, 0.875, 1.00)*1.3,Vec(),DIFF),
////  //lite
//		Sphere(1e5, Vec(50, 1e5+130, 0),  Vec(1,1,1)*1.3,Vec(),DIFF), //lite
//		Sphere(1e2, Vec(50, -1e2+2, 47),  Vec(),Vec(1,1,1)*.7,DIFF), //grnd
//
//		Sphere(1e4, Vec(50, -30, 300)+Vec(-sin(50*M_PI/180),0,cos(50*M_PI/180))*1e4, Vec(), Vec(1,1,1)*.99,SPEC),// mirr L
//		Sphere(1e4, Vec(50, -30, 300)+Vec(sin(50*M_PI/180),0,cos(50*M_PI/180))*1e4,  Vec(), Vec(1,1,1)*.99,SPEC),// mirr R
//		Sphere(1e4, Vec(50, -30, -50)+Vec(-sin(30*M_PI/180),0,-cos(30*M_PI/180))*1e4,Vec(), Vec(1,1,1)*.99,SPEC),// mirr FL
//		Sphere(1e4, Vec(50, -30, -50)+Vec(sin(30*M_PI/180),0,-cos(30*M_PI/180))*1e4, Vec(), Vec(1,1,1)*.99,SPEC),// mirr
//
//
//		Sphere(4, Vec(50,6*.6,47),   Vec(),Vec(.13,.066,.033), DIFF),//"tree"
//		Sphere(16,Vec(50,6*2+16*.6,47),   Vec(), tc,  DIFF),//"tree"
//		Sphere(11,Vec(50,6*2+16*.6*2+11*.6,47),   Vec(), tc,  DIFF),//"tree"
//		Sphere(7, Vec(50,6*2+16*.6*2+11*.6*2+7*.6,47),   Vec(), tc,  DIFF),//"tree"
//
//		Sphere(15.5,Vec(50,1.8+6*2+16*.6,47),   Vec(), sc,  DIFF),//"tree"
//		Sphere(10.5,Vec(50,1.8+6*2+16*.6*2+11*.6,47),   Vec(), sc,  DIFF),//"tree"
//		Sphere(6.5, Vec(50,1.8+6*2+16*.6*2+11*.6*2+7*.6,47),   Vec(), sc,  DIFF),//"tree"
//};

//// wada2
////double R=60;
//double R=120;     // radius
//double T=30*M_PI/180.;
//double D=R/cos(T);     //distance
//// double D=60;     //distance
//// double R=D*sqrt(2);
//double Z=62;
//Vec C=Vec(0.275, 0.612, 0.949);
//Sphere spheres[] = {//Scene: radius, position, emission, color, material
//
//		Sphere(R, Vec(50,28,Z)+Vec( cos(T),sin(T),0)*D,    C*6e-2,Vec(1,1,1)*.996, SPEC), //red
//		Sphere(R, Vec(50,28,Z)+Vec(-cos(T),sin(T),0)*D,    C*6e-2,Vec(1,1,1)*.996, SPEC), //grn
//		Sphere(R, Vec(50,28,Z)+Vec(0,-1,0)*D,              C*6e-2,Vec(1,1,1)*.996, SPEC), //blue
//		Sphere(R, Vec(50,28,Z)+Vec(0,0,-1)*R*2*sqrt(2./3.),C*0e-2,Vec(1,1,1)*.996, SPEC), //back
////  Sphere(1e5, Vec(50,28,Z)+Vec(0,0,1e5+170),   Vec(1,1,1)*0,Vec(1,1,1)*.996, SPEC), //front
////  Sphere(2*R*2*sqrt(2./3.)-R*2*sqrt(2./3.)/3., Vec(50,28,Z)+Vec(0,0,-R*2*sqrt(2./3.)/3.),   Vec(1,1,1)*0,Vec(1,1,1)*.3333, SPEC), //front
//		Sphere(2*2*R*2*sqrt(2./3.)-R*2*sqrt(2./3.)/3., Vec(50,28,Z)+Vec(0,0,-R*2*sqrt(2./3.)/3.),   Vec(1,1,1)*0,Vec(1,1,1)*.5, SPEC), //front
//};

// island
// Inspired by cover of "Time Planet Earth: An Illustrated History"
Vec Cen(50,-20,-860);
Sphere spheres[] = {//Scene: radius, position, emission, color, material
		// center 50 40.8 62
		// floor 0
		// back  0
		//     rad       pos                   emis           col     refl

		Sphere(160,  Cen+Vec(0, 600, -500),Vec(1,1,1)*2e2, Vec(),  DIFF), // sun
		Sphere(800, Cen+Vec(0,-880,-9120),Vec(1,1,1)*2e1, Vec(),  DIFF), // horizon
		Sphere(10000,Cen+Vec(0,0,-200), Vec(0.0627, 0.188, 0.569)*1e0, Vec(1,1,1)*.4,  DIFF), // sky

//  Sphere(1000, Cen+Vec(0,-1080,-8020),Vec(1,1,1)*2e1, Vec(),  DIFF), // horizon
//  Sphere(10000,Cen+Vec(0,0,-200), Vec(0.0627, 0.188, 0.569)*1e0, Vec(1,1,1)*.3,  DIFF), // sky

//  Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0, 0.588, 0.8),  REFR), // water
//  Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0.106, 0.725, 0.949),  REFR), // water
//  Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0.110, 0.988, 0.945),  REFR), // water
		Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0.110, 0.898, 1.00)*.996,  REFR), // water
		Sphere(790, Cen+Vec(0,-720,-200),Vec(),  Vec(.4,.3,.04)*.6,    DIFF), // earth
		Sphere(325, Cen+Vec(0,-255,-50), Vec(),  Vec(.4,.3,.04)*.8,       DIFF), // island
		Sphere(275, Cen+Vec(0,-205,-33), Vec(),  Vec(.02,.3,.02)*.75,      DIFF), // grass
};


int numSpheres = sizeof(spheres) / sizeof(Sphere);

inline double clamp(double x)
{ return x < 0 ? 0 : x > 1 ? 1 : x; }

inline int toInt(double x)
{ return int(pow(clamp(x), 1 / 2.2) * 255 + .5); }

inline bool intersect(const Ray &r, double &t, int &id)
{
	double n = sizeof(spheres) / sizeof(Sphere), d, inf = t = 1e20;
	for (int i = int(n); i--;)
		if ((d = spheres[i].intersect(r)) && d < t) {
			t = d;
			id = i;
		}
	return t < inf;
}

Vec radiance(const Ray &r, int depth, unsigned short *Xi, int E = 1)
{
	double t;                               // distance to intersection
	int id = 0;                               // id of intersected object
	if (!intersect(r, t, id)) return Vec(); // if miss, return black
	const Sphere &obj = spheres[id];        // the hit object
	Vec x = r.o + r.d * t, n = (x - obj.p).norm(), nl = n.dot(r.d) < 0 ? n : n * -1, f = obj.c;
	double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z; // max refl
	if (++depth > 5 || !p) if (erand48(Xi) < p) f = f * (1 / p); else return obj.e * E;
	if (obj.refl == DIFF) {                  // Ideal DIFFUSE reflection
		double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
		Vec w = nl, u = ((fabs(w.x) > .1 ? Vec(0, 1) : Vec(1)) % w).norm(), v = w % u;
		Vec d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();

		// *** Loop over any lights ***
		Vec e;
		for (int i = 0; i < numSpheres; i++) {
			const Sphere &s = spheres[i];
			if (s.e.x <= 0 && s.e.y <= 0 && s.e.z <= 0) continue; // skip non-lights

			Vec sw = s.p - x, su = ((fabs(sw.x) > .1 ? Vec(0, 1) : Vec(1)) % sw).norm(), sv = sw % su;
			double cos_a_max = sqrt(1 - s.rad * s.rad / (x - s.p).dot(x - s.p));
			double eps1 = erand48(Xi), eps2 = erand48(Xi);
			double cos_a = 1 - eps1 + eps1 * cos_a_max;
			double sin_a = sqrt(1 - cos_a * cos_a);
			double phi = 2 * M_PI * eps2;
			Vec l = su * cos(phi) * sin_a + sv * sin(phi) * sin_a + sw * cos_a;
			l.norm();
			if (intersect(Ray(x, l), t, id) && id == i) {  // shadow ray
				double omega = 2 * M_PI * (1 - cos_a_max);
				e = e + f.mult(s.e * l.dot(nl) * omega) * M_1_PI;  // 1/pi for brdf
			}
		}

		return obj.e * E + e + f.mult(radiance(Ray(x, d), depth, Xi, 0));
	}
	else if (obj.refl == SPEC)              // Ideal SPECULAR reflection
		return obj.e + f.mult(radiance(Ray(x, r.d - n * 2 * n.dot(r.d)), depth, Xi));
	Ray reflRay(x, r.d - n * 2 * n.dot(r.d));     // Ideal dielectric REFRACTION
	bool into = n.dot(nl) > 0;                // Ray from outside going in?
	double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;
	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)    // Total internal reflection
		return obj.e + f.mult(radiance(reflRay, depth, Xi));
	Vec tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
	double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
	double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
	return obj.e + f.mult(depth > 2 ? (erand48(Xi) < P ?   // Russian roulette
									   radiance(reflRay, depth, Xi) * RP : radiance(Ray(x, tdir), depth, Xi) * TP) :
						  radiance(reflRay, depth, Xi) * Re + radiance(Ray(x, tdir), depth, Xi) * Tr);
}

int main(int argc, char *argv[])
{
	int w = 1024, h = 768, samps = argc == 2 ? atoi(argv[1]) / 4 : 1; // # samples
	Ray cam(Vec(50, 52, 295.6), Vec(0, -0.042612, -1).norm()); // cam pos, dir
	Vec cx = Vec(w * .5135 / h), cy = (cx % cam.d).norm() * .5135, r, *c = new Vec[w * h];
#pragma omp parallel for schedule(dynamic, 1) private(r)       // OpenMP
	for (int y = 0; y < h; y++) {                       // Loop over image rows
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100. * y / (h - 1));
		for (unsigned short x = 0, Xi[3] = {0, 0, y * y * y}; x < w; x++)   // Loop cols
			for (int sy = 0, i = (h - y - 1) * w + x; sy < 2; sy++)     // 2x2 subpixel rows
				for (int sx = 0; sx < 2; sx++, r = Vec()) {        // 2x2 subpixel cols
					for (int s = 0; s < samps; s++) {
						double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						Vec d = cx * (((sx + .5 + dx) / 2 + x) / w - .5) +
								cy * (((sy + .5 + dy) / 2 + y) / h - .5) + cam.d;
						r = r + radiance(Ray(cam.o + d * 140, d.norm()), 0, Xi) * (1. / samps);
					} // Camera rays are pushed ^^^^^ forward to start in interior
					c[i] = c[i] + Vec(clamp(r.x), clamp(r.y), clamp(r.z)) * .25;
				}
	}
	FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
	for (int i = 0; i < w * h; i++)
		fprintf(f, "%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
}