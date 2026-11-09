# Rendering Strategies — Practice

> **Track:** SD · **Type:** concept · **Scheduled:** Week 5 · **Runtime:** Node v22.14.0 · **Generated:** 2026-11-05

## How to Use This File

Use this after reading the Deep Dive; every answer is collapsed. Do the warm-ups and self-checks from memory, then answer the explain-back prompts out loud. The build exercise connects this topic to Week 5’s `infinite scroll + intersection observer`. Log your own attempt in your notes, in your own words.

## Warm-Up

### W1 — Two moments

```js
const shell = '<div id="app"></div>';
const filled = "<ul><li>Trail shoes</li></ul>";
console.log(shell);
console.log(filled);
```

<details>
<summary>Output and why</summary>

```text
<div id="app"></div>
<ul><li>Trail shoes</li></ul>
```

The first value is an empty document shell; the second is content after the browser-side code has produced it. The program models that boundary; it does not run a browser.

</details>

### W2 — One saved page

```js
const stockAtBuild = 12;
const html = `<p>${stockAtBuild} left</p>`;
console.log(html);
console.log(html);
```

<details>
<summary>Output and why</summary>

```text
<p>12 left</p>
<p>12 left</p>
```

Both visits reuse the same build-time string. A new request alone does not make a saved snapshot current.

</details>

### W3 — Two requests

```js
const renderRequest = (stock) => `<p>${stock} left</p>`;
console.log(renderRequest(12));
console.log(renderRequest(11));
```

<details>
<summary>Output and why</summary>

```text
<p>12 left</p>
<p>11 left</p>
```

The model calls the renderer with the value for each request. Real SSR also has data-fetching, network, and server costs.

</details>

### W4 — A match check

```js
const serverMarkup = "<button>Buy</button>";
const clientMarkup = "<button>Buy</button>";
console.log(serverMarkup === clientMarkup);
```

<details>
<summary>Output and why</summary>

```text
true
```

Matching initial output is a React hydration requirement. This string comparison is only a tiny model; it does not perform hydration.

</details>

## Spot It

### S1 — The blank first view

A public shoe category initially shows an empty content area. A moment later, its JavaScript request completes and the product list appears. Name the rendering path most directly implicated and one reason the first view can be delayed.

<details>
<summary>Answer and why</summary>

CSR is the initial path most directly implicated: the browser needs its JavaScript and data before it can build this list. The delay can include bundle transfer and execution plus the data request. A hybrid page could still provide server HTML for the stable parts.

</details>

### S2 — One old response, then a new one

The first visitor after a freshness window sees the prior category HTML. Regeneration starts in the background; a later visitor sees updated stock. Name the strategy and the behavior that makes this possible.

<details>
<summary>Answer and why</summary>

This describes time-based ISR as documented by Next.js: a stale cached page can be returned while regeneration runs, and a later request can receive the regenerated page. It is not a universal rule for every framework or deployment.

</details>

### S3 — Visible, not yet usable

The “Buy” button is visible in the first HTML response, but clicking does nothing until the client app starts. Which neighboring concept explains the missing behavior, and why is it not the initial rendering strategy?

<details>
<summary>Answer and why</summary>

Hydration explains the missing client behavior: it connects client code to existing server HTML. It is a follow-up step, not the name of the path that generated the initial HTML.

</details>

### S4 — A header change

A team changes a CDN freshness header and asks whether that means its pages are now SSG. What is the category error?

<details>
<summary>Answer and why</summary>

They changed an HTTP caching policy, not necessarily where or when HTML is generated. A cache can reuse SSR output too; the page-generation path must be determined separately.

</details>

### S5 — From markup to pixels

A teammate asks why a stylesheet delays the visible product grid after the HTML has already arrived. Which adjacent topic owns that question, rather than this one?

<details>
<summary>Answer and why</summary>

The browser rendering pipeline / critical rendering path owns how received markup and styles become a displayed page. Rendering strategy is about where and when the HTML is produced.

</details>

### S6 — Navigation label

A project uses client-side route transitions and calls itself a single-page app. A reviewer concludes that its first page must be CSR. What is missing from that conclusion?

<details>
<summary>Answer and why</summary>

Navigation style does not determine the initial HTML source. A single-page app can start from an SSR or SSG response and then navigate on the client.

</details>

## Build It

### B1 — Choose the first page path

*Scheduled: Week 5 · `infinite scroll + intersection observer` · Mode: from memory*

**Task.** Before implementing the scheduled infinite-scroll behavior, write a pure JavaScript helper that chooses an initial page strategy for four catalog cases: public content that can wait for a build, public inventory that may be minutes stale, a price that must be read per request, and member picks that are personal. Then explain why loading later pages with an observer is a separate client interaction.

<details>
<summary>Hint 1 — where to look</summary>

For each region, write down who the data is for and how fresh it must be before choosing how its first HTML arrives.

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Use request-time work only where the data needs request context or that freshness; a public region may tolerate generated output.

</details>

<details>
<summary>Solution</summary>

**Approach.** Keep this a small decision aid, not a universal framework selector. The helper prioritizes personal data, then per-request freshness, then a short stale window, and otherwise uses a build snapshot. The observer can fetch later pages after the initial route is shown, regardless of how that route’s first HTML was produced.

```js
function chooseCatalogStrategy({ personalized, freshness }) {
  if (personalized) return "SSR shell + client island";
  if (freshness === "per-request") return "SSR";
  if (freshness === "minutes") return "ISR";
  return "SSG";
}
const cases = [
  { name: "public category", personalized: false, freshness: "build" },
  { name: "changing inventory", personalized: false, freshness: "minutes" },
  { name: "live price", personalized: false, freshness: "per-request" },
  { name: "member picks", personalized: true, freshness: "per-request" },
];
for (const item of cases) {
  console.log(`${item.name}: ${chooseCatalogStrategy(item)}`);
}
```

```text
public category: SSG
changing inventory: ISR
live price: SSR
member picks: SSR shell + client island
```

**Edge cases handled.** Personal data takes precedence; per-request freshness selects SSR; public data with a short stale window selects ISR; otherwise the helper chooses a build snapshot. These are the helper’s stated rules, not universal rules for production systems.

**Uses ideas:** #8, #10

</details>

## Self-Check Questions

### Q1 — One decision

What two questions does a rendering strategy answer?

<details>
<summary>Answer</summary>

It answers where the page HTML is produced and when that work happens. Those choices affect data freshness, user-visible waits, and which systems do the work.

</details>

### Q2 — An empty root

Why can a CSR page show little content until JavaScript and data are ready?

<details>
<summary>Answer</summary>

The initial response may contain only a root element or shell. The browser must load and run the code that creates the UI, and often must wait for a data request too. That can add work before content appears.

</details>

### Q3 — Current data

What condition makes SSR a better candidate than a saved build snapshot for the initial HTML?

<details>
<summary>Answer</summary>

Choose SSR when the initial response must use request-specific context or data that needs to be current for that request. It costs per-request server work and still depends on the data path being healthy.

</details>

### Q4 — A build change

A product’s inventory changes after static HTML was generated. Why does the next request not automatically show the new number?

<details>
<summary>Answer</summary>

The generated HTML is a snapshot of the data used at build time. It must be rebuilt or refreshed, or a separately updated region must supply the live value. A request by itself does not mutate that snapshot.

</details>

### Q5 — Stale while refreshing

In the documented Next.js time-based ISR flow, what may the first request after the interval receive, and what happens next?

<details>
<summary>Answer</summary>

It may receive the stale cached page while regeneration starts in the background. After a successful regeneration, later requests can receive the new cached page. Treat this as Next.js behavior, not a universal contract for every framework.

</details>

### Q6 — Hydration mismatch

Why should the client’s initial React output match the server HTML during hydration?

<details>
<summary>Answer</summary>

React expects the initial rendered content to match the existing HTML. A mismatch can cause warnings and recovery work, and React does not guarantee every mismatch will be patched. Render the same initial result, then update when client-only information is ready.

</details>

### Q7 — Two milestones

Give one reason that visible server HTML does not prove a control is ready to use.

<details>
<summary>Answer</summary>

The HTML can arrive before client JavaScript has loaded and connected behavior. The user can see the button while its event logic is not ready; content visibility and interactivity must be considered separately.

</details>

### Q8 — Choose by constraints

A public category page can be a minute stale, while its member-only recommendations cannot be shared. What is a reasonable starting design?

<details>
<summary>Answer</summary>

Use generated or incrementally refreshed output for the public category if its stale window is acceptable, and isolate member data behind a private client or request-time path. The exact split depends on where authorization is enforced and what the framework supports.

</details>

### Q9 — Cache versus render

Why does adding a CDN cache not make SSR become SSG?

<details>
<summary>Answer</summary>

Rendering describes when and where HTML is produced; caching describes whether an existing response is reused and for how long. A CDN can cache server-generated output, so the two decisions can be combined.

</details>

### Q10 — Mix strategies

Why might one product page use more than one rendering path?

<details>
<summary>Answer</summary>

Its public shell, live price, and personal recommendations can have different freshness, privacy, and interaction needs. Choosing separately by region can avoid making the whole page pay for the strictest requirement, while keeping data boundaries explicit.

</details>

## Explain It Back

### E1 — Teach the four paths

Explain CSR, SSR, SSG, and ISR to a new teammate in two minutes, starting from the shop analogy.

<details>
<summary>Rubric</summary>

- Maps the analogy to where and when HTML is made.
- Distinguishes browser work, per-request server work, and build-time output.
- Describes ISR as framework-specific refresh of generated output.
- Separates visible HTML from client interactivity.
- Names one freshness or compute trade-off rather than claiming a universal winner.

</details>

### E2 — Defend a hybrid

An interviewer says, “Pick one strategy for the whole storefront.” Explain why you would first ask about public content, freshness, and personalization, then make a route or region-level choice.

<details>
<summary>Rubric</summary>

- Asks how current the initial data must be.
- Separates shared public data from private user data.
- Gives a plausible generated, request-time, or client-side path for each region.
- Mentions caching and invalidation as separate decisions.
- Names the cost or failure path of the chosen design.

</details>
