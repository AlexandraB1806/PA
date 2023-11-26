// Bobocu Alexandra-Florentina, 321CA
#include <bits/stdc++.h>
using namespace std;
#define MOD ((int)1e9 + 7)
#define NUM_LETTERS 26

class Task {
	public:
    void solve() {
		read_input();
		print_output();
    }

	private:
    int n, l;
    string k;
    string s;

	int freq[NUM_LETTERS];

	void read_input() {
        ifstream fin("crypto.in");

        // Lungimea cheii k
		fin >> n;
        // Lungimea subsirului s
        fin >> l;

        // Citesc cele 2 stringuri, k si s
        fin >> k;
        fin >> s;

        fin.close();
    }

    void print_output() {
		ofstream fout("crypto.out");
		fout << get_max_appearances() << "\n";
		fout.close();
	}

	long long get_max_appearances() {
		// Construiesc un vector de frecventa de dimensiunea 26
		// in care, pentru fiecare litera mica din alfabetul englez,
		// numar de cate ori apare in s
		for (char ch : s) {
			freq[ch - 'a' + 1]++;
		}

		long long num_different_characters = 0;
		for (int i = 1; i <= NUM_LETTERS; i++) {
			// Determin numarul de caractere ce difera intre ele
			// in subsirul s
			if (freq[i] != 0) {
				num_different_characters++;
			}
		}

		long long power = 1;

		// Numire recurenta: dp[i][j] = numarul de aparitii alesubsirului s
		// in cheia k
		vector<vector<long long>> dp(l + 1, vector<long long>(n + 1));

		// Gasire recurenta:
		// Caz de baza:
		// 1) dp[i][1] = 1, daca gasim '?' sau chiar primul caracter din sir
		// 			   = 0, altfel
		// Completez prima coloana din matrice
		for (int i = 1; i <= l; i++) {
			unsigned int j = 1;
			// Verific daca in cheie am intalnit '?' sau chiar
			// caracterul din subsirul s (ma intereseaza de fiecare
			// data ultimul sau caracter; daca subsirul s este alcatuit
			// doar dintr-o singura litera, atunci acea litera este si
			// ultimul caracter).
			if (i == 1 && (k[0] == '?' || k[0] == s[j - 1])) {
				dp[i][1] = 1;
				if (j < s.length()) {
					j++;
				}
			} else {
				dp[i][1] = 0;
			}
		}

		if (k[0] == '?') {
			power = power * num_different_characters;
		}

		// 2) dp[1][j]
		// Completez ce a mai ramas din prima linie din matrice
		int num_asking_mark = 0;
		int num_asking_mark_prec;

		// Voi tine cont doar de s[0], deoarece vreau sa
		// completez prima linie
		for (int j = 2; j <= n; j++) {
			// Am nimerit semnul intrebarii
			if (k[j - 1] == '?') {
				num_asking_mark++;

				dp[1][j] = (((num_different_characters % MOD) *
							(dp[1][j - 1] % MOD)) % MOD +
						(power % MOD)) % MOD;
				power = ((power % MOD) *
						(num_different_characters % MOD)) % MOD;
			} else if (k[j - 1] == s[0]) {  // caracterul intalnit si in subsir
				dp[1][j] = ((dp[1][j - 1] % MOD) + (power % MOD)) % MOD;
			} else if (k[j - 1] != s[0]) {
				// Iau valoarea de dinainte, deoarece nu am gasit
				// in cheie caracterul din subsir
				dp[1][j] = dp[1][j - 1] % MOD;
			}
		}

		// Caz general
		for (int i = 2; i <= l; i++) {
			for (int j = 2; j <= n; j++) {
				// Daca dau de semnul intrebarii
				if (k[j - 1] == '?') {
					dp[i][j] = ((dp[i - 1][j - 1] % MOD) +
					((num_different_characters % MOD) *
					(dp[i][j - 1] % MOD)) % MOD) % MOD;
				} else if (k[j - 1] == s[i - 1]) {
					// Am nevoie de valoarea calculata anterior,
					// dar si de cea calculata cu o casuta mai sus
					// si la stanga in matrice
					dp[i][j] = (dp[i][j - 1] % MOD +
								dp[i - 1][j - 1] % MOD) % MOD;
				} else if (k[j - 1] != s[i - 1]) {
					// Iau valoarea calculata anterior
					dp[i][j] = dp[i][j - 1] % MOD;
				}
			}
		}
		return dp[l][n];
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
