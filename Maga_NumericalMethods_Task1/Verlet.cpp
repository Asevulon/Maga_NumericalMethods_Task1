#include "pch.h"
#include "Verlet.h"

#define pow2(x) ((x) * (x))


void Verlet::CreateStartPosition()
{
	srand(time(NULL));
	double x = 0;
	double y = 0;
	int cap = RAND_MAX * vacancy;
	int N = L;
	EnterCriticalSection(&csdata);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (std::rand() < cap)continue;
			x = j * a + c * rand(-1, 1);
			y = i * a + c * rand(-1, 1);
			data.push_back(point(x, y));
		}
	}
	LeaveCriticalSection(&csdata);
	ActualVacancy = (double)data.size() / (double)N / (double)N;
}

void Verlet::CreateStartSpeed()
{
	if (data.empty())return;

	srand(time(NULL));
	double x = 0;
	double y = 0;
	int N = data.size();
	V.resize(N);
	double Vmax = sqrt(3 * kb * T / m) * V0level;
	double Vmin = (1 - V0shift) * Vmax;
	Vmax *= 1 + V0shift;

	double v = 0;
	double fi = 0;

	double xavg = 0;
	double yavg = 0;

	for (int i = 0; i < N; i++)
	{
		v = rand(Vmin, Vmax);
		fi = rand(0, Pi2);

		x = v * cos(fi);
		y = v * sin(fi);

		xavg += x;
		yavg += y;

		V[i] = point(x, y);
	}

	xavg /= N;
	yavg /= N;

	for (int i = 0; i < N; i++)
	{
		V[i].first -= xavg;
		V[i].second -= yavg;
	}
}

UINT64 Verlet::GetIterations()
{
	return IterationCounter;
}

double Verlet::GetActualTime()
{
	return IterationCounter * dt;
}

void Verlet::SetDr(double val)
{
	dr = val;
	dra = dr * a;
}

std::vector<double> Verlet::GetGKeys()
{
	std::vector<double> res(a * 7.5 / dra, 0);
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = double(i + 1) * dr;
	}
	return res;
}

void Verlet::SetT(double val)
{
	T = val;
}

void Verlet::SetV0(double val)
{
	V0level = val;
}

void Verlet::SetV0Shift(double val)
{
	V0shift = val;
}

double Verlet::GetCurrentTemperature()
{
	EnterCriticalSection(&csCurT);
	double temp = CurrentTemperature;
	LeaveCriticalSection(&csCurT);
	return temp;
}

void Verlet::SetTemperatureMaintenance(bool val)
{
	TMaintenance = val;
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
	EnterCriticalSection(&csFx);
	EnterCriticalSection(&csFy);
	for (int i = 0; i < size; i++)
	{
		Fx[i] = 0;
		Fy[i] = 0;
	}
	LeaveCriticalSection(&csFx);
	LeaveCriticalSection(&csFy);

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
			EnterCriticalSection(&csdata);
			p1 = data[i];
			p2 = data[j];
			LeaveCriticalSection(&csdata);

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
			
			EnterCriticalSection(&csFx);
			Fx[i] += f * dx;
			Fx[j] -= f * dx;
			LeaveCriticalSection(&csFx);

			EnterCriticalSection(&csFy);
			Fy[i] += f * dy;
			Fy[j] -= f * dy;
			LeaveCriticalSection(&csFy);
		}
	}

	double D12 = D * 12 * a6;
	EnterCriticalSection(&csFx);
	EnterCriticalSection(&csFy);
	for (int i = 0; i < size; i++)
	{
		Fx[i] *= D12;
		Fy[i] *= D12;
	}
	LeaveCriticalSection(&csFx);
	LeaveCriticalSection(&csFy);
}

inline void Verlet::Separate(double& dx, double& dy)
{
	if (fabs(dx) > 0.5 * La) dx -= sgn(dx) * La;
	if (fabs(dy) > 0.5 * La) dy -= sgn(dy) * La;
}

void Verlet::CalcEk()
{
	double ek = 0;
	for (auto& v : V)
	{
		ek += m * (pow2(v.first) + pow2(v.second)) / 2.;
	}
	Ek100.push_back(ek);
	if(Ek100.size() == TPushE)
	{
		double avg = 0;
		for (auto& item : Ek100)avg += item;
		EnterCriticalSection(&csEk);
		Ek.push_back(avg * 6.241506363094e+18 / TPushE);
		LeaveCriticalSection(&csEk);
	}
	EnterCriticalSection(&csCurT);
	CurrentTemperature = ek / V.size() / kb;
	LeaveCriticalSection(&csCurT);
}

void Verlet::CalcEp()
{
	std::vector<double>tempg(g.size(), 0);
	int size = data.size();

	double r2 = 0;
	double r1 = 0;
	double r6 = 0;
	double r12 = 0;
	double rmax = 1.75 * a;
	double rmin = 1.15 * a;
	double rdif = rmin - rmax;
	double k = 1;
	point p1;
	point p2;
	double dx = 0;
	double dy = 0;
	double f = 0;
	double sigma = a / pow(2., 1. / 6.);
	double sigma6 = sigma * sigma * sigma * sigma * sigma * sigma;
	double sigma12 = pow2(sigma6);
	double ep = 0;

	double L4 = La / 4;
	double L34 = La * 3;
	double a75 = a * 7.5;
	int id = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			EnterCriticalSection(&csdata);
			p1 = data[i];
			p2 = data[j];
			LeaveCriticalSection(&csdata);

			dx = p1.first - p2.first;
			dy = p1.second - p2.second;
			Separate(dx, dy);

			r2 = pow2(dx) + pow2(dy);
			r1 = sqrt(r2);

			if (r1 <= a75)
			{
				if ((p1.first > L4) && (p1.first < L34) && (p1.second > L4) && (p1.second < L34))
				{
					id = r1 / dra;
					tempg[id] += 2. / (id + 1) / dr;
				}
			}

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
	double D4 = D * 4;
	ep *= D4;

	Ep100.push_back(ep);
	if(Ep100.size() == TPushE)
	{
		double avg = 0;
		for (auto& item : Ep100)avg += item;
		EnterCriticalSection(&csEp);
		Ep.push_back(avg * 6.241506363094e+18 / TPushE);
		LeaveCriticalSection(&csEp);
	}

	EnterCriticalSection(&csg);
	g = tempg;
	LeaveCriticalSection(&csg);
}

void Verlet::CalcE()
{
	CalcEk();
	CalcEp();
	if ((Ek100.size() == TPushE) || (Ep100.size() == TPushE))
	{
		E.push_back(Ek.back() + Ep.back());
		Ek100.clear();
		Ep100.clear();
		return;
	}
}

void Verlet::CalcG()
{
	EnterCriticalSection(&csg);
	for (int i = 0; i < g.size(); i++)
	{
		g[i] = 0;
	}
	LeaveCriticalSection(&csg);

	double L4 = La / 4;
	double L34 = La * 3;
	double a75 = a * 7.5;

	int size = data.size();
	point p1;
	point p2;
	double dx = 0;
	double dy = 0;
	double r2 = 0;
	double r1 = 0;
	int id = 0;
	int counter = 0;

	for (int i = 0; i < size; i++)
	{
		EnterCriticalSection(&csdata);
		p1 = data[i];
		LeaveCriticalSection(&csdata);

		if (p1.first < L4)continue;
		if (p1.first > L34)continue;
		if (p1.second < L4)continue;
		if (p1.second > L34)continue;
		for (int j = i + 1; j < size; j++)
		{
			EnterCriticalSection(&csdata);
			p2 = data[j];
			LeaveCriticalSection(&csdata);
			
			dx = p1.first - p2.first;
			dy = p1.second - p2.second;
			Separate(dx, dy);

			r2 = pow2(dx) + pow2(dy);
			r1 = sqrt(r2);
			
			if (r1 > a75)continue;

			id = r1 / dra;
			EnterCriticalSection(&csg);
			g[id] += 2. / (id + 1) / dr;
			LeaveCriticalSection(&csg);
			counter++;
		}
	}

	/*EnterCriticalSection(&csg);
	for (int i = 0; i < g.size(); i++)
	{
		g[i] /= (double)(i + 1) / dr;
	}
	LeaveCriticalSection(&csg);*/
}

void Verlet::MaintenanceTemperature()
{
	for (int i = 0; i < V.size(); i++)
	{
		TemperatureMaintenaceAvg += pow2(V[i].first) + pow2(V[i].second);
	}
	TemperatureMaintetanceCounter++;

	if (TemperatureMaintetanceCounter < TemperatureMaintetancePeriod)
	{
		return;
	}

	TemperatureMaintenaceAvg *= m / 10.;
	
	double betta = sqrt(2 * V.size() * kb * T / TemperatureMaintenaceAvg);

	for (int i = 0; i < V.size(); i++)
	{
		V[i].first *= betta;
		V[i].second *= betta;
	}

	TemperatureMaintenaceAvg = 0;
	TemperatureMaintetanceCounter = 0;
}

Verlet::Verlet()
{
	a6 = a * a * a * a * a * a;
	InitializeCriticalSection(&csFx);
	InitializeCriticalSection(&csFy);
	InitializeCriticalSection(&csE);
	InitializeCriticalSection(&csEk);
	InitializeCriticalSection(&csEp);
	InitializeCriticalSection(&csg);
	InitializeCriticalSection(&csdata);
	InitializeCriticalSection(&csCurT);
	Pi2 = 8 * atan(1);
}

Verlet::~Verlet()
{
	DeleteCriticalSection(&csFx);
	DeleteCriticalSection(&csFy);
	DeleteCriticalSection(&csE);
	DeleteCriticalSection(&csEk);
	DeleteCriticalSection(&csEp);
	DeleteCriticalSection(&csg);
	DeleteCriticalSection(&csdata);
	DeleteCriticalSection(&csCurT);
}

void Verlet::main()
{
	IterationCounter = 0;
	Continue = true;
	TPushECounter = 0;

	g.resize(a * 7.5 / dra);
	Fx.resize(data.size());
	Fy.resize(data.size());
	gradU();

	while (Continue)
	{
		VerletStep();
		CalcE();
		if (TMaintenance)MaintenanceTemperature();
	}
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
	EnterCriticalSection(&csdata);
	auto temp = data;
	LeaveCriticalSection(&csdata);
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
	EnterCriticalSection(&csE);
	auto temp = E;
	LeaveCriticalSection(&csE);
	return temp;
}

std::vector<double> Verlet::GetEk()
{
	EnterCriticalSection(&csEk);
	auto temp = Ek;
	LeaveCriticalSection(&csEk);
	return temp;
}

std::vector<double> Verlet::GetEp()
{
	EnterCriticalSection(&csEp);
	auto temp = Ep;
	LeaveCriticalSection(&csEp);
	return temp;
}

std::vector<double> Verlet::GetG()
{
	EnterCriticalSection(&csg);
	auto temp = g;
	LeaveCriticalSection(&csg);
	return temp;
}

void Verlet::Stop()
{
	Continue = false;
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}