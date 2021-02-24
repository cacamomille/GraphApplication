#include "pch.h"
#include "Graph.h"
#include "GraphApplicationDlg.h"



// =============================================================================
// CONVEX HULL =================================================================
// =============================================================================

double PosicioRespeteRecta(CGPoint& a, CGPoint& b, CGPoint &c)
{
	return (a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y);
}

// AreaTriangle ================================================================

double AreaTriangle(CGPoint& a, CGPoint& b, CGPoint c)
{
	return abs((a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y)) / 2.0;
}

// QuickHull ===================================================================

CConvexHull QuickHull(CGraph& g)
{
	return CConvexHull(&g);
}