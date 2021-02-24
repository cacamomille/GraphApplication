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

void AlgoBab2(CGraph& graph, int n, vector<vector<double>>& dist_table, priority_queue<Path*, vector<Path*>, Comp>& p_q) {
	Path* top = p_q.top();
	p_q.pop();
	int last = top->m_Vertices.back();
	if (top->m_Unvisited.size() == 1) {		
		double minCVj = INFINI;
		double maxCVj = 0;
		for (int raw = 0; raw < n; raw++) {
			if (raw != last) {
				if (dist_table[raw][n] > maxCVj) {
					maxCVj = dist_table[raw][n];
				}
				if (dist_table[raw][n] < minCVj) {
					minCVj = dist_table[raw][n];
				}
			}
		}
		Path* p = new Path(*top);
		p->AddVertex(n);
		p->cota_inf += dist_table[last][n] - minCVj;
		p->cota_sup += dist_table[last][n] - maxCVj;
		p_q.push(p);
	}
	else {
		for (int j : top->m_Unvisited) {
			if (j != n) {
				double minCVj = INFINI;
				double maxCVj = 0;
				for (int raw = 0; raw <= n; raw++) {
					if (j != raw && raw != n) {
						if (dist_table[raw][j] > maxCVj) {
							maxCVj = dist_table[raw][j];
						}
						if (dist_table[raw][j] < minCVj) {
							minCVj = dist_table[raw][j];
						}
					}
				}
				Path* p = new Path(*top);
				p->AddVertex(j);
				p->cota_inf += dist_table[last][j] - minCVj;
				p->cota_sup += dist_table[last][j] - maxCVj;
				p_q.push(p);
			}
		}
	}
	if(p_q.top()->m_Vertices.size() <= n){
		AlgoBab2(graph, n, dist_table, p_q);
	}
}

CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits& visits) {
	double t1, t2;
	int n = visits.m_Vertices.size() - 1;
	CTrack track = CTrack(&graph);
	vector<vector<double>> dist_table;
	vector<vector<CTrack>> track_table;
	vector<CVertex*> vert_table;
	int i = 0;
	int j = 0;
	t1 = Clock();
	for (CVertex* vi : visits.m_Vertices) {
		vert_table.push_back(vi);
		DijkstraQueue(graph, vi);
		vector<double> v1;
		vector<CTrack> v2;
		for (CVertex* vj : visits.m_Vertices) {
			if (vi->m_Name == vj->m_Name) {
				v1.push_back(INFINI);
				v2.push_back(NULL);
			}
			else if (i == n) {
				v1.push_back(INFINI);
				v2.push_back(NULL);
			}
			else if (j == 0) {
				v1.push_back(INFINI);
				v2.push_back(NULL);
			}
			else {
				//v1.push_back(ShortestPath(graph, vi, vj).Length());
				CTrack& s_p = ShortestPath(graph, vi, vj);
				v2.push_back(s_p);
				v1.push_back(s_p.Length());
			}
			j++;
		}
		dist_table.push_back(v1);
		track_table.push_back(v2);
		i++;
	}
	t2 = Clock();
	std::cout << "TEMPS creer tableau :  " << endl << fixed << t2-t1 << endl;
	priority_queue<Path*, vector<Path*>, Comp> p_q;
	Path* p = new Path(&graph);
	for (int k = 0; k <= n; k++) {
		p->m_Unvisited.push_back(k);
	}
	p->AddVertex(0);
	p->cota_inf = 0;
	p->cota_sup = 0;
	for (int column = 1; column < n; column++) {
		double minCVj = INFINI;
		double maxCVj = 0;
		for (int raw = 0; raw < n; raw++) {
			if (raw != column && raw != n - 1 && column != 0 && (column != n - 1 || raw != 0)) {
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
	Path optimum = Path(&graph);
	while (!p_q.empty()) {
		Path* top = p_q.top();
		p_q.pop();
		int last = top->m_Vertices.back();
		if (top->m_Unvisited.size() == 1) {
			double minCVj = INFINI;
			double maxCVj = 0;
			for (int raw = 0; raw < n; raw++) {
				if (raw != last) {
					if (dist_table[raw][n] > maxCVj) {
						maxCVj = dist_table[raw][n];
					}
					if (dist_table[raw][n] < minCVj) {
						minCVj = dist_table[raw][n];
					}
				}
			}
			Path* p = new Path(*top);
			p->AddVertex(n);
			p->cota_inf += dist_table[last][n] - minCVj;
			p->cota_sup += dist_table[last][n] - maxCVj;
			p_q.push(p);
		}
		else {
			for (int j : top->m_Unvisited) {
				if (j != n) {
					double minCVj = INFINI;
					double maxCVj = 0;
					for (int raw = 0; raw <= n; raw++) {
						if (j != raw && raw != n) {
							if (dist_table[raw][j] > maxCVj) {
								maxCVj = dist_table[raw][j];
							}
							if (dist_table[raw][j] < minCVj) {
								minCVj = dist_table[raw][j];
							}
						}
					}
					Path* p = new Path(*top);
					p->AddVertex(j);
					p->cota_inf += dist_table[last][j] - minCVj;
					p->cota_sup += dist_table[last][j] - maxCVj;
					p_q.push(p);
				}
			}
		}
		if (p_q.top()->m_Vertices.size() == n+1) {
			optimum = *(p_q.top());
			delete p;
			break;
		}
	}

	t1 = Clock();
	int begin = optimum.m_Vertices.front();
	for (auto it = optimum.m_Vertices.begin(); it != optimum.m_Vertices.end(); it) {
		it++;
		track.Append(track_table[begin][*it]);
		begin = *it;
		if (*it == optimum.m_Vertices.back()) {
			break;
		}
	}
	t2 = Clock();
	std::cout << "TEMPS creer track :  " << endl << fixed << t2 - t1 << endl;
	/*CVertex* first = vert_table[optimum.m_Vertices.front()];
	for (auto it = optimum.m_Vertices.begin(); it != optimum.m_Vertices.end(); it) {
		DijkstraQueue(graph, first);
		it++;
		track.Append(ShortestPath(graph, first, vert_table[*it]));
		first = vert_table[*it];
		if(*it == optimum.m_Vertices.back()) {
			break;
		}
	}*/
	return track;
}

// SalesmanTrackBranchAndBound3 ===================================================


void BaB3Tool(CGraph& graph, int n, vector<CVertex*>& vert_table, vector<vector<double>>& dist_table, int current_index, double& current_length, vector<CVertex*>& current_solution, double& best_length, vector<vector<CVertex*>>& best_solution) {
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
						BaB3Tool(graph, n, vert_table, dist_table, j, current_length, current_solution, best_length, best_solution);
					}
				}
				dist_table[current_index][j] = current_dist;
				current_length -= current_dist;
				current_solution.pop_back();
			}
		}
	}
}

CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits& visits) {
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
		BaB3Tool(graph, n, vert_table, dist_table, current_index, current_length, current_solution, best_length, best_solution);
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

