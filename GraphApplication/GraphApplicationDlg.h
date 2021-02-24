
// GraphApplicationDlg.h: archivo de encabezado
//

#pragma once

#include "CGraphCtrl.h"

// Cuadro de diálogo de CGraphApplicationDlg
class CGraphApplicationDlg : public CDialogEx
{
// Construcción
public:
	CGraphApplicationDlg(CWnd* pParent = nullptr);	// Constructor estándar

// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPHAPPLICATION_DIALOG };
#endif
	CGraphCtrl m_GraphCtrl;
	CGraph m_Graph;
	CVisits m_Visits;
	CTrack m_Track;
	CTrack m_Track2;
	CSpanningTree m_SpanningTree;
	CConvexHull m_ConvexHull;

	bool m_Continue;
	bool m_Break;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	CButton m_PanningCtrl;
	CButton m_NewVerticesCtrl;
	CButton m_EditVerticesCtrl;
	CButton m_MoveVerticesCtrl;
	CButton m_DeleteVerticesCtrl;
	CButton m_NewEdgesCtrl;
	CButton m_NewEditEdges;
	CButton m_DeleteEdgesCtrl;
	CButton m_NewVisitsCtrl;

	CButton m_DisplayNamesCtrl;
	CButton m_DisplayDistancesCtrl;
	CButton m_DisplayVerticesCtrl;
	CButton m_DisplayEdgesCtrl;
	CButton m_DisplayVisitsCtrl;
	CButton m_DisplayTrackCtrl;
	CButton m_DisplaySpanningTreeCtrl;

	void UpdateGrafDisplay();

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckPanning();
	afx_msg void OnBnClickedButtonLoadGraph();
	afx_msg void OnBnClickedButtonSaveGraph();
	afx_msg void OnBnClickedButtonCleanGraph();
	afx_msg void OnBnClickedCheckNewVertices();
	afx_msg void OnBnClickedCheckMoveVertices();
	afx_msg void OnBnClickedCheckDeleteVertices();
	afx_msg void OnBnClickedCheckNewEdges();
	afx_msg void OnBnClickedCheckEditEdges();
	afx_msg void OnBnClickedCheckDeleteEdges();
	afx_msg void OnBnClickedCheckEditVertices();
	afx_msg void OnBnClickedCheckNewVisits();
	afx_msg void OnBnClickedButtonCleanVisits();
	afx_msg void OnBnClickedButtonLoadVisits();
	afx_msg void OnBnClickedButtonSaveVisits();
	afx_msg void OnBnClickedButtonLoadTrack();
	afx_msg void OnBnClickedButtonSaveTrack();
	afx_msg void OnBnClickedButtonCleanTrack();
	afx_msg void OnBnClickedButtonLoadSpanningTree();
	afx_msg void OnBnClickedButtonSaveSpanningTree();
	afx_msg void OnBnClickedButtonCleanSpanningTree();
	afx_msg void OnBnClickedButtonCreateRandomGraph();
	afx_msg void OnBnClickedButtonCreateRandomVisits();
	afx_msg void OnBnClickedButtonPrim();
	afx_msg void OnBnClickedButtonKruskal();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonDijkstra();
	afx_msg void OnBnClickedButtonDijkstraQueue();
	afx_msg void OnBnClickedButtonGreedy();
	afx_msg void OnBnClickedButtonBacktracking();
	afx_msg void OnBnClickedButtonBacktrackingGreedy();
	afx_msg void OnBnClickedButtonBranchBound1();
	afx_msg void OnBnClickedButtonBranchBound2();
	afx_msg void OnBnClickedButtonBranchBound3();
	afx_msg void OnBnClickedCheckDisplayNames();
	afx_msg void OnBnClickedCheckDisplayVertices();
	afx_msg void OnBnClickedCheckDisplayEdges();
	afx_msg void OnBnClickedCheckDisplayDistances();
	afx_msg void OnBnClickedCheckDisplayVisits();
	afx_msg void OnBnClickedCheckDisplayTrack();
	afx_msg void OnBnClickedCheckDisplaySpanningTree();
	afx_msg void OnBnClickedButtonLoadDistances();
	afx_msg void OnBnClickedButtonSaveDistances();
	afx_msg void OnBnClickedButtonCleanDistances();
	afx_msg void OnBnClickedButtonTrobaCami1();
	afx_msg void OnBnClickedButtonTrobaCami2();
	afx_msg void OnBnClickedButtonTrobaCami3();
	afx_msg void OnBnClickedButtonLoadConvexHull();
	afx_msg void OnBnClickedButtonSaveConvexHull();
	afx_msg void OnBnClickedButtonCleanConvexHull();
	afx_msg void OnBnClickedButtonQuickHull();
	afx_msg void OnBnClickedButtonBreak();
	afx_msg void OnBnClickedButtonContinue();
	CButton m_BreakCtrl;
	CButton m_ContinueCtrl;
	afx_msg void OnBnClickedButtonToPlannar();
	afx_msg void OnBnClickedButtonTrobaCamiBB1();
	afx_msg void OnBnClickedButtonTrobaCamiBB2();
	afx_msg void OnBnClickedButtonTrobaCamiProgramacioDinamica();
	afx_msg void OnBnClickedButtonTrobaCamiProgramacioDinamicaBB();
	afx_msg void OnBnClickedButtonTrobaCamiBB4();
};

extern CGraphApplicationDlg* pGraphApplicationDlg;
void WaitLoop();