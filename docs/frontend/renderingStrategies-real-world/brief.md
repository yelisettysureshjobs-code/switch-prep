# Hydration, Streaming SSR & Islands — Brief

> **Track:** SD · **Type:** concept · **Scheduled:** Week 5 · **Runtime:** Node v22.14.0 · **Generated:** 2026-11-06

## Explain It Like I'm 5

A shop can build its display while you wait, prepare it before opening, or bring out the first shelves while the rest are still coming. Some signs are already readable, but the buttons do not work until someone connects them. The shop can put touchscreens only where shoppers need them.

In real terms: rendering strategies decide where and when page HTML is made, while hydration and islands decide how selected parts gain browser behavior.

## What It Is & Where It Shows Up

Rendering strategies decide when and where a page's initial HTML is produced; hydration is a separate client step for adding behavior to existing server HTML. A page can look complete before its controls work, and a large client bundle can delay that work.

It appears in product listings, product details, and marketing pages with a mix of stable content and interactive or personalized regions. Interviewers ask “SSR vs. CSR?”, “What does hydration do?”, or “How would you render a product listing?” Myntra and Meesho are motivating marketplace examples here, not claims about their current implementations.

## Glossary

| Term | In plain words | Precise definition |
|---|---|---|
| Client-side rendering (CSR) | The browser builds the page view. | JavaScript in the browser creates or updates the DOM for the UI. |
| Server-side rendering (SSR) | A server makes HTML for a request. | The server renders UI output into HTML in response to a navigation/request. |
| Static site generation (SSG) | HTML is prepared before visitors arrive. | Pages are rendered ahead of requests, commonly during a build. |
| Incremental Static Regeneration (ISR) | A framework refreshes cached static output later. | A framework-specific policy regenerates previously generated output after invalidation or revalidation. |
| Hydration | Browser code connects behavior to existing HTML. | Client-side scripts attach application state and interactivity to server-rendered HTML. |
| Hydration mismatch | The first browser output differs from existing HTML. | A difference between server-rendered markup and the initial client render during hydration. |
| Streaming SSR | HTML is sent in parts as work becomes ready. | Server rendering emits a response progressively rather than waiting for all content. |
| Shell | The first useful page structure sent by a stream. | In React streaming docs, the rendered content outside Suspense boundaries that determines the earliest loading state. |
| Islands architecture | A page stays mostly static while selected regions do extra work. | An architecture that renders most page content as HTML and isolates selected client-interactive or dynamic server-rendered regions. |
| Client island | A selected widget gets browser behavior. | An interactive component hydrated separately from the rest of the page. |
| Server island | A selected dynamic region is rendered separately on the server. | A server-rendered component isolated from the main page render, often allowing the surrounding content to be cached or sent earlier. |
| React Server Component | A React component executed in a server/build environment. | A component rendered in a separate server environment; it is distinct from HTML SSR and client hydration. |

## Core Ideas

| # | Idea | Level | In one line | Taught in | Tested by |
|---:|---|---|---|---|---|
| 1 | Rendering location | basic | CSR, SSR, SSG, and ISR differ in when and where initial output is produced. | [deep-dive.md](deep-dive.md), Level 1 — Intuition | W1, Q1 |
| 2 | Hydration | basic | Client code connects behavior to server HTML; it does not produce that HTML. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | S1, Q2 |
| 3 | Output agreement | intermediate | The initial client output should agree with the server output. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | W1, W2, S2, Q3, B1 |
| 4 | Hydration cost | intermediate | Server HTML can still require client download, execution, and hydration work. | [deep-dive.md](deep-dive.md), Level 3 — The Formal Picture | S1, S5, Q4 |
| 5 | Streaming SSR | intermediate | The server can send a shell before slower sections are ready. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | W3, S3, Q5, B2 |
| 6 | Progressive rendering | intermediate | Earlier HTML does not necessarily mean earlier interaction. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | W3, S3, Q6 |
| 7 | Islands | intermediate | Selected regions can receive client behavior while much of a page stays static. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | S5, Q7, B2 |
| 8 | Partial interactivity | intermediate | Islands help only when the other regions do not need browser behavior. | [deep-dive.md](deep-dive.md), Level 3 — The Formal Picture | S5, S6, Q8, B2 |
| 9 | Content freshness | intermediate | Rendering and cache policy determine how quickly changed data appears. | [deep-dive.md](deep-dive.md), Level 3 — The Formal Picture | W4, S4, Q9, B2 |
| 10 | Commerce tradeoffs | advanced | Choose per route based on visibility, interactivity, freshness, and cost. | [deep-dive.md](deep-dive.md), Limits & Trade-offs | S5, Q10, B2, E3 |

## Know This Cold

1. CSR builds the view in the browser; SSR renders HTML for a request; SSG prepares HTML ahead of requests; ISR is framework-specific cache regeneration.
2. SSR creates HTML; hydration connects client behavior to existing HTML.
3. Keep the server output and first client output aligned.
4. Streaming controls delivery timing; it does not guarantee interactivity.
5. Islands limit browser behavior to selected regions; those regions still cost JavaScript.
6. Cache freshness is separate from streaming; validate price and stock at purchase boundaries.
7. State what to measure and avoid unsupported claims about a named company's stack.

Canonical agreement check, copied from [deep-dive.md](deep-dive.md) → Level 2 — How It Actually Works:

```js
const server = ["h1: Sneakers", "button: Add"];
const client = ["h1: Sneakers", "button: Add"];
console.log(server.every((node, index) => node === client[index]));
```

```text
true
```

## Costs & Limits

| What it costs | When it bites |
|---|---|
| JavaScript transfer and startup | A hypothetical `150 KB` bundle over `1.5 Mbps` needs about `800 ms` for idealized transfer alone. |
| Hydration work | Large interactive trees or low-end devices; profile actual devices. |
| Request-time rendering | Server data/render time can delay the response and consume compute. |
| Cached output | Price or availability can be stale without a freshness and validation policy. |
| Islands | Shared state and cross-region coordination add complexity. |

Hard limits: streaming does not define freshness; SSR does not guarantee interactivity; islands do not eliminate JavaScript for interactive regions. The `800 ms` figure is derived, illustrative, and not a company measurement.

## Connections

- **Prerequisites:** HTML and DOM basics (no notes yet); React rendering/reconciliation (no notes yet; scheduled Week 6); HTTP caching (no notes yet; scheduled Week 7). The deep dive includes short primers.
- **Confusables:** SSR creates initial HTML; hydration connects client behavior. Streaming is about delivery timing; islands are about interactive scope. ISR is cache regeneration, not streaming. React Server Components describe component execution, not hydration.
- **Unlocks:** Week 6 — browser rendering pipeline, CRP; Week 9 — React perf, lazy, Suspense, Profiler; Week 10 — design a product listing page at scale; Week 14 — caching strategies.

## Mastery Checklist

- [ ] You can distinguish CSR, SSR, SSG, and ISR without notes.
- [ ] You can explain why visible HTML may still have inert controls.
- [ ] You can identify a server/client initial-output mismatch and say how to avoid it.
- [ ] You can explain what streaming changes and what it does not change.
- [ ] You can mark which parts of a 24-item listing truly need client behavior.
- [ ] You can state a freshness rule for price and availability.
- [ ] You can explain the topic to someone else in two minutes, starting from the analogy.

## 2-Minute Revision

CSR: browser builds the view. SSR: server renders per request. SSG: prepare ahead.
ISR: framework-specific cache refresh. Hydration: connect client behavior to existing HTML.
Server and first client output should match.
Streaming sends a shell and later content; it is not hydration.
Islands add client behavior only where needed; interactive regions still cost JS.
Price/stock freshness is separate from when HTML arrives.
For a listing: early product HTML, usable filters, explicit freshness, measured client work.
Do not claim Myntra or Meesho uses a particular approach without a direct source.