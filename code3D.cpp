#include <bits/stdc++.h>
using namespace std;

class DisjointSet
{
public:
  vector<int> parent, size;

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
};

struct Point
{
  int wt_x = 0.0, wt_y = 0.0, wt_z = 0.0;
};

struct weight
{
  int wt_x, wt_y, wt_z;
};

struct Edges1
{
  int u, v, wt_x, wt_y, wt_z;
};

struct Edges2
{
  int u, v, wt;
};

bool comparator(Edges2 e1, Edges2 e2)
{
  return e1.wt < e2.wt;
}

int INF = INT_MAX;
int n;
vector<Edges1> edges1;
vector<Edges2> edges2;
map<pair<int, int>, weight> vertexToWeight;
vector<Point> ans;
ofstream outputFile("data2.txt");

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
    int u, v, wt_x, wt_y, wt_z;
    inputFile >> u >> v >> wt_x >> wt_y >> wt_z;

    struct Edges1 e;
    e.u = u;
    e.v = v;
    e.wt_x = wt_x;
    e.wt_y = wt_y;
    e.wt_z = wt_z;
    edges1.push_back(e);

    struct weight w;
    w.wt_x = wt_x;
    w.wt_y = wt_y;
    w.wt_z = wt_z;
    vertexToWeight[make_pair(e.u, e.v)] = w;
  }

  // Output the read graph
  for (const auto &e : edges1)
  {
    cout << e.u << " " << e.v << " " << e.wt_x << " " << e.wt_y << " " << e.wt_z << endl;
  }

  inputFile.close();
}

Point computeNormal(Point p1, Point p2, Point p3)
{
  int x1 = p2.wt_x - p1.wt_x;
  int y1 = p2.wt_y - p1.wt_y;
  int z1 = p2.wt_z - p1.wt_z;

  int x2 = p3.wt_x - p1.wt_x;
  int y2 = p3.wt_y - p1.wt_y;
  int z2 = p3.wt_z - p1.wt_z;

  Point normal;

  int x = abs(y1 * z2 - y2 * z1);
  int y = abs(x2 * z1 - x1 * z2);
  int z = abs(x1 * y2 - x2 * y1);

  int hcf = __gcd(__gcd(x, y), z);

  normal.wt_x = x / hcf;
  normal.wt_y = y / hcf;
  normal.wt_z = z / hcf;

  return normal;
}

Point kruskal_algo()
{
  // cout << "kruskal_algo\n";
  DisjointSet ds(n);

  Point p4;

  for (auto e : edges2)
  {
    int parent_u = ds.findParent(e.u);
    int parent_v = ds.findParent(e.v);

    if (parent_u != parent_v)
    {
      ds.unionBySize(parent_u, parent_v);
      weight wt = vertexToWeight[make_pair(e.u, e.v)];
      p4.wt_x += wt.wt_x;
      p4.wt_y += wt.wt_y;
      p4.wt_z += wt.wt_z;
    }
  }

  return p4;
}

Point computeP4(Point p1, Point p2, Point p3)
{
  Point normal = computeNormal(p1, p2, p3);

  for (auto e : edges1)
  {
    int newWt = e.wt_x * normal.wt_x + e.wt_y * normal.wt_y + e.wt_z * normal.wt_z;
    struct Edges2 e2;
    e2.u = e.u;
    e2.v = e.v;
    e2.wt = newWt;
    edges2.push_back(e2);
  }

  sort(edges2.begin(), edges2.end(), comparator);

  Point p4 = kruskal_algo();

  return p4;
}

bool isEqual(Point p1, Point p2)
{
  return p1.wt_x == p2.wt_x and p1.wt_y == p2.wt_y and p1.wt_z == p2.wt_z;
}

void between(Point p1, Point p2, Point p3) {
  Point p4 = computeP4(p1, p2, p3);

  if (isEqual(p1, p4) or isEqual(p2, p4) or isEqual(p3, p4))
  {
    return;
  }
  
  ans.push_back(p4);

  between(p4, p2, p3);
  between(p1, p4, p3);
  between(p1, p2, p4);
}

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  initialize(); // initialize the graph

  struct Point p1, p2, p3;
  p1.wt_x = p2.wt_y = p3.wt_z = INF;

  between(p1, p2, p3);

  // End the timer
  auto end = std::chrono::high_resolution_clock::now();

  // Calculate the duration
  std::chrono::duration<double> duration = end - start;

  // Print the duration in seconds
  std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;

  for(auto e : ans) {
    outputFile << e.wt_x << " " << e.wt_y << " " << e.wt_z << "\n";
    cout << e.wt_x << " " << e.wt_y << " " << e.wt_z << "\n";
  }

  outputFile.close();

  return 0;
}