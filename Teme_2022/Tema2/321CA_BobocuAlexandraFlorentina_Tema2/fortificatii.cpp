// Bobocu Alexandra-Florentina, 321CA
#include <bits/stdc++.h>
using namespace std;

#define NMAX 100001
#define INF ((long long)(1e15))

class Task {
	public:
    void solve() {
		read_input();
		print_output();
    }

	// Algoritmul lui Dijkstra
    void dijkstra(int source, vector<long long>& d) {
        // Initializarea vectorului de distante cu distante infinite
        fill(d.begin(), d.end(), INF);

        // Coada de prioritati, reprezentata sub forma unui set, in care
        // nodurile sunt ordonate crescator dupa distanta minima fata de sursa
        // cunoscuta pana in acel moment.
        // Elemente de forma: {d[node], node}.
        set<pair<long long, int>> pq;

        // Inserarea nodului de plecare in coada + actualizarea distantei
        d[source] = 0;
        pq.insert({d[source], source});

        // Cat timp mai exista elemente in coada
        while (!pq.empty()) {
            // Scoatem din coada nodul cu distanta minima, ce se afla
			// chiar in varf
            auto top = pq.begin();
            int node = top->second;
            pq.erase(top);

            // Parcurg toti vecinii nodului extras
            for (const auto &[neigh, w] : adj[node]) {
				// Daca intalnesc un nod barbar, nu aplic algoritmul pe el
				if (enemies[neigh] == true) {
					continue;
				}
                // Incerc relaxarea muchiei
                if (d[node] + w < d[neigh]) {
                    // Scot vechea pereche din coada
                    pq.erase({d[neigh], neigh});

                    // Actualizez distanta
                    d[neigh] = d[node] + w;

                    // Inserez noua distanta in coada
                    pq.insert({d[neigh], neigh});
                }
            }
        }

        // Toate nodurile catre care distanta este inca INF nu pot fi atinse
        // din nodul source, deci le setam pe -1.
        for (int i = 1; i <= n; i++) {
            if (d[i] == INF) {
                d[i] = -1;
            }
        }
    }

	private:
    int n, m, k, b, b_nodes;

    vector<pair<int, int>> adj[NMAX];
    int source = 1;

	// Vector in care:
	// true = avem inamic
	// false = avem nod din imperiu
	bool enemies[NMAX] = {false};

	vector<long long> distances;

    void read_input() {
		ifstream fin("fortificatii.in");

        fin >> n >> m >> k;
        fin >> b;

        for (int i = 1; i <= b; i++) {
            fin >> b_nodes;
            enemies[b_nodes] = true;
        }

        for (int i = 1, x, y, w; i <= m; i++) {
            fin >> x >> y >> w;
            // Avem graf neorientat
            adj[x].push_back({y, w});
			adj[y].push_back({x, w});
        }

        fin.close();
    }

    void print_output() {
		ofstream fout("fortificatii.out");
        fout << minimum_time_arrival();
		fout.close();
	}

	struct comparator {
		template<class T>
		bool operator()(T const& a, T const& b) const {
			return a < b;
		}
	};

    long long minimum_time_arrival() {
		// Vector de distante pentru algoritmul lui Dijkstra
        vector<long long> d(n + 1, 0);

		// Fac o singura parcurgere Dijkstra din capitala
		dijkstra(source, d);

		// Parcurg fiecare muchie a fiecarui nod si verific ca nodul curent
		// sa nu fie barbar, iar nodul urmator sa fie barbar.
		// Stiind distanta pana la nodurile din imperiu, populez vectorul
		// distances cu distantele calculate pana la fiecare nod barbar la
		// care este conectat.
		for (int node = 1; node <= n; node++) {
			// Daca nodul curent este barbar, trec mai departe
			if (d[node] == -1) {
				continue;
			}

			// Altfel, am intalnit un nod din imperiu si ii explorez vecinii
			for (const auto &[neigh, w] : adj[node]) {
				// Vecinul urmator este barbar
				if (enemies[neigh] == true) {
					// Populez vectorul cu toate distantele posibile de la
					// barbari la capitala
					distances.push_back(d[node] + w);
				}
			}
		}

		// Sortez crescator vectorul de distante
		sort(distances.begin(), distances.end(), comparator());

		int count = 1;

		for (unsigned int i = 0; i < distances.size() - 1; i++) {
			// Valoarea maxima posibila pe care as putea sa o adaug la
			// distanta curenta
			int quotient = k / count;
			// Cat as mai avea nevoie pentru a ajunge la valoarea
			// distantei de pe pozitia urmatoare
			int diff = distances[i + 1] - distances[i];

			// Valoarea pe care o adaug propriu-zis la distanta curenta
			int minimum = min(quotient, diff);
			distances[i] = distances[i] + minimum;

			// Actualizez numarul de fortificatii
			k = k - minimum * count;

			// Daca nu am suficiente fortificatii de pus pentru actualizarea
			// distantelor, timpul minim in care poate ajunge un barbar in
			// capitala este distanta curenta, deoarece toate celelealte
			// eventuale distante dinaintea sa au aceeasi valoare
			if (k < count) {
				return distances[i];
			}
			if (distances[i] == distances[i + 1]) {
				// Actualizez numarul de distante egale intre ele
				count++;
			}
		}

		// Daca inca mai avem fortificatii de pus, o sa actualizez
		// distantele cu numarul maxim de fortificatii ce pot fi puse
		int quotient = k / count;
		distances[distances.size() - 1] = distances[distances.size() - 1]
										+ quotient;

		return distances[distances.size() - 1];
    }
};

int main() {
    auto* task = new (nothrow) Task();

    if (!task) {
        return -1;
    }
    task->solve();
    delete task;

    return 0;
}
