# Hydration, Streaming SSR & Islands — Practice

> **Track:** SD · **Type:** concept · **Scheduled:** Week 5 · **Runtime:** Node v22.14.0 · **Generated:** 2026-11-06

## How to Use This File

Use your design-question study method: read, answer from memory, then compare.
Every answer is collapsed; open only the smallest hint or answer you need.
Use the product-listing scenario from the deep dive throughout.
Write any attempt log in your own notes, in your own words.

## Warm-Up

### W1 — Two first views

```js
const server = ["h1: Sneakers", "button: Add"];
const client = ["h1: Sneakers", "button: Add"];
console.log(server.every((node, index) => node === client[index]));
```

<details>
<summary>Output and why</summary>

```text
true
```

The toy arrays match at both positions. This models initial-output agreement only; it does not run React or inspect a DOM.

</details>

### W2 — One changed value

```js
const server = "price: 499";
const client = "price: 599";
console.log(server === client);
```

<details>
<summary>Output and why</summary>

```text
false
```

The initial strings differ. A real React hydration mismatch is a browser/framework behavior, but the shared-data principle is visible here.

</details>

### W3 — A first section and later work

```js
console.log("shell: product title and skeleton");
Promise.resolve("products: 24 items").then((value) => console.log(value));
console.log("server: response stream continues");
```

<details>
<summary>Output and why</summary>

```text
shell: product title and skeleton
server: response stream continues
products: 24 items
```

The promise callback runs after the current synchronous script. This is not an HTTP stream; it is a small ordering model for “first shell, later work.”

</details>

### W4 — A cached listing

```js
const cached = { price: 499, state: "stale" };
console.log(cached.state, cached.price);
cached.price = 459;
cached.state = "fresh";
console.log(cached.state, cached.price);
```

<details>
<summary>Output and why</summary>

```text
stale 499
fresh 459
```

This toy mutation illustrates stale and refreshed values. A real framework decides when cache regeneration or invalidation happens.

</details>

## Spot It

### S1 — The page looks ready

A category page displays product names, but its filter does nothing until a large script finishes loading. Name the two distinct states involved.

<details>
<summary>Answer</summary>

HTML is visible, but the filter's client behavior is not ready. Server rendering and client interactivity are separate milestones. *(Ideas 2, 4)*

</details>

### S2 — A changed badge

The server renders “Price: 499”; the first client render reads a newer value and immediately shows “Price: 459.” What can go wrong, and what should the initial render use?

<details>
<summary>Answer</summary>

The initial client output does not match the existing HTML, so hydration can report a mismatch or recover imperfectly. Use the same initial data snapshot on both sides, then update after hydration or use a stable placeholder. *(Idea 3)*

</details>

### S3 — Reviews arrive later

The title and first products appear while the reviews section still shows a fallback. The filter remains inert. Is this evidence that streaming failed?

<details>
<summary>Answer</summary>

No. The early content is consistent with streaming; the inert filter is about client JavaScript and hydration. Delivery timing and interactivity scope are different axes. *(Ideas 5, 6)*

</details>

### S4 — A page refreshed after the sale

A visitor receives a cached page after its refresh interval; the system regenerates it in the background. Another engineer calls this “streaming.” What is the better distinction?

<details>
<summary>Answer</summary>

This describes a revalidation/cache policy such as framework-specific ISR. Streaming describes sending portions of one response over time. One does not imply the other. *(Idea 9)*

</details>

### S5 — A very quiet page

A page has mostly product copy and native links. A proposal hydrates the whole page as one large app “because SSR is always faster.” What evidence or distinction is missing?

<details>
<summary>Answer</summary>

SSR may show content earlier but does not guarantee faster interactivity; the client bundle can still cost transfer, parse, execution, and hydration time. Count what truly needs browser behavior and compare measured route/device performance. This is a decoy for a blanket “SSR always wins” answer. *(Ideas 4, 8, 10)*

</details>

### S6 — “It runs on the server”

A teammate says React Server Components and hydration are the same because both involve the server. Give the shortest correction.

<details>
<summary>Answer</summary>

Server Components describe where component code executes and what is sent to the client; hydration connects client behavior to existing HTML. They are distinct concepts, and interactive client components can still need browser JavaScript. *(Ideas 2, 8)*

</details>

## Build It

### B1 — Week 5: rendering strategies CSR/SSR/SSG/ISR

*Scheduled: Week 5 · Mode: from memory*

**Task.** Implement a tiny checker for whether server and first-client markup agree. Use the same trail-shoe title on both sides, then change the client title to include “- 20% off.” State what this toy does not prove about React.

<details>
<summary>Hint 1 — where to look</summary>

Compare the two initial descriptions before any later update.

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Hydration needs the first client output to agree with existing server HTML.

</details>

<details>
<summary>Solution</summary>

**Approach.** Compare the strings as a deliberately small model of the agreement requirement. It does not implement React, the DOM, hydration, or mismatch recovery.

```js
function sameInitialMarkup(serverMarkup, clientMarkup) {
  return serverMarkup === clientMarkup;
}
const server = "<h1>Trail shoes</h1>";
console.log(sameInitialMarkup(server, "<h1>Trail shoes</h1>"));
console.log(sameInitialMarkup(server, "<h1>Trail shoes - 20% off</h1>"));
```

```text
true
false
```

Observed with Node v22.14.0.

**Edge cases handled.** Identical output; changed initial text. This toy does not handle attribute comparison or React recovery.

**Uses ideas:** #2, #3

</details>

### B2 — Week 10: design a product listing page at scale

*Scheduled: Week 10 · Mode: out loud, timed*

**Task.** For a category page with 120 products and 24 shown at a time, map the catalog, filters, reviews, and bag count to rendering responsibilities. State which data must be fresh and why this is a route plan rather than a universal prescription.

<details>
<summary>Hint 1 — where to look</summary>

For each region, ask when its data is ready, who needs to interact with it, and how stale it may be.

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Rendering location, stream timing, interactive scope, and freshness are separate decisions.

</details>

<details>
<summary>Solution</summary>

**Approach.** A possible illustrative route plan keeps stable catalog content cacheable, gives filters client behavior, streams reviews only if they are independently slow, and treats a personalized bag count as dynamic. Validate current price and stock at purchase boundaries; do not infer a company's production stack from this exercise.

```js
const regions = [
  ["catalog", "cached HTML"],
  ["filters", "client island"],
  ["reviews", "streamed section"],
  ["bag count", "personalized response"],
];
for (const [region, strategy] of regions) {
  console.log(region + ": " + strategy);
}
```

```text
catalog: cached HTML
filters: client island
reviews: streamed section
bag count: personalized response
```

Observed with Node v22.14.0. This program prints a planning sketch; it is not framework configuration.

**Edge cases handled.** Stale price/stock is not treated as authoritative at purchase; reviews may be slow; only selected regions need client behavior.

**Uses ideas:** #5, #7, #8, #9, #10

</details>

## Self-Check Questions

### Q1 — Where the first view comes from

Name the main distinction among CSR, SSR, and SSG.

<details>
<summary>Answer</summary>

CSR creates the view in the browser; SSR creates HTML in response to a request; SSG creates HTML ahead of requests. The distinction is about where and when the initial output is produced. *(Idea 1)*

</details>

### Q2 — A page that looks ready

Why can server-rendered HTML be visible while a button is still inert?

<details>
<summary>Answer</summary>

HTML can be parsed and painted before the client bundle has loaded and connected the button's behavior. Rendering HTML is not the same work as making client controls interactive. *(Idea 2)*

</details>

### Q3 — Two different prices

Why should a server render and the first client render use the same data snapshot?

<details>
<summary>Answer</summary>

Hydration expects the client tree's initial output to match the server HTML. Different prices can cause a mismatch; use the same initial snapshot and apply subsequent updates separately. *(Idea 3)*

</details>

### Q4 — A large client bundle

What work remains after SSR if the page still hydrates a large interactive tree?

<details>
<summary>Answer</summary>

The client still transfers, parses, and executes JavaScript and hydrates the relevant tree. SSR can improve early content without eliminating client startup cost. *(Idea 4)*

</details>

### Q5 — A slow reviews panel

What can a Suspense boundary allow a React server stream to do?

<details>
<summary>Answer</summary>

React can send the shell and a fallback before the boundary's content is ready, then send the content later. This improves progressive delivery, not necessarily interactivity. *(Idea 5)*

</details>

### Q6 — What streaming does not promise

Why does receiving the first HTML chunk not prove the filter is usable?

<details>
<summary>Answer</summary>

The client JavaScript may not have loaded or hydrated the filter yet. Content arrival and event handling readiness are separate milestones. *(Idea 6)*

</details>

### Q7 — A static category page

What makes a region a candidate for an island, and what does not become free?

<details>
<summary>Answer</summary>

A region is a candidate when it needs client behavior while much of the page does not. Its code, data, startup work, and coordination still have a cost. *(Idea 7)*

</details>

### Q8 — Twenty-four quick-add buttons

Why might an islands approach save less work than expected in this case?

<details>
<summary>Answer</summary>

If every product card has a client-side quick-add control, many regions still need browser behavior. Islands reduce unnecessary interactivity only when the supposedly static regions really are static. *(Idea 8)*

</details>

### Q9 — A price changed after caching

Why is streaming not an answer to a stale price?

<details>
<summary>Answer</summary>

Streaming decides when portions of a response are delivered; it does not define cache invalidation or data freshness. Define revalidation and validate price/stock at the purchase boundary. *(Idea 9)*

</details>

### Q10 — A two-minute recommendation

What would a defensible rendering recommendation for a marketplace category page include?

<details>
<summary>Answer</summary>

State the route's content and freshness needs, separate visible HTML from interactivity, choose which regions need client code, use streaming only for independently slow content, and name what you would measure. Do not claim a named company uses that design without a source. *(Idea 10)*

</details>

## Explain It Back

### E1 — From the shop display

Explain the rendering choices to a new teammate in two minutes, starting from the analogy.

<details>
<summary>Rubric</summary>

- Distinguishes browser, request-time server, and build-time output.
- Separates visible HTML from interactive controls.
- Describes hydration's initial-output agreement requirement.
- Explains streaming as progressive delivery, not interactivity.
- Names one limit of the analogy.

</details>

### E2 — Streaming or islands?

An interviewer asks whether a slow review panel should use streaming SSR or islands. Answer by separating what each technique controls.

<details>
<summary>Rubric</summary>

- Streaming controls when HTML arrives.
- Islands control which regions need client behavior.
- They can address different needs and may be combined by a framework.
- Neither choice alone specifies cache freshness.

</details>

### E3 — Why a marketplace cares

Explain why rendering strategy matters for a Myntra/Meesho-like marketplace without claiming to know either company's implementation.

<details>
<summary>Rubric</summary>

- Early product visibility can matter to browsing.
- Filters and purchase controls need usable interactivity.
- Price and availability need an explicit freshness contract.
- Client JavaScript and server rendering both have costs.
- The example is illustrative, not a documented company stack claim.

</details>