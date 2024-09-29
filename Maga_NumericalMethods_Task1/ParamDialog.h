#pragma once
#include "afxdialogex.h"


// Диалоговое окно ParamDialog

class ParamDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ParamDialog)

public:
	ParamDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ParamDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	bool Tmaintenance = false;
	double L;
	double dt;
	double vacancy;
	double shift;
	double dr;
	double V0;
	double T;
	CButton Ton;
	CButton Toff;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	double V0Shift;
};
