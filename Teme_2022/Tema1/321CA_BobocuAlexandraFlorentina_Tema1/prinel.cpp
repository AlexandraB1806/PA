// Bobocu Alexandra-Florentina, 321CA
#include <bits/stdc++.h>
using namespace std;

#define MAX_TARGET 100001
#define MAX_POINTS 1000001
#define MAX_N 1001
#define SUM_MAX_TARGET 24000

class Task {
	public:
    void solve() {
		read_input();
		print_output();
    }

	private:
    int n, k, num;

	int target[MAX_N];
	int p[MAX_N];
	int all_operations[MAX_TARGET];

	void read_input() {
        ifstream fin("prinel.in");

		fin >> n;
        fin >> k;
		if (k > SUM_MAX_TARGET) {
			k = SUM_MAX_TARGET;
		}

		for (int i = 1; i <= n; i++) {
			fin >> num;
			target[i] = num;
		}

		for (int i = 1; i <= n; i++) {
			fin >> num;
			p[i] = num;
		}

        fin.close();
    }

	void print_output() {
		ofstream fout("prinel.out");
		fout << get_max_points() << "\n";
		fout.close();
	}

	// Calcularea tuturor divizorilor unui numar si retinerea lor
	// intr-un vector. Nu includ si numarul insusi.
	vector<int> calculate_divisors(int number) {
		vector<int> divisors;
    	for (int i = 1; i * i <= number; i++) {
        	if (number % i == 0) {
				// Daca divizorul si perechea sa sunt patrate perfecte
				if (number / i == i) {
					divisors.push_back(i);
				} else {
					if (i == 1) {
						// Adaug doar divizorul 1
						divisors.push_back(i);
					} else {
						// Adaug divizorul si perechea sa
						divisors.push_back(i);
						divisors.push_back(number / i);
					}
				}
       		}
		}
		return divisors;
	}

	// Calcularea numarului de operatii care trebuiesc efectuate
	// pentru a ajunge de la valoarea initiala 1, pana la un
	// anumit numar din vectorul tinta
	void get_num_operations() {
		// Vector pentru retinerea divizorilor unui numar
		vector<int> divisors;

		// Cazul de baza: pentru a ajunge la 1 nu am nevoie de nicio operatie
		all_operations[1] = 0;

		// Cazul general
		for (int i = 2; i <= MAX_TARGET; i++) {
			// Pentru fiecare valoare posibila din vectorul target,
			// determin lista de divizori (fara a-l include pe insusi numarul)
			divisors = calculate_divisors(i);
			int minim = INT_MAX;
			// Parcurg divizorii numarului i
			for (unsigned int j = 0; j < divisors.size(); j++) {
				// Determin divizorul pentru care as putea obtine un numar
				// minim de operatii
				int div = divisors[j];
				if (all_operations[i - div] < minim) {
					minim = all_operations[i - div];
				}
			}
			// Numarul concret de operatii care trebuie efectuate
			// pentru a ajunge la o anumita valoare
			all_operations[i] = 1 + minim;
		}
	}

	// Algoritmul rucsacului mai eficient, folosind un vector
	// de o dimensiune in loc de 2 dimensiuni
	int knapsack(int operations[], int p[]) {
		int dp[MAX_POINTS];
		memset(dp, 0, sizeof(dp));

		for (int i = 1; i <= n; i++) {
			for (int cap = k; cap >= 0; cap--) {
				if (cap - operations[i] >= 0) {
					dp[cap] = max(dp[cap],
								dp[cap - operations[i]] + p[i]);
				}
			}
		}
		return dp[k];
	}

	int get_max_points() {
        int num_max = 0;

		// Vector de operatii
		int operations[MAX_N];

		get_num_operations();

		// Parcurg toate numerele din target
		for (int i = 1; i <= n; i++) {
			// In acest vector voi retine doar acele valori prezente in
			// vectorul target. Accesand elementul de pe o anumita pozitie
			// din target, voi obtine elementul la care vreau sa ajung, iar
			// pentru vectorul all_operations[], aceasta valoare semnifica de
			// fapt pozitia corespunzatoare inspectarii numarului necesar de
			// operatii pentru a ajunge la acea valoare.
			operations[i] = all_operations[target[i]];
		}

		// Aplic algoritmul rucsacului intre vectorul de punctaje
		// si vectorul in care retin numarul de operatii necesar
		num_max = knapsack(operations, p);

		return num_max;
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
