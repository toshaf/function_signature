#include <iostream>
#include <stdexcept>

template<int a, int b, int c>
struct cubic
{
	cubic(double x)
	: x(x)
	, y(a*x*x*x+b*x*x+c)
	, dy(3*a*x*x+2*b*x)
	, d2y(6*x+2*b)
	{ }
	double x, y, dy, d2y;
};

template<typename T>
class nullable
{
	T v;
	bool h;
public:
	nullable() : v(), h(false) { }
	nullable(T v) : v(v), h(true) { }
	operator T() { if (!h) throw std::runtime_error("no value"); return v; }
	bool has() const { return h; }
};

struct point
{
	point() : x(0), y(0) { }
	point(double x, double y) : x(x), y(y) { }
	double x, y;
};

point operator+(const point& a, const point& b)
{
	return point(a.x+b.x, a.y+b.y);
}

point operator/(const point& a, double f)
{
	return point(a.x/f, a.y/f);
}

template<typename T>
int sign(T v)
{
	return v < 0 ? -1 : 1;
}

template<typename T>
double pow(T v, uint64_t p)
{
	if (p == 0)
		return 1;

	T r = v;
	for (int i; i < p; ++i)
		r = r * 10;
	
	return r;
}

double round(double v, uint64_t places)
{
	double f = pow(10, places);
	return ((int64_t)(v*f+0.5*sign(v)))/f;
}

std::ostream& operator<<(std::ostream& os, const point& pt)
{
	return os << "(" << round(pt.x, 2) << "," << round(pt.y, 2) << ")";
}

bool between(double v, double l, double u)
{
	return (v >= l && v < u) || (v > u && v <= l);
}

struct points
{
	void track(double x, double y)
	{
		point pt(x, y);
		if (last_pt.has())
		{
			point lpt = last_pt;
			double dx = pt.x-lpt.x;
			double dy = (pt.y-lpt.y)/dx;
			if (last_dy.has())
			{
				double ldy = last_dy;
				if (between(0, dy, ldy))
				{
					std::cout << (pt+lpt)/2;
					if (dy > 0)
						std::cout << "/";
					if (dy < 0)
						std::cout << "\\";
				}

				double d2y = (dy-ldy)/dx;
				if (last_d2y.has())
				{
					double ld2y = last_d2y;
					if (between(0, d2y, ld2y))
					{
						std::cout << (pt+lpt)/2;
						if (dy > 0)
							std::cout << "/";
						if (dy < 0)
							std::cout << "\\";
					}
				}
				last_d2y = d2y;
			} else {
				if (dy > 0)
					std::cout << "/";
				if (dy < 0)
					std::cout << "\\";
			}
			last_dy = dy;
		}
		last_pt = pt;
	}

	nullable<point> last_pt;
	nullable<double> last_dy;
	nullable<double> last_d2y;
};

int main()
{
	points pts;
	for (double x = -20; x <= 10; x+=0.001)
	{
		cubic<1, 12, 0> c(x);
		pts.track(x, c.y);
	}
	std::cout << std::endl;
	return 0;
}
