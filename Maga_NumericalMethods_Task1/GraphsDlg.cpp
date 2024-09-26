// GraphsDlg.cpp: файл реализации
//

#include "pch.h"
#include "Maga_NumericalMethods_Task1.h"
#include "afxdialogex.h"
#include "GraphsDlg.h"

// Диалоговое окно GraphsDlg
#define MY_CREATE WM_USER + 1
#define MY_SHOW WM_USER + 2

IMPLEMENT_DYNAMIC(GraphsDlg, CDialogEx)

GraphsDlg::GraphsDlg(CWnd* pParent /*=nullptr*/)
	//: CDialogEx(IDD_GRAPH_DIALOG, pParent)
{
	Create(IDD_GRAPH_DIALOG);
}

GraphsDlg::~GraphsDlg()
{
	if (NeedAcknowledge)	AcknowledgeTarget->PostMessageW(GRAPH_CLOSED, id);
}

void GraphsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH1, Graph1);
	DDX_Control(pDX, IDC_GRAPH2, Graph2);
	DDX_Control(pDX, IDC_GRAPH3, Graph3);
	DDX_Control(pDX, IDC_GRAPH4, Graph4);
}


BEGIN_MESSAGE_MAP(GraphsDlg, CDialogEx)
	ON_MESSAGE(MY_CREATE, &GraphsDlg::OnMyCreate)
	ON_MESSAGE(MY_SHOW, &GraphsDlg::OnMyShow)
END_MESSAGE_MAP()




afx_msg LRESULT GraphsDlg::OnMyCreate(WPARAM wParam, LPARAM lParam)
{
	Create(IDD_GRAPH_DIALOG);
	PostMessageW(MY_SHOW);
	return 0;
}


afx_msg LRESULT GraphsDlg::OnMyShow(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOW);
	return 0;
}


BOOL GraphsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	Graph1.SetPadding(22, 5, 10, 10);
	Graph2.SetPadding(22, 5, 10, 10);
	Graph3.SetPadding(22, 5, 10, 10);
	Graph4.SetPadding(22, 5, 10, 10);
	Graph1.SetTitle(L"Полная эенергия (Е)");
	Graph2.SetTitle(L"Кинетическая эенергия (Еk)");
	Graph3.SetTitle(L"Потенциальная эенергия (Еп)");
	ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}

void GraphsDlg::InvalidateGraphs()
{
	Graph1.Invalidate();
	Graph2.Invalidate();
	Graph3.Invalidate();
	Graph4.Invalidate();
}
