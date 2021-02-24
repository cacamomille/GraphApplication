// CEditEdgeDlg.cpp: archivo de implementación
//

#include "pch.h"
#include "framework.h"
#include "GraphApplication.h"
#include "CEditEdgeDlg.h"
#include "afxdialogex.h"


// Cuadro de diálogo de CEditEdgeDlg

IMPLEMENT_DYNAMIC(CEditEdgeDlg, CDialogEx)

CEditEdgeDlg::CEditEdgeDlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_EDIT_EDGE_DIALOG, pParent)
	, m_Name(_T(""))
	, m_Value(0)
{

}

CEditEdgeDlg::~CEditEdgeDlg()
{
}

void CEditEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_Value);
}


BEGIN_MESSAGE_MAP(CEditEdgeDlg, CDialogEx)
END_MESSAGE_MAP()


// Controladores de mensajes de CEditEdgeDlg


BOOL CEditEdgeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Agregue aquí la inicialización adicional
	m_Name = m_pEdge->m_Name.c_str();
	m_Value = m_pEdge->m_Length;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPCIÓN: las páginas de propiedades OCX deben devolver FALSE
}


void CEditEdgeDlg::OnOK()
{
	// TODO: Agregue aquí su código especializado o llame a la clase base
	if (!UpdateData(true)) return;
	if (m_Name.Find(" ") != -1) {
		AfxMessageBox("nnames with spaces are not allowed");
		return;

	}
	CEdge *pE = m_pGraph->FindEdge(m_Name);
	if (pE != NULL && pE != m_pEdge) {
		AfxMessageBox("Duplicated name");
		return;
	}
	m_pEdge->m_Name = m_Name;
	m_pEdge->m_Length = m_Value;
	CDialogEx::OnOK();
}
