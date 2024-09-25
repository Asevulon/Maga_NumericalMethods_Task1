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

	const double a = 3.82e-10;
	const double D = 1.6502426499e-21;
	const double m = 6.63352599096e-26;
	const double tau = 2e-12;
	double a6 = 0;

	double c = 0;
	double L = 0;
	double La = 0;
	double vacancy = 0;
	double dt = 0;

	double ActualVacancy = 0;
	UINT64 IterationCounter = 0;

	bool Continue = true;

	CRITICAL_SECTION cs;
protected:
	inline double rand(double left, double right);
	inline double range2(point& p1, point& p2);
	void gradU();
	inline void Separate(double& dx, double& dy);
	void CalcEk();
	void CalcEp();
	void CalcE();
public:
	Verlet();
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
	void Stop();
	void CreateStartPosition();
};

template <typename T> int sgn(T val);