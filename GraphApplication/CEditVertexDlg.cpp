// CEditVertexDlg.cpp: archivo de implementación
//

#include "pch.h"
#include "framework.h"
#include "GraphApplication.h"
#include "CEditVertexDlg.h"
#include "afxdialogex.h"


// Cuadro de diálogo de CEditVertexDlg

IMPLEMENT_DYNAMIC(CEditVertexDlg, CDialogEx)

CEditVertexDlg::CEditVertexDlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_EDIT_VERTEX_DIALOG, pParent)
	, m_Name(_T(""))
	, m_X(0)
	, m_Y(0)
{

}

CEditVertexDlg::~CEditVertexDlg()
{
}

void CEditVertexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_X, m_X);
	DDX_Text(pDX, IDC_EDIT_Y, m_Y);
}


BEGIN_MESSAGE_MAP(CEditVertexDlg, CDialogEx)
END_MESSAGE_MAP()


// Controladores de mensajes de CEditVertexDlg


BOOL CEditVertexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Agregue aquí la inicialización adicional
	m_Name = m_pVertex->m_Name.c_str();
	m_X = m_pVertex->m_Point.m_X;
	m_Y = m_pVertex->m_Point.m_Y;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPCIÓN: las páginas de propiedades OCX deben devolver FALSE
}

void CEditVertexDlg::OnOK()
{
	if (!UpdateData(true)) return;
	if (m_Name.Find(" ") != -1) {
		AfxMessageBox("nnames with spaces are not allowed");
		return;
	}
	CVertex *pV=m_pGraph->FindVertex(m_Name);
	if (pV != NULL && pV != m_pVertex) {
		AfxMessageBox("Duplicated name");
		return;
	}
	m_pVertex->m_Name = m_Name;
	m_pVertex->m_Point.m_X = m_X;
	m_pVertex->m_Point.m_Y = m_Y;
	CDialogEx::OnOK();
}
