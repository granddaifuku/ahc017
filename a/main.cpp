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
int WEIGHT = 10000;

using P = pair<int, int>;

struct edge {
  int num;
  int u;
  int v;
  int cost;

  bool operator<(const edge& rhs) const {
    if (cost == rhs.cost) {
      return num < rhs.num;
    }

    return cost < rhs.cost;
  }
};

vector<edge> g;

// 初手に使ったエッジで生成する
set<int> kruskal(vector<edge>& edges) {
  set<int> used;
  DisjointSet dj(N);
  // sort(edges.begin(), edges.end());

  for (const auto& e : edges) {
    int num = e.num, u = e.u, v = e.v;
    if (!dj.isSame(u, v)) {
      used.insert(num);
      dj.link(u, v);
    }
  }

  return used;
}

set<int> weighted_kruskal(const set<int>& unused, int day) {
  vector<edge> edges(M);
  int idx = 0;
  rep(i, M) {
    // 未使用の辺である
    if (unused.find(i) != unused.end()) {
      continue;
    }
    edges[idx] = g[idx];
    idx++;
  }
  for (const auto index : unused) {
    edges[index] = g[index];
  }

  return kruskal(edges);
}

// =================================================

// 残り日数で全ての辺の修理が可能かどうか
bool is_finishable(int edges, int days) { return edges <= days * K; }

// ==================================================

vector<int> solve() {
  vector<int> result(M);
  int remain_days = D;

  // 修理対象の辺
  set<int> unfixed;
  rep(i, M) { unfixed.insert(i); }

  // 残っている修理が必要な辺
  rep(day, D) {
    // 修理対象の辺がない場合は終了
    if (int(unfixed.size()) == 0) {
      break;
    }
    remain_days--;

    // クラスカル法で最小全域木を構成
    set<int> st = weighted_kruskal(unfixed, day);
    set<int> fixed = st;
    // 修理済みの辺を除く
    for (const auto e : st) {
      // 未修理の辺の集合に存在したら continue
      if (unfixed.find(e) != unfixed.end()) {
        continue;
      }
      fixed.erase(e);
    }

    // 修理可能な辺の数を超えた場合
    if (int(fixed.size()) > K) {
      int count = 0;
      auto iter = fixed.begin();
      while (count < K) {
        int e = *iter;
        result[e] = day + 1;
        unfixed.erase(e);
        iter++;
        count++;
      }
    } else {
      // 今回使用された辺を未使用から取り除く
      for (const auto& e : fixed) {
        // 結果に追加
        result[e] = day + 1;
        // 未使用から取り除く
        unfixed.erase(e);
      }
    }

    // 予定日までに終了できない場合は、修理対象の辺から追加する
    while (!is_finishable((int)unfixed.size(), remain_days)) {
      // 先頭要素を取得
      auto begin = unfixed.begin();
      int e = *begin;
      unfixed.erase(e);
      result[e] = day + 1;
    }
  }

  return result;
}

// ==================================================

void input() {
  cin >> N >> M >> D >> K;
  g.resize(M);
  rep(i, M) {
    int u, v, w;
    cin >> u >> v >> w;
    u--, v--;
    edge e = {i, u, v, w};
    g[i] = e;
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
