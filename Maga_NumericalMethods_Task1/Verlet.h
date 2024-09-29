#pragma once

#include <vector>

//x - first, y - second
typedef  std::pair<double, double> point;
class Verlet
{
private:
	std::vector<point> data;
	std::vector<double> Fx;
	std::vector<double> Fy;
	std::vector<point>V;
	std::vector<double> Fxprev;
	std::vector<double> Fyprev;
	std::vector<double> Ek;
	std::vector<double> Ep;
	std::vector<double> E;
	std::vector<double> g;
	std::vector<double> E100;
	std::vector<double> Ek100;
	std::vector<double> Ep100;

	const double a = 3.82e-10;
	const double D = 1.6502426499e-21;
	const double m = 6.63352599096e-26;
	const double tau = 2e-12;
	const double kb = 1.380649e-23;
	double Pi2 = 0;
	double a6 = 0;

	double c = 0;
	double L = 0;
	double La = 0;
	double vacancy = 0;
	double dt = 0;
	double dr = 0;
	double dra = 0;
	double T = 0;
	double V0level = 0;
	double V0shift = 0;
	bool TMaintenance = false;

	double ActualVacancy = 0;
	UINT64 IterationCounter = 0;
	double CurrentTemperature = 0;

	int TPushE = 100;
	int TPushECounter = 0;

	int TemperatureMaintetancePeriod = 10;
	int TemperatureMaintetanceCounter = 0;
	double TemperatureMaintenaceAvg = 0;

	bool Continue = true;
	CRITICAL_SECTION csFx;
	CRITICAL_SECTION csFy;
	CRITICAL_SECTION csE;
	CRITICAL_SECTION csEk;
	CRITICAL_SECTION csEp;
	CRITICAL_SECTION csg;
	CRITICAL_SECTION csdata;
	CRITICAL_SECTION csCurT;

protected:
	inline double rand(double left, double right);
	inline double range2(point& p1, point& p2);
	void gradU();
	inline void Separate(double& dx, double& dy);
	void CalcEk();
	void CalcEp();
	void CalcE();
	void CalcG();
	void MaintenanceTemperature();
public:
	Verlet();
	~Verlet();
	void main();
	void SetC(double val);
	void SetL(double val);
	void SetVacancy(double val);
	void SetDt(double val);
	std::vector<point>GetData();
	double GetA();
	double GetActualVacancy();
	void VerletStep();
	std::vector<double> GetE();
	std::vector<double> GetEk();
	std::vector<double> GetEp();
	std::vector<double> GetG();
	void Stop();
	void CreateStartPosition();
	void CreateStartSpeed();
	UINT64 GetIterations();
	double GetActualTime();
	void SetDr(double val);
	std::vector<double> GetGKeys();
	void SetT(double val);
	void SetV0(double val);
	void SetV0Shift(double val);
	double GetCurrentTemperature();
	void SetTemperatureMaintenance(bool val);
};

template <typename T> int sgn(T val);