import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;

public class sol_2020_sp {
    static class Task {
        public static final String INPUT_FILE = "in";
        public static final String OUTPUT_FILE = "out";

        // numarul maxim de noduri
        public static final int NMAX = (int)1e5 + 5; // 10^5 + 5 = 100.005

        // n = numar de noduri, m = numar de muchii/arce
        int n, m;

        // adj[node] = lista de adiacenta a nodului node
        // exemplu: daca adj[node] = {..., neigh, ...} => exista muchia (node, neigh)
        @SuppressWarnings("unchecked")
        ArrayList<Integer> adj[] = new ArrayList[NMAX];

        boolean[] visited;

        // nivelul unui nod in arbore (radacina are nivel 1, fii radacinii nivel 2 etc.)
        int[] lvl;

        // nivelul minim la care un nod poate ajunge daca taiem muchia catre parintele sau
        int[] low_link;

        public void solve() {
            readInput();
            writeOutput(getResult());
        }

        private void readInput() {
            try {
                Scanner sc = new Scanner(new BufferedReader(new FileReader(
                                INPUT_FILE)));
                n = sc.nextInt();
                m = sc.nextInt();

                for (int i = 1; i <= n; i++)
                    adj[i] = new ArrayList<>();

                for (int i = 1; i <= m; i++) {
                    int x, y; // muchie (x, y)
                    x = sc.nextInt();
                    y = sc.nextInt();
                    adj[x].add(y);
                    adj[y].add(x);
                }
                sc.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private void writeOutput(ArrayList<Integer> result) {
            try {
                PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(
                                OUTPUT_FILE)));
                for (int node : result) {
                    pw.printf("%d ", node);
                }
                pw.printf("\n");
                pw.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private ArrayList<Integer> getResult() {
            // TODO: Gasiti nodurile critice ale grafului neorientat stocat cu liste de adiacenta in adj.
            ArrayList<Integer> sol = new ArrayList<>();

            visited = new boolean[n + 1];
            lvl = new int[n + 1];
            low_link = new int[n + 1];

            for (int i = 1; i <= n; ++i) {
                if (!visited[i])
                    dfs(i, 0, sol);
            }

            return sol;
        }

        void dfs(int node, int parent, ArrayList<Integer> sol) {
            // Marcam nodul ca vizitat si calculam nivelul sau
            visited[node] = true;
            lvl[node] = lvl[parent] + 1;

            // Initial, low_link va fi nivelul nodului
            low_link[node] = lvl[node];

            // children va retine toti copii lui node
            ArrayList<Integer> children = new ArrayList<>();

            for (int v: adj[node]) {
                if (!visited[v]) {
                    // Nodul nu a fost vizitat, deci e un copil
                    children.add(v);

                    // vizitez nodul
                    dfs(v, node, sol);

                    // E posibil ca din node sa se ajunga mai sus mergand pe v
                    // si apoi luand o muchie de intoarcere
                    low_link[node] = Math.min(low_link[node], low_link[v]);
                }
                else if (v != parent) {
                    // am gasit o muchie de intoarcere
                    low_link[node] = Math.min(low_link[node], lvl[v]);
                }
            }

            // daca nodul este radacina, atunci este punct critic <=> are >= 2 copii
            if (parent == 0) {
                if (children.size() >= 2)
                    sol.add(node);
                return;
            }

            // altfel, trebui sa merg prin toti copii nodului
            // daca gasesc un copil v pentru care lowlink[v] >= lvl[node], atunci node este punct critic.
            for (int v : children) {
                if (low_link[v] >= lvl[node]) {
                    sol.add(node);
                    return;
                }
            }
        }
    }

    public static void main(String[] args) {
        new Task().solve();
    }
}
