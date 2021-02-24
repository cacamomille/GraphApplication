#include "pch.h"
#include "Graph.h"
#include "GraphApplicationDlg.h"
#include <queue>

// =============================================================================
// ProgramacioDinamica_INICI_DESTI =============================================
// =============================================================================

CTrack DijkstraQueueINICI_DESTI(CGraph& graph)
{
	CVertex* pInici = graph.GetVertex("INICI");
	CVertex* pDesti = graph.GetVertex("DESTI");
	return CTrack(&graph);
}

// =============================================================================
// TrobaCamiProgramacioDinamicaBB ==============================================
// =============================================================================

CTrack TrobaCamiProgramacioDinamicaBB(CGraph& graph)
{
	CVertex* pInici = graph.GetVertex("INICI");
	CVertex* pDesti = graph.GetVertex("DESTI");
	return CTrack(&graph);
}
