/* ===================== GRAPH THEOREM SHEET =====================
 Every hypothesis here matters. A theorem quoted without its precondition is a wrong answer.

 MATCHING AND COVERING
   KONIG (BIPARTITE only):  max matching = min vertex cover.
     Constructive: Z = vertices reachable by alternating paths from the UNMATCHED left vertices;
     the cover is (X \ Z) union (Y and Z).
   GALLAI: for EVERY graph on n vertices,  alpha + tau = n   (independent set + vertex cover).
           for every graph WITH NO ISOLATED VERTEX,  nu + rho = n   (matching + edge cover).
           Bipartite + Konig gives  alpha = n - nu.
   HALL (bipartite): a matching saturating every vertex of the LEFT part X exists iff
     |N(S)| >= |S| for every S contained in X.   (Not every S of V - the side matters.)
   ORE'S DEFECT FORM:  nu(G) = |X| - max over S contained in X of ( |S| - |N(S)| ).
   HALL FOR REGULAR BIPARTITE: every k-regular bipartite graph (k >= 1) has a perfect matching,
     so its edges split into exactly k perfect matchings. Equivalently KONIG'S EDGE COLOURING
     THEOREM: chi'(G) = Delta(G) for every bipartite MULTIGRAPH. (Timetabling, "split into
     Delta rounds", Latin-square completion.)
   BERGE: a matching is maximum iff there is no augmenting path. (Why Kuhn / HK / Blossom stop.)
   TUTTE (general graphs): G has a PERFECT matching iff odd(G - U) <= |U| for every U contained
     in V, where odd(H) = the number of components of H with an ODD number of vertices.
     U = empty forces n even.
   TUTTE-BERGE:  nu(G) = (1/2) * min over U of ( |V| + |U| - odd(G - U) ).
   DILWORTH (FINITE poset): min number of chains covering P = size of the maximum antichain.
   MIRSKY (dual): min number of antichains partitioning P = length of the longest chain.
   SPERNER: the largest antichain in the subset lattice of [n] has size C(n, floor(n/2)).
   PATH COVERS - the distinction that most notebooks get wrong:
     min VERTEX-DISJOINT path cover of a DAG = n - (max matching of the split graph, no closure).
     min chain cover for Dilworth = n - (max matching of the TRANSITIVE CLOSURE's split graph).

 CONNECTIVITY AND FLOW
   MENGER: for NON-ADJACENT s, t: min s-t VERTEX cut = max internally-vertex-disjoint s-t paths.
     For any s != t: min s-t EDGE cut = max edge-disjoint s-t paths.
   WHITNEY: kappa(G) <= lambda(G) <= delta(G)  (vertex conn <= edge conn <= min degree).
     A graph on >= 3 vertices is 2-connected iff every two vertices lie on a common cycle.
   MAX-FLOW MIN-CUT, with INTEGRALITY: integer capacities => some maximum flow is integral (and
     Dinic produces one). Corollary used constantly: a 0/1-capacity max flow decomposes into that
     many edge-disjoint s-t paths.
   ROBBINS: an undirected graph has a STRONG orientation iff it is connected and BRIDGELESS.
     Constructive in O(V+E): DFS, orient tree edges away from the root and back edges toward it.
   ESWARAN-TARJAN (augmentation):
     directed - min arcs to make a digraph strongly connected = max(#sources, #sinks) of the
       condensation (0 if the condensation is a single node).
     undirected - for a CONNECTED graph, min edges to make it 2-edge-connected = ceil(L/2) where
       L is the number of leaves of the bridge tree (0 if the bridge tree is one node).

 DEGREE SEQUENCES
   ERDOS-GALLAI: d1 >= ... >= dn >= 0 is the degree sequence of a SIMPLE graph iff sum d_i is
     even and for every k in [1, n]:
       sum_{i<=k} d_i  <=  k(k-1) + sum_{i>k} min(d_i, k).
     Only k up to the largest index with d_k >= k needs checking (the Durfee square), so it is
     O(n) after sorting with prefix sums.
   HAVEL-HAKIMI: graphical iff d1 <= n-1 and, after deleting d1 and subtracting 1 from the next
     d1 entries (re-sorted), the rest is graphical. Gives a CONSTRUCTION, which Erdos-Gallai
     does not. O(n^2) naive.
   LANDAU (tournaments): s1 <= ... <= sn is a score sequence iff sum_{i<=k} s_i >= C(k,2) for
     every k, WITH EQUALITY at k = n.

 TOURNAMENTS
   REDEI: every finite tournament has a Hamiltonian PATH (insertion sort, O(n log n)).
   CAMION: every STRONGLY CONNECTED tournament has a Hamiltonian CYCLE.
   MOON: every strongly connected tournament is vertex-pancyclic - for every vertex v and every
     k in [3, n] there is a k-cycle through v.

 COUNTING
   MATRIX-TREE (Kirchhoff), undirected: #spanning trees (or the sum over trees of the product of
     weights) = ANY cofactor of L = D - A, loops excluded.
   MATRIX-TREE, DIRECTED (Tutte), with A_ij = #arcs i -> j:
     L_out = D_out - A, delete row AND column r  ->  arborescences CONVERGING TO r (in-trees).
     L_in  = D_in  - A, delete row AND column r  ->  arborescences DIVERGING FROM r (out-trees).
     These two are the pair everyone swaps. Verify on the 2-vertex graph 1 -> 2.
   BEST THEOREM: for a CONNECTED digraph with indeg(v) = outdeg(v) everywhere, the number of
     Eulerian circuits up to starting point is  ec(G) = t_w(G) * prod_v (outdeg(v) - 1)!,
     where t_w is the number of arborescences CONVERGING TO any fixed w (the L_out form above);
     t_w is the same for every w in such a graph. Multiply by outdeg(v) to fix a start vertex.
   CAYLEY: n^(n-2) labelled trees; with prescribed degrees, (n-2)! / prod (d_i - 1)!;
     labelled forests with k trees whose roots are k specified vertices: k * n^(n-k-1).

 EULERIAN CONDITIONS  (the check EulerianPath.cpp needs before it starts)
   UNDIRECTED: a circuit exists iff every degree is even and all EDGES lie in one component.
     A trail exists iff exactly 0 or 2 vertices have odd degree (start at an odd one), same
     connectivity condition.
   DIRECTED: a circuit exists iff indeg = outdeg at every vertex and all vertices of nonzero
     degree lie in one component of the underlying undirected graph. A trail exists iff exactly
     one vertex has out - in = 1 (the start), exactly one has in - out = 1 (the end), the rest
     are balanced, same connectivity condition.

 COLOURING
   BROOKS: for connected G, chi(G) <= Delta(G) UNLESS G is complete or an odd cycle, where
     chi = Delta + 1. Greedy on any order gives Delta+1; greedy on a degeneracy order gives d+1.
   VIZING (SIMPLE graphs): Delta <= chi' <= Delta + 1. Multigraphs: chi' <= Delta + mu, and
     Shannon's chi' <= floor(3 Delta / 2). Deciding class 1 vs class 2 is NP-complete - do not
     look for a polynomial algorithm mid-contest. Bipartite is the exception (Konig above).
   GALLAI-HASSE-ROY-VITAVER: chi(G) = 1 + min over orientations of (longest directed path length).
     Equivalently every orientation contains a directed path on chi(G) vertices.

 EXTREMAL
   TURAN: a K_{r+1}-free graph has e <= (1 - 1/r) n^2 / 2, with equality only for the balanced
     complete r-partite Turan graph. r = 2 is MANTEL: triangle-free => e <= floor(n^2/4).
   Every graph has a bipartite subgraph with >= m/2 edges (greedy max-cut).
   A graph is bipartite iff it has no odd cycle.
   RAMSEY, the known small values: R(3,3)=6, R(3,4)=9, R(3,5)=14, R(3,6)=18, R(3,7)=23,
     R(3,8)=28, R(3,9)=36, R(4,4)=18, R(4,5)=25. R(4,6) in [36,40] and R(5,5) in [43,46] are
     OPEN. Bound: R(s,t) <= C(s+t-2, s-1).
   NASH-WILLIAMS ARBORICITY: a(G) = max over subgraphs S with >= 2 vertices of
     ceil( m_S / (n_S - 1) ). Consequences: degeneracy d satisfies a <= d <= 2a - 1; m <= a(n-1);
     triangle enumeration is O(m * a).
   TUTTE / NASH-WILLIAMS TREE PACKING: G has t edge-disjoint spanning trees iff every partition
     of V into k non-empty parts has at least t(k-1) crossing edges.
   PETERSEN: every BRIDGELESS CUBIC graph has a perfect matching. Every 2k-regular graph splits
     into k edge-disjoint 2-factors, so it has an Eulerian circuit and an orientation with
     indeg = outdeg = k.

 PLANARITY
   EULER: connected plane graph V - E + F = 2; with c components, V - E + F = 1 + c.
   Simple planar with V >= 3: E <= 3V - 6. Triangle-free (girth >= 4): E <= 2V - 4.
   Every planar graph has a vertex of degree <= 5, i.e. it is 5-degenerate.
   KURATOWSKI / WAGNER: planar iff no K5 and no K_{3,3} subdivision (minor).
   FOUR COLOUR: every planar graph is 4-colourable - but there is no usable algorithm, so never
     plan around it. FIVE COLOUR is constructive in O(n) from the degree-<=5 vertex.
   K5, K_{3,3} and the Petersen graph are non-planar. In a planar graph min cut = shortest path
     in the dual.

 HAMILTONICITY (sufficient conditions only - the general problem is NP-complete)
   DIRAC: n >= 3 and min degree >= n/2  =>  Hamiltonian.
   ORE: deg(u) + deg(v) >= n for every non-adjacent pair  =>  Hamiltonian.
   Both are constructive by rotation-extension.

 TREE FACTS
   JORDAN: every tree has 1 or 2 CENTRES; if 2 they are adjacent; they are the middle of any
     diameter path.
   CENTROID: every tree has 1 or 2 centroids; c is a centroid iff every component of T - c has
     <= n/2 vertices iff c minimises the sum of distances. If 2 they are adjacent and n is even.
     That <= n/2 bound is exactly what makes centroid decomposition O(log n) deep.
   DIAMETER: the greedy two-BFS is valid on trees and on positively weighted trees, NOT on
     general graphs. Every longest path passes through a centre. Joining two trees by an edge
     gives diameter max(d1, d2, r1 + r2 + 1), minimised by joining centre to centre.
   A tree is bipartite, has >= 2 leaves, and sum of degrees = 2(n-1).

 WHAT IS NP-HARD, AND WHAT ONLY LOOKS IT
   NP-hard: max clique, chromatic number (>= 3 colours), Hamiltonian path/cycle, TSP, MIXED
     Chinese postman, vertex cover / independent set on general graphs, max cut (general),
     Steiner tree with unbounded terminals, longest path, dominating set, feedback vertex/arc
     set, edge-colouring class decision, bandwidth.
   Polynomial despite appearances: vertex cover and independent set on BIPARTITE graphs (Konig)
     and on trees / interval / chordal graphs; 2-colouring; 2-SAT; max cut on PLANAR graphs;
     min mean cycle; min-cost flow; directed MST; maximum matching in a GENERAL graph (Blossom);
     min path cover of a DAG; Steiner tree with k <= ~15 terminals; TSP with n <= ~20.
=============================================================== */
