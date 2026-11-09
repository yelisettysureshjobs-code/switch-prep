# Rendering Strategies — Brief

> **Track:** SD · **Type:** concept · **Scheduled:** Week 5 · **Runtime:** Node v22.14.0 · **Generated:** 2026-11-05

## Explain It Like I'm 5

Imagine a shoe shop that needs to show a list. A helper can make the list in your hands, the shop can make one when you ask, or the shop can prepare it before anyone arrives. A prepared list is quick to hand out, but it may show yesterday’s number. Sometimes the shop replaces it later, and sometimes it adds a little part just for you.

In real terms: CSR, SSR, SSG, and ISR are ways to decide where and when a web page is made, and how fresh its information needs to be.

## What It Is & Where It Shows Up

A rendering strategy decides where and when a page’s initial HTML is produced. It matters because that choice changes when content appears, how current it is, and whether the browser or server does more work.

You encounter it when a React app fills an empty root in the browser, when a server sends a product list in its response, or when a build publishes ready-made category pages. An interviewer may ask, “Would you use CSR, SSR, SSG, or ISR for this page, and what does that choice cost?”

## Glossary

| Term | In plain words | Precise definition |
| --- | --- | --- |
| Rendering strategy | Where and when a page is made | The approach used to produce the initial HTML content for a route or region. |
| Client-side rendering (CSR) | The browser makes the page | JavaScript in the browser generates the HTML content. |
| Server-side rendering (SSR) | The server makes the page when asked | The server generates HTML for a request and sends it to the client. |
| Static site generation (SSG) | The page is made ahead of visitors | A build process generates static HTML before requests arrive. |
| Incremental static regeneration (ISR) | A saved page can be refreshed later | A framework-specific approach that regenerates cached static output after a time or invalidation trigger. |
| Hydration | The browser connects code to existing page content | A client framework attaches its behavior to HTML that was rendered earlier, commonly on the server. |
| Cache | A saved result that can be reused | A stored response or data result returned again under a freshness and invalidation policy. |
| CDN | A group of servers that can serve content nearer to visitors | A content delivery network that distributes content from geographically distributed servers. |
| Largest Contentful Paint (LCP) | When the biggest visible piece shows up | A Core Web Vitals metric measuring when the largest content element in the viewport is rendered. |

## Core Ideas

| # | Idea | Level | In one line | Taught in | Tested by |
| --- | --- | --- | --- | --- | --- |
| 1 | Where rendering happens | basic | A strategy says where and when the page HTML is produced. | [deep-dive.md](deep-dive.md), Level 1 — Intuition | Q1, E1 |
| 2 | CSR | basic | The browser creates the page after its code and data are ready. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | W1, S1, Q2 |
| 3 | SSR | basic | The server creates HTML for a request, using the data available to it. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | W3, Q3 |
| 4 | SSG | basic | A build creates HTML ahead of requests, so it can be a snapshot. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | W2, Q4 |
| 5 | ISR | intermediate | A supported framework can refresh generated output while reusing cached pages. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | S2, Q5 |
| 6 | Hydration | intermediate | Client code can connect behavior to existing server-generated HTML. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | W4, S3, Q6 |
| 7 | First display vs. interactivity | intermediate | Visible HTML and working controls can arrive at different times. | [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works | S3, Q7 |
| 8 | Choose by page needs | intermediate | Freshness and personalization guide the rendering choice. | [deep-dive.md](deep-dive.md), Level 3 — The Formal Picture | B1, Q8 |
| 9 | Rendering vs. caching | intermediate | Rendering chooses how HTML is made; caching chooses when a result is reused. | [deep-dive.md](deep-dive.md), Level 3 — The Formal Picture | S4, Q9 |
| 10 | Hybrid rendering | advanced | Different routes or regions can use different rendering paths. | [deep-dive.md](deep-dive.md), Level 3 — The Formal Picture | B1, Q10, E2 |

## Know This Cold

### Rules

1. CSR: the browser creates the page after JavaScript and data are ready.
2. SSR: the server creates HTML for a request; it costs request-time work.
3. SSG: a build creates a snapshot; new requests do not make that snapshot current.
4. ISR: a framework can refresh generated output; stale and failure behavior is framework-specific.
5. HTML can be visible before its client behavior is ready; hydration is a separate step.
6. Rendering and caching are separate decisions, and one page can mix strategies.
7. Choose by freshness and personalization needs, then measure the actual page.

**Canonical model** — copied from [deep-dive.md](deep-dive.md), Level 2 — How It Actually Works:

```js
const render = (stock) => `<li>Trail shoes: ${stock} left</li>`;
const currentStock = 11;
const cachedHtml = render(12);
console.log("CSR shell:", '<div id="app"></div>');
console.log("CSR after fetch:", render(currentStock));
console.log("SSR request:", render(currentStock));
console.log("SSG build:", cachedHtml);
console.log("SSG later:", cachedHtml);
console.log("ISR stale:", cachedHtml);
console.log("ISR refreshed:", render(currentStock));
```

```text
CSR shell: <div id="app"></div>
CSR after fetch: <li>Trail shoes: 11 left</li>
SSR request: <li>Trail shoes: 11 left</li>
SSG build: <li>Trail shoes: 12 left</li>
SSG later: <li>Trail shoes: 12 left</li>
ISR stale: <li>Trail shoes: 12 left</li>
ISR refreshed: <li>Trail shoes: 11 left</li>
```

This is a Node model of the data distinction, not a browser or framework execution.

## Costs & Limits

| What it costs | When it bites |
| --- | --- |
| JavaScript transfer, execution, and a data wait | CSR-heavy first views on a slow connection or low-end device |
| Per-request render and data work | SSR routes with expensive data or high traffic |
| Staleness and refresh/invalidation work | SSG/ISR pages with prices or stock that must be current |
| Client code connecting to existing HTML | Server content appears before hydration finishes |

Illustrative arithmetic: 10,000 visits/day with one uncached CSR data request each means `10,000 × 1 = 10,000` extra requests/day, or `10,000 ÷ 86,400 ≈ 0.116` requests/second averaged across the day. A 180 KB compressed JavaScript bundle over a sustained 1.5 Mbps connection takes `180 KB × 8 ÷ 1,500 kb/s = 0.96 s` to transfer alone, before overhead and execution. Both inputs are assumptions, not measurements of your app.

Limits: a strategy name does not guarantee a good LCP or search visibility. ISR support and cache behavior depend on the framework and deployment; Next.js documents that ISR is not supported with static export.

## Connections

- **Prerequisites:** HTTP request/response basics, browser rendering pipeline / CRP, React rendering and hydration, and HTTP caching — no notes yet; see the short primers in [deep-dive.md](deep-dive.md), Compared With What You Know.
- **Confusables:** The browser pipeline is about turning delivered resources into pixels; rendering strategy is about producing the initial HTML. HTTP caching decides reuse and freshness; rendering strategy decides where and when HTML is made. Hydration connects client behavior to existing HTML; it does not produce that initial HTML. A single-page app describes navigation; it does not require CSR.
- **Unlocks:** Week 10 — design a product listing page at scale; Week 11 — design an image-heavy feed; Week 14 — Core Web Vitals and caching strategies; Week 15 — bundle analysis and a real perf pass.

## Mastery Checklist

- [ ] You can explain all four paths without notes, starting from the shop analogy.
- [ ] You can predict whether the sample page shows 12 or 11 and say why.
- [ ] You can choose a candidate strategy for a public, changing, or personal region without notes.
- [ ] You can distinguish rendering, caching, hydration, and SPA navigation without notes.
- [ ] You can name two costs and the condition that makes each matter.
- [ ] You can explain why a page might combine server HTML with a client-only region without notes.
- [ ] You can explain it to someone else in two minutes, starting from the analogy.

## 2-Minute Revision

1. Rendering asks where and when initial HTML is made.
2. CSR: browser; SSR: per request; SSG: build time; ISR: refresh generated output.
3. SSG is a snapshot until rebuilt or refreshed.
4. ISR’s stale and refresh rules belong to the framework.
5. Server HTML can be visible before client behavior is ready.
6. Hydration connects client behavior to existing HTML.
7. Rendering is not caching; either can be combined with the other.
8. Choose by data freshness and personalization, per route or region.
9. In the model: build/SSG stays at 12; current request/CSR can show 11.
10. Top traps: “SSR is always faster” and “SPA means CSR.”
