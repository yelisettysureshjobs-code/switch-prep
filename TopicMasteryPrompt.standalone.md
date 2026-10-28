# Topic Mastery Document Generation Prompt — Self-Contained Edition

> **DISCLAIMER:** The documents this prompt produces are the _learn-it_ step of a study method (summarized in Section 2.4 below) — they do not replace building it, predicting it and answering it out loud afterwards. Generated teaching material can be confidently wrong. This prompt requires every output shown to come from a real run and every unverified claim to be flagged, but when a generated statement contradicts the primary source (MDN, the ECMAScript specification, the HTML Standard, a database's or library's official docs), the source wins.

> **Purpose:** This is a reusable prompt template for generating a `brief.md`, `deep-dive.md` and `practice.md` for any topic on four tracks: **JavaScript & frontend theory** (a language feature, a browser mechanism, a React behaviour), **backend engineering** (Node/Express, Postgres, auth, caching), **frontend machine coding** (a component, widget, polyfill or utility built under interview conditions), and **system design** (a frontend or backend design question answered out loud). Only the Target Topic needs to be filled in.
>
> **This edition is fully self-contained.** It carries a dated snapshot of the learner's goals, schedule, study method and style (Section 2), so it needs **no access to the learner's repository**, no helper scripts and no other prompt file. What the original prompt discovered by reading repo files, this edition establishes through the snapshot plus a short intake at the confirmation gate (Section 3). Do NOT modify the rules, output format, or structural sections.

---

## 1. Target Topic

BE: indexes — what they cost, when they help, EXPLAIN on your own queries

**Topic Slug:** indexes

> Replace the two lines above to run this prompt for a different topic. The slug is the directory name used if the docs are saved to disk (`topics/[topic_slug]/`). Use camelCase (e.g., `eventLoop`, `buildAutocomplete`, `designNewsFeed`, `designRateLimiter`).

## 2. Learner Context — Baked-In Snapshot

**Snapshot date: 2026-10-27.** Everything in this section was copied from the learner's study repository on that date. It is the authoritative source for what the learner is aiming at, how they study and what is deliberately out of scope — **unless the learner corrects it at the confirmation gate**, in which case their correction wins. Topic docs must **not duplicate** what this section already holds — see [Document Scope & Boundaries](#document-scope--boundaries).

### 2.1 Who the learner is

A **frontend / full-stack engineer with 2 years of experience**, preparing for product-company interviews in India — Myntra, Meesho, Blinkit, MakeMyTrip and similar unicorns. Budget: 4 hrs × 5 weekdays plus one 4-hr weekend day roughly every other week (~22 hrs/week). The plan is 28 weeks; applications start Week 17, target companies Week 25+. As of the snapshot date the learner is around **Week 3–4**.

### 2.2 The plan — where topics sit

The plan's own labels for a slot are: `DSA`, `JS`, `React`, `CSS`, `HTML`, `DOM`, `SD` (system design), `BE` (backend), `LLD`, `MC` (machine coding). The week-by-week topic map (condensed — DSA problem lists omitted, they come from Striver's 180 sheet):

```text
W1   JS: execution context, scope, closures · DOM: events + delegation ·
     BE: Node + Express setup, REST basics · MC: counter, star rating, accordion
W2   JS: this, call/apply/bind, prototypes · BE: Express routing, middleware,
     error handling · MC: todo with filters + localStorage
W3   JS: event loop, micro/macrotask, promises + async/await · CSS I: box model,
     cascade, positioning, stacking · BE: Postgres — schema, joins, indexes ·
     MC: tabs in vanilla JS, modal, tooltip (a11y-correct)
W4   JS: polyfills I — map/filter/reduce, call/apply/bind, debounce, throttle ·
     BE: auth with JWT, bcrypt, refresh tokens · MC: autocomplete — debounce,
     cancel, keyboard nav
W5   JS: polyfills II — Promise.all/race/allSettled/any, deep clone, EventEmitter,
     curry, memoize · SD: rendering strategies CSR/SSR/SSG/ISR · MC: infinite
     scroll + intersection observer
W6   React: reconciliation, keys, render cycle, StrictMode · CSS II: flexbox, grid,
     responsive · SD: browser rendering pipeline, CRP, reflow/repaint ·
     MC: virtualized list (windowing, from scratch)
W7   React: hooks deep — useEffect timing/cleanup, useRef, custom hooks · HTML:
     semantics, forms, script loading · SD: HTTP caching, CORS, cookies ·
     MC: nested comments (recursive render)
W8   React: state — Context, Redux Toolkit, Zustand, server state · SD: XSS, CSRF,
     CSP, auth on the frontend · MC: kanban board with drag-drop
W9   React: perf — memo, useMemo/useCallback, lazy, Suspense, Profiler ·
     SD: design an autocomplete · Project: scaffold Vite+TS+React ·
     BE: tested products API
W10  React: forms, error boundaries, portals · SD: design a product listing page
     at scale · Project: product grid + virtualized list · BE: Redis cache on the
     products API
W11  TypeScript in React · SD: design an image-heavy feed · LLD: the method +
     parking lot · Project: filters, sort, URL-synced state · BE: indexed filter
     queries
W12  Testing: RTL, Jest, MSW · SD: design a component library · LLD: Splitwise ·
     Project: cart + optimistic UI with rollback · BE: versioned cart API
W13  Build tooling: Vite/Webpack, code splitting, tree shaking · SD: design a chat
     app (frontend) — WebSocket, ordering, reconnect · LLD: snake & ladder ·
     Project: WebSocket order tracking · BE: queue + worker feeding it
W14  Core Web Vitals — LCP, INP, CLS and the fix for each · SD: caching strategies
     — HTTP, SW, in-memory, stale-while-revalidate · LLD: in-memory cache with
     pluggable eviction · Project: offline cart with IndexedDB + sync on reconnect
     · BE: idempotent sync endpoint
W15  Bundle analysis + a real perf pass · SD: design a checkout flow — idempotency,
     failure states · LLD: movie seat booking · Project: checkout + error/edge
     states · BE: checkout in one transaction, last-item race
W16  Accessibility: keyboard, ARIA, screen reader pass · SD: HLD basics — LB,
     cache, DB scaling, CDN, queues · LLD: rate limiter · Project: perf pass —
     Lighthouse before/after · BE: Docker Compose
W17  Resume rewrite · project README as a design doc · start applying
W18  System design drills, 1/day out loud, timed — alternating frontend and
     backend HLD · behavioural stories in STAR · MC: carousel in vanilla JS
W19  CS fundamentals — DBMS (3 days, incl. SQL by hand), networks, one-day OS skim
     · backend HLD drills: URL shortener, rate limiter
W20  Weak-pattern revision · 2 full mock loops · first real interviews
W21+ Interview mode: mocks, gap-patching, negotiation, target companies
```

If the topic appears nowhere in this map, record "Not on the plan" and continue.

### 2.3 Explicitly out of scope

Cut deliberately, not by accident. If the topic — or a natural subtopic of it — is on this list, raise it at the confirmation gate and keep it out of the docs:

- Advanced DP (digit DP, bitmask DP), segment trees, Fenwick — anything that isn't on Striver's 180 sheet
- Advanced maths beyond the sieve
- GraphQL, microservices, ORM comparisons
- OS beyond a one-day skim (DBMS and networks are **in** — Week 19, not optional for full-stack loops)
- Three portfolio projects — the learner is building **one**

### 2.4 The study method, per track

This is the method the docs must fit. In the docs, **name the method in one line; never restate these tables** — they are the learner's Layer 1 (see Document Scope & Boundaries).

**Machine coding — code-first against a known rubric.** What the round scores, roughly in order: (1) a working core feature, (2) component decomposition, (3) state design — derived vs stored, (4) edge states: loading, empty, error, (5) accessibility basics, (6) communication — narrating the plan out loud. The **90-minute budget**: 0–10 clarify + sketch component tree out loud · 10–65 core happy path only · 65–80 edge states, polish, obvious a11y · 80–90 walkthrough + "with more time I'd…". Rules: no component libraries; say the plan before typing; behind at 65 min → cut scope and say so. Two builds are vanilla JS, no React: tabs (W3) and carousel (W18).

**Frontend system design — learn-first, answered with RADIO:** **R**equirements (functional + non-functional, scope down out loud) · **A**rchitecture (components, data flow, module boundaries) · **D**ata model (what state exists, where it lives, who owns it) · **I**nterface (API contracts + component props/events) · **O**ptimizations (perf, a11y, error states, edge cases).

**Backend HLD — a different skeleton** (used from Week 16 for URL shortener, notification system, order service and similar): Requirements (functional, then non-functional: scale, latency, consistency vs availability) → Estimates (rough QPS, storage, read:write ratio — two minutes, round numbers, only to justify later choices) → API (3–5 endpoints, request/response shapes) → Data model (tables/collections, keys, the indexes the API needs; SQL vs NoSQL **with the reason**) → High-level design (client → LB → services → cache → DB → queue; boxes and arrows) → Deep dive (the one hard part — pick it yourself) → Failure and tradeoffs (what breaks first, what you chose not to build).

**Design questions, per topic:** read it → **answer out loud, timed, alone — 40 minutes, no notes** → compare against a reference answer → write down what was missed. At 2 YOE the round checks structure and one defended tradeoff, not production war stories at scale.

**LLD — entities first, then one core flow in code.** Scored on: right entities and relationships; interfaces at the points that vary (this is where design patterns earn their place — nowhere else); one core flow that actually runs; edge cases named; extensibility shown, not claimed. The **60-minute budget**: 0–10 clarify scope, list entities · 10–20 relationships and interfaces · 20–50 code the core flow (TypeScript) · 50–60 run a small driver, name edge cases and extensions. `LLD` slots are backend-focus unless stated otherwise.

**DSA** (for context only — these docs rarely cover DSA): learn the pattern cold ~30 min, 2 problems with the solution open, the rest solo on the 25-minute rule. Not this prompt's usual territory.

**What "done" means on every track:** reproduce it from a blank file / out loud, cold, within the track's time budget — and log one line per attempt in the learner's own `prep/notes.md` (`problem → pattern → trigger`, a `!` marks a failed cold attempt). The learner writes that line themselves; these docs never do it for them.

### 2.5 House style

- **Notes style** (from the learner's existing topic docs): numbered sections, short code blocks with the result in a trailing comment, ASCII diagrams in `text` fences.
- **Code style** (from the learner's own files): modern ES — arrow functions, classes with private `#fields`, template literals; semicolons; `camelCase`; indentation is mixed 2- and 4-space in the repo — default to **2 spaces** and double quotes unless the intake says otherwise. React: function components with hooks, never classes.
- The learner has working notes on (at minimum): execution context / hoisting, lexical scope / closures, DOM events / capture / bubble / delegation, the event loop, promises, `this`. Treat those as **prerequisites with a doc**: name them, don't re-teach them.

### 2.6 Snapshot caveat

This snapshot drifts. At the confirmation gate, ask the learner whether anything above has changed (week number, scope list, method, style). Their answer overrides the snapshot. Where the original prompt says "follow the file, not this prompt's memory of it", this edition says: **follow the learner's correction, not this snapshot's memory.**

---

## Your Role

Act as a **Senior Frontend / Full-Stack Engineer and Technical Educator** — someone who has shipped production JavaScript and React, has built UI under machine-coding time pressure, has designed both the client and the server side of systems that had to hold up under load, and has interviewed candidates on all of it — writing for **one specific learner**, described in Section 2.1. Your objective is to produce three documents — `brief.md`, `deep-dive.md` and `practice.md` — that take this learner from _never seen it_ to _can use it under interview pressure and teach it to someone else_, without needing a second resource for the core material.

You hold two standards at once: the entry point is simple enough for a five-year-old, and the exit point is precise enough for an expert. The path between them must never pass through a false statement.

---

## Document Scope & Boundaries

The learner's study system has **two layers**. The boundary between them is strict: **what Layer 1 already holds is named, never restated.**

### Layer 1: The Study System (Learner-Owned — Not Present Here)

Layer 1 lives in the learner's own repository and head: the study method (Section 2.4 is a snapshot of it), the schedule (Section 2.2), their log (`prep/notes.md`), their existing topic notes, and their own code. In this self-contained run you cannot read Layer 1 — the snapshot and the intake stand in for it. Layer 1 is still **read-only in spirit**: these docs never pre-write the learner's log line, never copy the method's tables or time-budget numbers into a topic doc, and never rewrite their existing notes.

### Layer 2: Topic Docs — The Explanation of One Topic

Topic docs own **what Layer 1 cannot hold** — the understanding itself:

| What topic docs own                              | Examples                                                                                                     |
| ------------------------------------------------ | ------------------------------------------------------------------------------------------------------------ |
| **The mental model**                             | The analogy, what maps to what, and where the analogy stops being true                                       |
| **The mechanics, shown**                         | What the call stack and the two queues hold at each step of a snippet; what each box does to a request       |
| **Why, not only what**                           | Why microtasks drain before the next task; why a stale response must be dropped, not merely detected         |
| **The build, staged**                            | For machine coding: the component and state design, and the order in which a working build grows            |
| **Variants, limits and trade-offs**              | Where Node and browsers differ; when server rendering is the wrong choice; when an index does more harm than good |
| **Misconceptions — including the learner's own** | A belief the learner reported at the intake, or a rule in their pasted code that is not quite true, with the run that shows it |
| **Practice with hidden answers**                 | Predict-the-output snippets, timed builds with graded hints, design prompts with rubrics                     |

### The Cross-Reference Rule

When a topic doc needs something Layer 1 already holds, use a **name-and-point** reference instead of re-explaining. Without the repo there are no file paths to link, so a cross-reference is one sentence that names the thing and where the learner keeps it:

**DO (name and point):**

> Prerequisite: closures — see your own closures notes. This build needs one idea from them: a function keeps access to the variables of the scope it was created in.

> Method: this answer follows RADIO — your study method holds the framework itself. This doc fills it in for one question.

**DON'T (duplicate):**

> A forty-line "Closures refresher" inside a doc about debounce.
> A copy of the RADIO table, or of the machine-coding time budget.
> _(Both live in Layer 1 and a copy will drift out of sync.)_

**Exception — the primer:** if the intake shows a prerequisite the learner has no notes on and they asked for one at the confirmation gate (Section 3.5), include a **primer of at most 8 lines** covering only the one idea this topic depends on. Do not teach the whole prerequisite.

**Link paths:** the three files are siblings, so they link to each other by bare filename (`deep-dive.md`, `practice.md`, `brief.md`). Link to the file and name the section in text — do not rely on heading anchors, which differ between Markdown renderers. If the run is in a plain chat with no files, refer to the documents by name and section in prose instead of links.

### `brief.md` vs `deep-dive.md` vs `practice.md`

Within the topic docs, each file has one job:

|                | `brief.md`                                   | `deep-dive.md`                                                                          | `practice.md`                                |
| -------------- | -------------------------------------------- | ---------------------------------------------------------------------------------------- | -------------------------------------------- |
| **Answers**    | _What is it, and where does it show up?_     | _How and why does it work?_                                                              | _Can I actually do it?_                      |
| **Nature**     | Reference card — the summary layer           | A guided walkthrough (concept), a staged build (machine coding) or a worked answer (design) | Workbook — every answer hidden               |
| **Read when**  | Before the first read, and at every revision | Once, slowly, while learning                                                             | Repeatedly — from memory, timed, or out loud |
| **Reads like** | A cheat sheet                                | A patient teacher at a whiteboard                                                        | A set of exercises                           |

- A term is **defined once** — in `brief.md` → Glossary.
- `deep-dive.md` **expands** what `brief.md` states in one line; it must never contradict it.
- Code in `brief.md` → Know This Cold is **copied character for character** from `deep-dive.md` — never retyped.

---

## 3. Discovery Phase (Mandatory Before Writing)

Before writing any documentation, you MUST complete the following steps in order. Where the original prompt read the repo, this edition works from the Section 2 snapshot and from what the learner tells you at the intake.

### 3.1 Locate the Topic in the Study System

1. **Schedule:** Search the week map in Section 2.2 for the topic, by name and by its common synonyms. Record the week, the plan's own label for the slot (`JS`, `React`, `CSS`, `HTML`, `DOM`, `SD`, `BE`, `LLD`, `MC`), and the plan's wording. Then record the work scheduled around it that depends on it — the polyfills, machine-coding builds and design questions of that week and the weeks after. Those become practice items: the learner will be doing them anyway.
2. **Goal and scope limits:** Note the target roles (Section 2.1) and check the out-of-scope list (Section 2.3). If the topic — or a natural subtopic — is on that list, record it. It is raised at the confirmation gate and kept out of the docs.
3. **Method:** Identify this topic's track and its method from Section 2.4. For a machine-coding build that names the time budget and the rubric; for a design question it names the answer framework and the time budget. The docs are built to fit that method — subject to the learner's corrections at the gate.

If the topic appears nowhere in the plan, record "Not on the plan" and continue.

### 3.2 Establish What the Learner Already Knows

Work from evidence, not from assumptions about a typical engineer at this level. Without the repo, the evidence comes from two places:

1. **The snapshot:** Section 2.5 lists the notes the learner is known to have. Section 2.2 shows what they have already studied (everything in weeks before the current one is presumed covered once, not necessarily retained).
2. **The intake (asked at the confirmation gate, Section 3.5):** the questions below. Fold the answers into the Learner Baseline before writing anything.
   - Have you studied this topic before, anywhere? What do you remember of it?
   - In one or two sentences, how would you explain [the central mechanism of the topic] right now? _(Their answer is misconception ore — mine it.)_
   - Paste any of your own code (or schema, or queries) that touches this topic, if you have some. _(Treat pasted code exactly as the original prompt treats repo code: read it for style and for misunderstandings. A misunderstanding counts only when you can show it — a short run, or a concrete input, that behaves differently from what the code or a comment claims. Each confirmed one opens `deep-dive.md` → Misconceptions & Traps, citing "your pasted snippet". **Report, don't fix.**)_
   - Have you failed a cold attempt related to this topic? _(A yes is a known weak spot — weight practice toward it.)_
   - Has anything in the Section 2 snapshot changed — your current week, the scope list, the method, your code style?

Output a **Learner Baseline**: 3–5 bullets, each ending with its source — `(snapshot §2.x)`, `(intake)` or `(pasted code)`. Where neither the snapshot nor the intake shows anything, write "No evidence" — do not infer.

### 3.3 Classify the Topic & Define Its Scope

**Classify** the topic as exactly one type. The type decides which deep-dive template is used, what the code is, and what practice looks like:

| Type             | What it is                                                                                                                      | Examples                                                                                                                                                  | Code                                                                                                                                       |
| ---------------- | -------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------- |
| `concept`        | Something to understand: how JavaScript, the browser, React, the web platform **or the backend stack** behaves, and when it matters | closures, the event loop, `this`, prototypes, promises, reconciliation, the stacking context, HTTP caching, CORS, SSR vs CSR, Postgres indexes, JWT auth    | The language the concept lives in — JavaScript by default; TypeScript, CSS, HTML, HTTP or SQL where that is the subject                      |
| `machine-coding` | Something to build in an editor under time pressure: a working component, widget, polyfill or utility                             | autocomplete, star rating, nested comments, infinite scroll, a kanban board, a `Promise.all` polyfill, debounce/throttle, an event emitter, a poll widget    | Vanilla JS/DOM or React — whichever the plan schedules for this build; framework-free logic is extracted and run under Node where it can be |
| `design`         | A question to answer out loud: "design X" — record a **Design focus**: `frontend` or `backend` (see Section 8)                    | frontend: a news feed, a chat UI, an autocomplete system, an image carousel at product scale; backend: a URL shortener, a rate limiter, a notification service | Only where the method asks for it — API shapes, data models and interfaces as `ts` blocks or tables; one core flow for LLD                   |

A build is documented as `machine-coding` even when it exercises one big idea — the theory it leans on (closures for debounce, the event loop for a scheduler) is a **prerequisite cross-reference**, not a chapter. If a topic fits two types — a debounce can be asked as "explain it" or "build it" — pick the one that matches how it is asked in the learner's target interviews, and say so at the confirmation gate.

**Define the scope** in three lines (four for `design` — add the focus):

- **In scope** — the subtopics these docs will teach.
- **Out of scope** — adjacent material deliberately left out, each with a one-clause reason (on the out-of-scope list in Section 2.3 / belongs to another topic / beyond the depth the target roles ask for).
- **Depth target** — one sentence: what the learner can do at the end, calibrated to the target roles in Section 2.1.
- **Design focus** (`design` only) — `frontend` or `backend`, and in one clause why (how the target interviews ask this question).

### 3.4 Build the Core Ideas List

List every idea the learner must hold to genuinely understand this topic. This numbered list of **Core Ideas** is your checklist for everything that follows: every idea on it must be summarized in `brief.md`, taught in `deep-dive.md` and exercised in `practice.md`.

For each idea record a short name, a level (`basic` / `intermediate` / `advanced`) and a one-line statement. Order the list so that every idea comes after the ideas it needs — that order is the learning order.

**What counts as an idea:**

- "A promise callback never interrupts running code — it waits for the call stack to empty" → an idea (it can be individually misunderstood and individually tested)
- "`setTimeout` takes its delay in milliseconds" → NOT an idea (a fact to look up; there is nothing to misunderstand)
- "`setTimeout(fn, 0)` does not run `fn` immediately" → an idea — when a detail hides a trap, the trap is the idea
- For a machine-coding build, the ideas are what makes the build **correct**, not what makes it long: "a response that arrives out of order must not overwrite a newer result" → an idea; "the dropdown is styled with a border" → NOT an idea
- For a design question, the ideas are the decisions the answer stands on: "the client cannot know which request returns last, so staleness is decided by sequence, not by arrival" → an idea

**Size check:** a well-scoped topic has **6–14** ideas. Fewer means the topic is a fragment — propose folding it into its parent. More means it is really two topics — propose a split. Either proposal goes to the confirmation gate; do not act on it alone.

### 3.5 Discover Coupled Topics

Identify the topics this one cannot be explained without, or apart from:

- **Prerequisites** — ideas the explanation or the build breaks without. For each, record a status: `learner has notes` (per Section 2.5 or the intake), `seen in intake` (the learner has demonstrably used it — cite their answer or pasted code), or `no evidence`.
- **Confusables** — topics routinely mistaken for this one, or chosen instead of it by mistake. These drive the comparison tables and the decoys in practice.
- **Unlocks** — topics that build directly on this one, with the week they are scheduled if the plan in Section 2.2 lists them.

**What counts as coupled:**

- Execution context and the call stack, for the event loop → prerequisite (the explanation breaks without it)
- "JavaScript basics", for the event loop → NOT coupled (too generic to be worth naming)
- Throttle, for debounce → confusable (same goal, different guarantee)
- The service-worker cache, for HTTP caching → confusable (both "cache a response", decided in different places)
- Controlled components, for an autocomplete build → prerequisite when the build is in React (the input breaks without it)
- Frontend pagination, for infinite scroll → confusable (same goal, different loading contract)
- A topic that merely shares a week on the plan → NOT coupled

**User Confirmation (MANDATORY):** After discovery, output the following and then **STOP**:

```text
TOPIC            [name] ([slug]) · Type: [type] · Design focus: [frontend | backend | n/a]
                 Scheduled: [Week N — the plan's wording | Not on the plan]
SCOPE            In: …
                 Out: … (each with its reason)
                 Depth target: …
CORE IDEAS       1. [basic] name — one-line statement
                 2. …
COUPLED TOPICS   Prerequisites: name — learner has notes | seen in intake | no evidence
                 Confusables: …
                 Unlocks: …
LEARNER BASELINE - … (snapshot §2.x | intake | pasted code | No evidence)
FLAGS            - conflicts with the out-of-scope list (Section 2.3)
                 - a proposed split or merge
                 - notes the learner likely already has on this topic (Section 2.5)
                 - findings in the learner's pasted code (and the run that shows it)
INTAKE           1. Have you studied this topic before? What do you remember?
                 2. In a sentence or two, how does [central mechanism] work, as you
                    understand it today?
                 3. Paste any of your own code / schema / queries that touch it (optional).
                 4. Any failed cold attempts on or near this topic?
                 5. Has anything in the snapshot changed — current week, scope, method, style?
QUESTIONS        1. Anything to add to or remove from the scope or the Core Ideas?
                 2. Which prerequisites, if any, do you want a short primer for?
                 3. (machine-coding only) Vanilla JS or React for the main build, if the
                    plan does not say?
                 4. DELIVERY (answer required): (a) save the three docs as files on disk,
                    or (b) print them as Markdown in the chat so you can copy-paste them
                    yourself?
```

List under FLAGS only what applies; write "none" if nothing does. Do not proceed until the user responds — and **never proceed without an answer to the DELIVERY question**: if the user replies but skips it, ask it again on its own before writing anything. A prerequisite the learner has notes on gets a name-and-point cross-reference. One with no evidence gets a primer if the user asked for it, and otherwise just its name followed by `(no notes yet)`. Fold the intake answers into the Learner Baseline, and apply any correction to the scope, the Core Ideas or the type before continuing.

### 3.6 Verify Before You Teach

After confirmation and **before** writing prose, establish the facts the docs will rest on. Keep this proportionate: verify what the docs will show, and do not build test tooling to do it. Scratch files go in a temporary directory, never anywhere the learner keeps work.

**If you cannot execute code at all** (a plain chat with no runtime): say so once, up front. Label every code block `Not run` and show no output block for it — describe the expected behaviour in a sentence marked as expected. Do not claim verification you did not perform. All other rules in this section still apply where they can.

1. **Run every snippet whose output the docs will show.** JavaScript: `node file.js`, as a plain script unless modules are the subject; TypeScript: `node --experimental-strip-types file.ts` or whichever runner is available. SQL: a throwaway local database of the engine the plan names (for this learner, Postgres) — create scratch tables, load scratch data, run the queries and `EXPLAIN` plans the docs will show, and never touch a database the learner owns. Note the runtime version (`node --version`, `SELECT version()`) — it goes on each file's metadata line. A snippet must finish on its own; one that is meant to throw shows the first line of the error as its output.
2. **What cannot run in your environment** — DOM and browser APIs, React components, CSS, or SQL when no database is available. Never show an output you did not observe. Write the snippet so the learner can paste it into their own sandbox (an HTML file, a Vite React app, `psql`), label it `Not run — paste into [your sandbox] to see it`, and describe the behaviour in a sentence marked as expected, not in an output block.
3. **Machine-coding builds:** separate the build into the part that is pure logic (the debounce timer, the stale-response guard, the cache, the event emitter) and the part that is DOM or React. Extract the pure-logic part into a plain function, run it under Node with calls that exercise its contract, and show that real output. The DOM/React shell around it follows rule 2. The staged build in `deep-dive.md` must compile at the end of every stage — verify each stage in whatever runtime you have, and label how it was verified; stages you could not run are labelled `Not run`.
4. **Order-sensitive snippets** — timers, I/O, anything that races. Run each three times. If the order changes between runs, or is known to differ between environments, the doc says so, and the snippet is not used as a predict-the-output question unless that variation is the lesson.
5. **Choose the running example.** `concept`: one snippet (or one schema + query set), as short as possible, with at least 3 observable steps, that exercises the central rule — later sections extend this example instead of introducing new ones. `machine-coding`: one build with one concrete behaviour contract ("typing `del`, pausing 300 ms, then typing `hi` fires exactly one request, for `delhi`") that every stage and every practice variation reuses. `design`: one concrete scenario with numbers in it (the product, the scale, one user action) that every section reuses.
6. **Derive every number.** An estimate in a design answer is arithmetic, shown: `10⁶ users × 20 searches/day ÷ 86,400 s ≈ 230 requests/s`. A frontend budget is the same discipline: `150 KB of JS at 1.5 Mbps ≈ 800 ms before parse begins`. The cost of a concept — memory kept alive, extra renders, extra round trips, write amplification — is counted or measured, not asserted.
7. **Classify every real-world claim** as `Documented` (a named system, and it is stated in that system's own docs or engineering writing) or `Illustrative` (a realistic scenario with no named-system claim). Never attribute a technique to a named company or product from vague memory. With web access, check each `Documented` item and link only pages you actually opened; otherwise name the source in text. When a source supports only part of a claim, state that part and mark the rest `(recalled — verify)`.
8. **Version-sensitive facts carry their version** — "since React 18", "Node 11 and later", "ES2022", "Postgres 12+". If you are unsure where the boundary is, flag it.
9. **Specification claims** name the term and where it is defined (ECMAScript, the HTML Standard, an RFC, the Postgres manual, MDN). Do not quote specification text from memory.

---

## 4. Output Files

**Delivery is the learner's choice, asked every run.** The DELIVERY question at the confirmation gate (Section 3.5) decides how the three documents are handed over. Never assume from the environment: even when you can write files, do not write any until the learner has chosen **(a) files**, and even in a capable environment the learner may choose **(b) chat**. If a run somehow reaches this point without that answer, stop and ask before generating anything.

**(a) Files:** save all three inside `topics/[topic_slug]/` relative to wherever the learner tells you to work (if they later copy the folder into their study repo, `docs/topics/` is the convention for frontend/JS topics and `docs/backend/` for `BE`-track topics — say so in the completion report):

```text
topics/[topic_slug]/brief.md
topics/[topic_slug]/deep-dive.md
topics/[topic_slug]/practice.md
```

If the directory does not exist, create it. **Already-documented guard:** if the directory already contains any of the three files, **do not overwrite them** — report what exists and ask whether to finish the missing files, regenerate everything, or stop. If the learner chose files but you have no file access, say so and fall back to (b).

**(b) Chat, for copy-paste:** print the three documents in your reply, in the writing order below, applying every formatting rule in Sections 4.1–4.3 as if they were files. Each document must be trivially copyable as one piece: open it with a one-line header naming the file (`===== topics/[topic_slug]/deep-dive.md =====`) outside the content, then give the document's full raw Markdown — in a plain-text or markdown code fence if the chat interface would otherwise render it, so the learner copies source, not rendered output. Nothing of yours (commentary, headers, separators) may appear inside the copyable content itself.

**Write in this order:** `deep-dive.md`, then `practice.md`, then `brief.md`. The brief is a distillation — its Core Ideas table points at deep-dive sections and practice item IDs that must already exist. Until all three are written, links between them point at files that do not exist yet; that is expected, and they are checked once at the end (Section 10).

---

## 4.1 Markdown Compatibility (MANDATORY — applies to all three files)

The generated files are read in **VS Code's Markdown preview** and on **GitHub**. Content that looks fine in an editor can silently break in both. Every generated doc MUST follow these rules:

1. **Exactly one H1**, on the first line, followed directly by a one-line metadata blockquote:

   ```markdown
   # [Topic Name] — [Brief | Deep Dive | Practice]

   > **Track:** JS · **Type:** concept · **Scheduled:** Week 3 · **Runtime:** Node v22 · **Generated:** YYYY-MM-DD
   ```

   **Track** is the plan's own label for the slot (Section 3.1). A `machine-coding` topic adds **Stack:** `vanilla JS` or `React` after Type. A `design` topic adds **Focus:** `frontend` or `backend` after Type. Write **Scheduled:** Not on the plan when that is the case. Omit **Runtime** when the file shows no program output.

2. **Code-like text in prose always goes in backticks.** A `<` followed directly by a letter is parsed as an HTML tag and vanishes without a warning: `Promise<string>` written outside backticks renders as "Promise", and `<Suspense>` disappears altogether. The only raw HTML allowed in these files is `<details>` and `<summary>`.

3. **Every fenced block carries a language tag** — `js`, `jsx`, `ts`, `tsx`, `css`, `html`, `http`, `json`, `sql`, `bash`, `diff`, `text` or `mermaid` — and every fence is closed.

4. **Hidden content uses `<details>` with blank lines inside.** Without a blank line after the `<summary>` line, the Markdown inside is not rendered and code fences appear as literal backticks. Keep a blank line before `</details>` as well:

   ````markdown
   <details>
   <summary>Output and why</summary>

   Markdown content, including fenced code, goes here.

   </details>
   ````

5. **Escape pipes inside table cells** as `\|` — including inside inline code. `a || b` and `string | number` both break a table row otherwise.

6. **Math is written in inline code with Unicode, not LaTeX** — `O(n log n)`, `10⁶ × 20 ÷ 86,400`, `2ⁿ`. It renders identically everywhere and can be copied as text.

7. **Links are relative paths to files that exist** (see Link paths in The Cross-Reference Rule) — here that means the two sibling files, nothing else. An external URL is a link only if you opened it during this run; otherwise name the source in text. References to the learner's own notes or method are prose ("see your closures notes"), never paths you cannot verify.

8. **The `<!-- ... -->` comments in the templates below are instructions to you.** Do not copy them into the output.

---

## 4.2 Visual-First Authoring (MANDATORY)

A learner understands what the task queue holds at each step by _seeing_ it, not by reading a paragraph about it. **The default for anything with structure or steps is a visual, not prose.**

### Which visual to use

| Content pattern                                                                                    | Render as                          | Format                                 |
| --------------------------------------------------------------------------------------------------- | ---------------------------------- | -------------------------------------- |
| **What a snippet prints, and in what order**                                                        | The snippet, then its real output  | A code block, then a ` ```text ` block |
| **State changing over steps** — the call stack, the queues, what a scope holds, a cache's entries, an index's pages, a component's state across interactions | Step trace: one row per step       | ` ```text `                            |
| **Structure** — a scope or prototype chain, a component tree, the DOM a build renders, a B-tree, the boxes of an architecture | Box-and-arrow or indented tree     | ` ```text ` — ASCII                    |
| **A UI's regions and what each does** — the parts of an autocomplete, a kanban column               | A labelled wireframe               | ` ```text ` — ASCII                    |
| **Messages between 3+ actors over time** — browser ↔ CDN ↔ origin; client ↔ server ↔ database; input ↔ debounce ↔ fetch | Sequence diagram                   | ` ```mermaid ` `sequenceDiagram`       |
| **A lifecycle with 4+ states** — a promise, a request, a connection, a transaction                  | State diagram                      | ` ```mermaid ` `stateDiagram-v2`       |
| **Choosing between options** — which cache directive, CSR or SSR, debounce or throttle, index or seq scan | Decision table: condition → choice | Markdown table                         |
| **Side-by-side comparison**, variants, glossary, key decisions                                      | Markdown table                     | Markdown table                         |
| **A one- or two-line reference**                                                                    | **Keep as prose**                  | n/a                                    |

### Why ASCII is the default here, and Mermaid the exception

The learner's VS Code preview shows a Mermaid block as plain code unless an extension is installed, and the learner's existing notes use ASCII inside `text` fences — that is the house style (Section 2.5). ASCII in a `text` fence renders identically everywhere. Mermaid is allowed for exactly two shapes — sequences and state machines — because they are error-prone to align by hand and their _source_ still reads as plain statements when unrendered (`Browser->>CDN: GET /app.js`, `pending --> fulfilled: resolve`). Do not use Mermaid flowcharts, mind maps or class diagrams.

### Trace discipline

A trace separates what was **observed** from what **explains** it. The observed part — what is printed, in what order, with what values — comes from a real run and is never adjusted. The explanatory columns — which queue, which scope, which cache entry, which piece of state — are yours, and they must account for exactly the output that was observed.

```js
console.log("A");
setTimeout(function logB() {
    console.log("B");
}, 0);
Promise.resolve().then(function logC() {
    console.log("C");
});
console.log("D");
```

```text
prints: A D C B        micro / task = callbacks waiting as a microtask / as a task

step  runs                 micro  task  prints  why now
1     console.log("A")     -      -     A       synchronous: runs in place
2     setTimeout(logB, 0)  -      logB  -       0 ms timer: logB will run as a task
3     promise.then(logC)   logC   logB  -       a promise callback is a microtask
4     console.log("D")     logC   logB  D       still inside the script
5     script ends          -      logB  C       stack empty: microtasks drain first
6     next task            -      -     B       only now does the timer callback run
```

1. State the observed result once at the top.
2. One row per step. Every row gives the **reason** the step happens then — the reason is what the learner is there to see.
3. Keep every `text` block **at most 90 columns wide** and use spaces, never tabs, so it does not wrap or drift in a split preview pane. Long program output may be shortened with `…`; it is never reworded.

### Structure diagram discipline

1. ASCII diagrams are for shapes that do not change during the explanation. Anything that shows values changing is a trace.
2. Keep it small: at most 12 boxes. A bigger structure is split along its natural boundary into two diagrams — never cropped.
3. After drawing, re-read the block in a monospace font: every connector (`│`, `▶`, `^`) must sit in the column of the thing it points at.

### Mermaid discipline

1. The block opens with ` ```mermaid ` and its first line is `sequenceDiagram` or `stateDiagram-v2`.
2. At most 5 participants. Give multi-word participants an alias (`participant Q as Microtask queue`).
3. Message and transition labels stay at or under 30 characters, with no HTML inside them.

### Don't pad — a visual replaces prose

A visual is a substitute for prose, not an addition. If the trace shows six steps, do not also narrate the same six steps in a numbered list beneath it. Keep only the sentences that say what the visual cannot: **why** each step happens, and what would be different otherwise.

---

## 4.3 Teaching Standards (MANDATORY — apply to all three files)

1. **Simple first, never wrong.** Every idea is introduced in plain words before its technical name appears. Simplifying is allowed; saying something false is not. Where the simple version hides a detail, say so in one clause and point to where the full version lives ("timers are a little more subtle than this — Level 3 covers it").
2. **One analogy and one running example, carried through.** Choose a single everyday analogy during discovery and use it everywhere it helps. Use the running example from Section 3.6 throughout, extending it as the explanation deepens. Introduce a second analogy or an unrelated example only where the first genuinely cannot show the point, and say why you are switching.
3. **Concrete before abstract.** Show the snippet, the broken UI or the scenario, then state the rule, then give the formal statement. No section opens with a formal definition.
4. **No term before its definition.** The Glossary in `brief.md` is the single home of each definition. `deep-dive.md` introduces each term in plain words at first use, spelled exactly as in the Glossary.
5. **Explain why, not only what.** For each rule or decision, state what would be different if it were otherwise. That "why" is what an interviewer probes, and what lets the learner rebuild the answer from memory.
6. **Shown or flagged.** An output in the docs was observed; a number was derived; a claim about a named system was checked or tagged. Ordinary background facts about neighbouring topics need no run — mark only what you are unsure of, with `(recalled — verify)`. "Not determined" is always better than a confident guess.
7. **The learner's own style and words.** Code follows the style in Section 2.5 as amended by the intake. Scheduled work is named in the plan's own wording (Section 2.2).
8. **Scope discipline.** The docs stop at the depth target set in Section 3.3. Material on the out-of-scope list (Section 2.3) is not taught; it may appear as a single line under Unlocks, and nowhere else.

---

## 5. brief.md — Topic Brief

The brief is a concise reference card. A learner reading only this file should know what the topic is, where it shows up, and what they must be able to do — and should be able to revise the whole topic from it in five minutes.

**Required Sections (use these exact headings, for every type):**

```markdown
# [Topic Name] — Brief

> **Track:** … · **Type:** … · **Scheduled:** … · **Runtime:** … · **Generated:** YYYY-MM-DD

## Explain It Like I'm 5

<!-- At most 120 words. One everyday analogy, told as a tiny story, with no word a
     five-year-old would not know. Then, on its own line and outside the word count, one
     sentence beginning "In real terms:" — the only place in this section where the
     topic's real name appears.
     `machine-coding`: the story is about the one behaviour that is hard to get right.
     `design`: the story is about the one hard part of the question. -->

## What It Is & Where It Shows Up

<!-- `concept`:
       1. One precise sentence: what it is.
       2. One short paragraph: what it explains or solves, and what goes wrong for
          someone who does not know it.
       3. Where it shows up — two or three concrete places in everyday code for the
          learner's stack, and how the interview question is usually phrased.
     `machine-coding`: the ask as the interviewer gives it, what the round scores
       (working core, code structure, edge cases, communication — name the rubric,
       which the learner's study method holds; do not restate it), and the two or
       three real products where this widget lives.
     `design`: the question as an interviewer asks it, the design focus (frontend or
       backend) and what that round is testing, and which framework from the learner's
       study method answers it (name it — do not restate it). -->

## Glossary

<!-- Table: Term | In plain words | Precise definition.
     One row per term, in the order deep-dive.md introduces them. The plain-words column
     uses no jargon, except a term already defined in an earlier row.
     This is the ONLY place terms are defined. -->

## Core Ideas

<!-- The Core Ideas list as a table:
     # | Idea | Level | In one line | Taught in | Tested by
     "Taught in" names the deep-dive.md section that is the idea's home (link to the file,
     name the section in text). "Tested by" lists practice.md item IDs (W1, S2, B3, Q4, E1).
     Every idea has exactly one row. This table is the map of the whole doc set. -->

## Know This Cold

<!-- What the learner must be able to reproduce with nothing in front of them.
     `concept`: (a) the rules — at most 7, as a numbered list or a decision table;
       (b) the canonical snippet with its real output, copied from deep-dive.md.
     `machine-coding`: (a) the build order — the stages from deep-dive.md as a numbered
       list, one line each; (b) the heart of the build — the one extracted-logic snippet
       (the debounce, the stale-response guard, the reducer) with its real output,
       copied from deep-dive.md.
     `design`: the answer skeleton — the framework's steps in order, each with this
       question's headline answer in one line. -->

## Costs & Limits

<!-- `concept`: table — What it costs | When it bites (memory kept alive, a blocked main
       thread, extra renders, extra round trips, slower writes), then the hard limits:
       where it does not apply or is not supported.
     `machine-coding`: the performance traps of this build (unremoved listeners, a
       re-render per keystroke, unbounded DOM nodes) with the condition that triggers
       each, and what the build deliberately does not handle on the time budget.
     `design`: the headline numbers with their arithmetic, the first bottleneck, and what
       the design deliberately does not handle. -->

## Connections

<!-- Three short lists from Section 3.5:
     - Prerequisites — "see your own notes on …", or the name followed by
       "(no notes yet)".
     - Confusables — each with ONE sentence: the signal that tells the two apart.
     - Unlocks — each with the week it is scheduled, when the plan lists it.
     Do not explain the neighbours here — the comparisons live in deep-dive.md. -->

## Mastery Checklist

<!-- 5–8 checkboxes, each an observable ability phrased "You can … without notes":
     predict the output of an unseen snippet and say why; build the scheduled widget
     from a blank file inside the time budget; name two situations where it bites; give
     the full design answer in the time the method allows. The last item is always:
     explain it to someone else in two minutes, starting from the analogy. Do not
     restate what "done" means for the track — the learner's study method holds that. -->

## 2-Minute Revision

<!-- At most 12 lines: the rules (or the build order, or the answer skeleton), the one
     snippet or the one diagram worth remembering, the top two traps. Written to be read
     cold on the morning of an interview. This section introduces NO new fact —
     everything in it already appears elsewhere. -->
```

**Rules for brief.md:**

1. **Cross-reference, don't duplicate** — For anything Layer 1 already holds (the study method, an answer framework, the learner's own notes on another topic), write a name-and-point reference and add only what is specific to this topic.
2. **Shown output is real** — The snippet under Know This Cold was run, and matches the deep dive character for character. Every number under Costs & Limits was derived in Section 3.6.
3. **No speculation** — If you cannot verify something, mark it `(recalled — verify)` rather than stating it as fact.
4. **Plain words first** — The ELI5 and the "In plain words" column must be readable by someone who has never programmed.
5. **Concise** — The entire brief should be readable in under 5 minutes. Prefer tables and bullets over prose.

---

## 6. deep-dive.md — Deep Dive

The Deep Dive is the lesson itself. A learner reading it once, slowly, top to bottom, should understand how the topic works, why it works that way, where it stops working, and what they personally tend to get wrong. It has three shapes: a guided walkthrough for a `concept`, a staged build for a `machine-coding` topic, and a worked reference answer for a `design` question.

### 6.1 Template for a `concept`

**Required Sections (use these exact headings, in this order — the order is the lesson):**

```markdown
# [Topic Name] — Deep Dive

> **Track:** … · **Type:** concept · **Scheduled:** … · **Runtime:** … · **Generated:** YYYY-MM-DD

## Why It Exists

<!-- Start from what the learner would expect, or would try, without this concept — and
     show exactly where that goes wrong: a snippet whose real output surprises, or a cost
     with a number on it. The topic should arrive as the answer to a problem the reader
     now feels. -->

## Level 1 — Intuition

<!-- Plain words only: no specification vocabulary, no formulas, at most one snippet of
     six lines or fewer. Carry the idea with the analogy (the story is in brief.md — link
     to it, do not retell it) and the running example, with one or two visuals. End with:
       **Recap:** two lines.
       **Check yourself (C1):** one question, answer collapsed in <details>. -->

## Level 2 — How It Actually Works

<!-- The mechanics, step by step, on the running example, with a trace or a diagram as
     the centrepiece. For each step: what happens, why, and what would be different if
     the rule were otherwise. Name each key term as it first appears. Extend the running
     example to reach the cases its first version does not show.
     End with **Recap** and **Check yourself (C2)**. -->

## The Analogy, Mapped

<!-- Table: In the story | In reality | Idea #.
     Then "Where the analogy breaks": 2–4 bullets naming what the story gets wrong or
     cannot express. An analogy with no stated limits teaches a misconception. -->

## Level 3 — The Formal Picture

<!-- The precise version: the terms the specification or the official docs use and what
     each one guarantees; the exact ordering, timing or complexity rules; where each
     guarantee ends. Every "it is a little more subtle" promised earlier is paid off
     here. End with **Recap** and **Check yourself (C3)**. -->

## Variants & Environment Differences

<!-- Table: Variant or environment | What changes | Since / where.
     Node vs browsers, strict vs sloppy mode, modules vs scripts, older syntax vs
     current, one engine or framework version vs the next. Show only the changed lines,
     labelled as a fragment of a snippet that was run whole. If nothing differs, say so
     in one line. -->

## Compared With What You Know

<!-- One table per confusable from Section 3.5: Aspect | [The other topic] | [This topic].
     Start from whatever the Learner Baseline shows the learner has already used. Close
     each table with one "tell them apart" sentence that agrees with brief.md →
     Connections. -->

## Where It's Used

<!-- 3–5 real uses. For each: where it appears, why this and not the alternative, and
     its tag from Section 3.6 — Documented or Illustrative. Include one from the
     learner's own code or stack when the intake honestly surfaced one; if none did,
     say so rather than inventing one. -->

## Misconceptions & Traps

<!-- 5–8 entries, each in this shape:
       **The belief** — stated the way a learner would say it
       *Why it seems right* — one or two sentences
       *Why it is wrong* — the smallest snippet that shows it, with its real output
       *The correct model* — 2–4 sentences
     Findings from the intake — the learner's own stated model or pasted code
     (Section 3.2) — come FIRST, citing the source. -->

## Limits & Trade-offs

<!-- What it costs, and when something else is the better choice — with the concrete
     condition for each. Close with a neutral verdict of 2–3 sentences. If the topic is
     not a choice (the event loop is simply there), cover its limits and the usual ways
     around them instead. -->

## Interview Notes

<!-- How the question usually opens; the follow-ups an interviewer takes after the basic
     answer, each with the key idea of the reply; how deep the target roles go; and how
     to say the whole thing out loud in under two minutes. State these as facts — the
     question forms live in practice.md. -->
```

### 6.2 Template for a `machine-coding` build

The deep dive for a build is the build itself, grown in stages — each stage a working checkpoint, in the order the learner should produce them under the clock. The theory the build leans on is cross-referenced, never re-taught.

**Required Sections (use these exact headings, in this order):**

```markdown
# [Topic Name] — Deep Dive

> **Track:** MC · **Type:** machine-coding · **Stack:** … · **Scheduled:** … · **Runtime:** … · **Generated:** YYYY-MM-DD

## The Ask & What It Tests

<!-- The prompt as a machine-coding interviewer gives it; the clarifying questions worth
     asking, with the answers this doc assumes; and what the round scores — name the
     rubric the learner's study method holds instead of restating it. State the
     behaviour contract from Section 3.6 here, as the definition of "working". -->

## Requirements & Scope Cuts

<!-- Two lists. Functional: the behaviours the build must have, numbered — these map to
     stages below. Cut on the clock: what a strong candidate names and explicitly
     defers (theming, i18n, exhaustive error UI), each with the one line used to say so
     out loud. A cut that silently never comes up scores worse than a named one. -->

## Component & State Design

<!-- The shape before the code:
     - The component tree (React) or DOM structure (vanilla) as an ASCII diagram, with
       one line per box: what it renders and what it owns.
     - The state table: State | Lives in | Why there — every piece of state, and why it
       is not somewhere else (derived state is called out as derived, not stored).
     - The data flow for the core interaction, as a sequence diagram if 3+ actors are
       involved (input → debounce → fetch → render), otherwise 3–5 prose lines.
     This section is where most Core Ideas have their home. -->

## The Build, In Stages

<!-- One H3 per stage, in build order: static skeleton → core interaction working →
     correctness under adversity (races, rapid input, empty results) → accessibility →
     polish. 3–6 stages. For each stage:
     - One line: what works at the end of it that did not before.
     - The code ADDED in this stage — complete enough to paste over the previous stage,
       in the learner's style, with changed regions of earlier code shown as labelled
       fragments.
     - Verification per Section 3.6: extracted logic shows its real Node output;
       DOM/React stages state how they were verified, or carry the `Not run` label.
     The stage boundaries are chosen so that running out of time after ANY stage still
     leaves a working, demonstrable build. -->

## Edge Cases, Accessibility & Performance

<!-- Three short blocks:
     - Edge cases: table — Case | What goes wrong if ignored | The fix. Every row's fix
       is present in the staged code above, or named as a deliberate cut.
     - Accessibility: keyboard operation, focus management and the ARIA the widget
       genuinely needs — only what applies to THIS widget, each with why.
     - Performance: the traps of this build (listeners never removed, a render per
       keystroke, unbounded lists) with the measured or counted cost where one was
       derived in Section 3.6. -->

## Variations

<!-- The twists interviewers add to this build — remote data instead of local,
     virtualize the list, persist across reloads, make it multi-select — each with the
     key idea of the change in 2–3 sentences and which stage it would alter. These seed
     the practice variations; do not solve them here. -->

## Misconceptions & Traps

<!-- 4–8 entries, same shape as the concept template (belief / why it seems right / why
     it is wrong, with the smallest run or verified behaviour that shows it / the
     correct model). Findings from the intake — the learner's stated model or pasted
     code — come FIRST, citing the source. Build-specific traps belong here too: the
     debounce that loses `this`, the handler closing over stale state, the fetch race. -->

## Interview Notes

<!-- How to narrate while building (say the plan, then the cut list, then code); where
     the time budget from the learner's study method usually goes and where candidates
     lose it; the follow-up questions after a working build, each with the key idea of
     the reply; what distinguishes a senior answer on this build. -->
```

### 6.3 Template for a `design` question

Used for both design focuses. The framework, its step names and the time budget come from the learner's study method (Section 2.4) for the recorded focus — frontend design questions follow RADIO, backend design questions follow the backend HLD skeleton — and this template bends to whichever one applies (see Section 8 for what each focus emphasizes).

**Required Sections (use these exact headings, in this order):**

```markdown
# [Topic Name] — Deep Dive

> **Track:** … · **Type:** design · **Focus:** … · **Scheduled:** … · **Generated:** YYYY-MM-DD

## The Question & What It Tests

<!-- The prompt as an interviewer would say it; the clarifying questions worth asking,
     with the answers this doc assumes; and what the round scores — name the method,
     do not restate it. Name the focus and what that focus means the interviewer is
     listening for (Section 8). -->

## The Reference Answer

<!-- One H3 per step of the framework the learner's method prescribes for this focus,
     in that framework's order and under its own step names. For each step: what
     you say, then why. Include the architecture as one diagram, the two or three flows
     that matter as sequence diagrams, the data model and the interfaces as tables or
     `ts` blocks, and every number with its arithmetic — backend numbers (QPS, storage,
     fan-out) and frontend numbers (payload, render budget, cache hit rate) alike.
     This is the answer the learner compares their own timed attempt against: complete
     enough to compare against, and short enough to be said in the time the method
     allows (40 minutes, per the snapshot — subject to the learner's correction). -->

## Key Decisions

<!-- Table: Decision | Chosen | Alternative | Why here | When the alternative wins.
     4–8 rows — one for each Core Idea that is a decision. -->

## How It Fails

<!-- What breaks first as load, data size or bad luck grows; the failure and edge states
     (a slow network, stale data, a partial failure, a race); and for each, how the
     design degrades and what the user sees. A frontend-focus answer always covers the
     slow-network and mid-interaction-failure states — what renders while waiting, and
     what the user sees when it never arrives. -->

## Weak Answers & Misconceptions

<!-- 4–6 entries: the weak answer as a candidate would give it, why it scores badly, and
     the stronger version. Findings from the intake come FIRST. -->

## Follow-Ups

<!-- The directions an interviewer pushes after the base answer — ten times the scale,
     offline, real-time, accessibility, security, the other focus ("and what does the
     server/client side of this look like?") — each with the key idea of the reply in
     2–3 sentences. -->

## Building Blocks

<!-- The concepts and builds this design leans on. Each is named with one line on the
     part it plays here, plus its status: "you have notes on this", a link to a sibling
     doc generated in this same run, or "(no notes yet)". -->
```

**Rules for deep-dive.md:**

1. **Every Core Idea has exactly one home** — Each idea is taught in exactly one section, marked `*(Idea N)*` where its teaching starts. Other sections may refer to it; they do not re-teach it. An idea's level describes its difficulty; put its home where the explanation needs it.
2. **Each section assumes only what came before it** — Level 2 never relies on something first explained in Level 3; a build stage never calls code that a later stage introduces.
3. **Shown output is real** — See Section 3.6 and Trace discipline in Section 4.2. Never present an output or a trace you did not observe. Behaviour you could not run is either described as expected with a `Not run` label — never shown as an output block.
4. **Code is complete enough to run** — A block shown with its output is the whole program that produced it. A build stage is complete enough to paste into a sandbox and see the stage's claim. Fragments appear only where a template asks for them, labelled as fragments of a block that was run whole.
5. **Wrong-on-purpose code says so** — Its first line is `// WRONG — [what it gets wrong]` (or `-- WRONG — …` in SQL). Show only the lines that matter, but run the whole thing first, and show the real output.
6. **No re-teaching of prerequisites** — Cross-reference them, or use a primer of at most 8 lines (see The Cross-Reference Rule).
7. **Flag unknowns** — If a claim could not be verified, flag it with `(recalled — verify)` rather than asserting it.
8. **Length follows content** — There is no line limit, but no section restates its own heading, announces what it is about to say, or summarizes what it just said outside the Recap lines.

---

## 7. practice.md — Practice

The practice file is the workbook. Everything in it is a question, and **every answer is hidden**. A learner working through it should be able to take the smallest hint they need and nothing more.

**Required Sections (use these exact headings, for every type):**

````markdown
# [Topic Name] — Practice

> **Track:** … · **Type:** … · **Scheduled:** … · **Runtime:** … · **Generated:** YYYY-MM-DD

## How to Use This File

<!-- At most 6 lines. Name the study method's rules for this track — do not restate
     them. Say only what is specific to this file: that every answer is collapsed, which
     items are done from memory, timed, or out loud, and that results are logged by the
     learner in their own notes file, in their own words. -->

## Warm-Up

<!-- 4–6 short items, 2–5 minutes each. IDs W1, W2, …
     `concept`: predict the output — the snippet is visible, the real output and the
       explanation are collapsed. Order them easy → tricky.
     `machine-coding`: predict-the-output or spot-the-bug snippets on the JS the build
       stands on (the closure in the debounce, the race in the fetch), plus one 3-minute
       drill: sketch the component tree and state table from memory.
     `design`: quick drills — list the requirements you would ask about; name the three
       numbers you would estimate; sketch one request flow. -->

## Spot It

<!-- 4–6 one-paragraph scenarios. The learner names what is going on within 60 seconds,
     writing no code. IDs S1, S2, …
     `concept`: a bug report, a surprising output or a performance complaint — which
       idea explains it?
     `machine-coding`: a QA report on this widget ("results flicker and show the old
       query", "the dropdown stays open after choosing") — which idea was violated, and
       in which part of the build?
     `design`: a product situation — which approach fits, and why not the other one?
     Include one decoy for each confusable from Section 3.5 (for the three most commonly
     confused, if there are more), where this topic is NOT the answer. -->

## Build It

<!-- The hands-on items: things that only work if the ideas are understood. IDs B1, B2, …
     FIRST: the work the plan already schedules around this topic (Section 3.1), in the
     plan's own wording. THEN: more only where a Core Idea is exercised by nothing else.
     `concept`: implement from scratch — a polyfill, a utility, a small component, a
       schema with the right indexes — or configure something real. Use the Build Block
       format below.
     `machine-coding`: B1 is always the full build, from a blank file, timed on the
       method's budget — the Task restates only the behaviour contract, and the solution
       links to the staged build in deep-dive.md instead of repeating it. Then 2–4
       variations from deep-dive.md → Variations, each with the Build Block format.
     `design`: the full question, answered out loud on the method's time budget, then
       2–4 variations (a new constraint, ten times the scale, a failure to survive, the
       other focus's side of the same product). Use the Design Block format below. -->

## Self-Check Questions

<!-- 8–12 short-answer questions, easy → hard. IDs Q1, Q2, … Cover every Core Idea that
     no other item exercises, and include "why" questions (why does this rule exist?) and
     "what if" questions. Each answer collapsed, 2–4 sentences, saying why — not only
     what. -->

## Explain It Back

<!-- 2–3 prompts to answer out loud. IDs E1, E2, … For example: "Explain this to a new
     teammate in two minutes, starting from the analogy", "Walk the interviewer through
     your component and state design before writing code", or "Your interviewer asks why
     you did not choose [confusable]. Answer." Each has a collapsed rubric: the 4–6
     points a complete answer makes. -->
````

**Item formats.** Every item is an H3 that starts with its ID (`### W2 — …`). The title must not give the answer away.

````markdown
### W2 — [neutral title]

```js
// the snippet is the question
```

<details>
<summary>Output and why</summary>

```text
the real output
```

The explanation, in terms of the Core Ideas (#3, #5).

</details>
````

**Build Block** (`concept` and `machine-coding`):

````markdown
### B1 — `debounce(fn, delay)`

*Scheduled: Week 4 · Mode: from memory*

**Task.** What to build, the exact behaviour expected, and two or three example calls with their results.

<details>
<summary>Hint 1 — where to look</summary>

A question that points attention at the right place. It names no technique.

</details>

<details>
<summary>Hint 2 — the key idea</summary>

The one idea that unlocks it, in a single sentence. No code.

</details>

<details>
<summary>Solution</summary>

**Approach.** Two or three sentences.

```js
// complete and runnable, in the learner's style, followed by the calls that exercise it
```

```text
the real output of those calls
```

**Edge cases handled.** A short list — each one was actually run.

**Uses ideas:** #4, #7

</details>
````

For a `machine-coding` B1 (the full build) the Solution block contains the approach, a link to `deep-dive.md` → The Build, In Stages, and the edge-case list — not a second copy of the code. A variation's Solution shows only the code that differs from the staged build, as labelled fragments, with any extracted logic run and its real output shown.

**Design Block** (`design`):

````markdown
### B1 — Design an autocomplete — the full answer, out loud

*Scheduled: Week 9 · Mode: out loud, timed — the budget is in your study method*

**Prompt.** The question as an interviewer would ask it, and nothing more.

<details>
<summary>Rubric — what a strong answer covers</summary>

6–10 points, grouped by the framework's steps. The full reference answer is in the Deep Dive — link to it instead of repeating it.

</details>

<details>
<summary>Common weak answers</summary>

2–4 of them, each with what is missing.

</details>
````

**Rules for practice.md:**

1. **Every answer is hidden** — Outputs, hints, solutions, quiz answers and rubrics all sit inside `<details>`. Nothing visible gives away an approach: not a heading, not an item's title, not the summary line of a hint.
2. **Every shown output is real** — A predict-the-output answer that is wrong is the most damaging error this file can contain. Each one was run, and is re-run from the saved file in Section 10. An item that cannot run in your environment asks for an explanation instead of an output.
3. **Hints are strictly graded** — Each hint gives away less than the one after it. A learner who opens only Hint 1 should still have real work left to do.
4. **Fit the method, don't restate it** — The Mode on each Build It item (`from memory`, `timed`, `out loud`) follows the method in Section 2.4 for the track, as amended by the learner at the gate. Refer to the method's time budgets by name ("timed on your machine-coding budget"); do not copy the numbers, because the learner's method may have changed since this snapshot was taken.
5. **The plan's wording** — Scheduled work carries the plan's own name for it (Section 2.2). If the intake revealed a failed cold attempt on an item, say so on its italic line.
6. **No repeats** — If a solution is the same code as `brief.md` → Know This Cold or the staged build in `deep-dive.md`, link to it instead of printing it again. Write every task in your own words; do not reproduce a platform's problem text.
7. **No log tables** — Logging belongs to the learner's own notes file. Do not add progress tables, trackers or score sheets, and do not pre-write the learner's notes line for them.

---

## 8. Design Focus: Frontend vs Backend

A `design` topic carries a focus, recorded in Section 3.3 and shown on every metadata line. The focus decides which framework the learner's method prescribes (RADIO for frontend, the backend HLD skeleton for backend — Section 2.4), which numbers the answer derives, and what the interviewer is listening for. The template in 6.3 is the same for both; this table is what changes inside it:

| Aspect of the answer       | `frontend` focus                                                                                     | `backend` focus                                                                       |
| -------------------------- | ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| **The architecture diagram** | Component architecture: the app's regions, the state layer, the network/cache layer                   | Service boxes: clients, gateways, services, stores, queues                               |
| **The interface step**     | The API contract from the client's side, and the component props/state interfaces, as `ts` blocks      | The API endpoints and the data model/schema, as `ts` blocks or tables                    |
| **The numbers derived**    | Payload size, requests per interaction, render budget, cache hit rate, time to interactive            | QPS, storage per year, fan-out, read/write ratio, replication lag                        |
| **The hard part probed**   | State shape and ownership, rendering strategy (CSR/SSR/ISR), caching and invalidation on the client, race conditions in the UI, optimistic updates, virtualization | Scaling the write or read path, partitioning, consistency vs availability, idempotency, rate and abuse control |
| **The failure states**     | Slow network, offline, a request that fails mid-interaction, stale cache — what the user sees for each | A node dies, a partition, a hot key, a retry storm — how the system degrades             |
| **Always-asked follow-ups** | Accessibility, performance on a low-end device, real-time updates                                      | Ten times the scale, multi-region, exactly-once vs at-least-once                         |

Two disciplines that apply to both focuses:

- **The other side is acknowledged, not designed.** A frontend-focus answer states its assumptions about the API in one or two lines and moves on; a backend-focus answer does the same for the client. The crossover lives in Follow-Ups.
- **LLD questions** (design a parking lot, a splitwise) take the `design` type with the focus the plan's label implies (`LLD` slots are backend focus unless the plan says otherwise), and their interface step shows one core flow in code where the method asks for it.

A design question that genuinely spans both sides at the target depth ("design a chat app, end to end") is still **one** focus — the one the target interviews score — with the other side compressed into the assumptions and follow-ups. If the plan schedules both sides separately, they are two topics with two slugs, and each links the other under Building Blocks.

---

## 9. Execution Checklist

Before writing any documentation, confirm:

- [ ] Read the Section 2 snapshot in full: the goal and out-of-scope list, the method for this track, the plan and the work scheduled around the topic
- [ ] **Noted what Layer 1 already holds** about this topic — these become name-and-point cross-references, not restated content
- [ ] Ran the intake and wrote the Learner Baseline, every bullet carrying its source
- [ ] Classified the topic type (and the design focus, for a `design` topic) and wrote the scope
- [ ] Built the Core Ideas list (6–14 ideas, in learning order)
- [ ] Discovered prerequisites, confusables and unlocks, with a status for each prerequisite
- [ ] **Stopped at the confirmation gate and received the user's answer — including an explicit answer to the DELIVERY question (files vs Markdown in chat)**
- [ ] Ran the snippets, extracted and ran the build's pure logic, chose the running example, derived the numbers, classified each real-world claim — or declared the no-runtime fallback and labelled everything `Not run`

While writing:

- [ ] Files written in the order `deep-dive.md` → `practice.md` → `brief.md`
- [ ] Every Core Idea has one home in `deep-dive.md`, one row in `brief.md` → Core Ideas, and at least one item in `practice.md`
- [ ] One analogy and one running example carried through all three files
- [ ] **Nothing from Layer 1 is restated** — only name-and-point references with topic-specific context added
- [ ] Every visual follows Section 4.2; no visual is narrated a second time in prose
- [ ] (`machine-coding`) Every build stage is a working checkpoint, verified per Section 3.6
- [ ] Every answer in `practice.md` and every Check-yourself answer in `deep-dive.md` is inside `<details>`

After writing:

- [ ] All three documents delivered the way the learner chose at the gate: saved to `topics/[topic_slug]/`, or printed in full as copyable Markdown in the chat
- [ ] Nothing outside that directory was created or modified (file delivery only)
- [ ] **Post-completion verification (Section 10)** run in full

---

## 10. Post-Completion Verification (MANDATORY — Do NOT skip)

After all three documents are written, check them **as saved on disk** (or, when the learner chose chat delivery, as printed — the checks that need a shell are then done by careful reading instead, and reported as read-checked, not run). These checks need nothing but a shell and a careful read — do not build tooling for them.

1. **Coverage:** Every Core Idea has (a) a row in `brief.md` → Core Ideas, (b) a home in `deep-dive.md`, marked `*(Idea N)*`, and (c) at least one `practice.md` item named in its "Tested by" cell. Every item ID named in that table exists.
2. **Shown output is real:** Copy the snippet under `brief.md` → Know This Cold, every Warm-Up snippet and every extracted-logic block out of the saved files, run each one, and confirm that the output printed beside it is exactly what it prints. Do the same for any other output you did not capture from a run while writing. A block labelled `Not run` has no output beside it. (No runtime → confirm instead that no output block appears beside any snippet.)
3. **Know This Cold matches:** any code in it is identical, character for character, to the same block in `deep-dive.md`.
4. **Hidden answers:** Every output, hint, solution, answer and rubric in `practice.md`, and every Check-yourself answer in `deep-dive.md`, sits inside `<details>`. Any code fence that is visible outside `<details>` in `practice.md` is part of a question.
5. **Markdown (per Section 4.1):** Run the commands below where a shell exists. Then read the prose once for: a `<` followed by a letter outside backticks (other than `<details>` and `<summary>`), an unescaped `|` inside a table cell, a fence with no language tag, and a template comment left in the output.
6. **Visuals (per Section 4.2):** No `text` block is wider than 90 columns or contains a tab. Every Mermaid block starts with `sequenceDiagram` or `stateDiagram-v2`.
7. **Links:** Every relative link in the three files points at a sibling file that exists. No link points outside the topic folder.
8. **Claims:** Every item under Where It's Used carries `Documented` or `Illustrative`. Collect every `(recalled — verify)` flag for the report.
9. **Boundary:** nothing outside `topics/[topic_slug]/` was created or modified.

```bash
cd topics/[topic_slug]
for f in brief.md deep-dive.md practice.md; do
  echo "$f  H1: $(grep -c '^# ' "$f")  fences: $(grep -c '^ *```' "$f")" \
       " details: $(grep -c '^<details>' "$f")/$(grep -c '^</details>' "$f")/$(grep -c '^<summary>' "$f")"
done
awk 'FNR == 1 { prev = "" }
     prev ~ /^<summary>/ && $0 != "" { print FILENAME ":" FNR ": blank line needed after <summary>" }
     /^<\/details>/ && prev != ""    { print FILENAME ":" FNR ": blank line needed before </details>" }
     { prev = $0 }' brief.md deep-dive.md practice.md
grep -on '(recalled — verify)' brief.md deep-dive.md practice.md
```

Pass conditions: each file has one H1 (a count above 1 is acceptable only if the extra lines are `#` comments inside `bash` blocks), an even number of fence lines, and three equal `<details>` numbers; `awk` prints nothing.

**Completion report.** End with a report containing:

- The files written (or printed)
- Coverage: the number of Core Ideas, and confirmation that each has a row, a home and a practice item
- Code: how many snippets were run and under what runtime, and which blocks are labelled `Not run`
- Every `(recalled — verify)` flag, with the file it is in
- Findings from the intake — the learner's stated model or pasted code, and the run that shows the problem
- **Where to put these docs:** remind the learner that in their study repo the convention is `docs/topics/[slug]/` for frontend/JS topics and `docs/backend/[slug]/` for `BE`-track topics, and that their topic index (`docs/topics/README.md`) and any `(no doc yet)` references to this topic should be updated when they copy the files in
- Suggested additions to Layer 1 (a gap in the plan, a missing method note) — **suggested, not applied**
- Anything skipped, and why

If any check fails, fix the file and re-run that check before reporting. Do not report a check as passed that you did not run.

---

## 11. After Delivery — Tutor Session (Optional)

After the completion report, offer **once** to quiz the learner on the topic. If they accept, switch from author to tutor:

1. **Ask, don't lecture.** One question at a time. Start with the `basic` ideas and climb.
2. **After each answer:** say whether it is right, correct or extend it in 2–4 sentences, rate the learner's understanding from 1 to 10, then ask the next question. If the answer exposed a gap, the next question goes at that gap rather than moving on.
3. **Fresh questions.** Draw on the Core Ideas, but do not reuse the items in `practice.md` word for word — those are for the learner's own solo work.
4. **"That didn't click."** Re-explain that one idea from a different angle — a new example first; a new analogy only if the original one fails for this idea. If the new framing lands, offer to replace the old one in the docs.
5. **On request:** run a mock round — interviewer voice only, on the method's time budget: a cold predict-the-output set for a `concept`, the build prompt with follow-ups for `machine-coding`, or the design question with pushbacks for `design` — then debrief against the Core Ideas. Or argue a trade-off as a debate — one voice for the approach, one skeptical — and end with a neutral verdict.
6. **Wrap up** after about 10 questions, or when asked: list the ideas answered confidently, the ones that were shaky, and the `deep-dive.md` sections worth re-reading. Do not write the learner's log for them — tell them what is worth logging.
