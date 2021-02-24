#pragma once
#include "afxwin.h"
#include "Graph.h"


// =============================================================================
// CVViewTransform =============================================================
// =============================================================================
// Matriz de transformación geométrica

class CViewTransform {
private:
	// Transformación geometrica
	double m_T[2][3];
	double m_IT[2][3];

	//double m_T11,m_T12,m_T13;
	//double m_T21,m_T22,m_T23;
	// Transformación inversa
	//double m_IT11,m_IT12,m_IT13;
	//double m_IT21,m_IT22,m_IT23;
public:
	double operator()(int i, int j) { return m_T[i][j]; }
	double GetT(int i, int j) const { return m_T[i][j]; }
	double GetIT(int i, int j) const { return m_IT[i][j]; }
	CViewTransform();
	CViewTransform(const CViewTransform &t);
	CViewTransform operator=(const CViewTransform &t);
	void SetIdentity();
	void Translate(double tx, double ty);
	void Translate(const CPoint &point);
	//void Translate(const CGPoint &point);
	void Scale(double s);
	void Scale(double sx, double sy);
	void SetScale(double scale);
	void Rotate(double angle);
	void Compose(const CViewTransform &t);
	void CViewTransform::SetViewPort(
		const CRect &displayRect,
		const CGRect &worldRect,
		bool keepAspectRatio = true,
		double maxZoom = 0.0
	);

	CPoint Apply(double x, double y) const {
		return CPoint(
			int(m_T[0][0] * x + m_T[0][1] * y + m_T[0][2]),
			int(m_T[1][0] * x + m_T[1][1] * y + m_T[1][2]));
	}
	int ApplyX(double x, double y) const {
		return int(m_T[0][0] * x + m_T[0][1] * y + m_T[0][2]);
	}
	int ApplyY(double x, double y) const {
		return int(m_T[1][0] * x + m_T[1][1] * y + m_T[1][2]);
	}
	CPoint Apply(const CGPoint &point) const {
		return CPoint(
			int(m_T[0][0] * point.m_X + m_T[0][1] * point.m_Y + m_T[0][2]),
			int(m_T[1][0] * point.m_X + m_T[1][1] * point.m_Y + m_T[1][2]));
	}
	int ApplyX(const CGPoint &point) const {
		return int(m_T[0][0] * point.m_X + m_T[0][1] * point.m_Y + m_T[0][2]);
	}
	int ApplyY(const CGPoint &point) const {
		return int(m_T[1][0] * point.m_X + m_T[1][1] * point.m_Y + m_T[1][2]);
	}
	double ApplySize(double size) const {
		return m_T[0][0] * size;
	}

	CGPoint InvApply(const CPoint &point) const;
	//CGPoint InvApply(double x, double y) const;

	//double InvApplyX(const CGPoint &point) const;
	//double InvApplyX(double x, double y) const;

	//double InvApplyY(const CGPoint &point) const;
	//double InvApplyY(double x, double y) const;
	double InvApplySize(double size) const;

};

// =============================================================================
// CGraphCtrl ==================================================================
// =============================================================================

class CGraphCtrl : public CStatic
{
	DECLARE_DYNAMIC(CGraphCtrl)
public:
	enum EditMode { 
		PanningMode, 
		NewVerticesMode,  EditVerticesMode, MoveVerticesMode, DeleteVerticesMode,
		NewEdgesMode, NewEdgesDestinationMode, EditEdgesMode, DeleteEdgesMode,
		NewVisitsMode
	};
	EditMode m_EditMode;
	CViewTransform m_VT; // Transformación de visualización
	CGRect m_DefaulViewPort; // ViewPort por defecto. Se activa con CTRL-0
	bool m_ViewportAjust; // Ajustar la visualización a la ventana
	bool m_isPanning;
	CPoint m_p0;
	CVertex *m_pOrigin; // Vertice origen de una arista en creación
	int m_VertexRadius; // Radio del vertice

	CGraph *m_pGraph;
	CVisits *m_pVisits;
	CTrack *m_pTrack;
	CTrack* m_pTrack2;
	CSpanningTree* m_pSpanningTree;
	CConvexHull* m_pConvexHull;
	// Data displayed
	bool m_DisplayNames;
	bool m_DisplayDistances;
	bool m_DisplayVertices;
	bool m_DisplayEdges;
	bool m_DisplayVisits;
	bool m_DisplayTrack;
	bool m_DisplaySpanningTree;
public:
	CGraphCtrl();
	virtual ~CGraphCtrl();

	void SetEditMode(EditMode mode);

	void SetZoom(double zoom);
	void SetZoom(double zoom, CPoint point);
	void ZoomToWindow();
	void ZoomOut();
	void ZoomOut(CPoint point);
	void ZoomIn();
	void ZoomIn(CPoint point);
	bool ZoomP(double zoom);
	double GetZoom();

	void ClearDisplay();
	void Show();

protected:
	DECLARE_MESSAGE_MAP()
	// Generated message map functions
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

