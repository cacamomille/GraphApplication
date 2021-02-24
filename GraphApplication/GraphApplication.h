
// GraphApplication.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// Símbolos principales

#include "Graph.h"


// CGraphApplicationApp:
// Consulte GraphApplication.cpp para obtener información sobre la implementación de esta clase
//

class CGraphApplicationApp : public CWinApp
{
public:
	CGraphApplicationApp();

// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CGraphApplicationApp theApp;
