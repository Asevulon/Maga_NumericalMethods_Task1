
// MyDlg.h: файл заголовка
//

#pragma once
#include"ParamDialog.h"
#include"GraphsDlg.h"
#include"ModelDrawer.h"
#include"Verlet.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAGA_NUMERICALMETHODS_TASK1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	ParamDialog pardlg;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonParams();
	CStatic ParamsText;
	std::vector<GraphsDlg*>graphs;
	ModelDrawer ModelPicture;
	Verlet* ver;
	static DWORD WINAPI ModelThread(LPVOID);
	bool GraphsAvailible = false;
	UINT_PTR timerid;
	bool InProcess = false;
protected:
	afx_msg LRESULT OnGraphClosed(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton OkButton;
};
