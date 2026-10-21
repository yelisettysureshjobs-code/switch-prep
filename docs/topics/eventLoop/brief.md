# Event Loop — Call Stack, Task Queue & Microtask Queue — Brief

> **Track:** JS · **Type:** concept · **Scheduled:** Week 3 · Mon · **Runtime:** Node v22 · **Generated:** 2026-10-19

## Explain It Like I'm 5

There is one cook in a tiny kitchen. The cook makes one dish at a time and never stops in the middle — not even when the oven rings. The oven watches the clock so the cook doesn't have to; when it rings, nobody shouts — a new ticket just joins the line of orders. The cook also keeps sticky notes by the stove, and the kitchen has one strict rule: after every single dish, read **all** the sticky notes — even ones that got added while reading — before taking the next ticket. If one dish takes forever, everything waits: the tickets, the notes, even the waiter, who may only pick up plates between dishes.

In real terms: the event loop is the rule for which waiting callback JavaScript's single thread runs next — tasks wait in a queue, and promise microtasks always go first.

## What It Is & Where It Shows Up

The event loop is the cycle by which the host runs JavaScript: run the call stack until it is empty, drain the whole microtask queue, take one task from the task queue, repeat — with a chance to render in between.

It explains every "why did this print in that order?" and every frozen page. Without it you can only memorise output orders; with it you derive them — and you know why a timer fires late, why a spinner never appears, and why `await` doesn't make slow code fast. Someone who doesn't hold this model ships the bugs in Spot It: spinners hidden behind parses, pollers that overlap, "async" rewrites that still freeze.

It shows up in every debounced search box, every loading state, and every Node server handling thousands of users on one thread. Interviews ask it exactly the way the plan's slot does — "predict output of 6 tricky snippets" mixing `console.log`, `setTimeout` and promises — or as the open "explain the event loop", with "why does the page freeze and how do you fix it" as the follow-up.

## Glossary

| Term                 | In plain words                                                   | Precise definition                                                                              |
| -------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ |
| call stack           | The pile of half-finished functions; the top one is running      | The stack of execution contexts — see [Execution Context & Hoisting](../../../JS-docs/lexicalEnvExecContextHoisting.md) |
| blocking             | Keeping the cook busy so nothing else gets a turn                | Occupying the thread with synchronous work so no queued callback or render can run               |
| run to completion    | A started function always finishes before anything else runs     | No preemption: a running function is never interrupted by another JavaScript callback            |
| host                 | The kitchen around the cook — the browser, or Node               | The embedder that provides timers, I/O and the DOM, and files callbacks when they finish         |
| callback             | A function left behind to be called later                        | A function the host or loop invokes when its trigger (timer, event, I/O) fires                   |
| task                 | One ticket in the main line                                      | A unit of queued work (timer, click, I/O callback); the loop runs exactly one per turn           |
| task queue           | The line of tickets                                              | The queue of tasks awaiting the loop (also called the macrotask queue)                           |
| event loop           | The rule for what runs next                                      | The host's cycle: empty the stack, drain microtasks, take one task, maybe render, repeat         |
| promise              | A ticket for a value that arrives later                          | An object representing a future value; its registered callbacks are queued as microtasks        |
| microtask            | A sticky note — a small follow-up that cuts the line             | A callback in the microtask queue: promise reactions, `queueMicrotask`                           |
| microtask queue      | The sticky-note pile, fully cleared after every dish             | The queue drained to empty at each checkpoint, including entries added during the drain          |
| microtask checkpoint | The "read all notes now" moment                                  | The point after the script and after every task where the loop drains the microtask queue       |
| starvation           | The ticket line never moves because notes keep coming            | Tasks (and rendering) delayed indefinitely by self-replenishing microtasks                       |

## Core Ideas

| #  | Idea                        | Level        | In one line                                                                      | Taught in                                                | Tested by              |
| -- | --------------------------- | ------------ | -------------------------------------------------------------------------------- | -------------------------------------------------------- | ---------------------- |
| 1  | One stack, run to completion| basic        | JS runs one function at a time, and it finishes before anything else runs        | [deep-dive](deep-dive.md) → Level 1 — Intuition           | W1, S4, Q1, E3         |
| 2  | Blocking                    | basic        | While the stack is busy, nothing else runs — callbacks, clicks, paints all wait  | [deep-dive](deep-dive.md) → Why It Exists                 | S1, S4, Q4, B3         |
| 3  | Host hand-off               | basic        | Timers and I/O are counted down by the host; only the callback re-enters JS      | [deep-dive](deep-dive.md) → Level 1 — Intuition           | W1, B1, E2             |
| 4  | The loop rule               | basic        | A callback runs only when the stack is empty — `setTimeout(fn, 0)` is never now  | [deep-dive](deep-dive.md) → Level 2 — How It Actually Works | W1, W5, B3, Q2       |
| 5  | Delay is a minimum          | intermediate | The delay is the earliest possible moment, never the actual one                  | [deep-dive](deep-dive.md) → Level 2 — How It Actually Works | S5, B1, B2, E2       |
| 6  | A second queue              | intermediate | Promise callbacks and `queueMicrotask` go to the microtask queue                 | [deep-dive](deep-dive.md) → Level 2 — How It Actually Works | W2, B4, Q9           |
| 7  | The drain rule              | intermediate | After the script and every task, ALL microtasks run — even ones queued mid-drain | [deep-dive](deep-dive.md) → Level 3 — The Formal Picture  | W2, W5, S2, Q3, Q7, Q10 |
| 8  | `.then` is never synchronous| intermediate | Even a settled promise's callback waits for an empty stack                       | [deep-dive](deep-dive.md) → Level 3 — The Formal Picture  | W3, Q6, B4             |
| 9  | `await` = suspend + microtask | intermediate | Sync until the first `await`; the rest of the function resumes as a microtask  | [deep-dive](deep-dive.md) → Level 3 — The Formal Picture  | W4, W6, Q5             |
| 10 | Rendering fits between tasks| advanced     | The browser paints between tasks — long tasks and long drains both freeze it     | [deep-dive](deep-dive.md) → Level 3 — The Formal Picture  | S1, B3, Q4, Q9         |
| 11 | Environments differ         | advanced     | Node adds `nextTick`/`setImmediate`; the portable rule is sync → micro → tasks   | [deep-dive](deep-dive.md) → Variants & Environment Differences | S5, S3, Q8        |

## Know This Cold

The rules:

1. One call stack — the running function finishes before anything else runs.
2. Timers and I/O are counted down by the host; finished callbacks wait in the task queue.
3. A callback enters the stack only when the stack is empty.
4. A delay is the earliest moment, never the exact one — `0` means "as soon as you're free".
5. Promise callbacks and `queueMicrotask` go to the microtask queue, not the task queue.
6. After the script and after every task, the whole microtask queue drains — including entries queued during the drain.
7. An async function runs synchronously to its first `await`; everything after resumes as a microtask.

The canonical snippet (from [deep-dive.md](deep-dive.md) → Level 2):

```js
console.log("start");
setTimeout(function onTimer() {
  console.log("timer");
}, 0);
Promise.resolve().then(function onPromise() {
  console.log("promise");
});
console.log("end");
```

```text
start
end
promise
timer
```

## Costs & Limits

| What it costs                                  | When it bites                                                                                     |
| ---------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| A long task blocks input AND paint             | Any sync work past the `1000 ÷ 60 ≈ 16.7 ms` frame budget skips frames; seconds of it = frozen page |
| Microtask drains block exactly like sync code  | A self-queueing chain (Spot It S2) runs to empty — progress UI never paints in between              |
| Timers are imprecise by design                 | A 100 ms timer behind a 500 ms loop fired at ≥ 500 ms (shown in the deep dive); nested clamping to 4 ms; background-tab throttling |
| CPU-heavy work has no home on the thread       | Big parses and image filters need chunking (Practice B3), a Web Worker, or the server               |

Hard limits: there is no preemption — nothing can interrupt a running function; and ordering between task sources is not portable (`setImmediate` vs `setTimeout(fn, 0)` was observed in both orders across runs).

## Connections

**Prerequisites**

- [Execution Context, Lexical Environment & Hoisting](../../../JS-docs/lexicalEnvExecContextHoisting.md) — the call stack this whole topic stands on
- [Lexical Scope, Scope Chain & Closures](../../../JS-docs/lexicalScopeScopeChainClosures.md) — every queued callback keeps its scope; debounce lives on this
- Promise basics — 8-line primer in [deep-dive.md](deep-dive.md) → Level 2; the full promises topic (no doc yet)

**Confusables**

- `setTimeout(fn, 0)` vs `Promise.resolve().then(fn)` — the signal: does anything else (clicks, paint) get a turn first? Task yes, microtask no.
- `async`/`await` vs multithreading — the signal: CPU-heavy work still freezes the page under `async`; only a Worker adds a thread.
- `setInterval` vs recursive `setTimeout` — the signal: can runs overlap or pile up? Interval fires on the clock; recursive timeout waits out each run.

**Unlocks**

- Promises & async/await — Week 3 · Wed
- Polyfills: `debounce`, `throttle` — Week 4 · Wed (B1, B2 here)
- Autocomplete build — debounce, cancellation — Week 4 · Tue/Thu
- Polyfills II: `Promise.all`/`race`/`allSettled`/`any` — Week 5 · Mon
- Browser rendering pipeline, CRP — Week 6 (SD)

## Mastery Checklist

- [ ] You can predict the output of an unseen snippet mixing sync code, timers, promises and `await` — and give the why for every line — without notes (the slot's own bar: "predict output of 6 tricky snippets")
- [ ] You can state the loop's full cycle — empty the stack, drain all microtasks, one task, maybe render — in one breath
- [ ] You can write `debounce` from a blank file and say which queue its callback waits in
- [ ] You can explain a timer that fired late without calling it a bug, naming at least two delayers
- [ ] You can name what blocks paint and give two fixes (chunk and yield; move it off the thread)
- [ ] You can state the one portable ordering rule and one Node-only exception
- [ ] You can explain it to someone else in two minutes, starting from the kitchen analogy

## 2-Minute Revision

1. One thread, one stack; the running function always finishes first.
2. The host counts timers and does I/O; finished callbacks wait as tasks.
3. The cycle: stack empty → drain ALL microtasks → one task → maybe paint → repeat.
4. Promise and `queueMicrotask` callbacks are microtasks; timers, clicks, I/O are tasks.
5. A delay is a minimum — `0` means "as soon as you're free".
6. `.then` never runs synchronously, even on a settled promise.
7. An async function is sync to its first `await`; the rest is a microtask.
8. Microtasks queued mid-drain run in the same drain — chains can starve the page.
9. The snippet: `start end promise timer` (Know This Cold).
10. Trap: a late timer isn't a bug — something blocked the stack.
11. Trap: "async" CPU work still freezes — promises move *when*, not *where*.
12. Portable rule: sync → microtasks → tasks. Everything else is environment detail.
