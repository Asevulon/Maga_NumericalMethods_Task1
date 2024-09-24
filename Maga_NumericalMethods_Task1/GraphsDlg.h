#pragma once
#include "afxdialogex.h"
#include "Drawer.h"

// Диалоговое окно GraphsDlg

class GraphsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GraphsDlg)

public:
	GraphsDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~GraphsDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	Drawer Graph1;
	Drawer Graph2;
	Drawer Graph3;
	Drawer Graph4;
protected:
	afx_msg LRESULT OnMyCreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyShow(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
