#pragma once


// Cuadro de diálogo de CEditEdgeDlg

class CEditEdgeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditEdgeDlg)

public:
	CEditEdgeDlg(CWnd* pParent = nullptr);   // Constructor estándar
	virtual ~CEditEdgeDlg();

	CEdge* m_pEdge;
	CGraph* m_pGraph;

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_EDGE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_Name;
	double m_Value;
};
