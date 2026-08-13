// Needs: struct Edge {int u,v; ll w;} and DSU (both from 01 - MST.cpp).
// DIRECTED MST (Chu-Liu / Edmonds) - minimum spanning ARBORESCENCE rooted at r, O(V*E).
// Every non-root vertex takes its cheapest incoming edge; if that creates a cycle, contract it,
// subtract the chosen weights, and repeat. Returns -1 if some vertex is unreachable from r.
ll directedMST(int n, int r, vector<Edge> e) {         // Edge = {u, v, w} meaning u -> v
    ll total = 0;
    vector<int> id(n), par(n);
    while (true) {
        vector<ll> best(n, llinf);
        vector<int> from(n, -1);
        for (auto& x : e) if (x.u != x.v && x.w < best[x.v]) best[x.v] = x.w, from[x.v] = x.u;
        best[r] = 0;
        for (int i = 0; i < n; i++) if (i != r && from[i] < 0) return -1;   // unreachable
        int cnt = 0;
        fill(all(id), -1), fill(all(par), -1);
        for (int i = 0; i < n; i++) {
            if (i != r) total += best[i];
            int v = i;
            while (v != r && par[v] == -1 && id[v] == -1) par[v] = i, v = from[v];
            if (v != r && id[v] == -1 && par[v] == i) {  // found a new cycle
                for (int u = from[v]; u != v; u = from[u]) id[u] = cnt;
                id[v] = cnt++;
            }
        }
        if (!cnt) break;                                // no cycle -> done
        for (int i = 0; i < n; i++) if (id[i] == -1) id[i] = cnt++;
        for (auto& x : e) {
            int u = x.u, v = x.v;
            x.u = id[u], x.v = id[v];
            if (x.u != x.v) x.w -= best[v];             // reduced cost inside the contraction
        }
        n = cnt, r = id[r];
    }
    return total;
}

// SECOND-BEST MST, O(E log V + E * log V). The second-best tree differs from the MST by exactly
// ONE edge swap. For each non-tree edge (u,v,w) the candidate is mst - maxEdgeOnPath(u,v) + w.
// You must track BOTH the maximum and the strict second maximum on the path, because if the
// maximum equals w the swap is not a real change - use the second maximum instead.
// With binary lifting: up[k][v], mx1[k][v], mx2[k][v] merged pairwise while lifting.
// (The Kruskal Reconstruction Tree in 01 - MST.cpp gives maxEdgeOnPath directly as val[lca].)

// BORUVKA - MST in O(E log V) by phases; each phase every component picks its cheapest outgoing
// edge and they are all merged at once. Use it when the edge set is IMPLICIT:
//   XOR-MST      : cheapest cross edge per component via a binary trie
//   Manhattan MST: cheapest cross edge per octant (see Transforms.cpp)
ll boruvka(int n, const vector<Edge>& e) {
    DSU d(n);
    ll total = 0;
    int comps = n;
    while (comps > 1) {
        vector<int> best(n, -1);
        for (int i = 0; i < (int)e.size(); i++) {
            int a = d.find(e[i].u), b = d.find(e[i].v);
            if (a == b) continue;
            if (best[a] < 0 || e[i].w < e[best[a]].w) best[a] = i;
            if (best[b] < 0 || e[i].w < e[best[b]].w) best[b] = i;
        }
        bool any = false;
        for (int i = 0; i < n; i++) if (best[i] >= 0) {
            auto& x = e[best[i]];
            if (d.join(x.u, x.v)) total += x.w, comps--, any = true;
        }
        if (!any) break;                                // disconnected
    }
    return comps == 1 ? total : -1;
}
