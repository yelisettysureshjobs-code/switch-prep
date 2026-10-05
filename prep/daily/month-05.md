# Month 5 — Design, Polish, First Applications (Weeks 17–20)

The schedule changes shape here. No new DSA topics — revision only. The hours move to output.

**New weekday split from Week 17:**

| Time | Block |
|---|---|
| 1.5 hr | DSA revision — cold re-solves from `notes.md`, mixed topics |
| 1.0 hr | System design drill — one problem, out loud, timed. Alternate frontend (RADIO) and backend HLD |
| 1.0 hr | Applications, referrals, resume, behavioural prep |
| 0.5 hr | Machine coding warm-up — one small component, timed |

---

## Week 17 — Resume & first applications

| Day | Plan |
|---|---|
| **Mon** | **Resume rewrite** — every bullet as *action → metric → tech*. Mine your day job first: any perf win, migration, component library, feature you owned end to end. A production number beats any side project.<br>**DSA rev:** 4 array + binary search problems cold |
| **Tue** | **Resume, round 2** — one page, no skills-bar graphics, project section with the perf numbers. Get it reviewed (peer, or an r/developersIndia thread).<br>**DSA rev:** 4 linked list + stack problems |
| **Wed** | **LinkedIn + GitHub cleanup** — headline, about, pinned repos, this repo's README. Recruiters read GitHub for frontend roles.<br>**DSA rev:** 4 tree problems · **SD drill:** design an autocomplete (again — you should be faster now) |
| **Thu** | **Referral outreach** — 20 messages. Alumni, ex-colleagues, LinkedIn cold DMs with a specific ask. Not portals.<br>**DSA rev:** 4 graph problems · **SD drill (backend):** news feed — fan-out on write vs read, timeline cache, the celebrity problem |
| **Fri** | **Apply: 15 roles**, mid-tier first — deliberately not your four targets. These are your calibration interviews.<br>**DSA rev:** 4 DP problems · **SD drill:** design a video streaming page |
| **Wknd** | *(if available)* 4h: mock interview #4 — full loop simulation with a peer |

## Week 18 — System design depth + behavioural

| Day | Plan |
|---|---|
| **Mon** | **SD:** design Myntra's PLP + PDP. Real product, real constraints.<br>**Behavioural:** write STAR story #1 — a conflict you handled<br>**DSA rev:** mixed 4 |
| **Tue** | **SD:** design a real-time order tracking system (you built one — now defend the design)<br>**Behavioural:** STAR #2 — a failure and what changed after<br>**DSA rev:** mixed 4 · **MC:** carousel in **vanilla JS**, timed 45 min |
| **Wed** | **SD (backend):** notification system — channels, queues, retries, dedupe, user preferences<br>**Behavioural:** STAR #3 — something you owned end to end<br>**DSA rev:** mixed 4 |
| **Thu** | **SD:** design an analytics/event-tracking SDK for the browser<br>**Behavioural:** STAR #4 — pushing back on a bad requirement<br>**DSA rev:** mixed 4 · **MC:** calendar/date picker, timed |
| **Fri** | **SD (backend):** order + inventory service — stock reservation, oversell, idempotent order creation *(you built the small version in W15)*<br>**Behavioural:** STAR #5 — mentoring or raising the bar<br>**Apply: 15 more roles** |
| **Wknd** | *(if available)* 4h: record yourself answering 3 SD questions. Watch it back. Painful, and the fastest fix for rambling. |

## Week 19 — CS fundamentals + backend HLD

> Not optional for full-stack loops. DBMS gets three days because it is what backend rounds probe hardest; networks one; OS a single skim. Explain each topic against your own project's schema and API, not from a textbook definition.

| Day | Plan |
|---|---|
| **Mon** | **DBMS:** normalization 1NF–3NF, indexes (B-tree vs hash), query plans · **SD drill (backend):** URL shortener — the classic warm-up, full HLD skeleton · **DSA rev:** mixed 4 |
| **Tue** | **DBMS:** ACID, isolation levels, locking, deadlocks — reproduce one anomaly in your own Postgres · **MC** timed · **DSA rev:** mixed 4 |
| **Wed** | **SQL by hand:** 10 queries — joins, GROUP BY / HAVING, subqueries, window functions · **SD drill (backend):** rate limiter in depth — distributed counters in Redis, where it sits, what happens when Redis is down · **DSA rev:** mixed 4 |
| **Thu** | **Networks:** TCP vs UDP, HTTP/1.1 vs 2 vs 3, TLS handshake, DNS resolution — then "what happens when you type a URL", out loud, 5 min · **DSA rev:** mixed 4 |
| **Fri** | **OS skim, one sitting:** process vs thread, context switching, deadlock conditions, paging · **Apply: 15 roles** |
| **Wknd** | *(if available)* 4h: mock interview #5 |

## Week 20 — Weak-pattern sweep + real interviews begin

| Day | Plan |
|---|---|
| **Mon** | Pull every problem you've failed twice out of `notes.md`. That list is the whole week.<br>Solve 5 of them. Understand the *trigger*, not the code. |
| **Tue** | 5 more weak problems · **MC:** file explorer tree, timed · **SD drill:** design an image CDN |
| **Wed** | 5 more · **JS:** rewrite every polyfill cold, one more time (they get asked at 2 YOE, constantly) |
| **Thu** | 5 more · **MC:** the build you're slowest at, again, timed · post-mortem any real interview so far |
| **Fri** | Full 4-hr simulated loop, alone, timed: DSA 45 min → JS/React 45 min → machine coding 90 min → SD 40 min<br>**Apply: 15 roles** |
| **Wknd** | *(if available)* 4h: **Month 5 audit** — is the resume converting? If under ~10% callback rate, the resume is the problem, not you. Rewrite it. |

---

**Month 5 exit check**
- [ ] ~200 problems, everything revised at least twice
- [ ] Resume converting (10%+ callbacks) — if not, fix the resume before applying more
- [ ] 45+ applications out, mostly via referral
- [ ] 12+ system design problems answered out loud, at least 5 of them backend HLD
- [ ] Can explain isolation levels and an index choice using your own project's tables
- [ ] 5 STAR stories written and rehearsed
- [ ] First real interviews happening
