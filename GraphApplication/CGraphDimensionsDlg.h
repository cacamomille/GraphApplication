#pragma once
#include "afxwin.h"
#include "Graph.h"


// Cuadro de diálogo de CGraphDimensionsDlg

class CGraphDimensionsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDimensionsDlg)

public:
	CGraphDimensionsDlg(CWnd* pParent = nullptr);   // Constructor estándar
	virtual ~CGraphDimensionsDlg();

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RANDOM_GRAPH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_Vertices;
	int m_Edges;
	virtual void OnOK();
};
