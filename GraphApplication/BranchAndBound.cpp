#include "pch.h"
#include "Graph.h"
#include <queue>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <time.h>
constexpr auto INFINI = 1.79769e+308;



// SalesmanTrackBranchAndBound1 ===================================================

void BaB1Tool(CGraph& graph, int n, vector<CVertex*>& vert_table, vector<vector<double>>& dist_table, int current_index, double& current_length, vector<CVertex*>& current_solution, double& best_length, vector<vector<CVertex*>>& best_solution) {
	for (int j = 0; j < n - 1; j++) {
		if (dist_table[current_index][j] < 1e+300) {
			if (std::find(current_solution.begin(), current_solution.end(), vert_table[j]) == current_solution.end()) {
				current_solution.push_back(vert_table[j]);
				double current_dist = dist_table[current_index][j];
				current_length += current_dist;
				dist_table[current_index][j] = INFINI;
				if (current_solution.size() == vert_table.size() - 1) {
					current_length += dist_table[j][n - 1];
					current_solution.push_back(vert_table[n - 1]);
					if (current_length <= best_length) {
						if (!best_solution.empty()) {
							best_solution.pop_back();
						}
						best_solution.push_back(current_solution);
						best_length = current_length;
					}
					current_solution.pop_back();
					current_length -= dist_table[j][n - 1];
				}
				else {
					if (current_length < best_length) {
						BaB1Tool(graph, n, vert_table, dist_table, j, current_length, current_solution, best_length, best_solution);
					}
				}
				dist_table[current_index][j] = current_dist;
				current_length -= current_dist;
				current_solution.pop_back();
			}
		}
	}
}

CTrack SalesmanTrackBranchAndBound1(CGraph& graph, CVisits& visits) {
	if (visits.m_Vertices.size() == 2) {
		DijkstraQueue(graph, visits.m_Vertices.front());
		return ShortestPath(graph, visits.m_Vertices.front(), visits.m_Vertices.back());
	}
	else {
		int n = visits.m_Vertices.size();
		CTrack track = CTrack(&graph);
		vector<vector<double>> dist_table;
		vector<CVertex*> vert_table;
		int i = 0;
		int j = 0;
		for (CVertex* vi : visits.m_Vertices) {
			// construction of vert_table :
			vert_table.push_back(vi);
			// construction of dist_table :
			DijkstraQueue(graph, vi);
			vector<double> v1;
			j = 0;
			for (CVertex* vj : visits.m_Vertices) {
				if (vi->m_Name == vj->m_Name) {
					v1.push_back(INFINI);
				}
				else if (i == n) {
					v1.push_back(INFINI);
				}
				else if (j == 0) {
					v1.push_back(INFINI);
				}
				else {
					v1.push_back(ShortestPath(graph, vi, vj).Length());
				}
				j++;
			}
			dist_table.push_back(v1);
			i++;
		}
		int current_index = 0;
		double current_length = 0;
		vector<CVertex*> current_solution;
		current_solution.push_back(visits.m_Vertices.front());
		double best_length = INFINI;
		vector<vector<CVertex*>> best_solution;
		BaB1Tool(graph, n, vert_table, dist_table, current_index, current_length, current_solution, best_length, best_solution);
		current_solution.clear();
		current_solution = best_solution.back();
		CVertex* first = current_solution.front();
		for (auto it = current_solution.begin(); it != current_solution.end(); ++it) {
			DijkstraQueue(graph, first);
			track.Append(ShortestPath(graph, first, *it));
			first = *it;
		}
		return track;
	}
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits& visits) {
	int n = visits.m_Vertices.size() - 1;
	CTrack track = CTrack(&graph);
	vector<vector<double>> dist_table;
	vector<CVertex*> vert_table;
	int i = 0;
	int j = 0;
	for (CVertex* vi : visits.m_Vertices) {
		vert_table.push_back(vi);
		DijkstraQueue(graph, vi);
		vector<double> v1;
		j = 0;
		for (CVertex* vj : visits.m_Vertices) {
			if (vi->m_Name == vj->m_Name  || i == n || j == 0) {
				v1.push_back(INFINI);
			}
			else {
				v1.push_back(ShortestPath(graph, vi, vj).Length());
			}
			j++;
		}
		i++;
		dist_table.push_back(v1);
	}
	
	priority_queue<Path*, vector<Path*>, Comp> p_q;
	Path* p = new Path(&graph);
	for (int k = 0; k <= n; k++) {
		p->m_Unvisited.push_back(k);
	}

	p->AddVertex(0);
	p->cota_inf = 0;
	for (int column : p->m_Unvisited) {
		double minCVj = INFINI;
		double maxCVj = 0;
		for (int raw : p->m_Unvisited) {
			if (raw != column && raw!=n && (column != n || raw != 0)) {
				if (dist_table[raw][column] < minCVj) {
					minCVj = dist_table[raw][column];
				}
			}
		}
		p->cota_inf += minCVj;
	}
	p_q.push(p);
	Path optimum = Path(&graph);

	while (!p_q.empty()) {
		Path* top = p_q.top();
		p_q.pop();
		int last = top->m_Vertices.back();

		if (top->m_Unvisited.size() == 0) {
			optimum = *top;
			delete p;
			break;
		}
		else if (top->m_Unvisited.size() == 1) {			
			double minCVj = INFINI;
			for (int raw = 0; raw < n; raw++) {
				if ( raw != last) {
					if (dist_table[raw][n] < minCVj) {
						minCVj = dist_table[raw][n];
					}
				}
			}
			Path* p = new Path(*top);
			p->AddVertex(n);
			p->cota_inf += dist_table[last][n] - minCVj;
			p_q.push(p);
			if (p == p_q.top()) {
				optimum = *p;
				delete p;
				break;
			}
		}
		else {
			for (int j : top->m_Unvisited) {
				if (j != n) {
					double minCVj = INFINI;
					for (int raw = 0; raw < n; raw++) {
						if (raw != j) {
							if (dist_table[raw][j] < minCVj) {
								minCVj = dist_table[raw][j];
							}
						}
					}
					Path* p = new Path(*top);
					p->AddVertex(j);
					p->cota_inf += dist_table[last][j] - minCVj;
					p_q.push(p);
				}
			}
		}
	}

	int begin = optimum.m_Vertices.front();
	for (auto it = optimum.m_Vertices.begin(); it != optimum.m_Vertices.end(); it) {
		it++;
		DijkstraQueue(graph, vert_table[begin]);
		track.Append(ShortestPath(graph,vert_table[begin], vert_table[*it]));
		begin = *it;
		if (*it == optimum.m_Vertices.back()) {
			break;
		}
	}

	return track;
}


// SalesmanTrackBranchAndBound3 ===================================================


CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits& visits) {
	double t1, t2;
	int n = visits.m_Vertices.size() - 1;
	CTrack track = CTrack(&graph);
	vector<vector<double>> dist_table;
	vector<CVertex*> vert_table;
	int j = 0;

	for (CVertex* vi : visits.m_Vertices) {
		vert_table.push_back(vi);
		DijkstraQueue(graph, vi);
		vector<double> v1;
		j = 0;
		for (CVertex* vj : visits.m_Vertices) {
			if (vi->m_Name == vj->m_Name || j == 0) {
				v1.push_back(INFINI);
			}
			else {
				v1.push_back(ShortestPath(graph, vi, vj).Length());
			}
			j++;
		}
		dist_table.push_back(v1);
	}

	priority_queue<Path*, vector<Path*>, Comp> p_q;
	Path* p = new Path(&graph);
	for (int k = 0; k <= n; k++) {
		p->m_Unvisited.push_back(k);
	}

	p->AddVertex(0);
	p->cota_inf = 0;
	p->cota_sup = 0;
	for (int column : p->m_Unvisited) {
		double minCVj = INFINI;
		double maxCVj = 0;
		for (int raw : p->m_Unvisited) {
			if (column != 0 && raw != column && (column != n || raw != 0)) {
				if (dist_table[raw][column] > maxCVj) {
					maxCVj = dist_table[raw][column];
				}
				if (dist_table[raw][column] < minCVj) {
					minCVj = dist_table[raw][column];
				}
			}
		}
		p->cota_inf += minCVj;
		p->cota_sup += maxCVj;
	}
	p_q.push(p);
	double cota_sup_global = p->cota_sup + 10e-4;
	Path optimum = Path(&graph);

	while (!p_q.empty()) {
		Path* top = p_q.top();
		p_q.pop();
		int last = top->m_Vertices.back();

		if (top->m_Unvisited.size() == 1) {
			Path* p = new Path(*top);
			p->AddVertex(n);
			optimum = *p;
			delete p;
			break;
		}
		else if (top->m_Unvisited.size() == 2) {
			Path* p = new Path(*top);
			int n1;
			if (top->m_Unvisited.front() != n) {
				n1 = top->m_Unvisited.front();
			}
			else {
				n1 = top->m_Unvisited.back();
			}
			p->AddVertex(n1);
			p->m_length += dist_table[last][n1];
			if (p->m_length <= cota_sup_global) {
				p->AddVertex(n);
				p->m_length += dist_table[n1][n];
				optimum = *p;
				delete p;
				break;
			}
		}
		else {
			for (int j : top->m_Unvisited) {
				if (j != n) {
					double sum_minCVj = 0;
					double sum_maxCVj = 0;
					for (int column : top->m_Unvisited) {
						if (column != j) {
							double minCVj = INFINI;
							double maxCVj = 0;
							for (int raw : top->m_Unvisited) {
								if (column != j && raw != column && raw != n && (column != n || raw != j)) {
									if (dist_table[raw][column] > maxCVj) {
										maxCVj = dist_table[raw][column];
									}
									if (dist_table[raw][column] < minCVj) {
										minCVj = dist_table[raw][column];
									}
								}
							}
							sum_minCVj += minCVj;
							sum_maxCVj += maxCVj;
						}
					}
					Path* p = new Path(*top);
					p->AddVertex(j);
					p->m_length += dist_table[last][j];
					p->cota_inf = p->m_length + sum_minCVj;
					p->cota_sup = p->m_length + sum_maxCVj;
					if (p->cota_inf <= cota_sup_global) {
						p_q.push(p);
						if (p->cota_sup < cota_sup_global) {
							cota_sup_global = p->cota_sup + 10e-4;
						}
					}

				}
			}
		}
	}

	int begin = optimum.m_Vertices.front();
	for (auto it = optimum.m_Vertices.begin(); it != optimum.m_Vertices.end(); it) {
		it++;
		DijkstraQueue(graph, vert_table[begin]);
		track.Append(ShortestPath(graph, vert_table[begin], vert_table[*it]));
		begin = *it;
		if (*it == optimum.m_Vertices.back()) {
			break;
		}
	}

	return track;
}