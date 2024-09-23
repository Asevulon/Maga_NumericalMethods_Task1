// ParamDialog.cpp: файл реализации
//

#include "pch.h"
#include "Maga_NumericalMethods_Task1.h"
#include "afxdialogex.h"
#include "ParamDialog.h"


// Диалоговое окно ParamDialog

IMPLEMENT_DYNAMIC(ParamDialog, CDialogEx)

ParamDialog::ParamDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARAM_DIALOG, pParent)
	, L(30)
	, dt(0.01)
	, vacancy(0.2)
	, shift(0.05)
	, dr(0.1)
	, V0(0)
	, T(300)
{

}

ParamDialog::~ParamDialog()
{
}

void ParamDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, L);
	DDX_Text(pDX, IDC_EDIT2, dt);
	DDX_Text(pDX, IDC_EDIT3, vacancy);
	DDX_Text(pDX, IDC_EDIT4, shift);
	DDX_Text(pDX, IDC_EDIT5, dr);
	DDX_Text(pDX, IDC_EDIT7, V0);
	DDX_Text(pDX, IDC_EDIT6, T);
	DDX_Control(pDX, IDC_RADIO1, Ton);
	DDX_Control(pDX, IDC_RADIO2, Toff);
}


BEGIN_MESSAGE_MAP(ParamDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &ParamDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений ParamDialog


void ParamDialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	if (Ton.GetCheck() == BST_CHECKED)Tmaintenance = true;
	else Tmaintenance = false;
	CDialogEx::OnOK();
}


BOOL ParamDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	if (Tmaintenance)Ton.SetCheck(BST_CHECKED);
	else Toff.SetCheck(BST_CHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}
