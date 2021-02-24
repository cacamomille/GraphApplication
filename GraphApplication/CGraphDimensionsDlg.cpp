// CGraphDimensionsDlg.cpp: archivo de implementación
//

#include "pch.h"
#include "framework.h"
#include "GraphApplication.h"
#include "CGraphDimensionsDlg.h"
#include "afxdialogex.h"


// Cuadro de diálogo de CGraphDimensionsDlg

IMPLEMENT_DYNAMIC(CGraphDimensionsDlg, CDialogEx)

CGraphDimensionsDlg::CGraphDimensionsDlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_DIALOG_RANDOM_GRAPH, pParent)
	, m_Vertices(0)
	, m_Edges(0)
{

}

CGraphDimensionsDlg::~CGraphDimensionsDlg()
{
}

void CGraphDimensionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VERTICES, m_Vertices);
	DDX_Text(pDX, IDC_EDIT_EDGES, m_Edges);
}


BEGIN_MESSAGE_MAP(CGraphDimensionsDlg, CDialogEx)
END_MESSAGE_MAP()


// Controladores de mensajes de CGraphDimensionsDlg


void CGraphDimensionsDlg::OnOK()
{
	if (UpdateData(true)) {


		CDialogEx::OnOK();
	}

}
