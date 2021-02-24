#include "pch.h"
#include "Graph.h"
#include <stack>
#include <algorithm>
#include <vector>
auto INFINI = 1.79769e+308;

// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

void ShortestPathBacktracking(CGraph& graph, CVertex* begin, CVertex* end, stack<CTrack>& sp_solution, double& sp_best_length, CTrack& shortest_path) {
	for (CEdge* e : begin->m_Edges) {
		if (e->m_Visited == false && e->m_pReverseEdge->m_Visited == false) {
			CVertex* n;
			if (begin->m_Name == e->m_pDestination->m_Name) {
				n = e->m_pOrigin;
			}
			else {
				n = e->m_pDestination;
			}
			if (n->m_JaHePassat == false) {
				shortest_path.AddLast(e);
				e->m_Visited = true;
				if (shortest_path.Length() < sp_best_length) {
					n->m_JaHePassat = true;
					if (n->m_Name == end->m_Name) {
						if (!sp_solution.empty()) {
							sp_solution.pop();
						}
						sp_solution.push(shortest_path);
						sp_best_length = shortest_path.Length();
					}
					else {
						ShortestPathBacktracking(graph, n, end, sp_solution, sp_best_length, shortest_path);
					}
					n->m_JaHePassat = false;
				}
				e->m_Visited = false;
				shortest_path.Delete(e);
			}
		}
	}
}

void BacktrackingTool(CGraph& graph, CVisits& visits, stack<CTrack>& solution, CVertex* actual, CTrack& path, int count, double& best_length) {
	for (CVertex* a : visits.m_Vertices) {
		if (a->m_Visited == false && a->m_Marca == true) {
			if (a != visits.m_Vertices.back() || (a == visits.m_Vertices.back() && count == visits.GetNVertices() - 1)) {
				stack<CTrack> sp_solution;
				double sp_best_length = INFINI;
				CTrack shortest_path = CTrack(&graph);
				ShortestPathBacktracking(graph, actual, a, sp_solution, sp_best_length, shortest_path);
				path.Append(sp_solution.top());
				if (path.Length() < best_length) {
					a->m_Visited = true;
					count++;
					if (count == visits.GetNVertices()) {
						if (!solution.empty()) {
							solution.pop();
						}
						solution.push(path);
						best_length = path.Length();
					}
					else {
						BacktrackingTool(graph, visits, solution, a, path, count, best_length);
					}
					count--;
					a->m_Visited = false;
				}
				while (path.m_Edges.back() != sp_solution.top().m_Edges.front()) {
					path.m_Edges.pop_back();
				}
				path.m_Edges.pop_back();
			}
		}
	}
}

CTrack SalesmanTrackBacktracking(CGraph& graph, CVisits& visits) {
	for (CVertex& vertex1 : graph.m_Vertices) {
		vertex1.m_Marca = false;
		vertex1.m_Visited = false; // visited for vertices in visits
		vertex1.m_JaHePassat = false; // JaHePassat for other vertices
	}
	for (CVertex* vertex2 : visits.m_Vertices) {
		vertex2->m_Marca = true;
	}
	for (CEdge& edge : graph.m_Edges) {
		edge.m_Visited = false;
	}
	CTrack path = CTrack(&graph);
	path.Clear();
	stack<CTrack> solution;
	visits.m_Vertices.front()->m_Visited = true;
	CVertex* first = visits.m_Vertices.front();
	double best_length = INFINI;
	int count = 1;
	BacktrackingTool(graph, visits, solution, first, path, count, best_length);
	return solution.top();
}

// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================

void BacktrackingGreedyTool(CGraph& graph, CVisits& visits, stack<CTrack>& solution, CVertex* actual, CTrack& path, int count, double& best_length) {
	for (CVertex* a : visits.m_Vertices) {
		if (a->m_Visited == false && a->m_Marca == true) {
			if (a != visits.m_Vertices.back() || (a == visits.m_Vertices.back() && count == visits.GetNVertices() - 1)) {
				CTrack shortest_path = CTrack(&graph);
				DijkstraQueue(graph, actual);
				shortest_path = ShortestPath(graph, actual, a);
				path.Append(shortest_path);
				if (path.Length() < best_length) {
					a->m_Visited = true;
					count++;
					if (count == visits.GetNVertices()) {
						if (!solution.empty()) {
							solution.pop();
						}
						solution.push(path);
						best_length = path.Length();
					}
					else {
						BacktrackingGreedyTool(graph, visits, solution, a, path, count, best_length);
					}
					count--;
					a->m_Visited = false;
				}
				while (path.m_Edges.back() != shortest_path.m_Edges.front()) {
					path.m_Edges.pop_back();
				}
				path.m_Edges.pop_back();
			}
		}
	}
}

CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits) {
	for (CVertex& vertex1 : graph.m_Vertices) {
		vertex1.m_Marca = false;
		vertex1.m_Visited = false; // visited for vertices in visits
	}
	for (CVertex* vertex2 : visits.m_Vertices) {
		vertex2->m_Marca = true;
	}
	CTrack path = CTrack(&graph);
	path.Clear();
	stack<CTrack> solution;
	visits.m_Vertices.front()->m_Visited = true;
	CVertex* first = visits.m_Vertices.front();
	double best_length = INFINI;
	int count = 1;
	BacktrackingGreedyTool(graph, visits, solution, first, path, count, best_length);
	return solution.top();
}