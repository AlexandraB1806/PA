#include <bits/stdc++.h>
using namespace std;

class Task {
public:
    void solve() {
        read_input();
		generate_pow2_array();
        print_output(get_result());
    }

private:
    int n, x, y;

    void read_input() {
        ifstream fin("in");
        fin >> n >> x >> y;
        fin.close();
    }

	// Declar separat un vector care contine puterile
	// lui 2 pana la n
	vector<int> pow2_array;

	// Construirea vectorului
	void generate_pow2_array() {
		int power = 1;
		for (int i = 0; i <= n; i++) {
			pow2_array.push_back(power);
			power = power * 2;
		}
	}

    int get_value(int n, int x, int y) {
        // De calculat valoarea de pe pozitia (x, y) din matricea de dimensiune 2^n x 2^n

		// Cazul de baza: matricea este de fapt formata
		// dintr-un singur element; acel element este 1
		if (n == 0) {
			return 1;
		}
		
		// lungime = 2^(n-1)
		int quadrant_length = pow2_array[n - 1];
		// dimensiune = 2^(n-1) * 2^(n-1)
		int quadrant_size = quadrant_length * quadrant_length;

		// Verific in ce cadran ma aflu:
		// Cadranul I
		if (y <= quadrant_length && x <= quadrant_length) {
			return get_value(n - 1, x, y);
		// Cadranul II
		} else if (y > quadrant_length && x <= quadrant_length) {
			return quadrant_size + get_value(n - 1, x, y - quadrant_length);
		// Cadranul III
		} else if (y <= quadrant_length && x > quadrant_length) {
			return 2 * quadrant_size + get_value(n - 1, x - quadrant_length, y);
		// Cadranul IV
		} else if (y > quadrant_length && x > quadrant_length) {
			return 3 * quadrant_size + get_value(n - 1, x - quadrant_length, y - quadrant_length);
		}

		// Daca nu s-a intrat pe niciunul dintre celelealte cazuri
		return 0;
    }

    int get_result() {
		return get_value(n, x, y);
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
