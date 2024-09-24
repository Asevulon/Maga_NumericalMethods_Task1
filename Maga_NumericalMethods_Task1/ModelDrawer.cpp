﻿// ModelDrawer.cpp: файл реализации
//

#include "pch.h"
#include "ModelDrawer.h"


// ModelDrawer

IMPLEMENT_DYNAMIC(ModelDrawer, CStatic)

ModelDrawer::ModelDrawer()
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
	radius.X = 7.1e-11;
	radius.Y = 7.1e-11;
}

ModelDrawer::~ModelDrawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(ModelDrawer, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений ModelDrawer




void ModelDrawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Graphics ToWindow(lpDrawItemStruct->hDC);
	int width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);

	if (data.empty())
	{
		ToWindow.DrawImage(&bmp, 0, 0);
		return;
	}

	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = width / actWidth;
	double yScale = -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);


	float steplenY = (top - bot) / (GridLinesAmount - 1);
	float steplenX = (right - left) / (GridLinesAmount - 1);

	FontFamily FF(L"Arial");
	Gdiplus::Font font(&FF, 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color::Black);
	for (int i = 0; i < GridLinesAmount; i++)
	{
		PointF p1, p2;
		p1.X = left;
		p1.Y = top - i * steplenY;

		p2.X = right;
		p2.Y = top - i * steplenY;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		p1.X = right - i * steplenX;
		p1.Y = top;

		p2.X = right - i * steplenX;
		p2.Y = bot;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		CString str;
		str.Format(L"%.f", (top - i * steplenY) * MarksScaler);

		PointF strPoint;
		strPoint.X = left - actWidth / 100 - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale;
		strPoint.Y = top - i * steplenY -6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		str.Format(L"%.f", (right - i * steplenX) * MarksScaler);

		strPoint.X = right - i * steplenX - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale / 2.;
		strPoint.Y = bot + 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	Pen pen(Color::Black, 1.1F);
	PointF p1(left, top), p2(left, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);

	p1 = PointF(left, bot);
	p2 = PointF(right, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);


	if (!title.IsEmpty())
	{

		p1.X = actLeft + actWidth / 2 - CalcStringLen(lpDrawItemStruct->hDC, title) / 2 / xScale;
		p1.Y = (actTop + top) / 2 - 6 / yScale;
		matr.TransformPoints(&p1);
		gr.DrawString(title, title.GetLength() + 1, &font, p1, &brush);
	}



	SolidBrush DataBrush(Color(200, 100, 0));

	double twidth = 0;
	double theight = 0;
	for (auto&point:data)
	{
		p1 = p2 = point;
		p1 = p1 - radius;
		p2 = p2 + radius;
		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);
		twidth = p2.X - p1.X;
		theight = p2.Y - p1.Y;
		gr.FillEllipse(&DataBrush, p1.X, p1.Y, twidth, theight);
	}
	
	ToWindow.DrawImage(&bmp, 0, 0);
}


void ModelDrawer::SetData(std::vector<Gdiplus::PointF>y)
{
	data = y;
}

void ModelDrawer::SetRange(PointF& lb, PointF& rt)
{
	left = lb.X;
	bot = lb.Y;
	right = rt.X;
	top = rt.Y;
}

void ModelDrawer::SetPadding(double lP, double rP, double tP, double bP)
{
	lPadding = lP;
	rPadding = rP;
	tPadding = tP;
	bPadding = bP;
}
void ModelDrawer::SetMarksLen(int len)
{
	MarksLen = len;
}
void ModelDrawer::SetTitle(CString str)
{
	title = str;
}

void ModelDrawer::SetGridLinesAmount(int val)
{
	GridLinesAmount = val;
}

void ModelDrawer::SetMarksScaler(double val)
{
	MarksScaler = val;
}


double ModelDrawer::CalcStringLen(HDC hDC, CString str)
{
	double numlen = 0;
	int size = str.GetLength() + 1;
	for (int i = 0; i < size; i++)
	{
		ABCFLOAT abc;
		GetCharABCWidthsFloatW(hDC, str[i], str[i], &abc);
		numlen += abc.abcfA + abc.abcfB + abc.abcfC;
	}
	return numlen;
}