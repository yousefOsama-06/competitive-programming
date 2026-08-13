# Contest Intel — what ECPC / ACPC / Codeforces actually ask for

Evidence gathered 2026-08-12. This is the *why* behind the priorities in [PLAYBOOK.md](PLAYBOOK.md).
Everything here is measured or read directly, not remembered.

---

## 1. Codeforces tag trends — measured, not guessed

Pulled the full Codeforces problemset (11,356 problems) and contest list (2,139 contests) from the
public API, joined on contest start time, and computed tag frequency by era. Restricted to problems
rated **1900+**, because that's where a notebook actually changes the outcome.

Sample sizes: 1,714 problems (2018–21), 755 (2022–23), 999 (2024–26).

| tag | 2018–21 | 2022–23 | 2024–26 | shift |
|---|---:|---:|---:|---:|
| dp | 32.6% | 38.0% | **38.5%** | +6.0 |
| math | 26.0% | 37.9% | **35.6%** | **+9.7** |
| greedy | 25.5% | 30.1% | **34.2%** | **+8.7** |
| data structures | 28.1% | 33.6% | **31.6%** | +3.6 |
| brute force | 14.6% | 21.9% | 22.2% | +7.6 |
| constructive | 19.4% | 24.6% | 21.7% | +2.4 |
| implementation | 13.5% | 17.1% | 20.7% | +7.2 |
| binary search | 13.8% | 17.9% | 18.3% | +4.5 |
| trees | 15.1% | 19.6% | 17.7% | +2.7 |
| **combinatorics** | 10.0% | 16.6% | **17.6%** | **+7.6** |
| graphs | 18.0% | 17.5% | 17.2% | −0.8 |
| dfs and similar | 13.6% | 18.4% | 15.8% | +2.2 |
| bitmasks | 9.2% | 11.4% | 12.6% | +3.4 |
| number theory | 8.2% | 10.5% | 10.6% | +2.4 |
| divide and conquer | 5.7% | 7.8% | 8.6% | +2.9 |
| interactive | 5.8% | 5.8% | **7.6%** | +1.8 |
| dsu | 4.9% | 7.9% | 6.7% | +1.8 |
| strings | 6.3% | 4.6% | 5.2% | −1.1 |
| games | 2.8% | 2.5% | **4.5%** | +1.7 |
| shortest paths | 4.5% | 5.0% | 3.5% | −1.0 |
| **geometry** | 4.7% | 4.1% | **2.5%** | **−2.2** |
| flows | 2.8% | 2.8% | 2.3% | −0.5 |
| fft | 2.9% | 2.8% | 2.2% | −0.7 |
| suffix structures | 1.8% | 1.2% | 1.4% | −0.4 |
| graph matchings | 2.0% | 2.6% | 1.3% | −0.7 |

Same computation at **2400+** (605 problems in 2024–26) tells the same story harder:
math +10.8, greedy +9.9, combinatorics +7.7, implementation +7.8, dp +6.5 — while geometry −2.5,
graphs −2.1, flows −0.7, strings −0.9.

At **1900–2299** (318 problems), the biggest riser is **data structures +10.1**, then math +9.1,
greedy +7.5, binary search +7.2, combinatorics +7.0, number theory +5.0.

### What this means for the notebook

1. **Combinatorics is the fastest-growing tag and this notebook has two files in that section.**
   That is the single worst mismatch between the notebook and the meta.
2. **Math + greedy + dp + data structures now co-occur on the same problem.** The modern hard
   problem is "notice a math identity, then implement it with a data structure", not "run a named
   algorithm". Reference pages of *identities* (binomial, Möbius, floor-sum) are worth as much as code.
3. **Heavy classical machinery is fading.** Flows, matchings, suffix structures, FFT and geometry
   are all flat or declining on Codeforces. Do not spend the first week of prep on Blossom.
   (ICPC regionals still ask for flows and geometry more than Codeforces does — see §2 — so
   they stay on the list, just not at the top.)
4. **Interactive and games are quietly rising.** Both are cheap to prepare: a query-budget binary
   search template and a Grundy/mex page. This notebook has neither.

---

## 2. ECPC 2024 — the real thing, problem by problem

Read the full released problem set: *The 2024 ICPC Egyptian Collegiate Programming Contest*,
AAST Alexandria, July 2024. 15 problems, A–O. This is the closest available proxy for what
ECPC 2025/2026 will look like.

| # | Problem | What it actually needs | Notebook status |
|---|---|---|---|
| A | Antiquities of Acre | **Interactive**, ≤20 queries on n≤10⁵ → binary search on a query oracle | ❌ no interactive template |
| B | Biblical Journeys in Bethlehem | Queue with patience/abandonment, n≤10⁶, ΣN≤10⁶ → greedy + fast I/O | ⚠️ fastIO exists but `write_int` always emits `\n` |
| C | Canals of Beersheba | Algebra: `12n²+3a²+3b² = 12an+12bm−12m²+12c` → complete the square to `3(a−2n)²+3(b−2m)²=12c`, and `c ≤ 0` forces `c=0` | ✅ pure observation |
| D | Diverse Culture of Al-Khalil | Online interval insert + "min lamps to cover [L,R]" → **binary lifting over farthest-reach**, with a segment tree for max-reach | ❌ no standalone binary lifting, ❌ no interval-cover pattern |
| E | Exploration of Jericho's Oases | Angle bisector, rotation direction, lattice input up to 10⁹, 10⁵ test cases | ⚠️ has Point2D/angles, ❌ no exact orientation, no signed-rotation helper |
| F | Fishing Traditions of Tiberias | `ΣΣ lcm(aᵢ,aⱼ)·lcm(i,j)` mod p, n≤10⁵ → **gcd/divisor-sum convolution, Möbius or Euler-φ identity** | ❌ has Möbius *sieve* but no divisor-sum / Dirichlet machinery |
| G | Gateways of Beisan | Expected time to a repeat + counting distinct strings after one insertion → **modular inverse + expected value + string runs (Z-function)** | ⚠️ pieces exist, no expected-value pattern |
| H | Harbor Life in Jaffa | Range **AND**-assign, point assign, query **max XOR subset of a subarray** → segment tree whose nodes hold a **XOR basis**, plus range-AND amortization | ❌ `Basis` has no merge, can't live in a segtree; ❌ no range-AND (beats/ODT) |
| I | Interwoven Cultures of Nablus | Longest palindromic substring where `OR == SUM` (bit-disjoint) → **Manacher + two pointers / sparse-table OR** | ✅ Manacher; ⚠️ combining pattern not written down |
| J | Journey in Safed's Mysteries | Max subarray sum after ≤1 swap, **for every prefix**, n≤10⁶ | ❌ no Kadane-family page at all |
| K | Kaleidoscope of Ramla's Diversity | Sum of an array (giveaway) | ✅ |
| L | Logistics of Lod's Connectivity | Restore two permutations maximizing `Σ|aᵢ−bᵢ|` → constructive + greedy insertion | ✅ thinking problem |
| M | Majestic Crossroads Al-Nasserah | Count subarrays where every adjacent diff `≥ −x`, over many `x` → **offline query sorting + run-length counting / DSU** | ❌ no offline-query pattern page |
| N | Navigation Challenges in Gaza | `distanceⱼ = min j−i` s.t. prefix-sum condition holds on the whole range → reduces to "leftmost i with `P[i−1] ≤ min_{k} (P[k]−a_{k+1})`" → **segment tree descent** | ❌ **no segment tree descent** — the exact missing operation |
| O | Urban Exploration in Al-Quds | Tree; power = min XOR of a node's latency against any ancestor; query = min power over a subtree → **binary trie with erase, maintained along the DFS stack**, + Euler tour + subtree min | ❌ `BinaryTrie` has **no erase**; ❌ no DFS-add/remove pattern |

**Score: of 15 problems, 7 need something the notebook does not contain, and 3 more need a
pattern that exists only as fragments.** The gaps that actually cost points here are:
segment tree descent (N), XOR basis inside a segment tree (H), binary trie with erase (O),
binary lifting on reach (D), divisor-sum number theory (F), interactive scaffolding (A),
Kadane variants (J), offline query sorting (M).

Note what is *not* in this set: no max flow, no matching, no suffix automaton, no FFT, no
convex hull. One geometry problem, and it's angle arithmetic rather than hull machinery.

---

## 3. ECPC qualification rounds — a different animal

Profiled 68 accepted solutions from ECPC Qualifications 2024 (days 1–4, ~13 problems each) and the
2025 qualification solutions.

Technique frequency across those 68 files: BFS/DFS/priority-queue 14%, modpow/modular inverse 11%,
bitmask 10%, DP 8%, sorting/greedy 8%, combinatorics 7%, strings 7%, hash maps 7%, sieve 5%,
BIT 5%, sets 5%, binary search 4%, **segment tree 2%, DSU 1%, geometry 1%**.

Read directly, the 2025 qualification solutions are: subtraction of two numbers; integer division;
sort + greedy pairing of frequencies against values; factorials with `invFact` built by the
descending-inverse trick; and solving `x·y = B, x+y = S` by **integer square root with a correction
loop** (`sqrtl` then `while (sd*sd < D) sd++;`).

**Implication:** qualification rounds are won on speed and correctness of *elementary* code, not on
templates. What pays there is: a clean I/O template, `nCr` with O(n) inverse factorials, integer
`isqrt`/`icbrt` with correction, overflow discipline, and a debug macro. The heavy templates only
start paying at the ECPC finals and ACPC.

---

## 4. ACPC's own training taxonomy

The ACPC Scientific Committee maintains a public topic-focused training archive. The axes they
train on are literally the directory names: **ad-hoc**, **dp**, **data-structure**,
**number theory**, **graph** — each split easy / medium / hard.

So the committee's own model of the region is: ad-hoc + DP + data structures + number theory +
graphs. Strings, geometry, flows and games are not first-class training axes. That matches the
ECPC 2024 set almost exactly.

---

## 5. Priority model used in the playbook

Combining the four sources above:

- **T0 — build first.** Appears in the ECPC 2024 set, *or* is a top-6 rising Codeforces tag, *or*
  is a primitive that unlocks a whole family (Miller–Rabin, Gaussian elimination). Missing T0
  items are the ones that lose you problems.
- **T1 — build second.** Regular ICPC-regional material, or a Codeforces tag in the 5–15% band.
  You will use several of these across a season.
- **T2 — build last / read-only.** Rare, declining, or so specific that a page of notes beats code.

One deliberate override: **flows, matching and geometry are declining on Codeforces but still
appear at ICPC regionals**, so a minimum viable set of each stays at T1 rather than dropping to T2.

---

## Sources

- Codeforces API: `problemset.problems`, `contest.list` (accessed 2026-08-12)
- *The 2024 ICPC Egyptian Collegiate Programming Contest* problem set (AAST Alexandria, July 2024)
- ECPC Qualifications 2024 & 2025 accepted-solution archives
- ACPC Topic-Focused Training Archive (ACPC Scientific Committee)
- `ShahjalalShohag/code-library` (387 files) as the reference implementation library
