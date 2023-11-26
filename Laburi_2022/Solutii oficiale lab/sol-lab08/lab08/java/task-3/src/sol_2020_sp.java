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

        class Edge {
            int x, y;

            public Edge(int x, int y) {
                this.x = x;
                this.y = y;
            }
        }

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
                    int x, y;
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

        private void writeOutput(ArrayList<Edge> result) {
            try {
                PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(
                                OUTPUT_FILE)));
                pw.printf("%d\n", result.size());
                for (Edge e : result) {
                    pw.printf("%d %d\n", e.x, e.y);
                }
                pw.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private ArrayList<Edge> getResult() {
            // TODO: Gasiti muchiile critice ale grafului neorientat stocat cu liste de adiacenta in adj.
            ArrayList<Edge> sol = new ArrayList<>();

            visited = new boolean[n + 1];
            lvl = new int[n + 1];
            low_link = new int[n + 1];

            for (int i = 1; i <= n; ++i) {
                if (!visited[i])
                    dfs(i, 0, sol);
            }

            return sol;
        }

        void dfs(int node, int parent, ArrayList<Edge> sol) {
            // Marcam nodul ca vizitat si calculam nivelul sau
            visited[node] = true;
            lvl[node] = lvl[parent] + 1;

            // Initial, low_link va fi nivelul nodului
            low_link[node] = lvl[node];

            for (int v: adj[node]) {
                if (!visited[v]) {
                    // Nodul nu a fost vizitat, asa ca il vizitez
                    dfs(v, node, sol);

                    // E posibil ca din node sa se ajunga mai sus mergand pe v si apoi luand o muchie de intoarcere
                    low_link[node] = Math.min(low_link[node], low_link[v]);

                    // Daca din v nu se poate ajunge pana la node (sau mai sus) atunci cand stergem muchia node-v,
                    // atunci, muchia node-v este o punte.
                    if (low_link[v] > lvl[node])
                        sol.add(new Edge(node, v));
                }
                else if (v != parent) {
                    // am gasit o muchie de intoarcere
                    low_link[node] = Math.min(low_link[node], lvl[v]);
                }
            }
        }
    }

    public static void main(String[] args) {
        new Task().solve();
    }
}
