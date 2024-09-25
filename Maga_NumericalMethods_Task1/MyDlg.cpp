
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Maga_NumericalMethods_Task1.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMsMyCreate(WPARAM wParam, LPARAM lParam);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAGA_NUMERICALMETHODS_TASK1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARAMS_TEXT, ParamsText);
	DDX_Control(pDX, IDC_MODEL_PICTURE, ModelPicture);
	DDX_Control(pDX, IDOK, OkButton);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PARAMS, &MyDlg::OnBnClickedButtonParams)
	ON_MESSAGE(GRAPH_CLOSED, &MyDlg::OnGraphClosed)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (InProcess)
	{
		ver->Stop();
		KillTimer(timerid);
		OkButton.SetWindowTextW(L"Начать");
		InProcess = false;
	}
	else
	{
		GraphsDlg* dlg = new GraphsDlg;
		dlg->id = graphs.size();
		dlg->NeedAcknowledge = true;
		dlg->AcknowledgeTarget = this;
		graphs.push_back(dlg);
		GraphsAvailible = true;

		ver = new Verlet;
		ver->SetC(pardlg.shift);
		ver->SetL(pardlg.L);
		ver->SetVacancy(pardlg.vacancy);
		ver->SetDt(pardlg.dt);
		ver->CreateStartPosition();

		double a = ver->GetA();
		ModelPicture.SetRange(PointF(0, 0), PointF((pardlg.L - 1) * a, (pardlg.L - 1) * a));
		ModelPicture.SetPadding(10, 5, 10, 10);
		ModelPicture.SetTitle(L"Модель");
		ModelPicture.SetGridLinesAmount(pardlg.L);
		ModelPicture.SetMarksScaler(1. / a);
		ModelPicture.SetData(ver->GetData());
		ModelPicture.Invalidate();


		CString actstr;
		ParamsText.GetWindowTextW(actstr);
		CString str;
		if (actstr.IsEmpty())str.Format(L"Вакансии (эксп.): %.2f", ver->GetActualVacancy());
		else str.Format(L", Вакансии (эксп.): %.2f", ver->GetActualVacancy());
		ParamsText.SetWindowTextW(actstr + str);

		timerid = SetTimer(123, 100, NULL);

		OkButton.SetWindowTextW(L"Стоп");
		InProcess = true;

		CreateThread(NULL, NULL, ModelThread, ver, NULL, NULL);
	}
}


void MyDlg::OnBnClickedButtonParams()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (pardlg.DoModal() != IDOK)return;
	CString str;
	if (pardlg.Tmaintenance)str.Format(L"L:%.f, dt: %.f, dr: %.f, Вакансии: %.f, Сдвиг: %.f, V0: %.f, T0: %.f",
		pardlg.L, pardlg.dt, pardlg.dr, pardlg.vacancy, pardlg.shift, pardlg.V0, pardlg.T);
	else str.Format(L"L:%.f, dt: %.f, dr: %.f, Вакансии: %.f, Сдвиг: %.f, V0: %.f",
		pardlg.L, pardlg.dt, pardlg.dr, pardlg.vacancy, pardlg.shift, pardlg.V0);
	ParamsText.SetWindowTextW(str);
}

DWORD __stdcall MyDlg::ModelThread(LPVOID p)
{
	Verlet* v = (Verlet*)p;
	v->main();
	delete v;
	return 0;
}


afx_msg LRESULT CAboutDlg::OnMsMyCreate(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT MyDlg::OnGraphClosed(WPARAM wParam, LPARAM lParam)
{
	if (wParam == graphs.size() - 1);
	delete graphs[wParam];
	return 0;
}


void MyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (GraphsAvailible)
	{
		graphs.back()->Graph1.SetData(ver->GetE());
		graphs.back()->Graph2.SetData(ver->GetEk());
		graphs.back()->Graph3.SetData(ver->GetEp());
		
		//graphs.back()->Invalidate();
		graphs.back()->InvalidateGraphs();
	}
	ModelPicture.SetData(ver->GetData());
	ModelPicture.Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}
