#include "pch.h"
#include "Graph.h"
#include <queue>
#include <vector>
#include <iterator>
constexpr auto INFINI = 1.79769e+308;

// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void Dijkstra(CGraph& graph, CVertex* pStart) {
	for (CVertex& a : graph.m_Vertices) {
		a.m_DijkstraDistance = INFINI;
		a.m_DijkstraVisit = 0;
	}
	pStart->m_DijkstraDistance = 0;
	CVertex* pActual = pStart;
	int visited = 1;
	double minimum;
	while (visited != graph.GetNVertices()) {
		pActual->m_DijkstraVisit = 1;
		minimum = INFINI;
		for (CEdge* b : pActual->m_Edges) {
			if (pActual == b->m_pDestination) {
				if (b->m_pOrigin->m_DijkstraDistance > pActual->m_DijkstraDistance + b->m_Length) {
					b->m_pOrigin->m_DijkstraDistance = pActual->m_DijkstraDistance + b->m_Length;
				}
			}
			else if (pActual == b->m_pOrigin) {
				if (b->m_pDestination->m_DijkstraDistance > pActual->m_DijkstraDistance + b->m_Length) {
					b->m_pDestination->m_DijkstraDistance = pActual->m_DijkstraDistance + b->m_Length;
				}
			}
		}
		for (CVertex& c : graph.m_Vertices) {
			if (c.m_DijkstraVisit == 0 && c.m_DijkstraDistance < minimum) {
				minimum = c.m_DijkstraDistance;
				pActual = &c;
			}
		}
		if (minimum != INFINI) {
			visited++;
		}
		else {
			cout << "All vertices can't be visited" << endl;
			break;
		}
	}
}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

struct Comparador {
	bool operator()(CVertex* cv1, CVertex* cv2) {
		return cv1->m_DijkstraDistance > cv2->m_DijkstraDistance;
	}
};

void DijkstraQueue(CGraph& graph, CVertex* pStart) {
	for (CVertex& a : graph.m_Vertices) {
		a.m_DijkstraDistance = INFINI;
		a.m_DijkstraVisit = 0;
	}
	pStart->m_DijkstraDistance = 0;
	priority_queue<CVertex*, vector<CVertex*>, Comparador> pQueue;
	pQueue.push(pStart);
	while (!pQueue.empty()) {
		CVertex* va = pQueue.top();
		pQueue.pop();
		if (va->m_DijkstraVisit == 0) {
			for (CEdge* b : va->m_Edges) {
				if (va == b->m_pDestination) {
					if (b->m_pOrigin->m_DijkstraDistance > va->m_DijkstraDistance + b->m_Length) {
						b->m_pOrigin->m_DijkstraDistance = va->m_DijkstraDistance + b->m_Length;
						pQueue.push(b->m_pOrigin);
					}
				}
				else if (va == b->m_pOrigin) {
					if (b->m_pDestination->m_DijkstraDistance > va->m_DijkstraDistance + b->m_Length) {
						b->m_pDestination->m_DijkstraDistance = va->m_DijkstraDistance + b->m_Length;
						pQueue.push(b->m_pDestination);
					}
				}
			}
			va->m_DijkstraVisit = 1;
		}
	}
}