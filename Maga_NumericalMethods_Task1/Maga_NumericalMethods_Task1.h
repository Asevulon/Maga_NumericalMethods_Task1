
// Maga_NumericalMethods_Task1.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMagaNumericalMethodsTask1App:
// Сведения о реализации этого класса: Maga_NumericalMethods_Task1.cpp
//

class CMagaNumericalMethodsTask1App : public CWinApp
{
public:
	CMagaNumericalMethodsTask1App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMagaNumericalMethodsTask1App theApp;
