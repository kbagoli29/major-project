#include <bits/stdc++.h>
using namespace std;

struct edge
{
    int u, v, cap, flow;
};

struct Dinic
{
    vector<edge> e;
    vector<int> ptr;
    vector<int> level;
    vector<vector<int>> adj;
    int n, source, sink;
    Dinic(int n, int s, int t) : n(n), source(s), sink(t)
    {
        level.assign(n, -1);
        adj.assign(n, vector<int>());
        ptr.assign(n, 0);
    }
    void add_edge(int a, int b, int c)
    {
        int k = e.size();
        e.push_back({a, b, c, 0});
        e.push_back({b, a, c, c});
        adj[a].push_back(k);
        adj[b].push_back(k + 1);
    }
    bool bfs()
    {
        fill(level.begin(), level.end(), -1);
        level[source] = 0;
        queue<int> Q;
        Q.push(source);
        while (!Q.empty())
        {
            int x = Q.front();
            Q.pop();
            for (int i : adj[x])
            {
                if (level[e[i].v] == -1 && e[i].flow < e[i].cap)
                {
                    level[e[i].v] = level[x] + 1;
                    Q.push(e[i].v);
                }
            }
        }
        return level[sink] != -1;
    }
    int dfs(int x, int pushed)
    {
        if (x == sink)
            return pushed;
        for (int &id = ptr[x]; id < (int)adj[x].size(); id++)
        {
            int i = adj[x][id];
            if (level[e[i].v] == level[x] + 1 && e[i].flow < e[i].cap)
            {
                int f = dfs(e[i].v, min(pushed, e[i].cap - e[i].flow));
                if (f > 0)
                {
                    e[i].flow += f;
                    e[i ^ 1].flow -= f;
                    return f;
                }
            }
        }
        return 0;
    }
    int calc()
    {
        int ans = 0;
        while (bfs())
        {
            fill(ptr.begin(), ptr.end(), 0);
            while (true)
            {
                int f = dfs(source, INT_MAX);
                if (f == 0)
                    break;
                ans += f;
            }
        }
        return ans;
    }
};

int n, m;

int horizontal_edges_node_number(int row, int col)
{
    return m * row + col + 1;
}

int vertical_edges_node_number(int row, int col)
{
    return (n - 1) * m + n * col + row + 1;
}

int main()
{

    cin >> n >> m;
    string s[n];
    for (int i = 0; i < n; i++)
    {
        cin >> s[i];
    }

    int source = 0;
    int sink = (n - 1) * m + n * (m - 1) + 1;
    int total_nodes = (n - 1) * m + n * (m - 1) + 2;
    int total_vertices = 0;
    int total_pipeable_cells = 0;

    Dinic MXF(total_nodes, source, sink);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (s[i][j] == 'Y')
            {
                total_pipeable_cells++;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i > 0 && s[i - 1][j] == 'Y' && s[i][j] == 'Y')
            {
                MXF.add_edge(source, horizontal_edges_node_number(i - 1, j), 1);
                total_vertices++;
            }
            if (j > 0 && s[i][j - 1] == 'Y' && s[i][j] == 'Y')
            {
                MXF.add_edge(vertical_edges_node_number(i, j - 1), sink, 1);
                total_vertices++;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (s[i][j] == 'Y')
            {
                if (i < n - 1 && j < m - 1 && s[i + 1][j] == 'Y' && s[i][j + 1] == 'Y')
                {
                    MXF.add_edge(horizontal_edges_node_number(i, j),
                                 vertical_edges_node_number(i, j), 1);
                }
                if (i > 0 && j < m - 1 && s[i - 1][j] == 'Y' && s[i][j + 1] == 'Y')
                {
                    MXF.add_edge(horizontal_edges_node_number(i - 1, j),
                                 vertical_edges_node_number(i, j), 1);
                }
                if (i < n - 1 && j > 0 && s[i + 1][j] == 'Y' && s[i][j - 1] == 'Y')
                {
                    MXF.add_edge(horizontal_edges_node_number(i, j),
                                 vertical_edges_node_number(i, j - 1), 1);
                }
                if (i > 0 && j > 0 && s[i - 1][j] == 'Y' && s[i][j - 1] == 'Y')
                {
                    MXF.add_edge(horizontal_edges_node_number(i - 1, j),
                                 vertical_edges_node_number(i, j - 1), 1);
                }
            }
        }
    }

    int maximum_matching = MXF.calc();

    int minimum_vertex_cover = maximum_matching;

    int maximum_independent_set = total_vertices - minimum_vertex_cover;

    int pipes_needed = total_pipeable_cells - maximum_independent_set;

    cout << "minimum pipes required -> " << pipes_needed << '\n';
}