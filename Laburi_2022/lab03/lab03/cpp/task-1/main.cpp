#include <bits/stdc++.h>
using namespace std;

#define INF 1000000

class Task {
public:
    void solve() {
        read_input();
        print_output(get_result());
    }

private:
    int n, S;
    vector<int> v;

    void read_input() {
        ifstream fin("in");
        fin >> n >> S;
        v.push_back(-1); // adaugare element fictiv - indexare de la 1
        for (int i = 1, e; i <= n; i++) {
            fin >> e;
            v.push_back(e);
        }
        fin.close();
    }

    int get_result() {
        // TODO: Aflati numarul minim de monede ce poate fi folosit pentru a obtine
        // suma S. Tipurile monedelor sunt stocate in vectorul v, de dimensiune n.
        return coins();
    }

	// T = O(S * n) - 2 parcurgeri
	// S = O(S) - vectorul dp
	// unde S = suma, ca dimensiune
	int coins() {
		// dp[i] = nr min de monede cu care pot forma suma i
        vector<int> dp(S + 1, INF); // vector de dimensiune S + 1 initializat cu INF

        // Caz de baza: i = 0 (suma 0 are nevoie de 0 monede)
        dp[0] = 0;

        // Caz general
        // Parcurgerea sumelor - indexare de la 1
        for (int i = 1; i <= S; i++) {
            // Parcurgerea vectorului v de tipuri de monede
            // v[j] = tipul de moneda
            for (int j = 1; j <= n; j++) {
                // Verific sa ma incadrez cu tipul de moneda in suma
                if (v[j] <= i) {
                    // suma_i = moneda_i + (suma_i - moneda_i) <=> 
					// dp[suma_i] = dp[moneda_i] + dp[suma_i + moneda_i];
					// unde dp[moneda_i] = 1 - moneda apare o singura data
                    dp[i] = min(dp[i], dp[i - v[j]] + 1);
                }
            }
        }

		// Verificare daca am putut forma suma S
        if (dp[S] != INF) {
            return dp[S];
        } else {
            return -1;
        }
	}

    void print_output(int result) {
        ofstream fout("out");
        fout << result;
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
