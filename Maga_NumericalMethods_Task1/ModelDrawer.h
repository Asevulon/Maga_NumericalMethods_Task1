#pragma once


// ModelDrawer
#include<gdiplus.h>
#include<vector>
#include<algorithm>
using namespace Gdiplus;


class ModelDrawer : public CStatic
{
	DECLARE_DYNAMIC(ModelDrawer)
private:
	std::vector<Gdiplus::PointF>data;
	PointF radius;

	double lPadding;
	double rPadding;
	double tPadding;
	double bPadding;
	int MarksLen;

	double left;
	double right;
	double bot;
	double top;


	ULONG_PTR token;
	
	CString title;

	int GridLinesAmount = 11;
	double MarksScaler = 1;
public:
	ModelDrawer();
	virtual ~ModelDrawer();

protected:
	double CalcStringLen(HDC hDC, CString string);
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetData(std::vector<Gdiplus::PointF>data);
	void SetRange(PointF& lb, PointF& rt);
	//������ �������� � ��������� (�������� �������� �� 0 �� 100)
	void SetPadding(double lPadding, double rPadding, double tPadding, double bPadding);
	void SetMarksLen(int len);
	void SetTitle(CString str);
	void SetGridLinesAmount(int val);
	void SetMarksScaler(double val);
};


