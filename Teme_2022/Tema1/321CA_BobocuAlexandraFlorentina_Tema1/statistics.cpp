// Bobocu Alexandra-Florentina, 321CA
#include <bits/stdc++.h>
using namespace std;

// Structura specifica fiecarui cuvant criptat
struct Gigel_word {
	int freq;
	int length;
};

// Functie care calculeaza numarul de aparitii
// ale unei litere intr-un string
int num_appearances(char letter, string& str) {
    int count = 0;
    for (char elem : str) {
       	if (elem == letter) {
			count++;
		}
	}
	return count;
}

class Task {
	public:
    void solve() {
        read_input();
		print_output();
    }

	private:
    int n;
	string value;
	vector<string> list_words;

    void read_input() {
        ifstream fin("statistics.in");

		fin >> n;
		for (int i = 1; i <= n; i++) {
			// Citesc pe rand cate un cuvant
			fin >> value;
			// Creez lista de cuvinte
			list_words.push_back(value);
		}

        fin.close();
    }

	void print_output() {
		ofstream fout("statistics.out");
		fout << get_max_encrypted_value() << "\n";

		fout.close();
	}

	int get_max_encrypted_value() {
		int num_max_words = -1;

		// Parcurg cele 26 de litere din alfabetul englez
		for (char letter = 'a'; letter <= 'z'; letter++) {
			// Pentru fiecare litera in parte, retin un contor, util
			// pentru calcularea maximului
			int count = 0;
			// Vectorul de structuri, in care voi retine, pentru
			// fiecare cuvant in parte, lungimea cuvantului si
			// numarul de aparitii ale fiecarei litere mici din
			// alfabetul englez
			vector<Gigel_word> encrypted_words;
			// Pentru fiecare cuvant din lista de cuvinte, extrag
			// informatiile necesare
			for (string w : list_words) {
				// Element din vectorul de structuri
				Gigel_word encrypted_word;
				// Populez campurile acestui element
				encrypted_word.length = w.length();
				encrypted_word.freq = num_appearances(letter, w);
				// Populez vectorul de structuri
				encrypted_words.push_back(encrypted_word);
			}
			// Sortarea vectorului nou creat pe baza criteriului
			// ca numarul de aparitii ale literei alese ca fiind cea
			// dominanta sa fie mai mare strict decat lungimea cuvantului
			// impartita la 2 => 2 * freq - length > 0.
			sort(encrypted_words.begin(), encrypted_words.end(),
			[](Gigel_word& w1, Gigel_word& w2)
				{return 2 * w1.freq - w1.length > 2 * w2.freq - w2.length ;});

			Gigel_word concat_encrypted_word;
			concat_encrypted_word.length = 0;
			concat_encrypted_word.freq = 0;
			// Iterez prin vectorul sortat
			for (unsigned int i = 0; i < encrypted_words.size(); i++) {
				concat_encrypted_word.length += encrypted_words[i].length;
				concat_encrypted_word.freq += encrypted_words[i].freq;

				// Daca chiar si dupa concatenarea cuvintelor, conditia
				// pentru litera ce era dominanta inca se respecta (adica este
				// dominanta si in concatenare), atunci incrementez numarul de cuvinte
				if (2 * concat_encrypted_word.freq - concat_encrypted_word.length > 0) {
					count++;
				} else {
					// Litera nu era de fapt dominanta si reiau cautarile
					break;
				}
			}
			// Fac maximul intre numarul de cuvinte calculat deja si cel
			// anterior
			if (count > num_max_words) {
				num_max_words = count;
			}
		}
		return num_max_words;
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
