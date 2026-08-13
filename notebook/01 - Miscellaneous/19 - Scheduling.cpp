/* ============== SCHEDULING / EXCHANGE-ARGUMENT THEOREM CARD ==============
 Each of these turns a hard-looking problem into a sort. Prove any new one by the EXCHANGE
 ARGUMENT: assume an optimal schedule, swap two ADJACENT jobs, and compare the two costs.

 SMITH'S RULE  (1 || sum w_j C_j : minimise the weighted sum of completion times)
   Sort by p_j / w_j ASCENDING. Adjacent swap of h,k improves iff w_k*p_h - w_h*p_k > 0,
   so ALWAYS compare by cross-multiplication (w_k*p_h vs w_h*p_k), never by float division.

 MOORE-HODGSON  (1 || sum U_j : minimise the NUMBER of late jobs), O(n log n)
   Sort by deadline ascending; keep a max-heap of the processing times taken so far.
   For each job: take it, push p_j, add p_j to the running time; if the running time exceeds
   this job's deadline, pop the largest p and subtract it. The heap size is the answer
   (the rejected jobs can go last in any order).

 EARLIEST DEADLINE FIRST  (preemptive, one machine)
   If ANY schedule meets all deadlines, EDF does. Feasibility check for non-preemptive
   unit-machine: sort by deadline and verify prefix sums of p_j never exceed d_j.

 JOHNSON'S RULE  (F2 || Cmax : two-machine flow shop, minimise makespan)
   Set A = jobs with p1 <  p2, sorted by p1 ASCENDING
   Set B = jobs with p1 >= p2, sorted by p2 DESCENDING
   Optimal order = A then B.

 LAWLER  (1 | prec | f_max : minimise the maximum penalty, with precedence), O(n^2)
   Build the schedule BACK TO FRONT. With t = total remaining time, among the jobs that have
   no unscheduled successor pick the one minimising f_j(t), and place it last.

 CLASSIC GREEDY COMPANIONS
   Interval scheduling (max #non-overlapping): sort by RIGHT endpoint, take greedily.
   Interval point cover (stab all intervals): sort by right endpoint, place a point at it.
   Minimum #machines for overlapping intervals: max overlap = sweep +1/-1 over endpoints.
   Deadline scheduling with profits (each job unit length): sort by profit DESC, place each
   job as late as possible before its deadline (DSU "next free slot" makes it near-linear).
   Fractional knapsack: sort by value/weight DESC.
   Minimise sum of |x - a_i|: x = the MEDIAN. Minimise sum of (x - a_i)^2: x = the MEAN.
========================================================================= */

// Moore-Hodgson: maximum number of jobs finished on time. jobs = {processing time, deadline}.
int maxOnTime(vector<pair<ll, ll>> jobs) {
    sort(all(jobs), [](auto& a, auto& b) { return a.second < b.second; });
    priority_queue<ll> pq;
    ll t = 0;
    for (auto [p, d] : jobs) {
        t += p, pq.push(p);
        if (t > d) t -= pq.top(), pq.pop();
    }
    return pq.size();
}
// Johnson's rule: optimal two-machine flow-shop order (returns the job indices).
vector<int> johnson(const vector<pair<ll, ll>>& jobs) {     // {p on machine 1, p on machine 2}
    vector<int> A, B;
    for (int i = 0; i < (int)jobs.size(); i++) (jobs[i].first < jobs[i].second ? A : B).push_back(i);
    sort(all(A), [&](int x, int y) { return jobs[x].first < jobs[y].first; });
    sort(all(B), [&](int x, int y) { return jobs[x].second > jobs[y].second; });
    A.insert(A.end(), all(B));
    return A;
}
