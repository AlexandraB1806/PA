#include <bits/stdc++.h>
using namespace std;

class Task {
public:
    void solve() {
        read_input();
        print_output(get_result());
    }

private:
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

    // Nu este nevoie de stiva de vizitare.

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

    vector<int> get_result() {
        //
        // TODO: Gasiti toate nodurile critice ale grafului neorientat stocat cu liste de adiacenta in adj.
        // Rezultatul se va returna sub forma unui vector cu toate punctele critice (ordinea nu conteaza).
        //
        // Indicație: Folosiți algoritmul lui Tarjan pentru CV.
        //

        return tarjan_cv();
    }

    // Algoritmul lui Tarjan pentru gasirea punctelor critice.
    // Folosim o singura parcurgere DFS.
    vector<int> tarjan_cv() {
        // Initializari:
        parent = vector<int>(n + 1, -1);
        found = vector<int>(n + 1, -1);
        low_link = vector<int>(n + 1, -1);

        // Vizitez toate nodurile
        // Avem set pentru a elimina eventualele duplicate
        unordered_set<int> all_cvs;
        int timestamp = 0;
        for (int node = 1; node <= n; node++) {
            // Nodul care nu are parinte este chiar radacina
            // si setam ca parintele radacinii sa fie chiar ea
            if (parent[node] == -1) {
                parent[node] = node;

                // Se incepe o parcurgere DFS
                dfs(node, timestamp, all_cvs);
            }
        }

        return {all_cvs.begin(), all_cvs.end()};
    }

    // Parcurgere DFS
    void dfs(int node, int& timestamp, unordered_set<int>& all_cvs) {
        // Un nou nod este vizitat, deci actualizez:
        // - timpul de descoperire;
        // - si totodata pozitia in vectorul pentru timpii de start
        found[node] = ++timestamp;
        // Cel mai mic timp de descoperire (intial)
        low_link[node] = found[node];

        // Vizitez vecinii nodului din lista de adiacenta a acestuia
        int children = 0;
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
            // Numar copiii
            children++;

            // Vizitez recursiv copiii din subarbore
            // Ma duc pe vecin
            dfs(neigh, timestamp, all_cvs);

            low_link[node] = min(low_link[node], low_link[neigh]);

            // node este CV daca:
            // - node is NOT root
            // - low_link[neigh] >= found[node]
            if (parent[node] != node && low_link[neigh] >= found[node]) {
                all_cvs.insert(node);
            }
        }

        // node este CV daca:
        // - node IS root
        // - node are mai mult de 2 copii
        if (parent[node] == node && children > 1) {
            all_cvs.insert(node);
        }
    }

    void print_output(const vector<int>& all_cvs) {
        ofstream fout("out");
        for (auto cv : all_cvs) {
            fout << cv << ' ';
        }
        fout << '\n';
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
