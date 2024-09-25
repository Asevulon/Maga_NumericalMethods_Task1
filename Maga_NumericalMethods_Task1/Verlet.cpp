#include "pch.h"
#include "Verlet.h"

#define pow2(x) (x) * (x)
void Verlet::CreateStartPosition()
{
	double x = 0;
	double y = 0;
	int cap = RAND_MAX * vacancy;
	int N = L;
	EnterCriticalSection(&cs);
	/*for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (std::rand() < cap)continue;
			x = j * a + c * rand(-1, 1);
			y = i * a + c * rand(-1, 1);
			data.push_back(point(x, y));
			V.push_back(point(0, 0));
		}
	}*/
	data.push_back(point(10 * a, 10 * a));
	data.push_back(point(11 * a, 11 * a));
	data.push_back(point(10 * a, 11 * a));
	V.push_back(point(0, 0));
	V.push_back(point(0, 0));
	V.push_back(point(0, 0));
	LeaveCriticalSection(&cs);
	ActualVacancy = (double)data.size() / (double)N / (double)N;
}

inline double Verlet::rand(double left, double right)
{
	return left + (right - left) * double(std::rand()) / double(RAND_MAX);
}


inline double Verlet::range2(point& p1, point& p2)
{
	return pow2(p1.first - p2.first) + pow2(p1.second - p2.second);
}

void Verlet::gradU()
{
	int size = data.size();
	EnterCriticalSection(&cs);
	for (int i = 0; i < size; i++)
	{
		Fx[i] = 0;
		Fy[i] = 0;
	}
	LeaveCriticalSection(&cs);

	double r2 = 0;
	double r1 = 0;
	double r6 = 0;
	double r8 = 0;
	double rmax = 1.75 * a;
	double rmin = 1.15 * a;
	double rdif = rmin - rmax;
	double k = 0;
	point p1;
	point p2;
	double dx = 0;
	double dy = 0;
	double f = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			EnterCriticalSection(&cs);
			p1 = data[i];
			p2 = data[j];
			LeaveCriticalSection(&cs);

			dx = p1.first - p2.first;
			dy = p1.second - p2.second;
			Separate(dx, dy);

			r2 = pow2(dx) + pow2(dy);
			r1 = sqrt(r2);

			if (r1 >= rmax)continue;
			if (r1 >= rmin)
			{
				k = (r1 - rmin) / (rdif);
				k *= k;
				k = 1 - k;
				k *= k;
			}
			else
			{
				k = 1;
			}

			r6 = r2 * r2 * r2;
			r8 = r6 * r2;

			f = (a6 / r6 - 1) / r8 * k;
			
			EnterCriticalSection(&cs);
			Fx[i] += f * dx;
			Fy[i] += f * dy;

			Fx[j] -= f * dx;
			Fy[j] -= f * dy;
			LeaveCriticalSection(&cs);
		}
	}

	double D12 = -D * 12 * a6;
	EnterCriticalSection(&cs);
	for (int i = 0; i < size; i++)
	{
		Fx[i] *= D12;
		Fy[i] *= D12;
	}
	LeaveCriticalSection(&cs);
}

inline void Verlet::Separate(double& dx, double& dy)
{
	if (fabs(dx) > 0.5 * La) dx -= sgn(dx) * La;
	if (fabs(dy) > 0.5 * La) dy -= sgn(dy) * La;
}

void Verlet::CalcEk()
{
	double ek = 0;
	EnterCriticalSection(&cs);
	for (auto& v : V)
	{
		ek += m * (pow2(v.first) + pow2(v.second)) / 2.;
	}
	Ek.push_back(ek * 6.241506363094e+18);
	LeaveCriticalSection(&cs);
}

void Verlet::CalcEp()
{
	int size = data.size();

	double r2 = 0;
	double r1 = 0;
	double r6 = 0;
	double r12 = 0;
	double rmax = 1.75 * a;
	double rmin = 1.15 * a;
	double rdif = rmin - rmax;
	double k = 0;
	point p1;
	point p2;
	double dx = 0;
	double dy = 0;
	double f = 0;
	double sigma = a / pow(2., 1. / 6.);
	double sigma6 = sigma * sigma * sigma * sigma * sigma * sigma;
	double sigma12 = pow2(sigma6);
	double ep = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			EnterCriticalSection(&cs);
			p1 = data[i];
			p2 = data[j];
			LeaveCriticalSection(&cs);

			dx = p1.first - p2.first;
			dy = p1.second - p2.second;
			Separate(dx, dy);

			r2 = pow2(dx) + pow2(dy);
			r1 = sqrt(r2);

			if (r1 >= rmax)continue;
			if (r1 >= rmin)
			{
				k = (r1 - rmin) / (rdif);
				k *= k;
				k = 1 - k;
				k *= k;
			}
			else
			{
				k = 1;
			}

			r6 = r2 * r2 * r2;
			r12 = r6 * r6;

			f = sigma12 / r12 - sigma6 / r6;
			
			ep += f;
		}
	}

	double D8 = D * 8;
	ep *= D8;
	EnterCriticalSection(&cs);
	Ep.push_back(ep * 6.241506363094e+18);
	LeaveCriticalSection(&cs);
}

void Verlet::CalcE()
{
	CalcEk();
	CalcEp();
	E.push_back(Ek.back() + Ep.back());
}

Verlet::Verlet()
{
	a6 = a * a * a * a * a * a;
}

void Verlet::main()
{
	InitializeCriticalSection(&cs);
	IterationCounter = 0;
	Continue = true;

	Fx.resize(data.size());
	Fy.resize(data.size());

	gradU();

	while (Continue)
	{
		VerletStep();
		CalcE();
	}
	DeleteCriticalSection(&cs);
}

void Verlet::SetC(double val)
{
	c = val * a;
}

void Verlet::SetL(double val)
{
	L = val;
	La = L * a;
}

void Verlet::SetVacancy(double val)
{
	vacancy = val;
}

void Verlet::SetDt(double val)
{
	dt = val * tau;
}

std::vector<point> Verlet::GetData()
{
	EnterCriticalSection(&cs);
	auto temp = data;
	LeaveCriticalSection(&cs);
	return temp;
}

double Verlet::GetA()
{
	return a;
}

double Verlet::GetActualVacancy()
{
	return ActualVacancy;
}

void Verlet::VerletStep()
{
	for (int i = 0; i < data.size(); i++)
	{
		data[i].first += V[i].first * dt + Fx[i] * pow2(dt) / 2. / m;
		data[i].second += V[i].second * dt + Fy[i] * pow2(dt) / 2. / m;

		if (data[i].first < 0)data[i].first += La;
		if (data[i].second < 0)data[i].second += La;
		if (data[i].first > La)data[i].first -= La;
		if (data[i].second > La)data[i].second -= La;
	}
	
	Fxprev = Fx;
	Fyprev = Fy;

	gradU();

	for (int i = 0; i < data.size(); i++)
	{
		V[i].first += (Fx[i] + Fxprev[i]) * dt / 2. / m;
		V[i].second += (Fy[i] + Fyprev[i]) * dt / 2. / m;
	}
	
	IterationCounter++;
}

std::vector<double> Verlet::GetE()
{
	EnterCriticalSection(&cs);
	auto temp = E;
	LeaveCriticalSection(&cs);
	return temp;
}

std::vector<double> Verlet::GetEk()
{
	EnterCriticalSection(&cs);
	auto temp = Ek;
	LeaveCriticalSection(&cs);
	return temp;
}

std::vector<double> Verlet::GetEp()
{
	EnterCriticalSection(&cs);
	auto temp = Ep;
	LeaveCriticalSection(&cs);
	return temp;
}

void Verlet::Stop()
{
	Continue = false;
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}