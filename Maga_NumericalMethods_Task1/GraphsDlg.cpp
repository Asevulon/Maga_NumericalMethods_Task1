// GraphsDlg.cpp: файл реализации
//

#include "pch.h"
#include "Maga_NumericalMethods_Task1.h"
#include "afxdialogex.h"
#include "GraphsDlg.h"


// Диалоговое окно GraphsDlg

IMPLEMENT_DYNAMIC(GraphsDlg, CDialogEx)

GraphsDlg::GraphsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
{

}

GraphsDlg::~GraphsDlg()
{
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
END_MESSAGE_MAP()


// Обработчики сообщений GraphsDlg
