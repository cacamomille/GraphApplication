#include "pch.h"
#include "framework.h"
#include "GraphApplication.h"
#include <assert.h>
#include "CGraphCtrl.h"
#include "CEditVertexDlg.h"
#include "CEditEdgeDlg.h"

#define ZOOM_STEP 1.3f
#define SELECT_RADIUS 20
// =============================================================================
// CViewTransform =============================================================
// =============================================================================

// CViewTransform =============================================================

CViewTransform::CViewTransform()
{
	SetIdentity();
}

CViewTransform::CViewTransform(const CViewTransform &t)
{
	m_T[0][0] = t.m_T[0][0]; m_T[0][1] = t.m_T[0][1]; m_T[0][2] = t.m_T[0][2];
	m_T[1][0] = t.m_T[1][0]; m_T[1][1] = t.m_T[1][1]; m_T[1][2] = t.m_T[1][2];

	m_IT[0][0] = t.m_IT[0][0]; m_IT[0][1] = t.m_IT[0][1]; m_IT[0][2] = t.m_IT[0][2];
	m_IT[1][0] = t.m_IT[1][0]; m_IT[1][1] = t.m_IT[1][1]; m_IT[1][2] = t.m_IT[1][2];
}

// operator= =============================================================

CViewTransform CViewTransform::operator=(const CViewTransform &t)
{
	m_T[0][0] = t.m_T[0][0]; m_T[0][1] = t.m_T[0][1]; m_T[0][2] = t.m_T[0][2];
	m_T[1][0] = t.m_T[1][0]; m_T[1][1] = t.m_T[1][1]; m_T[1][2] = t.m_T[1][2];

	m_IT[0][0] = t.m_IT[0][0]; m_IT[0][1] = t.m_IT[0][1]; m_IT[0][2] = t.m_IT[0][2];
	m_IT[1][0] = t.m_IT[1][0]; m_IT[1][1] = t.m_IT[1][1]; m_IT[1][2] = t.m_IT[1][2];
	return *this;
}

// SetIdentity =============================================================

void CViewTransform::SetIdentity()
{
	m_T[0][0] = 1.0; m_T[0][1] = 0.0; m_T[0][2] = 0.0;
	m_T[1][0] = 0.0; m_T[1][1] = 1.0; m_T[1][2] = 0.0;

	m_IT[0][0] = 1.0; m_IT[0][1] = 0.0; m_IT[0][2] = 0.0;
	m_IT[1][0] = 0.0; m_IT[1][1] = 1.0; m_IT[1][2] = 0.0;
}

// Translate ===================================================================

void CViewTransform::Translate(double tx, double ty)
{

	m_T[0][2] += tx;
	m_T[1][2] += ty;

	m_IT[0][2] = m_IT[0][0] * -tx + m_IT[0][1] * -ty + m_IT[0][2];
	m_IT[1][2] = m_IT[1][0] * -tx + m_IT[1][1] * -ty + m_IT[1][2];
}

void CViewTransform::Translate(const CPoint &point)
{
	m_T[0][2] += point.x;
	m_T[1][2] += point.y;

	m_IT[0][2] = m_IT[0][0] * -point.x + m_IT[0][1] * -point.y + m_IT[0][2];
	m_IT[1][2] = m_IT[1][0] * -point.x + m_IT[1][1] * -point.y + m_IT[1][2];
}

// Scale ===================================================================

void CViewTransform::Scale(double s)
{
	m_T[0][0] *= s; m_T[0][1] *= s; m_T[0][2] *= s;
	m_T[1][0] *= s; m_T[1][1] *= s; m_T[1][2] *= s;

	m_IT[0][0] /= s; m_IT[0][1] /= s;
	m_IT[1][0] /= s; m_IT[1][1] /= s;
}

void CViewTransform::Scale(double sx, double sy)
{
	m_T[0][0] *= sx; m_T[0][1] *= sx; m_T[0][2] *= sx;
	m_T[1][0] *= sy; m_T[1][1] *= sy; m_T[1][2] *= sy;


	m_IT[0][0] /= sx; m_IT[0][1] /= sy;
	m_IT[1][0] /= sx; m_IT[1][1] /= sy;
}

// SetScale ====================================================================

void CViewTransform::SetScale(double scale)
{
	Scale(scale / abs(m_T[0][0]));
}

// Rotate ======================================================================
/*

|a11 a12 a13| |c -s 0| |ca11+sa12 -sa11+ca12 a13|
|a21 a22 a23|*|s  c 0|=|ca21+sa22 -sa21+ca22 a23|
|  0  0    1| |0  0 1| |    0          0      1 |

|c -s 0| |a11 a12 a13| |ca11-sa21 ca12-sa22  ca13-sa23|
|s  c 0|*|a21 a22 a23|=|sa11+ca21 sa12+ca22  sa13+ca23|
|0  0 1| |  0  0    1| |    0          0        1     |

*/

void CViewTransform::Rotate(double angle)
{
	double sina = sin(angle);
	double cosa = cos(angle);

	double t00 = m_T[0][0];
	double t01 = m_T[0][1];
	double t02 = m_T[0][2];

	m_T[0][0] = cosa * t00 - sina * m_T[1][0];
	m_T[0][1] = cosa * t01 - sina * m_T[1][1];
	m_T[0][2] = cosa * t02 - sina * m_T[1][2];

	m_T[1][0] = sina * t00 + cosa * m_T[1][0];
	m_T[1][1] = sina * t01 + cosa * m_T[1][1];
	m_T[1][2] = sina * t02 + cosa * m_T[1][2];

	sina = sin(-angle);
	cosa = cos(-angle);

	double it00 = m_IT[0][0];
	double it01 = m_IT[0][1];
	double it10 = m_IT[1][0];
	double it11 = m_IT[1][1];

	m_IT[0][0] = cosa * it00 + sina * it01;
	m_IT[0][1] = -sina * it00 + cosa * it01;

	m_IT[1][0] = cosa * it10 + sina * it11;
	m_IT[1][1] = -sina * it10 + cosa * it11;
}

// Compose =======================================================================

void CViewTransform::Compose(const CViewTransform &t)
{
	double t00 = m_T[0][0];
	double t01 = m_T[0][1];
	double t02 = m_T[0][2];
	double t10 = m_T[1][0];
	double t11 = m_T[1][1];
	double t12 = m_T[1][2];

	m_T[0][0] = t00 * t.m_T[0][0] + t01 * t.m_T[1][0];
	m_T[0][1] = t00 * t.m_T[0][1] + t01 * t.m_T[1][1];
	m_T[0][2] = t00 * t.m_T[0][2] + t01 * t.m_T[1][2] + t02;

	m_T[1][0] = t10 * t.m_T[0][0] + t11 * t.m_T[1][0];
	m_T[1][1] = t10 * t.m_T[0][1] + t11 * t.m_T[1][1];
	m_T[1][2] = t10 * t.m_T[0][2] + t11 * t.m_T[1][2] + t12;

	double it00 = m_IT[0][0];
	double it01 = m_IT[0][1];
	double it02 = m_IT[0][2];
	double it10 = m_IT[1][0];
	double it11 = m_IT[1][1];
	double it12 = m_IT[1][2];

	m_IT[0][0] = t.m_IT[0][0] * it00 + t.m_IT[0][1] * it10;
	m_IT[0][1] = t.m_IT[0][0] * it01 + t.m_IT[0][1] * it11;
	m_IT[0][2] = t.m_IT[0][0] * it02 + t.m_IT[0][1] * it12 + t.m_IT[0][2];

	m_IT[1][0] = t.m_IT[1][0] * it00 + t.m_IT[1][1] * it10;
	m_IT[1][1] = t.m_IT[1][0] * it01 + t.m_IT[1][1] * it11;
	m_IT[1][2] = t.m_IT[1][0] * it02 + t.m_IT[1][1] * it12 + t.m_IT[1][2];
}


// ViewPort ======================================================================

void CViewTransform::SetViewPort(
	const CRect &displayRect,
	const CGRect &worldRect,
	bool keepAspectRatio,
	double maxZoom
)
{
	double sx = (double)displayRect.Width() / max(1e-100, worldRect.Width());
	double sy = (double)displayRect.Height() / max(1e-100, worldRect.Height());
	if (maxZoom != 0) {
		sx = min(sx, maxZoom);
		sy = min(sy, maxZoom);
	}
	if (keepAspectRatio) {
		double s = min(fabs(sx), fabs(sy));
		sx = (sx > 0) ? s : -s;
		sy = (sy > 0) ? s : -s;
	}

	SetIdentity();
	Translate(-worldRect.m_P0.m_X, -worldRect.m_P0.m_Y);
	Scale(sx, sy);
	Translate(
		(displayRect.Width() - sx * worldRect.Width()) / 2 + displayRect.left,
		(displayRect.Height() - sy * worldRect.Height()) / 2 + displayRect.top);
	/*
	Translate(
		(displayRect.Width()-sx*worldRect.Width())/2,
		(displayRect.Height()-sy*worldRect.Height())/2);
	*/
}

// InvApply ====================================================================

CGPoint CViewTransform::InvApply(const CPoint &point) const
{
	return CGPoint(
		m_IT[0][0] * point.x + m_IT[0][1] * point.y + m_IT[0][2],
		m_IT[1][0] * point.x + m_IT[1][1] * point.y + m_IT[1][2]);
}

// InvApply ====================================================================
/*
CGPoint CViewTransform::InvApply(double x, double y) const
{
	return CGPoint(
		m_IT[0][0] * x + m_IT[0][1] * y + m_IT[0][2],
		m_IT[1][0] * x + m_IT[1][1] * y + m_IT[1][2]);
}
*/
// InvApplyX ===================================================================
/*
double CViewTransform::InvApplyX(const CGPoint &point) const
{
	return m_IT[0][0] * point.m_X + m_IT[0][1] * point.m_Y + m_IT[0][2];
}
*/
// InvApplyX ===================================================================
/*
double CViewTransform::InvApplyX(double x, double y) const
{
	return m_IT[0][0] * x + m_IT[0][1] * y + m_IT[0][2];
}
*/
// InvApplyY ===================================================================
/*
double CViewTransform::InvApplyY(const CGPoint &point) const
{
	return m_IT[1][0] * point.m_X + m_IT[1][1] * point.m_Y + m_IT[1][2];
}
*/
// InvApplyY ===================================================================
/*
double CViewTransform::InvApplyY(double x, double y) const
{
	return m_IT[1][0] * x + m_IT[1][1] * y + m_IT[1][2];
}
*/
// InvApplySize ================================================================

double CViewTransform::InvApplySize(double size) const
{
	return m_IT[0][0] * size;
}

// =============================================================================
// CGraphCtrl ==================================================================
// =============================================================================

IMPLEMENT_DYNAMIC(CGraphCtrl, CStatic)

CGraphCtrl::CGraphCtrl()
{
	m_EditMode = PanningMode;
	m_ViewportAjust = true;
	m_pGraph = NULL;
	m_pVisits = NULL;
	m_pTrack = NULL;
	m_pSpanningTree = NULL;
	m_DisplayNames = true;
	m_DisplayDistances = false;
	m_DisplayVertices = true;
	m_DisplayEdges = true;
	m_DisplayVisits = true;
	m_DisplayTrack = true;
	m_DisplaySpanningTree = true;

	m_isPanning = false;
	m_pOrigin = NULL;
}


CGraphCtrl::~CGraphCtrl()
{
}

BEGIN_MESSAGE_MAP(CGraphCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// OnPaint =====================================================================

void CGraphCtrl::OnPaint()
{
	/*
	CPaintDC tmpdc(this); // device context for painting
	CRect upRect;
	GetUpdateRect(&upRect);
	tmpdc.IntersectClipRect(upRect);
	CMemDC memdc(tmpdc, &upRect);
	CDC &dc = memdc.GetDC();
	*/


	if (!m_pGraph) return;
	CPaintDC paintdc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	CMemDC MDC(paintdc, &rect);
	CDC &dc = MDC.GetDC();
	//GetUpdateRect(&rect);
	//dc.IntersectClipRect(rect);
	CBrush br(GetSysColor(COLOR_WINDOW));
	dc.FillRect(rect, &br);

	CFont fuente, * oldfont;
	fuente.CreatePointFont(100, "Consolas");
	oldfont = dc.SelectObject(&fuente);
	CSize baseSize=dc.GetTextExtent("M");
	m_VertexRadius = int(baseSize.cx*1.5);

	if (m_pGraph->GetBackgroundImage()) {
		CVImage* pCVIm = m_pGraph->GetBackgroundImage();
		CPoint p0 = m_VT.Apply(double(pCVIm->GetOrigenX()) - 0.5, double(pCVIm->GetOrigenY()) - 0.5);
		CPoint p1 = m_VT.Apply(
			double(pCVIm->GetOrigenX()) + double(pCVIm->GetszX()) - 0.5,
			double(pCVIm->GetOrigenY() + pCVIm->GetszY()) - 0.5);

		CRect rim(p0, p1);
		pCVIm->Draw(dc, rim.left, rim.top, rim.right, rim.bottom, HALFTONE);
	}

	// Dibujar el grafo -----------------------------------------------------------
	// Dibujar aristas
	{
		if (m_DisplayEdges) {
			if (m_pGraph->m_Directed) {

			}
			else {
				for (CEdge& e : m_pGraph->m_Edges) e.m_Processed = false;
				for (CEdge& e : m_pGraph->m_Edges) if (!e.m_Processed) {
					CPen pen(PS_DASH, max(3, int(m_VertexRadius * 0.25)), e.m_Color);
					CPen* oldpen = dc.SelectObject(&pen);
					CPoint point1 = m_VT.Apply(e.m_pOrigin->m_Point);
					CPoint point2 = m_VT.Apply(e.m_pDestination->m_Point);
					dc.MoveTo(point1);
					dc.LineTo(point2);
					dc.SelectObject(oldpen);
					CPoint midle = m_VT.Apply((e.m_pOrigin->m_Point + e.m_pDestination->m_Point) / 2.0);
					dc.SetTextColor(RGB(0, 0, 0));
					CString str;
					int y1 = midle.y;
					if (m_DisplayNames) {
						str.Format("%s", e.m_Name.c_str());
						dc.TextOut(midle.x, midle.y, str);
						y1 += baseSize.cy;
					}
					if (m_DisplayDistances) {
						str.Format("%f", e.m_Length);
						dc.TextOut(midle.x, y1, str);
					}
					e.m_pReverseEdge->m_Processed = true;
				}
			}
		}

		// Dibujar vértices -------------------------------------------

		if (m_DisplayVertices) {
			for (CVertex& v : m_pGraph->m_Vertices) {
				COLORREF color;
				color = v.m_Color;
				CBrush br(color);
				CBrush* pBr = dc.SelectObject(&br);
				CPoint point = m_VT.Apply(v.m_Point);
				if (&v == m_pOrigin && m_EditMode == NewEdgesDestinationMode) dc.Ellipse(point.x - m_VertexRadius * 2, point.y - m_VertexRadius * 2, point.x + m_VertexRadius * 2, point.y + m_VertexRadius * 2);
				else dc.Ellipse(point.x - m_VertexRadius, point.y - m_VertexRadius, point.x + m_VertexRadius, point.y + m_VertexRadius);
				dc.SelectObject(pBr);
				dc.SetTextColor(RGB(0, 0, 0));
				int y1 = point.y + m_VertexRadius;
				CString str;
				if (m_DisplayNames) {
					str.Format("%s", v.m_Name.c_str());
					dc.TextOut(point.x + m_VertexRadius, y1, str);
					y1 += baseSize.cy;
				}
				if (m_DisplayDistances) {
					str.Format("%f", v.m_DijkstraDistance);
					dc.TextOut(point.x + m_VertexRadius, y1, str);
				}
			}
		}
	}
	// Dibujar las visitas  -------------------------------------------------------
	if (m_DisplayVisits) {
		CBrush br(RGB(255, 128, 64));
		CBrush *pBr = dc.SelectObject(&br);
		bool start = true;
		for (CVertex* pV :  m_pVisits->m_Vertices) {
			CPoint point = m_VT.Apply(pV->m_Point);
			if (start) {
				CBrush br(RGB(0, 0, 0));
				CBrush *pBr = dc.SelectObject(&br);
				dc.Rectangle(point.x - m_VertexRadius, point.y - m_VertexRadius, point.x + m_VertexRadius, point.y + m_VertexRadius);
				dc.SelectObject(pBr);
				start = false;
			}
			else if (pV !=m_pVisits->m_Vertices.back()) dc.Ellipse(point.x - m_VertexRadius, point.y - m_VertexRadius, point.x + m_VertexRadius, point.y + m_VertexRadius);
			else {
				CBrush br(RGB(255, 255, 255));
				CBrush *pBr = dc.SelectObject(&br);
				dc.Rectangle(point.x - m_VertexRadius, point.y - m_VertexRadius, point.x + m_VertexRadius, point.y + m_VertexRadius);
				dc.SelectObject(pBr);
				break;
			}
		}
		dc.SelectObject(pBr);
	}
	// Dibujar distancias ---------------------------------------------------------
	/*
	if (m_DrawDijkstraDistances) {
		dc.SetTextColor(RGB(0, 0, 0));
		for (list<CVertex>::const_iterator iter = m_pGraph->m_Vertices.cbegin(); iter != m_pGraph->m_Vertices.cend(); ++iter) {
			CPoint point(int((iter->m_Point.m_X - mins.m_X)*esc + rect.left), int((iter->m_Point.m_Y - mins.m_Y)*esc + rect.top));
			CString str;
			if (m_IndicesVertices) str.Format("V%03d:%.2lf", m_pGraph->GetVertexIndex(&*iter), iter->m_DijkstraDistance);
			else str.Format("%.2lf", iter->m_DijkstraDistance);
			dc.TextOut(point.x + 10, point.y, str);
		}
	}
	else if (m_IndicesVertices) {
		dc.SetTextColor(RGB(0, 0, 0));
		for (list<CVertex>::const_iterator iter = m_pGraph->m_Vertices.cbegin(); iter != m_pGraph->m_Vertices.cend(); ++iter) {
			CPoint point(int((iter->m_Point.m_X - mins.m_X)*esc + rect.left), int((iter->m_Point.m_Y - mins.m_Y)*esc + rect.top));
			CString str;
			str.Format("V%03d", m_pGraph->GetVertexIndex(&*iter));
			dc.TextOut(point.x + 10, point.y, str);
		}
	}
	*/
	// Dibujar CTrack -------------------------------------------------------------
	if (m_DisplayTrack) {
		// Dibujar el segundo Track
		// Dibujar las aristas del track
		for (CEdge* pE : m_pTrack2->m_Edges) {
			CPoint p0 = m_VT.Apply(pE->m_pOrigin->m_Point);
			CPoint p1 = m_VT.Apply(pE->m_pDestination->m_Point);
			CPen pen;
			if (m_pGraph->MemberP(pE)) pen.CreatePen(PS_SOLID, max(3, int(m_VertexRadius * 0.9)), RGB(100, 100, 100));
			else pen.CreatePen(PS_SOLID, 3, RGB(200, 0, 0));
			CPen* oldpen = dc.SelectObject(&pen);
			dc.MoveTo(p0); dc.LineTo(p1);

			int x = (p0.x * 2 + p1.x) / 3;
			int y = (p0.y * 2 + p1.y) / 3;

			//double dx = p1.x - p0.x;
			//double dy = p1.y - p0.y;
			//double d = sqrt(dx * dx + dy * dy);
			double dx = pE->m_pDestination->m_Point.m_X - pE->m_pOrigin->m_Point.m_X;
			double dy = pE->m_pDestination->m_Point.m_Y - pE->m_pOrigin->m_Point.m_Y;
			double d = sqrt(dx * dx + dy * dy);

			dx = dx / d;
			dy = dy / d;
			double nx = dy;
			double ny = -dx;
			dc.MoveTo((int)(x + nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y + ny * m_VertexRadius - dy * m_VertexRadius + 0.5)); dc.LineTo(x, y);
			dc.MoveTo((int)(x - nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y - ny * m_VertexRadius - dy * m_VertexRadius + 0.5));  dc.LineTo(x, y);
			dc.SelectObject(oldpen);
		}

		// Dibujar los vértices del track
		for (CEdge* pE : m_pTrack2->m_Edges) {
			if (pE == m_pTrack2->m_Edges.front()) {
				CPoint p = m_VT.Apply(pE->m_pOrigin->m_Point);
				CBrush brush(RGB(100, 0, 0));
				CBrush* pBr = dc.SelectObject(&brush);
				dc.Rectangle(p.x - m_VertexRadius*1.5, p.y - m_VertexRadius * 1.5, p.x + m_VertexRadius * 1.5, p.y + m_VertexRadius * 1.5);
				dc.SelectObject(pBr);
			}
			if (pE == m_pTrack2->m_Edges.back()) {
				CPoint p = m_VT.Apply(pE->m_pDestination->m_Point);
				CBrush brush(RGB(0, 100, 0));
				CBrush* pBr = dc.SelectObject(&brush);
				dc.Rectangle(p.x - m_VertexRadius * 1.5, p.y - m_VertexRadius * 1.5, p.x + m_VertexRadius * 1.5, p.y + m_VertexRadius * 1.5);
				dc.SelectObject(pBr);
			}
			else {
				CPoint p = m_VT.Apply(pE->m_pDestination->m_Point);
				CBrush brush(RGB(100, 100, 0));
				CBrush* pBr = dc.SelectObject(&brush);
				dc.Ellipse(p.x - m_VertexRadius * 1.5, p.y - m_VertexRadius * 1.5, p.x + m_VertexRadius * 1.5, p.y + m_VertexRadius * 1.5);
				dc.SelectObject(pBr);
			}
		}

		// Dibujar el primer Track
		// Dibujar las aristas del track
		for (CEdge* pE : m_pTrack->m_Edges) {
			CPoint p0 =m_VT.Apply(pE->m_pOrigin->m_Point);
			CPoint p1= m_VT.Apply(pE->m_pDestination->m_Point);
			CPen pen;
			if (m_pGraph->MemberP(pE)) pen.CreatePen(PS_SOLID, max(3, int(m_VertexRadius * 0.5)), RGB(0, 200, 0));
			else pen.CreatePen(PS_SOLID, 3, RGB(200, 0, 0));
			CPen* oldpen = dc.SelectObject(&pen);
			dc.MoveTo(p0); dc.LineTo(p1);

			int x = (p0.x * 2 + p1.x) / 3;
			int y = (p0.y * 2 + p1.y) / 3;
			double dx = pE->m_pDestination->m_Point.m_X - pE->m_pOrigin->m_Point.m_X;
			double dy = pE->m_pDestination->m_Point.m_Y - pE->m_pOrigin->m_Point.m_Y;
			double d = sqrt(dx * dx + dy * dy);
			dx = dx / d;
			dy = dy / d;
			double nx = dy;
			double ny = -dx;
			dc.MoveTo((int)(x + nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y + ny * m_VertexRadius - dy * m_VertexRadius + 0.5)); dc.LineTo(x, y);
			dc.MoveTo((int)(x - nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y - ny * m_VertexRadius - dy * m_VertexRadius + 0.5));  dc.LineTo(x, y);
			dc.SelectObject(oldpen);
		}

		// Dibujar los vértices del track
		for (CEdge* pE : m_pTrack->m_Edges) {
			if (pE==m_pTrack->m_Edges.front()) {
				CPoint p = m_VT.Apply(pE->m_pOrigin->m_Point);
				CBrush brush(RGB(255, 0, 0));
				CBrush* pBr = dc.SelectObject(&brush);
				dc.Rectangle(p.x - m_VertexRadius, p.y - m_VertexRadius, p.x + m_VertexRadius, p.y + m_VertexRadius);
				dc.SelectObject(pBr);
			}
			if (pE == m_pTrack->m_Edges.back()) {
				CPoint p = m_VT.Apply(pE->m_pDestination->m_Point);
				CBrush brush(RGB(0, 255, 0));
				CBrush* pBr = dc.SelectObject(&brush);
				dc.Rectangle(p.x - m_VertexRadius, p.y - m_VertexRadius, p.x + m_VertexRadius, p.y + m_VertexRadius);
				dc.SelectObject(pBr);
			}
			else {
				CPoint p = m_VT.Apply(pE->m_pDestination->m_Point);
				CBrush brush(RGB(255, 255, 0));
				CBrush* pBr = dc.SelectObject(&brush);
				dc.Ellipse(p.x - m_VertexRadius, p.y - m_VertexRadius, p.x + m_VertexRadius, p.y + m_VertexRadius);
				dc.SelectObject(pBr);
			}
		}
		// Dibujar el orden de los vértices del track
		/*
		int i2 = 0;
		list<CVertex*> apariciones;
		for (list<CVertex*>::iterator iter = m_pTrack->m_Vertices.begin(); iter != m_pTrack->m_Vertices.end();++iter,++i2) {
			CGPoint p = (*iter)->m_Point;
			int x1 = int((p.m_X - mins.m_X)*esc + rect.left);
			int y1 = int((p.m_Y - mins.m_Y)*esc + rect.top);
			int ap = 0;
			for (CVertex *pV : apariciones) {
				if (pV == *iter) ++ap;
			}
			dc.SetTextColor(RGB(255, 64, 0));
			CString str;
			str.Format("%d", i2);
			dc.TextOut(x1 + 15, y1 + 15 * ap, str);
			apariciones.push_back(*iter);
		}
		*/
	}
	// Dibujar CSpanningTree -------------------------------------------------------------
	if (m_DisplaySpanningTree) {
		// Dibujar las aristas del SpanningTree
		for (CEdge* pE : m_pSpanningTree->m_Edges) {
			CPoint p0 = m_VT.Apply(pE->m_pOrigin->m_Point);
			CPoint p1 = m_VT.Apply(pE->m_pDestination->m_Point);
			CPen pen;
			if (m_pGraph->MemberP(pE)) pen.CreatePen(PS_SOLID, max(3, int(m_VertexRadius*0.6)), RGB(200, 200, 0));
			else pen.CreatePen(PS_SOLID, max(3, int(m_VertexRadius * 0.6)), RGB(255, 0, 0));
			CPen* oldpen = dc.SelectObject(&pen);
			dc.MoveTo(p0); dc.LineTo(p1);

			int x = (p0.x * 2 + p1.x) / 3;
			int y = (p0.y * 2 + p1.y) / 3;
			double dx = p1.x - p0.x;
			double dy = p1.y - p0.y;
			double d = sqrt(dx * dx + dy * dy);
			dx = dx / d;
			dy = dy / d;
			double nx = dy;
			double ny = -dx;
			//dc.MoveTo((int)(x + nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y + ny * m_VertexRadius - dy * m_VertexRadius + 0.5)); dc.LineTo(x, y);
			//dc.MoveTo((int)(x - nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y - ny * m_VertexRadius - dy * m_VertexRadius + 0.5));  dc.LineTo(x, y);
			dc.SelectObject(oldpen);
		}
	}
	// Dibujar ConvexHull -------------------------------------------------------------
	if (!m_pConvexHull->m_Vertices.empty()) {
		// Dibujar los vértices unidos
		CVertex* pV = m_pConvexHull->m_Vertices.front();
		if (m_pConvexHull->m_Vertices.size() == 1) {
				CBrush br(RGB(0, 0, 0));
				CBrush* pBr = dc.SelectObject(&br);
				CPoint point = m_VT.Apply(pV->m_Point);
				dc.Ellipse(point.x - m_VertexRadius, point.y - m_VertexRadius, point.x + m_VertexRadius, point.y + m_VertexRadius);
				dc.SelectObject(pBr);
		}
		else {
			CVertex* pAnt = m_pConvexHull->m_Vertices.back();
			for (CVertex* pV : m_pConvexHull->m_Vertices) {
				CPoint p0 = m_VT.Apply(pAnt->m_Point);
				CPoint p1 = m_VT.Apply(pV->m_Point);
				CPen pen;
				pen.CreatePen(PS_SOLID, max(3, int(m_VertexRadius * 0.5)), RGB(0, 0, 0));
				CPen* oldpen = dc.SelectObject(&pen);
				dc.MoveTo(p0); dc.LineTo(p1);

				int x = (p0.x * 2 + p1.x) / 3;
				int y = (p0.y * 2 + p1.y) / 3;
				double dx = pV->m_Point.m_X - pAnt->m_Point.m_X;
				double dy = pV->m_Point.m_Y - pAnt->m_Point.m_Y;
				//double dx = p1.x - p0.x;
				//double dy = p1.y - p0.y;
				double d = sqrt(dx * dx + dy * dy);
				dx = dx / d;
				dy = dy / d;
				double nx = dy;
				double ny = -dx;
				dc.MoveTo((int)(x + nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y + ny * m_VertexRadius - dy * m_VertexRadius + 0.5)); dc.LineTo(x, y);
				dc.MoveTo((int)(x - nx * m_VertexRadius - dx * m_VertexRadius + 0.5), (int)(y - ny * m_VertexRadius - dy * m_VertexRadius + 0.5));  dc.LineTo(x, y);
				dc.SelectObject(oldpen);
				pAnt = pV;
			}
		}
	}
	dc.SelectObject(oldfont);
}

// SetEditMode =================================================================

void CGraphCtrl::SetEditMode(EditMode mode)
{
	m_EditMode = mode;
	m_pOrigin = NULL;
	Invalidate();
}

// SetZoom =====================================================================

void CGraphCtrl::SetZoom(double zoom)
{
	m_ViewportAjust = false;
	CRect wr;
	GetClientRect(wr);
	CPoint cw = CPoint((wr.left + wr.right) / 2, (wr.top + wr.bottom) / 2);
	m_VT.Translate(-cw);
	m_VT.SetScale(zoom);
	m_VT.Translate(cw);
	Invalidate();
}


// SetZoom =====================================================================

void CGraphCtrl::SetZoom(double zoom, CPoint point)
{
	m_ViewportAjust = false;
	m_VT.Translate(-point);
	m_VT.SetScale(zoom);
	m_VT.Translate(point);
	Invalidate();
}


// ZoomToWindow ================================================================

void CGraphCtrl::ZoomToWindow()
{
	m_ViewportAjust = true;
	CRect rect;
	GetClientRect(&rect);
	rect.InflateRect(CSize(-50, -50));
	m_VT.SetViewPort(rect, m_pGraph->RectHull());
	Invalidate();
}

// ZoomIn ======================================================================

void CGraphCtrl::ZoomIn()
{
	m_ViewportAjust = false;
	CRect wr;
	GetClientRect(wr);
	CPoint cw = CPoint((wr.left + wr.right) / 2, (wr.top + wr.bottom) / 2);
	m_VT.Translate(-cw);
	m_VT.Scale(ZOOM_STEP);
	m_VT.Translate(cw);
	Invalidate();
}

// ZoomIn ======================================================================

void CGraphCtrl::ZoomIn(CPoint point)
{
	m_ViewportAjust = false;
	m_VT.Translate(-point);
	m_VT.Scale(ZOOM_STEP);
	m_VT.Translate(point);
	Invalidate();
}


// ZoomOut ============================================================

void CGraphCtrl::ZoomOut()
{
	m_ViewportAjust = false;
	CRect wr;
	GetClientRect(wr);
	CPoint cw = CPoint((wr.left + wr.right) / 2, (wr.top + wr.bottom) / 2);
	m_VT.Translate(-cw);
	m_VT.Scale(1.0 / ZOOM_STEP);
	m_VT.Translate(cw);
	Invalidate();
}

// ZoomOut ============================================================

void CGraphCtrl::ZoomOut(CPoint point)
{
	m_ViewportAjust = false;
	m_VT.Translate(-point);
	m_VT.Scale(1.0 / ZOOM_STEP);
	m_VT.Translate(point);
	Invalidate();
}

// ZoomP =======================================================================

bool CGraphCtrl::ZoomP(double zoom)
{
	return abs(abs(m_VT(0, 0)) - zoom) < 0.000001;
}

// GetZoom =====================================================================

double CGraphCtrl::GetZoom()
{
	return abs(m_VT(0, 0));
}


// OnMouseMove =================================================================

void CGraphCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	//SetFocus();

	CStatic::OnMouseMove(nFlags, point);

	if (m_isPanning) {

		/*
		if (m_changeCursors)
		{
			HCURSOR newCursor = LoadCursorFromFile("PAN_CLOSE.cur");
			HCURSOR oldCursor= ::SetCursor(newCursor);
		}
		*/
		m_VT.Translate(double(point.x) - double(m_p0.x), double(point.y) - double(m_p0.y));
		m_p0 = point;
		m_ViewportAjust = false;
		Invalidate();
	}
	else if (m_pOrigin && m_EditMode == MoveVerticesMode) {
		m_pOrigin->m_Point = m_VT.InvApply(point);
		Invalidate();
	}
}

// OnMouseWheel ================================================================

BOOL CGraphCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect rectWindow;
	GetWindowRect(&rectWindow);
	CPoint pt2 = pt - rectWindow.TopLeft();
	if (zDelta > 0) ZoomIn(pt2);
	else ZoomOut(pt2);

	SetFocus();
	//SetCapture();

	return true;
	//return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}

// OnMButtonDown ===============================================================

void CGraphCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_p0 = point;
	m_isPanning = true;
	//m_zoomCursor = false;
	//m_panCursor = true;

	/*
	if (m_changeCursors)
	{
		HCURSOR newCursor = LoadCursorFromFile("PAN_OPEN.cur");
		HCURSOR oldCursor = ::SetCursor(newCursor);


		if (!m_cursorChanged)
		{
			m_oldCursor = oldCursor;
			m_cursorChanged = true;
		}
	}
	*/

	//CStatic::OnMButtonDown(nFlags, point);
}

// OnMButtonUp =================================================================

void CGraphCtrl::OnMButtonUp(UINT nFlags, CPoint point)
{
	if (m_isPanning) {
		ReleaseCapture();
		m_isPanning = false;
	}
	//CStatic::OnMButtonUp(nFlags, point);

	/*
	if ( (m_changeCursors) && (m_cursorChanged) )
	{
		::SetCursor(m_oldCursor);
		m_cursorChanged = false;
	}
	*/
}

// OnMButtonDblClk =============================================================

void CGraphCtrl::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	SetFocus();
	CStatic::OnMButtonDblClk(nFlags, point);
	ZoomToWindow();
}

// ClearDisplay ================================================================

void CGraphCtrl::ClearDisplay()
{
	m_pGraph = NULL;
	Invalidate();
}


// Show =======================================================================

void CGraphCtrl::Show()
{
	ZoomToWindow();
}

//  OnLButtonDown ==============================================================

void CGraphCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch (m_EditMode) {
	case PanningMode:
		SetCapture();
		m_p0 = point;
		m_isPanning = true;
		break;
	case NewVerticesMode:
		m_pGraph->NewVertex(m_VT.InvApply(point));
		Invalidate();
		break;
	case EditVerticesMode:
	{
		CVertex* pVertex = m_pGraph->FindVertex(m_VT.InvApply(point), m_VT.InvApplySize(m_VertexRadius*2));
		if (pVertex) {
			CEditVertexDlg dlg;
			dlg.m_pVertex = pVertex;
			dlg.m_pGraph = m_pGraph;
			if (dlg.DoModal()==IDOK) Invalidate();
		}
	}
	break;
	case MoveVerticesMode:
	{
		m_pOrigin = m_pGraph->FindVertex(m_VT.InvApply(point), m_VT.InvApplySize(m_VertexRadius * 2));
		if (m_pOrigin) {
			SetCapture();
			Invalidate();
		}
	}
	break;
	case DeleteVerticesMode:
	{
		CVertex* pVertex = m_pGraph->FindVertex(m_VT.InvApply(point), m_VT.InvApplySize(m_VertexRadius * 2));
		if (pVertex) {
			if (m_pVisits) m_pVisits->Delete(pVertex);
			if (m_pTrack) m_pTrack->Delete(pVertex);
			if (m_pSpanningTree) m_pSpanningTree->Delete(pVertex);
			m_pGraph->DeleteVertex(pVertex);
			Invalidate();
		}
	}
	break;
	case NewEdgesMode:
	{
		m_pOrigin = m_pGraph->FindVertex(m_VT.InvApply(point),m_VT.InvApplySize(m_VertexRadius * 2));
		if (m_pOrigin) {
			m_EditMode = NewEdgesDestinationMode;
			Invalidate();
		}
	}
	break;
	case NewEdgesDestinationMode:
	{
		CVertex *pDestination = m_pGraph->FindVertex(m_VT.InvApply(point), m_VT.InvApplySize(m_VertexRadius * 2));
		if (pDestination) {
			m_EditMode = NewEdgesMode;
			if (m_pGraph->MemberP(m_pOrigin, pDestination)) {
				AfxMessageBox("L'aresta ja existeix al graf", MB_OK | MB_ICONERROR);
			}
			else if (pDestination == m_pOrigin) {
				AfxMessageBox("L'aresta ha d'unir vertexs diferents", MB_OK | MB_ICONERROR);
			}
			else m_pGraph->NewEdge(m_pOrigin, pDestination);
			m_pOrigin = NULL;
			Invalidate();
		}
	}
	break;
	case EditEdgesMode:
	{
		CEdge* pEdge = m_pGraph->FindEdge(m_VT.InvApply(point), m_VT.InvApplySize(m_VertexRadius * 2));
		if (pEdge) {
			CEditEdgeDlg dlg;
			dlg.m_pEdge = pEdge;
			dlg.m_pGraph = m_pGraph;
			if (dlg.DoModal() == IDOK) Invalidate();
		}
	}
	break;
	case DeleteEdgesMode:
	{
		CEdge* pEdge = m_pGraph->FindEdge(m_VT.InvApply(point), m_VT.InvApplySize(m_VertexRadius * 2));
		if (pEdge) {
			m_pGraph->DeleteEdge(pEdge);
			Invalidate();
		}
	}
	break;
	case NewVisitsMode:
	{
		CVertex* pV = m_pGraph->FindVertex(m_VT.InvApply(point), m_VT.InvApplySize(m_VertexRadius * 2));
		if (pV) {
			if (m_pVisits->GetNVertices()>=2 && m_pVisits->m_Vertices.front() == m_pVisits->m_Vertices.back()) {
				AfxMessageBox("No es pot afegir vertexs a quan forman un cicle");
			}
			else if (!m_pVisits->m_Vertices.empty() && m_pVisits->m_Vertices.front() == pV) {
				// Cerrar ciclo
				m_pVisits->Add(pV);
				Invalidate();
			}
			else if (m_pVisits->MemberP(pV)) {
				AfxMessageBox("No es pot afegir vertexs que ja pertany a les visites");
			}
			else {
				// Añadir vértice que no pertenece a las visitas
				m_pVisits->Add(pV);
				Invalidate();
			}
		}
	}
	break;
	default: assert(false);
	}
}

// OnLButtonUp =================================================================

void CGraphCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isPanning) {
		ReleaseCapture();
		m_isPanning = false;
	}
	else if (m_pOrigin && m_EditMode == MoveVerticesMode) {
		ReleaseCapture();
		m_pOrigin = NULL;
	}
	CStatic::OnLButtonUp(nFlags, point);
}

// OnLButtonDblClk =============================================================

void CGraphCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pGraph) {
		CClientDC dc(this);
		SetFocus();
	}
	CStatic::OnLButtonDblClk(nFlags, point);
}

// OnRButtonDown ===============================================================

void CGraphCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	if (m_pGraph) {
		CClientDC dc(this);
	}
	CStatic::OnRButtonDown(nFlags, point);
}

// OnRButtonUp =================================================================

void CGraphCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_pGraph) {
		CClientDC dc(this);
	}
	CStatic::OnRButtonUp(nFlags, point);
}

// OnRButtonDblClk =============================================================

void CGraphCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	SetFocus();
	if (m_pGraph) {
		CClientDC dc(this);
	}
	CStatic::OnRButtonDblClk(nFlags, point);
}

// OnKeyDown ==================================================================

void CGraphCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pGraph) {
		CClientDC dc(this);
		switch (nChar) {
		case '0':
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				if (nFlags & (1 << 13)) {
					SetZoom(1);
				}
				else {
					ZoomToWindow();
				}
			}
			break;
		case 107: //'+':
		case 187: //'+':
			ZoomIn();
			break;
		case 109: //'-':
		case 189: //'-':
			ZoomOut();
			break;
		default:
			break;
		}
	}
}

// OnSetFocus ==================================================================

void CGraphCtrl::OnSetFocus(CWnd* pOldWnd)
{
	Invalidate();
	CStatic::OnSetFocus(pOldWnd);
}

// OnKillFocus =================================================================

void CGraphCtrl::OnKillFocus(CWnd* pNewWnd)
{
	Invalidate();
	CStatic::OnKillFocus(pNewWnd);
}


// OnSize ======================================================================


void CGraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: Agregue aquí su código de controlador de mensajes
	if (m_ViewportAjust) {
		ZoomToWindow();
	}
	//cout << "OnSize" << endl;
}
