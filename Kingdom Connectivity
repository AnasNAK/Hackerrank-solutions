#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <set>
#include <stack>

#define pb push_back
#define range(i, x, y) for(int i = x; i < y; i++)

using namespace std;

typedef vector<int> v;
typedef vector<v> vv;

vv graph;
v visited;
int n, m;
set<int> cycle_nodes;
set<int> path_nodes;
v cur_path;
v memo;
int MOD = 1000000000;

void read_graph();

void dfs(int node);

void update_path_nodes(int inc);

void update_cycle_nodes(int cycle_start);

bool found_infinite_solution();

int main() {
    read_graph();
    dfs(0);
    if (found_infinite_solution())
        cout << "INFINITE PATHS";
    else
        cout << memo[0];
    return 0;
}

bool found_infinite_solution() {
    for(int cycle_node : cycle_nodes) {
        if (path_nodes.find(cycle_node) != path_nodes.end())
            return true;
    }

    return false;
}

void dfs(int node) {
    visited[node] = true;
    cur_path.pb(node);

    if (node == n - 1)
        update_path_nodes(1);
    else {
        for (int next : graph[node]) {
            if (visited[next])
                update_cycle_nodes(next);
            else {
                if (memo[next] > 0)
                    update_path_nodes(memo[next]);
                if (memo[next] == 0)
                    dfs(next);
            }
        }
    }

    if (memo[node] == 0)
        memo[node] = -1;

    visited[node] = false;
    cur_path.pop_back();
}

void update_cycle_nodes(int cycle_start) {
    auto rit = cur_path.rbegin();
    for(; *rit != cycle_start; rit++) {
        cycle_nodes.insert(*rit);
    }
    cycle_nodes.insert(cycle_start);
}

void update_path_nodes(int inc) {
    for (int cur : cur_path) {
        path_nodes.insert(cur);
        memo[cur] += inc;
        memo[cur] %= MOD;
    }
}

void read_graph() {
    cin >> n >> m;
    range(i, 0, n) {
        graph.pb(v());
        visited.pb(false);
        memo.pb(0);
    }

    int x, y;
    range(i, 0, m) {
        cin >> x >> y;
        graph[--x].pb(--y);
    }
}
