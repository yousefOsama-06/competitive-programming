void solve() {
    int n, m;
    cin >> n >> m;
    int src = n, sink = n + 1;
    Dinic flw(n + 2);
    int sum_lower = 0;
    vector<int> ans(m + 1);
    for (int i = 1; i <= m; ++i) {
        int u, v, lower, upper;
        cin >> u >> v >> lower >> upper;
        u--,v--;

        flw.addEdge(u, v, upper - lower, i);
        flw.addEdge(src, v, lower, 0);
        flw.addEdge(u, sink, lower, 0);

        sum_lower += lower;

        ans[i] = lower;
    }

    int flow = flw.calc(src,sink);
    
    if (flow != sum_lower) {
        cout << "NO\n";
        return;
    }

    cout << "YES\n";
    for (int i = 0; i < flw.adj.size(); ++i) {
        for(auto &edge:flw.adj[i]){
            ans[edge.id] += edge.flow();
            flow += edge.flow();
        }
    }

    for (int i = 1; i <= m; ++i) {
        cout << ans[i] << '\n';
    }
}