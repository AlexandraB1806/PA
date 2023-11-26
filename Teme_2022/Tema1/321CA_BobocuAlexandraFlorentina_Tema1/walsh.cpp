// Bobocu Alexandra-Florentina, 321CA
#include <bits/stdc++.h>
using namespace std;

class Task {
	public:
    void solve() {
		read_and_print_output();
    }

	private:
    int n, k, x, y;

	void read_and_print_output() {
		ofstream fout("walsh.out");
		ifstream fin("walsh.in");
		bool res;
		fin >> n;
		fin >> k;
		for (int i = 1; i <= k; i++) {
			fin >> x >> y;
			res = get_value(n, x, y);
			fout << res << "\n";
		}

		fin.close();
		fout.close();
	}

	bool get_value(int n, int i, int j) {
		// Cazul de baza: matricea este de fapt formata
		// dintr-un singur element: cel de pe linia 1, coloana 1
		if (n == 1) {
			return 0;
		}
		// lungime = n / 2
		int quadrant_length = n / 2;

		// Verific in ce cadran ma aflu:
		// Cadranul I
		if (i <= quadrant_length && j <= quadrant_length) {
			return get_value(n / 2, i, j);
		// Cadranul II
		} else if (i <= quadrant_length && j > quadrant_length) {
			return get_value(n / 2, i, j - quadrant_length);
		// Cadranul III
		} else if (i > quadrant_length && j <= quadrant_length) {
			return get_value(n / 2, i - quadrant_length, j);
		}
		// Cadranul IV
		return !get_value(n / 2, i - quadrant_length, j - quadrant_length);
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
