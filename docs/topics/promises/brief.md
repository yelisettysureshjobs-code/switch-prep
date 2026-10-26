# Promises — Brief

> **Track:** JS · **Type:** concept · **Scheduled:** Week 3 · Wed · **Runtime:** Node v24 · **Generated:** 2026-10-24

## Explain It Like I'm 5

You order a pizza, and the shop hands you a little paper slip. The slip is not pizza — you
cannot eat it — but it stands for the pizza that is coming. And the slip is clever: you can
tell it what to do next. "When the pizza comes, slice it. Then share it. If the oven breaks
and no pizza comes, make sandwiches instead. Either way, clean the table after." Then you go
set the table — you do not stand at the door waiting. When the pizza arrives, your
instructions run, one after the other, in order. The pizza only ever arrives once; the slip
never changes its mind afterwards.

In real terms: a promise is an object that stands for one future value, and `.then`, `.catch`
and `.finally` are the instructions you attach to it.

## What It Is & Where It Shows Up

A promise is an object representing a single future result: it starts pending and settles
exactly once, either fulfilled with a value or rejected with a reason.

It solves the problem that an async result cannot be returned — before promises, every next
step had to nest inside a callback, and errors thrown "later" landed in nobody's `try/catch`.
Someone who does not hold the model guesses at output order, forgets to `return` chains
(so callers get `undefined` and errors vanish), and serialises independent requests without
noticing.

It shows up every time you `fetch`, in every loading/error state, and under every `await` in
the codebase. Interviews ask it exactly as the plan's slot words it — "promises — chaining,
error propagation, `finally`, common footguns" and "async/await, sequential vs parallel" —
usually as predict-the-output snippets or a find-the-bug on a forgotten `return`.

## Glossary

| Term             | In plain words                                            | Precise definition                                                                     |
| ---------------- | ---------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| promise          | The receipt — a thing you hold now for a result that comes later | An object representing one future result, with state and registered reactions           |
| pending          | Still waiting — no pizza yet                               | The initial state: not yet settled                                                      |
| fulfilled        | It arrived, and the receipt holds it                       | Settled with a value                                                                    |
| rejected         | It is not coming, and the receipt says why                 | Settled with a reason (conventionally an `Error`)                                       |
| settle           | The one moment the waiting ends, either way                | The single, irreversible move from pending to fulfilled or rejected                     |
| executor         | The kitchen — it starts cooking the moment you order       | The function passed to `new Promise(fn)`; runs synchronously, receives resolve/reject   |
| reaction         | An instruction left on the receipt                         | A callback registered via `.then`/`.catch`/`.finally`, queued as a microtask on settle  |
| microtask        | A follow-up that cuts the line as soon as you're free      | See the [Event Loop brief](../eventLoop/brief.md) → Glossary — that doc owns this term  |
| chain            | Instructions in a row, each feeding the next               | The pipeline of new promises returned by successive `.then`/`.catch`/`.finally` calls   |
| adopt            | "My result is whatever that other receipt says"            | A promise settling to match a promise (or thenable) returned by a callback; flattening  |
| thenable         | Anything receipt-shaped enough to be treated as one        | Any object with a `.then` method; adoption accepts these for interoperability           |
| floating promise | A receipt dropped on the floor — its bad news reaches no one | A promise neither returned, awaited, nor given a rejection handler                      |
| async function   | A function that always answers with a receipt              | A function declared `async`; it always returns a promise wired to its return/throw      |
| await            | "Pause *me* here until that receipt settles"               | Suspends only the enclosing async function; unwraps the value or throws the reason      |

## Core Ideas

| #  | Idea                              | Level        | In one line                                                                    | Taught in                                                | Tested by          |
| -- | --------------------------------- | ------------ | ------------------------------------------------------------------------------ | --------------------------------------------------------- | ------------------ |
| 1  | One-shot state machine            | basic        | Pending → fulfilled \| rejected; settles exactly once, then immutable           | [deep-dive](deep-dive.md) → Level 2 — How It Actually Works | W1, S3, Q1, B5     |
| 2  | The executor runs now             | basic        | `new Promise(fn)` runs `fn` synchronously; only reactions are deferred          | [deep-dive](deep-dive.md) → Level 2 — How It Actually Works | Q2, B1, B4         |
| 3  | `.then` returns a new promise     | basic        | A chain is a pipeline of transformations, not subscriptions                     | [deep-dive](deep-dive.md) → Level 2 — How It Actually Works | W2, Q3, B1         |
| 4  | The callback decides the next link| basic        | Return → fulfil; throw → reject; return a promise → adopt (flattening)          | [deep-dive](deep-dive.md) → Level 2 — How It Actually Works | W2, W5, Q4, B3, B5 |
| 5  | Rejections find the nearest handler| intermediate| A rejection skips every handler-less link and lands on the first `.catch`       | [deep-dive](deep-dive.md) → Level 2 — The error path       | W3, S1, Q5, B3     |
| 6  | `.catch` is recovery              | intermediate | A handler that returns normally puts the chain back on the success lane         | [deep-dive](deep-dive.md) → Level 2 — The error path       | W3, Q5, B3         |
| 7  | Two arguments are siblings        | intermediate | `.then(ok, bad)` ≠ `.then(ok).catch(bad)` — only the second catches `ok`'s throw | [deep-dive](deep-dive.md) → Level 3 — The Formal Picture   | W5, Q6             |
| 8  | `finally` observes, never transforms | intermediate | No argument in, return value ignored; only its own throw changes the outcome  | [deep-dive](deep-dive.md) → Level 3 — The Formal Picture   | W4, S1, Q7         |
| 9  | A promise not held is errors lost | intermediate | Broken chains and floating promises let rejections escape everything            | [deep-dive](deep-dive.md) → Misconceptions & Traps         | S2, S6, Q8, B4     |
| 10 | `await` unwraps, in one function  | intermediate | Pauses only its async function; value comes out, rejection throws               | [deep-dive](deep-dive.md) → Level 3 — async/await          | W6, Q9, B2         |
| 11 | Parallelism is decided at creation| advanced     | Work starts when the promise is made; await placement only decides the waiting  | [deep-dive](deep-dive.md) → Level 3 — async/await          | S5, Q10, B2        |

## Know This Cold

The rules:

1. A promise settles **once**: pending → fulfilled or rejected; every later settle is ignored.
2. The executor runs **synchronously**; a reaction **never** runs synchronously — even on an
   already-settled promise.
3. Every `.then`/`.catch`/`.finally` returns a **new** promise — a chain is a pipeline.
4. The callback decides the next link: **return** → fulfil, **throw** → reject, **return a
   promise** → adopt it.
5. A rejection **skips** to the nearest rejection handler; a handler that returns normally
   **recovers** the chain. `.catch(f)` is `.then(undefined, f)`.
6. `finally`: no argument, return value ignored — pass-through cleanup; only its own throw
   replaces the result.
7. Work starts at **creation**: create first and await after (or `Promise.all`) for parallel;
   `await` between creations only when the next step needs the previous result.

The canonical snippet (copied from [deep-dive.md](deep-dive.md) → Level 2, where its trace
lives):

```js
const order = Promise.resolve("pizza");

order
  .then(function slice(food) {
    console.log("slicing the", food);
    return food + " slices";
  })
  .then(function share(slices) {
    console.log("sharing the", slices);
    return slices + " for two";
  })
  .catch(function fallback() {
    console.log("making sandwiches instead");
    return "sandwiches";
  })
  .finally(function cleanUp() {
    console.log("cleaning the table");
  })
  .then(function eat(meal) {
    console.log("eating:", meal);
  });

console.log("setting the table");
```

```text
setting the table
slicing the pizza
sharing the pizza slices
cleaning the table
eating: pizza slices for two
```

And its mirror: start it with `Promise.reject(new Error("oven broke"))` instead, and it prints
`setting the table · making sandwiches instead · cleaning the table · eating: sandwiches` —
the skip, the recovery and the pass-through in one run (real output; the full version and its
trace are in the deep dive).

## Costs & Limits

| What it costs                                  | When it bites                                                                 |
| ----------------------------------------------- | ------------------------------------------------------------------------------ |
| Sequential awaits add latencies: two 300 ms waits measured ~600 ms vs ~300 ms parallel (run in the deep dive) | Independent requests written as stacked `await`s — the most common real-world cost |
| One promise object + one microtask hop per link (counted: one per `.then`, by rule 3)        | Only in hot loops creating thousands of chains per second — never per user action |
| An unhandled rejection crashes Node (observed on Node v24)                                   | Any floating promise the moment its work fails — see idea 9                      |
| `Promise.all` is fail-fast: one rejection discards every other result                        | Batch work where partial success matters (`allSettled` is Week 5's answer)       |

Hard limits: one value then done (streams want events); no cancellation from outside
(`AbortController` pairs with it — Week 4's build); pending carries no progress and no timeout
(you build one — practice B5).

## Connections

- **Prerequisites:**
  [Event Loop — Call Stack, Task Queue & Microtask Queue](../eventLoop/brief.md) — reactions
  are microtasks; that doc owns the ordering story.
  [Lexical Scope, Scope Chain & Closures](../../../JS-docs/lexicalScopeScopeChainClosures.md)
  — every reaction is a closure over the scope it was written in.
- **Confusables:**
  `setTimeout(fn, 0)` — the signal: do you need a value and an error lane back, or just
  scheduling? Timer is scheduling only.
  Events / `EventEmitter` — the signal: can it happen again? Many occurrences → event; one
  result → promise.
- **Unlocks:**
  "autocomplete — debounce, cancellation, keyboard nav" — Week 4 · Tue.
  "polyfills — `Promise.all`, `race`, `allSettled`, `any`" — Week 5 · Mon.
  "design an autocomplete" — Week 9.

## Mastery Checklist

- [ ] You can predict the output of an unseen snippet mixing chains, `catch`/`finally` and
  `await` — with the why for every line — without notes.
- [ ] You can state the three outcomes of a `.then` callback and what each does to the next
  promise, without notes.
- [ ] You can spot a broken chain or floating promise in a code review and say what the caller
  will observe, without notes.
- [ ] You can say what `.then(ok, bad)` misses that `.then(ok).catch(bad)` catches, without
  notes.
- [ ] You can rewrite stacked awaits as parallel creation — and say when that rewrite would be
  wrong — without notes.
- [ ] You can write `delay`, `retry` and `withTimeout` from a blank file, from memory.
- [ ] You can explain promises to someone else in two minutes, starting from the pizza
  receipt.

## 2-Minute Revision

1. One receipt, one result: pending → fulfilled \| rejected, settles once.
2. Executor runs now; reactions never run synchronously.
3. Every `.then` returns a new promise — a chain is a pipeline.
4. Callback outcome: return → fulfil · throw → reject · promise → adopt.
5. Rejection skips to the nearest handler; a returning `.catch` recovers the chain.
6. `finally`: nothing in, return ignored, runs on both lanes.
7. `await` unwraps and throws; it pauses only its own function.
8. Work starts at creation — parallel means create first, await after.
9. The snippet: `setting the table · slicing · sharing · cleaning · eating` (Know This Cold).
10. Trap: a promise you don't return, await or catch takes its errors down with it.
11. Trap: `.then(ok, bad)` — `bad` cannot catch what `ok` throws.
