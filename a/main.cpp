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

// ================================================

class DisjointSet {
 public:
  vector<int> rank, p, size;

  DisjointSet() {}
  DisjointSet(int s) {
    rank.resize(s, 0);
    p.resize(s, 0);
    size.resize(s, 0);
    rep(i, s) init(i);
  }

  void init(int x) {
    p[x] = x;
    rank[x] = 0;
    size[x] = 1;
  }

  bool isSame(int x, int y) { return root(x) == root(y); }

  void makeSet(int x, int y) {
    if (isSame(x, y)) return;
    link(root(x), root(y));
  }

  void link(int x, int y) {
    if (rank[x] > rank[y]) {
      p[y] = x;
      size[x] += size[y];
    } else {
      p[x] = y;
      size[y] += size[x];
      if (rank[x] == rank[y]) {
        rank[y]++;
      }
    }
  }

  int root(int x) {
    if (x != p[x]) {
      p[x] = root(p[x]);
    }
    return p[x];
  }

  int getSize(int x) { return size[root(x)]; }
};

// ================================================

int N, M, D, K;

using P = pair<int, int>;

vector<P> connections;

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

// TODO
// 各エッジがどこを結ぶかを記録する
// DFSで連結かどうか探索できる

// 道路が接続されているかを確認する
// Disjoint Set で連結かどうかを確認
bool is_connected(const set<int>& repair) {
  DisjointSet dj(N);
  rep(i, M) {
    if (repair.find(i) != repair.end()) {
      continue;
    }
    int a = connections[i].first, b = connections[i].second;
    dj.makeSet(a, b);
  }

  return dj.getSize(0) == N;
}

// 残り日数で全ての辺の修理が可能かどうか
bool is_finishable(int edges, int days) { return edges <= days * K; }

// ==================================================

vector<int> solve() {
  vector<int> result(M);
  int remain_days = D;

  // 残っている修理が必要な辺
  vector<int> tmp(M);
  iota(tmp.begin(), tmp.end(), 0);
  auto seed = chrono::system_clock::now().time_since_epoch().count();
  shuffle(tmp.begin(), tmp.end(), default_random_engine(seed));
  queue<int> edges;
  rep(i, M) edges.push(tmp[i]);

  rep(day, D) {
    remain_days--;
    // 今回修復する辺の番号の集合
    set<int> repair;

    // 既に確認済みの辺を管理する
    vector<bool> checked(M);

    while (int(repair.size()) < K) {
      // 修理する辺がなくなったらループを抜ける
      if (edges.size() == 0) {
        break;
      }

      int front = edges.front();
      edges.pop();

      // TODO この辺りの処理を見直す

      repair.insert(front);

      // 既に同じ日に2度チェックしている場合は修理する辺に追加する
      if (checked[front]) {
        // 修理を終わらせられる場合は翌日に進む
        if (is_finishable((int)edges.size(), remain_days)) {
          break;
        }
        // それ以外ならばさらに辺を追加する
        continue;
      }

      // グラフが連結でない場合はその辺は使用しない
      if (!is_connected(repair)) {
        repair.erase(front);
        edges.push(front);
        checked[front] = true;
      }
    }

    // 修理する日を登録する
    for (auto e : repair) {
      result[e] = day + 1;
    }
  }

  return result;
}

// ==================================================

void input() {
  cin >> N >> M >> D >> K;
  g.resize(N);
  connections.resize(M);
  rep(i, M) {
    int u, v, w;
    cin >> u >> v >> w;
    u--, v--;
    connections[i] = {u, v};
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
}

void output(const vector<int>& result) {
  assert(int(result.size()) == M);
  rep(i, M) {
    cout << result[i];
    if (i == M - 1) {
      cout << endl;
    } else {
      cout << " ";
    }
  }
}

// ==================================================

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(0);

  input();

  vector<int> result = solve();

  output(result);

  return 0;
}
