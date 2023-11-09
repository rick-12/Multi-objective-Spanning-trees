#include <bits/stdc++.h>
using namespace std;

class DisjointSet
{
public:
  vector<int> parent, size;
  // unordered_map<int, int> parent, size;

  DisjointSet(int n)
  {
    parent.resize(n);
    size.resize(n, 1);

    for (int i = 0; i < n; i++)
    {
      parent[i] = i;
    }
  }

  int findParent(int u)
  {
    if (u == parent[u])
    {
      return u;
    }

    return parent[u] = findParent(parent[u]);
  }

  void unionBySize(int u, int v)
  {
    parent[u] = v;
    size[v] += size[u];
  }

  void splitBySize(int u, int v)
  {
    size[v] -= size[u];
    parent[u] = u;
  }
};

struct Point
{
  double x, y;
};

struct Edges1
{
  int wt_x, wt_y, u, v;
};

struct Edges2
{
  int wt, u, v;
};

int n;
map<pair<int, int>, pair<int, int>> vertexToWeight;
vector<struct Edges1> edges1;
vector<struct Edges2> edges2;
ofstream outputFile("data.txt");
int n_sts = 0;

void initialize()
{
  ifstream inputFile("input.txt"); // Change the filename as needed

  if (!inputFile)
  {
    cerr << "Failed to open input file." << endl;
    return;
  }

  int m;
  inputFile >> n >> m;
  cout << n << " " << m << endl;

  while (m--)
  {
    int u, v, wt_x, wt_y;
    inputFile >> u >> v >> wt_x >> wt_y;

    struct Edges1 e;
    e.u = u;
    e.v = v;
    e.wt_x = wt_x;
    e.wt_y = wt_y;
    edges1.push_back(e);
    vertexToWeight[make_pair(e.u, e.v)] = make_pair(e.wt_x, e.wt_y);
  }

  // Output the read graph
  for (const auto &e : edges1)
  {
    cout << e.u << " " << e.v << " " << e.wt_x << " " << e.wt_y << endl;
  }

  inputFile.close();
}

// pair<int, int> kruskal_algo1()
// {
//   // cout << "Here" << "\n";
//   DisjointSet ds(n);

//   pair<int, int> ans;

//   for (auto e : edges1)
//   {
//     int parent_u = ds.findParent(e.u);
//     int parent_v = ds.findParent(e.v);

//     // cout << "Here" << "\n";
//     if (parent_u != parent_v)
//     {
//       ds.unionBySize(parent_u, parent_v);
//       ans.first += e.wt_x;
//       ans.second += e.wt_y;
//     }
//   }

//   return ans;
// }

pair<int, int> kruskal_algo()
{
  DisjointSet ds(n);

  pair<int, int> ans;

  for (auto e : edges2)
  {
    int parent_u = ds.findParent(e.u);
    int parent_v = ds.findParent(e.v);

    if (parent_u != parent_v)
    {
      ds.unionBySize(parent_u, parent_v);
      pair<int, int> wt = vertexToWeight[make_pair(e.u, e.v)];
      ans.first += wt.first;
      ans.second += wt.second;
    }
  }

  return ans;
}

bool comparator1(struct Edges1 e1, struct Edges1 e2)
{
  return e1.wt_x < e2.wt_x;
}

bool comparator2(struct Edges1 e1, struct Edges1 e2)
{
  return e1.wt_y < e2.wt_y;
}

bool comparator3(struct Edges2 e1, struct Edges2 e2)
{
  return e1.wt < e2.wt;
}

pair<int, int> computeT3(pair<int, int> &t1, pair<int, int> &t2)
{
  pair<int, int> lambda = make_pair(abs(t2.second - t1.second), abs(t1.first - t2.first));

  for (auto e : edges1)
  {
    int newWt = e.wt_x * lambda.first + e.wt_y * lambda.second;
    struct Edges2 e2;
    e2.u = e.u;
    e2.v = e.v;
    e2.wt = newWt;
    edges2.push_back(e2);
  }

  sort(edges2.begin(), edges2.end(), comparator3);
  pair<int, int> ans = kruskal_algo();

  return ans;
}

void between(pair<int, int> &t1, pair<int, int> &t2, set<pair<int, int>> &ans)
{
  pair<int, int> t3 = computeT3(t1, t2);
  // cout << ans[0].first << " " << ans[0].second << " " << ans[1].first << " " << ans[1].second << endl;
  // if (ans.find(t3) != ans.end())
  // {
  //   return;
  // }
  if (t3 == t1 or t3 == t2)
  {
    return;
  }
  // cout << t3.first << " " << t3.second << "\n";
  // outputFile << t3.first << " " << t3.second << "\n";
  // n_sts++;
  // cout << n_sts << "\n";
  ans.insert(t3);
  between(t1, t3, ans);
  between(t2, t3, ans);
}

void helper(vector<struct Edges1> &edges1, int index, int m, pair<int, int> &ans, pair<int, int> &sum, DisjointSet &ds, int cnt, bool flag)
{
  if (index == m)
  {
    if (cnt != n - 1)
    {
      return;
    }

    if (flag)
    {
      if (sum.first < ans.first)
      {
        ans = sum;
      }
      else if (sum.first == ans.first and sum.second < ans.second)
      {
        ans = sum;
      }
    }
    else
    {
      if (sum.second < ans.second)
      {
        ans = sum;
      }
      else if (sum.second == ans.second and sum.first < ans.first)
      {
        ans = sum;
      }
    }

    return;
  }

  int u = edges1[index].u;
  int v = edges1[index].v;
  int wt_x = edges1[index].wt_x;
  int wt_y = edges1[index].wt_y;

  int parent_u = ds.findParent(u);
  int parent_v = ds.findParent(v);

  if (parent_u != parent_v)
  {
    ds.unionBySize(parent_u, parent_v);
    sum.first += wt_x;
    sum.second += wt_y;

    helper(edges1, index + 1, m, ans, sum, ds, cnt + 1, flag);
    sum.first -= wt_x;
    sum.second -= wt_y;

    ds.splitBySize(parent_u, parent_v);
  }

  helper(edges1, index + 1, m, ans, sum, ds, cnt, flag); // not take
}

pair<pair<int, int>, pair<int, int>> computeLMST()
{
  pair<int, int> lmst1 = make_pair(INT_MAX, INT_MAX);
  pair<int, int> lmst2 = make_pair(INT_MAX, INT_MAX);

  pair<int, int> sum = make_pair(0, 0);
  DisjointSet ds(n);

  helper(edges1, 0, edges1.size(), lmst1, sum, ds, 0, true);

  helper(edges1, 0, edges1.size(), lmst2, sum, ds, 0, false);

  return make_pair(lmst1, lmst2);
}

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  initialize();
  
  pair<pair<int, int>, pair<int, int>> lmst = computeLMST();
  set<pair<int, int>> ans;
  ans.insert(lmst.first);
  ans.insert(lmst.second);

  between(lmst.first, lmst.second, ans);

  // End the timer
  auto end = std::chrono::high_resolution_clock::now();

  // Calculate the duration
  std::chrono::duration<double> duration = end - start;

  // Print the duration in seconds
  std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;

  for (auto e : ans)
  {
    outputFile << e.first << " " << e.second << "\n";
    cout << e.first << " " << e.second << "\n";
  }

  outputFile.close();

  // 6 nodes, 10 edges at min.
  // 50 nodes, 100-200 edges.
  // generate using uniformly rand()
  // gnu plot, matplotlib
  // testing is required.
  // plot the points T1, T2, T3, etc...
  // The lmsts should be lexicographically minimum.
  // make both the components positive.
  // take the inputs from a separate input file.

  return 0;
}