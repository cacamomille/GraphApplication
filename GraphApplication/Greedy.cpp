#include "pch.h"
#include "Graph.h"
#include <iterator>
constexpr auto INFINI = 1.79769e+308;

// SalesmanTrackGreedy =========================================================

CTrack ShortestPath(CGraph& graph, CVertex* begin, CVertex* end) {
	CTrack path = CTrack(&graph);
	if (graph.MemberP(begin, end) == 1) {
		path.AddFirst(graph.FindEdge(begin, end));
	}
	else {
		CVertex* vertex = end;
		while (vertex->m_Name != begin->m_Name) {
			for (CEdge* edge : vertex->m_Edges) {
				if (vertex == edge->m_pDestination) {
					if (vertex->m_DijkstraDistance < edge->m_Length + edge->m_pOrigin->m_DijkstraDistance + 0.0001 && vertex->m_DijkstraDistance > edge->m_Length + edge->m_pOrigin->m_DijkstraDistance - 0.0001) {
						path.AddFirst(edge->m_pReverseEdge);
						vertex = edge->m_pOrigin;
					}
				}
				else if (vertex == edge->m_pOrigin) {
					if (vertex->m_DijkstraDistance < edge->m_Length + edge->m_pDestination->m_DijkstraDistance + 0.0001 && vertex->m_DijkstraDistance > edge->m_Length + edge->m_pDestination->m_DijkstraDistance - 0.0001) {
						path.AddFirst(edge->m_pReverseEdge);
						vertex = edge->m_pDestination;
					}
				}
			}
		}
	}
	return path;
}

CTrack SalesmanTrackGreedy(CGraph& graph, CVisits& visits) {
	CTrack track = CTrack(&graph);
	CVertex* v = visits.m_Vertices.front();
	vector<CVertex*> candidats;
	for (CVertex* a : visits.m_Vertices) {
		if (a != visits.m_Vertices.front() && a != visits.m_Vertices.back()) {
			candidats.push_back(a);
		}
	}
	CVertex* v1 = v;
	while (!candidats.empty()) {
		DijkstraQueue(graph, v);
		double minimum = INFINI;
		for (CVertex* b : candidats) {
			if (b->m_DijkstraDistance < minimum) {
				v1 = b;
				minimum = b->m_DijkstraDistance;
			}
		}
		track.Append(ShortestPath(graph, v, v1));
		if (candidats.size() == 1) {
			candidats.clear();
		}
		else {
			auto it = candidats.begin();
			while (it != candidats.end()) {
				if ((*it)->m_Name != v1->m_Name) {
					it++;
				}
				else {
					swap(*it, candidats.back());
					candidats.pop_back();
					break;
				}
			}
		}
		v = v1;
	}
	DijkstraQueue(graph, v);
	track.Append(ShortestPath(graph, v, visits.m_Vertices.back()));
	return track;
}
