# Geometry — complete reference

Everything the notebook should carry for geometry, what each piece *does*, and the theorems and
techniques that go with it. Proposed code lives in [`docs/proposed/geometry/`](proposed/geometry/)
— six files, **~430 lines total**, replacing the current 532 lines while covering roughly three
times as much.

> **Verification.** Every routine below was compiled and stress-tested against an independent
> brute force (exact `__int128` oracle for segment intersection, O(n²) or exhaustive-grid oracles
> for the rest), tens of thousands of random cases each with small coordinates so degenerate
> configurations actually occur. That process caught three real bugs in my own first draft:
> an empty-polygon dereference in `area2`, transposed arguments in `lineInter` (which negated
> every returned intersection point), and vertices lying exactly on the cutting line being dropped
> from **both** halves in `cut`. All three are fixed. Do not put geometry into the notebook that
> has not been through this.

---

## 0. Rules that decide whether you get AC

**Use `P<ll>` (exact integers) for every predicate.** Orientation, point-in-polygon, convex hull,
segment intersection — all of these are *decisions*, and a decision made with `double` is a decision
made wrong. Switch to `P<ld>` only when the answer itself is a real number.

**Overflow table** (this is the thing that silently kills you):

| coordinates | `cross` / `dot` | `dist2` | safe in |
|---|---|---|---|
| \|c\| ≤ 10⁴ | ≤ 2·10⁸ | ≤ 2·10⁸ | `int` |
| \|c\| ≤ 10⁹ | ≤ 4·10¹⁸ | ≤ 2·10¹⁸ | `long long` (barely — `LLONG_MAX ≈ 9.2·10¹⁸`) |
| \|c\| ≤ 10⁹, cross of *differences* | ≤ 8·10¹⁸ | — | `long long`, no margin |
| product of two crosses | ~10³⁷ | — | **`__int128`** |

Anything that multiplies two cross products (e.g. comparing two slopes by cross-multiplication, or
`segInter`'s `ob - oa` scaling) needs `__int128` once coordinates exceed ~10⁶.

**Epsilon rules.** One `sgn(ld)` with `eps = 1e-9`, used everywhere; never compare floats with `==`
or `<`. If coordinates can be 10⁹, absolute eps is wrong — compare relative to magnitude. When the
problem says "answer within 1e-6", compute in `long double` and print with `setprecision(10)`.

**Degenerate cases are the test data.** Zero-length segments, duplicate points, all points collinear,
`n = 1`, `n = 2`. Every routine below is written to survive them; if you retype one, keep the guards.

---

## 1. Primitives — `01 - Core.cpp`

| Call | What it does | Use it for |
|---|---|---|
| `a + b`, `a - b`, `a * k`, `a / k` | vector arithmetic | everything |
| `a.dot(b)` | `\|a\|\|b\|cos θ` | perpendicularity (`== 0`), projection length, "is the angle acute" |
| `a.cross(b)` | `\|a\|\|b\|sin θ`, signed | is b CCW of a; twice the triangle area; parallelism (`== 0`) |
| `a.cross(b, c)` | orient(a,b,c) | **the single most used predicate**: is c left of a→b |
| `a.dist2()` | squared length | exact comparisons — never `sqrt` to compare |
| `a.dist()` | length | only when you need the real value |
| `a.angle()` | `atan2(y, x)` ∈ (−π, π] | printing angles; **not** for sorting |
| `a.perp()` | rotate +90° | normals, half-plane directions, calipers |
| `a.unit()` | normalize | directions |
| `a.rot(θ)` | rotate CCW by θ | rotating a shape |
| `angleTo(a,b)` | **signed** angle a→b ∈ (−π, π] | "by how much and in which direction do I rotate" |
| `angleAt(a,O,b)` | angle ∠aOb ∈ [0, 2π) | interior angles |
| `angCmp` | **exact** polar sort comparator | radial sweeps, half-plane ordering, "sort points by angle" |

**Why `angCmp` instead of `atan2`:** `atan2` costs precision and time and breaks ties badly. The
half-plane + cross trick is exact on integers and faster. Sorting 2·10⁵ points by angle with `atan2`
has cost real teams real problems.

---

## 2. Lines, segments, rays — `02 - Lines.cpp`

| Call | What it does |
|---|---|
| `lineDist(p,a,b)` | distance from p to the infinite line ab |
| `segDist(p,a,b)` | distance from p to the segment ab (exact-input safe) |
| `proj(p,a,b)` | foot of the perpendicular from p onto line ab |
| `refl(p,a,b)` | mirror image of p across line ab |
| `closestOnSeg(p,a,b)` | nearest point of segment ab to p |
| `onSeg(p,a,b)` | is p on segment ab, endpoints included — **exact** |
| `lineInter(a,b,c,d)` | `{1, pt}` unique · `{0, _}` parallel · `{−1, _}` same line |
| `segInter(a,b,c,d)` | 0, 1 or 2 points — 2 means a collinear overlap |
| `segCross(a,b,c,d)` | just the boolean; use this when you don't need the point |
| `segSegDist(a,b,c,d)` | distance between two segments |
| `onRay`, `rayDist` | same for rays |
| `perpBisector(a,b)` | the locus of points equidistant from a and b |
| `bisectorDir(a,b,c)` | direction of the interior angle bisector at b |
| `fromABC(A,B,C)` | convert `Ax + By = C` into two points |

**What they unlock:** point-line distance is the inner loop of "closest approach" and of
`width()`. Reflection solves mirror/billiard problems and "shortest path touching a line".
Perpendicular bisector solves "equidistant from two towns" and is the construction behind Voronoi.
The angle bisector is the shape of ECPC 2024 E.

---

## 3. Polygons — `03 - Polygon.cpp`

| Call | What it does |
|---|---|
| `area2(v)` | **signed** twice-area. Sign tells orientation: `> 0` ⇒ CCW. Exact on integers |
| `area(v)` | absolute area |
| `perimeter(v)` | perimeter |
| `centroid(v)` | area centroid (not the vertex average!) |
| `isConvex(v)` | all turns the same way |
| `inPoly(v,p,strict)` | point in **any simple** polygon, O(n), exact, boundary configurable |
| `hull(p)` | convex hull, CCW, strictly convex, O(n log n) |
| `inConvex(h,p)` | point in convex polygon in **O(log n)**: 1 in / 0 boundary / −1 out |
| `extreme(h,dir)` | index of the vertex furthest along `dir`, **O(log n)** |
| `cut(v,s,e)` | clip the polygon to the left of line s→e |
| `minkowski(A,B)` | Minkowski sum of two convex polygons, O(n+m) |
| `diameter2(h)` | max squared distance between hull points (rotating calipers, O(n)) |
| `width(h)` | min distance between parallel supporting lines |
| `minRectArea(h)` | minimum-area enclosing rectangle |
| `boundaryPts(v)` | lattice points **on** the boundary = Σ gcd(\|dx\|,\|dy\|) |
| `interiorPts(v)` | lattice points **strictly inside**, via Pick |

**What they unlock:**

- `area2`'s **sign** is how you normalize orientation before every convex routine. Half the geometry
  WAs are "my polygon was clockwise".
- `inPoly` with `strict=false` is the version you want when the statement says "on the border counts".
- `extreme` is the O(log n) engine behind: max/min dot product over a convex set, tangent lines from
  a far point, and **querying a convex hull many times** (e.g. "for each query direction, the
  furthest point") — turning O(nq) into O(q log n).
- `cut` repeated over many half-planes *is* half-plane intersection; it also answers "area of the
  polygon on one side of a line" and "clip a view frustum".
- `minkowski` turns "minimum distance between two convex polygons" into "distance from the origin to
  the Minkowski sum of A and −B", and "do these two convex bodies overlap after translation" into a
  single point-in-polygon test.
- `diameter2` = farthest pair. `width` = the thinnest slab containing the set — "can this shape fit
  through a corridor".

---

## 4. Circles — `04 - Circles.cpp`

| Call | What it does |
|---|---|
| `circleLine(o,r,a,b)` | 0/1/2 intersections of a circle with a line |
| `circleSeg(o,r,a,b)` | same, restricted to the segment |
| `circleCircle(a,r1,b,r2)` | 0/1/2 intersection **points** of two circles |
| `circleInterArea(a,r1,b,r2)` | **area** of the lens where two circles overlap |
| `tangents(c1,r1,c2,r2,inner)` | the 2 external (or 2 internal) common tangents, as touch-point pairs |
| `tangents(p,0,c,r,false)` | ⇒ the two tangent lines from a point to a circle |
| `circumcenter/circumradius` | circle through 3 points |
| `incenter/inradius` | inscribed circle of a triangle |
| `orthocenter` | intersection of the altitudes |
| `mec(pts)` | **minimum enclosing circle**, expected O(n) |
| `circlePolyArea(c,r,poly)` | area of (circle ∩ polygon) |
| `power(p,c,r)` | `\|p−c\|² − r²`; sign = inside/on/outside; value = (tangent length)² |
| `radicalAxis(a,r1,b,r2)` | locus of equal power — where tangent lengths match |

**What they unlock:** `circleInterArea` answers "overlap of two coverage discs". `circlePolyArea`
answers "how much of the field does the sprinkler water". `mec` answers "smallest radio tower
covering all houses". `power`/`radicalAxis` answer "equidistant in tangent length", and the radical
center of three circles is the classic construction problem. Tangents give "shortest path around a
circular obstacle" (straight-tangent-arc-tangent-straight).

---

## 5. Triangles — quick formulas (keep as a note, not code)

- Area = ½|cross(b−a, c−a)| = Heron `√(s(s−a)(s−b)(s−c))`, `s = (a+b+c)/2`.
- `R = abc / (4A)` (circumradius), `r = A / s` (inradius), `Euler: OI² = R(R − 2r)`.
- Centroid = `(a+b+c)/3`; it divides each median 2:1.
- Law of cosines `c² = a² + b² − 2ab cos C`; law of sines `a/sin A = 2R`.
- Point in triangle: three `orient` calls with the same sign; or barycentric coordinates
  `(α, β, γ)` with `α+β+γ = 1`, all ≥ 0.
- Median length `mₐ = ½√(2b² + 2c² − a²)`. Angle bisector splits the opposite side in ratio `b : c`.

---

## 6. Sets of objects / sweep — `05 - Sweep.cpp`

| Call | What it does |
|---|---|
| `closestPair(pts)` | closest pair of points, O(n log n) |
| `rectUnionArea(rects)` | area of the union of axis-aligned rectangles, O(n log n) |
| `maxOnLine(pts)` | maximum number of collinear points |
| `countSegInter(segs)` | number of intersecting segment pairs (O(n²) — fine for n ≤ 2000) |
| `onion(pts)` | convex layers, peeling hull after hull |

**The sweep skeleton is the reusable part.** `rectUnionArea` = sort events by x + a segment tree that
maintains "total covered length in y". Change the tree's payload and the same sweep gives:
perimeter of the union, "area covered ≥ k times", "number of distinct covered cells". The same
event-sweep also answers non-geometric interval problems.

---

## 7. 3D — `06 - Geometry3D.cpp`

| Call | What it does |
|---|---|
| `P3` ops, `cross`, `dot` | vectors; `cross` gives the normal, its length = 2·triangle area |
| `rot(θ, axis)` | Rodrigues rotation around an arbitrary axis |
| `Plane(a,b,c)` / `Plane(n,d)` | plane through 3 points, or normal + offset |
| `pl.side(p)`, `pl.dist(p)`, `pl.proj(p)`, `pl.refl(p)` | which side / distance / foot / mirror |
| `planeLine(pl,a,b)` | line ∩ plane: unique / parallel / contained |
| `planePlane(A,B)` | plane ∩ plane → a line `{point, direction}` |
| `vol6(a,b,c,d)` | 6× signed tetrahedron volume — **exact** orientation test in 3D |
| `tetraVolume` | actual volume |
| `sphereLine(o,r,a,b)` | line ∩ sphere |
| `greatCircle(...)` | distance along a sphere's surface (lat/lon in degrees) |

Not included: 3D convex hull. It is ~80 lines, appears roughly once a decade at this level, and is
the right thing to skip.

---

## 8. Theorems worth a printed page

| Theorem | Statement | What it's for |
|---|---|---|
| **Shoelace** | `2A = Σ (x_i·y_{i+1} − x_{i+1}·y_i)` | area of any simple polygon, exact on integers; sign = orientation |
| **Pick's** | `A = I + B/2 − 1` | lattice points inside a lattice polygon; converts a counting problem to an area problem |
| **Euler's formula** | `V − E + F = 2` for a connected planar graph | counting regions formed by lines/segments/circles |
| **Triangle inequality** | `a + b ≥ c` | feasibility of "can these lengths form a shape" |
| **Ptolemy** | cyclic quadrilateral: `AC·BD = AB·CD + BC·AD` | concyclic tests, distance identities |
| **Power of a point** | `PA·PB` is constant over all lines through P | tangent lengths, radical axis |
| **Inscribed angle** | angle subtended at the circle is half the central angle | "see the segment under a fixed angle" ⇒ locus is an arc |
| **Helly (2D)** | if every 3 of n convex sets intersect, all n do | feasibility of intersecting constraints |
| **Carathéodory** | a point in the hull of S is in the hull of ≤ 3 points of S | why MEC only needs 3 support points |
| **Rotating calipers** | antipodal pairs on a convex polygon are visited monotonically | diameter, width, min rectangle, min distance between hulls — all O(n) |
| **Minkowski sum** | `A ⊕ B` of convex sets is convex, with `n+m` edges sorted by angle | collision under translation, min distance between convex bodies |
| **Duality** | point `(a,b)` ↔ line `y = ax − b` | "max points on a line" ↔ "max concurrent lines"; convex hull ↔ upper envelope |
| **Chebyshev ↔ Manhattan** | rotate 45°: `(x,y) → (x+y, x−y)` | L∞ problems become L1 and vice-versa; makes "max Manhattan distance" a max over 4 sums |
| **Half-plane intersection = 2D LP** | feasible region of linear constraints is a convex polygon | "does an assignment exist", optimizing a linear objective |

---

## 9. Techniques — the problem shapes they crack

**Rotating calipers.** Two supporting lines walked around a convex polygon in lockstep. Gives, all in
O(n): farthest pair, closest pair of *hulls*, width, minimum enclosing rectangle, and maximum
inscribed triangle.

**Radial (angular) sweep.** Sort everything around a pivot with `angCmp`, then sweep. Cracks: "how
many points are visible", "maximum points inside a half-plane through the origin", "count triples
forming an angle ≤ θ", "maximum points inside a rotating unit circle".

**Line sweep + BIT/segment tree.** Sort events by x; maintain something over y. Cracks: rectangle
union, "count pairs of intersecting segments", "for each point, how many rectangles contain it",
skyline.

**Binary search / ternary search on a convex quantity.** A convex polygon's distance to a point is
unimodal along the boundary ⇒ ternary search. "Minimum enclosing circle of radius r exists?" is
monotone in r ⇒ binary search + feasibility.

**Convexity to prune.** If the answer is a maximum of linear functions, only hull vertices matter —
so `hull` + `extreme` reduces an O(nq) scan to O((n + q) log n).

**Reduce to integers.** Multiply through by denominators; compare `a/b < c/d` as `a·d < c·b` with
sign care (and `__int128` when needed). Every comparison you move from `double` to `ll` is a bug you
will not have.

**Reflect to straighten.** Shortest path touching a line = reflect one endpoint. Billiards = repeated
reflection. Also solves "shortest path visiting a wall".

**Rotate the coordinate system.** 45° rotation for Chebyshev ↔ Manhattan. Rotating so an edge is
horizontal simplifies min-rectangle and "fit inside a corridor".

**Randomize.** MEC is O(n) *because* of the shuffle. Random rotation by an irrational angle removes
degenerate ties (equal x-coordinates, vertical lines) at the cost of exactness — a good trick for
sweeps, a bad one for exact predicates.

**Coordinate compression.** Before any sweep with a segment tree over coordinates.

---

## 10. Deliberately excluded

| Not included | Why |
|---|---|
| Delaunay triangulation / Voronoi | ~100 lines, and closest-pair + MST cover the usual motivations |
| 3D convex hull | ~80 lines, extremely rare at this level |
| Bentley–Ottmann | O(n²) segment intersection is fine to n ≈ 2000; the log version is long and fiddly |
| Point location (persistent) | rare; `inConvex` covers the convex case in O(log n) |
| Dynamic convex hull | rare; the CHT files already cover the DP use case |
| Half-plane intersection | **kept** (existing file), but consider replacing it with repeated `cut` when there are few half-planes — much shorter to retype |

---

## 11. Line-count comparison (measured, not estimated)

"code" = lines with comments and blanks stripped, i.e. what you actually hand-type.

| | current raw | current code | proposed raw | proposed code |
|---|---:|---:|---:|---:|
| Point / core / angles | 101 | 78 | 49 | 38 |
| Lines & segments | 116 | 92 | 73 | 55 |
| Polygon (area, hull, calipers) | 55 | 44 | 168 | 137 |
| Circles (incl. MEC) | 87 | 71 | 115 | 94 |
| Sweep / sets of objects | 0 | 0 | 93 | 79 |
| 3D | 70 | 47 | 74 | 65 |
| Half-plane intersection | 103 | 71 | 103 | 71 (unchanged) |
| **total** | **532** | **403** | **675** | **539** |

**It grows by ~136 code lines (+34%), not shrinks.** The core and lines files get *shorter* — the
duplicated `ccCenter`, the four near-identical `line/seg/ray × Inter/Dist` families, and the unused
`Point` members all go — but polygon, circles and sweep grow because they gain point-in-polygon,
O(log n) convex queries, cut, Minkowski, min-rectangle, Pick, all four tangent families,
circle∩polygon, closest pair and rectangle union. That is roughly 3× the functionality.

If page budget forces a cut, drop in this order: `06 - Geometry3D` (65) → `05 - Sweep` (79) →
`minRectArea` + `onion` → half-plane intersection (71, replaceable by repeated `cut`).
Never drop: core, orientation, `inPoly`, `hull`.
