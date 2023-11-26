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

        bool operator==(const Edge& other) { return x == other.x && y == other.y; }
        bool operator!=(const Edge& other) { return !(*this == other); }
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

    // Stiva in care se adauga toate muchiile atunci cand se inainteaza in recursivitate
    stack<Edge> edges_stack;

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

    vector<vector<int>> get_result() {
        //
        // TODO: Găsiți componentele biconexe (BCC) ale grafului neorientat cu n noduri, stocat în adj.
        //
        // Rezultatul se va returna sub forma unui vector, fiecare element fiind un BCC (adică tot un vector).
        // * nodurile dintr-un BCC pot fi găsite în orice ordine
        // * BCC-urile din graf pot fi găsite în orice ordine
        //
        // Indicație: Folosiți algoritmul lui Tarjan pentru BCC.
        //

        return tarjan_bcc();
    }

    // Algoritmul lui Tarjan pentru gasirea componentelor biconexe.
    // Folosim o singura parcurgere DFS.
    vector<vector<int>> tarjan_bcc() {
        // Initializari:
        parent = vector<int>(n + 1, -1);
        found = vector<int>(n + 1, -1);
        low_link = vector<int>(n + 1, -1);

        // Vizitez toate nodurile
        vector<vector<int>> all_bccs; // toate componentele biconexe
        int timestamp = 0;
        for (int node = 1; node <= n; node++) {
            // Nodul care nu are parinte este chiar radacina
            // si setam ca parintele radacinii sa fie chiar ea
            if (parent[node] == -1) {
                parent[node] = node;

                // Se incepe o parcurgere DFS
                dfs(node, timestamp, all_bccs);
            }
        }

        return all_bccs;
    }

    // Parcurgerea DFS
    void dfs(int node, int& timestamp, vector<vector<int>>& all_bccs) {
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
            // Adaug muchia in stiva de muchii
            edges_stack.push(Edge(node, neigh));

            // Vizitez recursiv copiii din subarbore
            // Ma duc pe vecin
            dfs(neigh, timestamp, all_bccs);

            low_link[node] = min(low_link[node], low_link[neigh]);

            // Daca low_link[neigh] >= found[node], toate muchiile de deasupra (node, neigh)
            // din stiva sunt din aceeasi componenta biconexa
            if (low_link[neigh] >= found[node]) {
                all_bccs.push_back(get_bcc(Edge(node, neigh)));
            }
        }
    }

    // Functie in care extrag toate muchiile din stiva
    // de deasupra muchiei date ca parametru
    vector<int> get_bcc(Edge stop_edge) {
        unordered_set<int> bcc;

        Edge e;
        do {
            // Iau primul element din varful stivei
            e = edges_stack.top();
            // Il scot din stiva
            edges_stack.pop();

            bcc.insert(e.x);
            bcc.insert(e.y);
        } while (e != stop_edge);

        // Conversie din set in vector
        return vector<int>{bcc.begin(), bcc.end()};
    }

    void print_output(const vector<vector<int>>& all_bccs) {
        ofstream fout("out");
        fout << all_bccs.size() << '\n';
        for (auto& bcc : all_bccs) {
            for (auto node : bcc) {
                fout << node << " ";
            }
            fout << "\n";
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
