#pragma once


// Cuadro de diálogo de CVisitsDimensionsDlg

class CVisitsDimensionsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVisitsDimensionsDlg)

public:
	CVisitsDimensionsDlg(CWnd* pParent = nullptr);   // Constructor estándar
	virtual ~CVisitsDimensionsDlg();

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RANDOM_VISITS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_Visits;
	virtual void OnOK();
	BOOL m_Ciclic;
};
