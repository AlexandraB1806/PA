#include <bits/stdc++.h>
using namespace std;

class Task {
public:
    void solve() {
        read_input();
        print_output(get_result());
    }

private:
    struct Edge {
        int x;
        int y;

        Edge() { }
        Edge(int x, int y)
            : x(x)
            , y(y) { }
    };

    // numarul maxim de noduri
    static constexpr int NMAX = (int)1e5 + 5; // 10^5 + 5 = 100.005

    // n = numar de noduri, m = numar de muchii/arce
    int n, m;

    // adj[node] = lista de adiacenta a nodului node
    // exemplu: daca adj[node] = {..., neigh, ...} => exista arcul (node, neigh)
    vector<int> adj[NMAX];

    // Variabile in plus ajutatoare:
    // Parintele unui nod in parcurgerea DFS:
    vector<int> parent;

    // Tmpul de start din DFS (marchez timpul cand am gasit nodul
    // in momentul in care am inceput sa vizitez subarborele)
    vector<int> found;

    // Cel mai mic timp de descoperire al unui nod
    // low_link[node] =  min { found[x] | x is node OR x in ancestors(node) OR x in descendants(node) };
    vector<int> low_link;

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;
        for (int i = 1, x, y; i <= m; i++) {
            fin >> x >> y; // muchia (x, y)
            adj[x].push_back(y);
            adj[y].push_back(x);
        }
        fin.close();
    }

    vector<Edge> get_result() {
        //
        // TODO: Gasiti toate muchiile critice ale grafului neorientat stocat cu liste de adiacenta in adj.
        //
        // Rezultatul se va returna sub forma unui vector cu toate muchille critice (ordinea lor nu conteaza).
        //
        // Indicație: Folosiți algoritmul lui Tarjan pentru CE.
        //
        
        return tarjan_ce();
    }

    // Algoritmul lui Tarjan pentru gasirea muchiilor critice.
    // Folosim o singura parcurgere DFS.
    vector<Edge> tarjan_ce() {
        // Initializari:
        parent = vector<int>(n + 1, -1);
        found = vector<int>(n + 1, -1);
        low_link = vector<int>(n + 1, -1);

        // Vizitez toate nodurile
        vector<Edge> all_ces;
        int timestamp = 0;
        for (int node = 1; node <= n; node++) {
            // Nodul care nu are parinte este chiar radacina
            // si setam ca parintele radacinii sa fie chiar ea
            if (parent[node] == -1) {
                parent[node] = node;

                // Se incepe o parcurgere DFS
                dfs(node, timestamp, all_ces);
            }
        }

        return all_ces;
    }

    // Parcurgere DFS
    void dfs(int node, int& timestamp, vector<Edge>& all_ces) {
        // Un nou nod este vizitat, deci actualizez:
        // - timpul de descoperire;
        // - si totodata pozitia in vectorul pentru timpii de start
        found[node] = ++timestamp;
        // Cel mai mic timp de descoperire (intial)
        low_link[node] = found[node];

        // Vizitez vecinii nodului din lista de adiacenta a acestuia
        for (auto neigh : adj[node]) {
            // Daca nodul nu are parinte (ar putea fi radacina)
            if (parent[neigh] != -1) {
                if (neigh != parent[node]) {
                    low_link[node] = min(low_link[node], found[neigh]);
                }
                continue;
            }

            // Salvez parintele
            parent[neigh] = node;

            // Vizitez recursiv copiii din subarbore
            // Ma duc pe vecin
            dfs(neigh, timestamp, all_ces);

            low_link[node] = min(low_link[node], low_link[neigh]);

            // (node, neigh) este CE daca:
            // - low_link[neigh] > found[node], unde neigh apartine adj[node].
            if (low_link[neigh] > found[node]) {
                all_ces.push_back(Edge(node, neigh));
            }
        }
    }

    void print_output(const vector<Edge>& all_ces) {
        ofstream fout("out");
        fout << all_ces.size() << '\n';
        for (auto& ce : all_ces) {
            fout << ce.x << ' ' << ce.y << '\n';
        }
        fout.close();
    }
};

// [ATENTIE] NU modifica functia main!
int main() {
    // * se aloca un obiect Task pe heap
    // (se presupune ca e prea mare pentru a fi alocat pe stiva)
    // * se apeleaza metoda solve()
    // (citire, rezolvare, printare)
    // * se distruge obiectul si se elibereaza memoria
    auto* task = new (nothrow) Task(); // hint: cppreference/nothrow
    if (!task) {
        cerr << "new failed: WTF are you doing? Throw your PC!\n";
        return -1;
    }
    task->solve();
    delete task;
    return 0;
}
