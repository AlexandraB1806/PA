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

    // Retin nodurile care sunt vizitate intr-o stiva
    // (se vor retine in ordinea vizitarii)
    stack<int> visited_nodes_stack;

    // Vector in care retin pentru fiecare nod daca a fost
    // vizitat sau nu (adica exista in stiva)
    vector<bool> is_in_stack;

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;
        for (int i = 1, x, y; i <= m; i++) {
            fin >> x >> y;
            adj[x].push_back(y); // arc (x, y)
        }
        fin.close();
    }

    vector<vector<int>> get_result() {
        //
        // TODO: Găsiți componentele tare conexe  (CTC / SCC) ale grafului orientat cu n noduri, stocat în adj.
        //
        // Rezultatul se va returna sub forma unui vector, fiecare element fiind un SCC (adică tot un vector).
        // * nodurile dintr-un SCC pot fi găsite în orice ordine
        // * SCC-urile din graf pot fi găsite în orice ordine
        //
        // Indicație: Folosiți algoritmul lui Tarjan pentru SCC.
        //

        return tarjan_scc();
    }

    // Algoritmul lui Tarjan pentru gasirea componentelor tare-conexe.
    // Folosim o singura parcurgere DFS.
    vector<vector<int>> tarjan_scc() {
        // Initializari:
        parent =  vector<int>(n + 1, -1);
        found = vector<int>(n + 1, -1);
        low_link = vector<int>(n + 1, -1);
        is_in_stack = vector<bool>(n + 1, false);

        // Vizitez toate nodurile
        vector<vector<int>> all_sccs; // toate componentele tare-conexe
        int timestamp = 0;
        for (int node = 1; node <= n; node++) {
            // Nodul care nu are parinte este chiar radacina
            // si setam ca parintele radacinii sa fie chiar ea
            if (parent[node] == -1) {
                parent[node] = node;

                // Se incepe o parcurgere DFS
                dfs(node, timestamp, all_sccs);
            }
        }

        return all_sccs;
    }

    // Parcurgerea DFS
    void dfs(int node, int& timestamp, vector<vector<int>>& all_sccs) {
        // Un nou nod este vizitat, deci actualizez:
        // - timpul de descoperire;
        // - si totodata pozitia in vectorul pentru timpii de start
        found[node] = ++timestamp;
        // Cel mai mic timp de descoperire (intial)
        low_link[node] = found[node];
        // Marchez nodul ca vizitat (il adaug in stiva si
        // validez ca se afla acolo)
        visited_nodes_stack.push(node);
        is_in_stack[node] = true;

        // Vizitez vecinii nodului din lista de adiacenta a acestuia
        for (auto neigh : adj[node]) {
            // Daca nodul nu are parinte (ar putea fi radacina)
            if (parent[neigh] != -1) {
                // Daca se afla in stiva, inseamna ca deja a fost vizitat
                if (is_in_stack[neigh]) {
                    // Actualizez low_link, adica cel mai mic timp de descoperire
                    low_link[node] = min(low_link[node], found[neigh]);
                }
                continue;
            }

            // Salvez parintele
            parent[neigh] = node;

            // Vizitez recursiv copiii din subarbore
            // Ma duc pe vecin
            dfs(neigh, timestamp, all_sccs);

            low_link[node] = min(low_link[node], low_link[neigh]);
        }

        // Nodul este radacina
        if (low_link[node] == found[node]) {
            // Scoatem toate elementele de deasupra nodului din stiva
            vector<int> new_scc;
            do {
                // Iau primul element din varful stivei
                auto x = visited_nodes_stack.top();
                // Il scot din stiva si marchez ca nu mai exista
                visited_nodes_stack.pop();
                is_in_stack[x] = false;

                new_scc.push_back(x);
            } while (new_scc.back() != node);

            all_sccs.push_back(new_scc);
        }
    }

    void print_output(const vector<vector<int>>& all_sccs) {
        ofstream fout("out");
        fout << all_sccs.size() << '\n';
        for (const auto& scc : all_sccs) {
            for (auto node : scc) {
                fout << node << ' ';
            }
            fout << '\n';
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
