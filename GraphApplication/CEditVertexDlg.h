#pragma once


// Cuadro de diálogo de CEditVertexDlg

class CEditVertexDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditVertexDlg)

public:
	CEditVertexDlg(CWnd* pParent = nullptr);   // Constructor estándar
	virtual ~CEditVertexDlg();

	CVertex* m_pVertex;
	CGraph* m_pGraph;

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_VERTEX_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
	double m_X;
	double m_Y;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
