#include <bits/stdc++.h>

using namespace std;

#define rep(i, n) for (int i = 0; i < (int)n; ++i)
#define FOR(i, a, b) for (int i = (int)a; i < (int)b; ++i)
#define rrep(i, n) for (int i = ((int)n - 1); i >= 0; --i)

using ll = long long;
using ld = long double;

__attribute__((unused)) const ll INF = 1e18;
__attribute__((unused)) const int Inf = 1e9;
__attribute__((unused)) const double EPS = 1e-9;
__attribute__((unused)) const ll MOD = 1000000007;

int N, M, D, K;

vector<bool> repaired;

using P = pair<int, int>;

struct edge {
  int to;
  int cost;
};

vector<int> dist;
vector<vector<edge>> g;

void dijkstra(int s) {
  dist = vector<int>(N, Inf);
  dist[s] = 0;
  priority_queue<P, vector<P>, greater<P>> pq;
  pq.push(P(0, s));

  while (!pq.empty()) {
    P p = pq.top();
    pq.pop();
    int v = p.second;
    if (dist[v] < p.first) continue;

    rep(i, g[v].size()) {
      edge e = g[v][i];
      if (dist[e.to] > dist[v] + e.cost) {
        dist[e.to] = dist[v] + e.cost;
        pq.push(P(dist[e.to], e.to));
      }
    }
  }
}

// =================================================
void solve() {
  int e = 1;
  rep(day, D) {
    int count = 1;
    while (e <= M) {
      cout << day + 1;
      count++, e++;
      if (count < K) {
        cout << " ";
      } else {
        cout << endl;
        break;
      }
    }
  }
}

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(0);

  // Input
  cin >> N >> M >> D >> K;
  g.resize(N);
  repaired = vector<bool>(M);
  rep(i, M) {
    int u, v, w;
    cin >> u >> v >> w;
    u--, v--;
    edge e;
    e.to = v, e.cost = w;
    g[u].push_back(e);
    e.to = u;
    g[v].push_back(e);
  }
  rep(i, N) {
    int _x, _y;
    cin >> _x >> _y;
  }

  // Solve
  solve();

  return 0;
}
