#include <iostream>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <stdlib.h>

extern "C" {
#include "parse_input.h"
}

#include "dsu.hpp"

struct Coordinate {
	int m_x, m_y, m_z;

	Coordinate(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {}
};

double get_dist(const Coordinate& lhs, const Coordinate& rhs) {
	return std::sqrt( \
		std::pow(lhs.m_x-rhs.m_x, 2) + \
		std::pow(lhs.m_y-rhs.m_y, 2) + \
		std::pow(lhs.m_z-rhs.m_z, 2)
	);
}

struct Pair {
	double m_dist;
	int m_first, m_second;

	Pair(double d, int x, int y) : m_dist(d), m_first(x), m_second(y) {}
	bool operator<(const Pair& other) const {
		return m_dist < other.m_dist;
	}
	bool operator>(const Pair& other) const {
		return m_dist > other.m_dist;
	}
};

int main() {
	// Not gonna lie I'm not writing another parser.
	// We mix together C/C++ in this house.
	Matrix *m = matrix_parse("8.input", ',');

	const int MAX_NUM_CONNECTIONS = 1000;

	std::vector<Coordinate> coords;
	for (int i=0; i<matrix_rows(m); i++) {
		coords.emplace_back( \
			static_cast<int>(strtol(matrix_get(m, i, 0), NULL, 10)), \
			static_cast<int>(strtol(matrix_get(m, i, 1), NULL, 10)), \
			static_cast<int>(strtol(matrix_get(m, i, 2), NULL, 10)) \
		);
	}

	// Get MAX_NUM_CONNECTIONS closest connections
	std::priority_queue<Pair> max_heap;
	for (int i=0; i<matrix_rows(m); i++) {
		for (int j=i+1; j<matrix_rows(m); j++) {
			if (max_heap.size() >= MAX_NUM_CONNECTIONS) {
				if (get_dist(coords.at(i), coords.at(j)) >= \
					max_heap.top().m_dist)
						continue;
				max_heap.pop();
			}
			max_heap.emplace(get_dist(coords.at(i), coords.at(j)), i, j);
		}
	}

	// Create adjacency list
	std::vector<std::vector<int>> adj_list(coords.size(), std::vector<int>{});
	while (max_heap.size()) {
		adj_list.at(max_heap.top().m_first).push_back(max_heap.top().m_second);
		adj_list.at(max_heap.top().m_second).push_back(max_heap.top().m_first);
		max_heap.pop();
	}

	// No I never want to see this again. Go away.
	std::unordered_set<int> seen;
	std::vector<int> res;
	for (int i=0; i<coords.size(); i++) {
		if (seen.size() && seen.count(i))
			continue;
		
		std::stack<int> st({i});
		int curres = 0;
		while (st.size()) {
			int curr = st.top();
			st.pop();

			if (seen.count(curr))
				continue;

			seen.insert(curr);
			curres++;
			for (int j : adj_list.at(curr)) {
				if (!seen.count(j))
					st.push(j);
			}
		}

		res.push_back(curres);
	}

	std::sort(res.begin(), res.end());

	int res_1 = res.at(res.size()-1) * \
		res.at(res.size()-2) * \
		res.at(res.size()-3);
	
	std::cout << "Part 1: " << res_1 << std::endl;

	// Part 2
	init_dsu(coords.size());

	std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> min_heap;
	for (int i=0; i<matrix_rows(m); i++) {
		for (int j=i+1; j<matrix_rows(m); j++) {
			min_heap.emplace(get_dist(coords.at(i), coords.at(j)), i, j);
		}
	}

	long long res_2 = -1;
	while (min_heap.size()) {
		Pair curr = min_heap.top();
		min_heap.pop();
		if (union_sets(curr.m_first, curr.m_second)) {
			res_2 = static_cast<long long>(coords.at(curr.m_first).m_x) *
				coords.at(curr.m_second).m_x;
			break;
		}
	}

	std::cout << "Part 2: " << res_2 << std::endl;

	matrix_free(m);
	return 0;
}