// CVisitsDimensionsDlg.cpp: archivo de implementación
//

#include "pch.h"
#include "GraphApplication.h"
#include "CVisitsDimensionsDlg.h"
#include "afxdialogex.h"


// Cuadro de diálogo de CVisitsDimensionsDlg

IMPLEMENT_DYNAMIC(CVisitsDimensionsDlg, CDialogEx)

CVisitsDimensionsDlg::CVisitsDimensionsDlg(CWnd* pParent /*= nullptr*/)
	: CDialogEx(IDD_DIALOG_RANDOM_VISITS, pParent)
	, m_Visits(0)
	, m_Ciclic(FALSE)
{

}

CVisitsDimensionsDlg::~CVisitsDimensionsDlg()
{
}

void CVisitsDimensionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VISITS, m_Visits);
	DDX_Check(pDX, IDC_CHECK1, m_Ciclic);
}


BEGIN_MESSAGE_MAP(CVisitsDimensionsDlg, CDialogEx)
END_MESSAGE_MAP()


// Controladores de mensajes de CVisitsDimensionsDlg


void CVisitsDimensionsDlg::OnOK()
{
	if (UpdateData(true)) {
		CDialogEx::OnOK();
	}

}
