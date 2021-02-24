#include "pch.h"
#include "Graph.h"
#include <vector>
#include <queue>
#include <stack>
using namespace std;


// =============================================================================
// SpanningTreeKruskal =========================================================
// =============================================================================


CSpanningTree SpanningTreeKruskal(CGraph& g)
{
	return CSpanningTree(&g);
}


void Marcar(CGraph& g, CVertex* pVertex)
{
	stack<CVertex*> pila;
	for (CVertex& v : g.m_Vertices) v.m_Marca = false;
	pVertex->m_Marca = true;
	pila.push(pVertex);
	while (!pila.empty()) {
		CVertex* pV = pila.top();
		pila.pop();
		for (CEdge* pE : pV->m_Edges) {
			if (!pE->m_pDestination->m_Marca) {
				pE->m_pDestination->m_Marca = true;
				pila.push(pE->m_pDestination);
			}
		}
	}
}