# Event Loop — Call Stack, Task Queue & Microtask Queue — Practice

> **Track:** JS · **Type:** concept · **Scheduled:** Week 3 · Mon · **Runtime:** Node v22 · **Generated:** 2026-10-19

## How to Use This File

The plan's own wording for this slot is "predict output of 6 tricky snippets" — that is the Warm-Up, done on paper **before** running anything. Every answer here is collapsed; take the smallest hint you need and no more. Warm-Up and Self-Check are from memory; Build It items carry their mode on their italic line (the Week 4 Friday slot — "rewrite every Week-4 polyfill from memory, timed 45 min" — is the bar for B1–B2). Log results in `prep/notes.md` in your own words; method and budgets live in [how-to-study.md](../../../prep/how-to-study.md).

## Warm-Up

### W1 — Three prints

```js
console.log("A");
setTimeout(function logB() {
  console.log("B");
}, 0);
console.log("C");
```

<details>
<summary>Output and why</summary>

```text
A
C
B
```

`logB` is handed to the host and filed as a task; a task runs only when the call stack is empty, and the script is still on it when `C` prints (Ideas #3, #4).

</details>

### W2 — One timer, one promise

```js
setTimeout(function logA() {
  console.log("A");
}, 0);
Promise.resolve().then(function logB() {
  console.log("B");
});
console.log("C");
```

<details>
<summary>Output and why</summary>

```text
C
B
A
```

Scheduling order doesn't decide — queues do. `logB` is a microtask, `logA` a task; after the script ends the checkpoint drains microtasks before the loop takes any task (Ideas #4, #6, #7).

</details>

### W3 — A then on a ready value

```js
const done = Promise.resolve(1);
done.then(function logValue(value) {
  console.log("then", value);
});
console.log("after then call");
```

<details>
<summary>Output and why</summary>

```text
after then call
then 1
```

The value already exists, but a promise callback is never synchronous — it waits in the microtask queue until the stack empties (Idea #8).

</details>

### W4 — A greeting, interrupted

```js
async function greet() {
  console.log("hello");
  await null;
  console.log("goodbye");
}
greet();
console.log("between");
```

<details>
<summary>Output and why</summary>

```text
hello
between
goodbye
```

An async function runs synchronously up to its first `await` — `hello` prints in place. At the `await`, only `greet` suspends; the script continues, and the rest of `greet` resumes as a microtask (Idea #9).

</details>

### W5 — Nested on both sides

```js
setTimeout(function onTimer() {
  console.log("timer");
  Promise.resolve().then(function insideTimer() {
    console.log("promise in timer");
  });
}, 0);
Promise.resolve().then(function onPromise() {
  console.log("promise");
  setTimeout(function insidePromise() {
    console.log("timer in promise");
  }, 0);
});
console.log("sync");
```

<details>
<summary>Output and why</summary>

```text
sync
promise
timer
promise in timer
timer in promise
```

After the script: drain microtasks (`promise`, which files a new task). Take one task (`timer`) — and after **every** task comes another checkpoint, so `promise in timer` runs before the next task. The second timer, filed later, goes last (Ideas #4, #6, #7).

</details>

### W6 — Two async functions

```js
async function first() {
  console.log("first start");
  await null;
  console.log("first end");
}
async function second() {
  console.log("second start");
  await null;
  console.log("second end");
}
first();
second();
console.log("script end");
```

<details>
<summary>Output and why</summary>

```text
first start
second start
script end
first end
second end
```

Each call runs synchronously to its `await` and suspends. The two resumptions are microtasks, queued in call order, drained in order after the script ends (Ideas #7, #9).

</details>

## Spot It

Name what is going on within 60 seconds — no code.

### S1 — The invisible spinner

A teammate sets a loading spinner to visible, then parses a 40 MB JSON string synchronously, then hides the spinner. QA reports: "no spinner, the page just freezes and then it's done." Which idea explains the missing spinner?

<details>
<summary>Answer</summary>

Rendering happens **between tasks**, never mid-task (Idea #10). Show-spinner, the parse, and hide-spinner all run inside one task, so the browser's next chance to paint comes when the spinner is already hidden. The fix is to yield after showing the spinner (end the task — e.g. a timer) so a paint can happen before the parse, or chunk the parse (B3).

</details>

### S2 — The "optimization"

To make a batch job "snappier", someone replaced its `setTimeout(processNext, 0)` scheduling with `queueMicrotask(processNext)`. The batch does finish faster — but the progress bar no longer moves, and the tab can't be scrolled until it's done. What happened?

<details>
<summary>Answer</summary>

Each `processNext` now queues the next one as a microtask, and the checkpoint drains microtasks **to empty, including ones added during the drain** (Idea #7). The whole batch became one unbroken drain — tasks (clicks, scroll) and paints never got a turn. The task-queue version yielded between items; that "slowness" was the page breathing.

</details>

### S3 — The overlapping poller

A dashboard polls the server with `setInterval(fetchStatus, 1000)`. Under a slow network, two or three requests end up in flight at once and responses arrive out of order. A teammate blames "event loop ordering bugs". Are they right?

<details>
<summary>Answer</summary>

No — this is not an event-loop ordering problem; it's the wrong repeat primitive. `setInterval` fires **on the clock**, whether or not the previous async request has finished, so slow responses overlap. A recursive `setTimeout` re-armed after each response guarantees a gap between polls. (The confusable: interval = metronome, recursive timeout = rest between reps.)

</details>

### S4 — One slow user, everyone waits

A Node endpoint computes a big report with a synchronous loop that takes 3 seconds. Monitoring shows that while any report runs, *every* request to the server — including unrelated health checks — waits. Why does one user's work affect all of them?

<details>
<summary>Answer</summary>

Node runs all JavaScript on **one thread with one call stack** (Ideas #1, #2). A 3-second synchronous handler blocks the loop, so every other connection's callbacks sit in the task queue behind it. Fixes follow the Limits section: chunk the work, move it to a worker thread, or precompute.

</details>

### S5 — The drifting countdown

A 10-minute countdown built by subtracting one second inside `setTimeout(tick, 1000)` chains runs fine in the foreground — but after the user backgrounds the tab for a while, the countdown is minutes behind the wall clock. What's going on?

<details>
<summary>Answer</summary>

Delays are **minimums**, and hidden tabs throttle timers (Ideas #5, #11) — each "1000 ms" tick fires late, and the lateness compounds because the clock is built by *accumulating ticks*. Fix: on every tick, compute remaining time from `Date.now()` against the target time; use timers to schedule, never to measure.

</details>

### S6 — Async to the rescue?

An image filter freezes the page for 4 seconds, so a teammate rewrites it as an `async` function and sprinkles `await` before it: "now it's asynchronous, it won't block." The page still freezes. Why didn't `async` help?

<details>
<summary>Answer</summary>

`async`/`await` reorders work on the **same single thread** — it changes *when* the loop runs the filter, not *where* (the confusable: async is not multithreading). The 4-second loop still runs to completion and blocks input and paint. Real fixes: a Web Worker (an actual second thread), chunking with yields, or the server.

</details>

## Build It

### B1 — `debounce(fn, delay)`

*Scheduled: Week 4 · Wed — "polyfills — `call`, `apply`, `bind`, `debounce`, `throttle`" · Mode: from memory (the Week 4 Friday rewrite is timed — see the plan). No row in `prep/notes.md` yet.*

**Task.** Return a wrapped version of `fn` that only runs after `delay` ms of silence: every call cancels the pending run and re-arms the timer. Calls must forward `this` and all arguments. Example — with `delay` 100: calls `log("a")`, `log("b")`, `log("c")` back to back, then `log("d")` 300 ms later, print `ran: c` and then `ran: d`.

<details>
<summary>Hint 1 — where to look</summary>

What does the *previous* call leave behind that the next call must be able to find — and where can it live so every call to the wrapper sees it?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

A pending timer can be cancelled by its id; keep that id in the closure and kill it as the first act of every call.

</details>

<details>
<summary>Solution</summary>

**Approach.** The wrapper closes over one `timerId`. Each call clears the pending timer and arms a fresh one, so only the last call in a burst survives the delay. `fn.apply(this, args)` keeps the receiver and arguments intact (see [`this` — brief](../thisKeyword/brief.md)).

```js
function debounce(fn, delay) {
  let timerId;
  return function (...args) {
    clearTimeout(timerId);
    timerId = setTimeout(() => fn.apply(this, args), delay);
  };
}

const log = debounce(function (msg) {
  console.log("ran:", msg);
}, 100);

log("a");
log("b");
log("c");
setTimeout(function later() {
  log("d");
}, 300);
```

```text
ran: c
ran: d
```

**Edge cases handled.** Each one was actually run: a burst of calls collapses to the last (`a`,`b`,`c` → only `c`); a call after a quiet period runs on its own (`d`).

**Uses ideas:** #3, #4, #5

</details>

### B2 — `throttle(fn, interval)`

*Scheduled: Week 4 · Wed — same plan line as B1 · Mode: from memory. No row in `prep/notes.md` yet.*

**Task.** Return a wrapped `fn` that runs at most once per `interval` ms: the first call runs immediately, calls during the cooldown are dropped. Example — with `interval` 100: `log("a")` and `log("b")` back to back, `log("c")` at 50 ms, `log("d")` at 150 ms print `ran: a` then `ran: d`.

<details>
<summary>Hint 1 — where to look</summary>

Debounce remembered a pending timer. What single fact does throttle need to remember about the *past* instead?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Keep the timestamp of the last run in the closure; a call runs only if enough time has passed since it.

</details>

<details>
<summary>Solution</summary>

**Approach.** Close over `lastRun`. Each call compares `Date.now()` against it — measuring with the clock, not by counting timers, so it cannot drift. (This is the leading-edge variant; interviewers sometimes ask for a trailing call too — that version re-arms a timer like debounce.)

```js
function throttle(fn, interval) {
  let lastRun = 0;
  return function (...args) {
    const now = Date.now();
    if (now - lastRun >= interval) {
      lastRun = now;
      fn.apply(this, args);
    }
  };
}

const log = throttle(function (msg) {
  console.log("ran:", msg);
}, 100);

log("a");
log("b");
setTimeout(function at50() {
  log("c");
}, 50);
setTimeout(function at150() {
  log("d");
}, 150);
```

```text
ran: a
ran: d
```

**Edge cases handled.** Each one was actually run: the first call runs immediately (`a`); calls inside the cooldown are dropped, both synchronous (`b`) and timed (`c` at 50 ms); a call past the cooldown runs (`d` at 150 ms).

**Uses ideas:** #1, #5

</details>

### B3 — Unblock the loop

*Not on the plan — added because Ideas #2, #4 and #10 are exercised by nothing else hands-on.*

**Task.** Write `processChunked(items, chunkSize, onDone)`: process all items in chunks of `chunkSize`, yielding to the event loop between chunks so other callbacks can interleave. Prove it: schedule a 0 ms "heartbeat" timer before starting on 30 items with chunk size 10 — the heartbeat must print *between* chunk logs, not after all of them.

<details>
<summary>Hint 1 — where to look</summary>

What has to happen to the current task before anything else — the heartbeat, a click, a paint — is allowed to run?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Ending the task is the yield: process one chunk, then schedule the *next* chunk as a new task with `setTimeout(..., 0)`.

</details>

<details>
<summary>Solution</summary>

**Approach.** A recursive helper processes one slice synchronously, then either re-arms itself as a task or finishes. Because each chunk is its own task, the loop can serve anything that arrived in between — exactly why the heartbeat lands after chunk 1. A microtask would NOT work here (see S2): the drain would run all chunks back to back.

```js
function processChunked(items, chunkSize, onDone) {
  let i = 0;
  function runChunk() {
    const end = Math.min(i + chunkSize, items.length);
    for (; i < end; i++) {
      // the "work" — one item at a time
    }
    console.log("processed up to", i);
    if (i < items.length) {
      setTimeout(runChunk, 0);
    } else {
      onDone();
    }
  }
  runChunk();
}

setTimeout(function heartbeat() {
  console.log("heartbeat — a click would be handled here");
}, 0);

processChunked(new Array(30).fill(0), 10, function onDone() {
  console.log("all 30 done");
});
```

```text
processed up to 10
heartbeat — a click would be handled here
processed up to 20
processed up to 30
all 30 done
```

**Edge cases handled.** Each one was actually run: a waiting task interleaves after the first chunk; the final partial-or-full chunk triggers `onDone` exactly once; `Math.min` guards the last slice.

**Uses ideas:** #2, #4, #10

</details>

### B4 — `myQueueMicrotask(fn)`

*Not on the plan — added because Idea #6 is exercised by nothing else hands-on; a warm-up for Week 5's promise polyfills.*

**Task.** Implement `queueMicrotask` using only promises, and prove the queue it lands in: against a 0 ms timer and a synchronous log, your microtask must print second — after the sync log, before the task.

<details>
<summary>Hint 1 — where to look</summary>

Which built-in operation is *defined* to schedule its callback as a microtask?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

A `.then` callback on an already-resolved promise is a microtask with no waiting involved.

</details>

<details>
<summary>Solution</summary>

**Approach.** `Promise.resolve()` costs nothing to settle; `.then(fn)` files `fn` straight into the microtask queue. The printed order is the proof.

```js
function myQueueMicrotask(fn) {
  Promise.resolve().then(fn);
}

setTimeout(function onTimer() {
  console.log("task");
}, 0);
myQueueMicrotask(function onMicro() {
  console.log("microtask");
});
console.log("sync");
```

```text
sync
microtask
task
```

**Edge cases handled.** Each one was actually run: ordering against both a pending task and synchronous code. (The real `queueMicrotask` also reports `fn`'s exceptions globally instead of as a rejected promise — out of scope here.)

**Uses ideas:** #6, #8

</details>

## Self-Check Questions

### Q1 — Why queues at all?

Why does JavaScript make callbacks wait in a queue instead of running them the moment they are ready?

<details>
<summary>Answer</summary>

Because running them immediately would mean interrupting whatever is mid-run, and the language guarantees run to completion (Idea #1). The queue converts "whenever it's ready" into "when the stack is empty", so no function ever sees its variables change under its feet. The cost is latency; the gain is no locks and no data races.

</details>

### Q2 — The meaning of zero

What exactly does the `0` in `setTimeout(fn, 0)` promise — and what does it not promise?

<details>
<summary>Answer</summary>

It promises `fn` becomes *eligible* immediately: the host files it as a task at once (Ideas #4, #5). It does not promise when `fn` runs — that waits on the stack emptying, the full microtask drain, and every task ahead of it. "As soon as you're free", never "now".

</details>

### Q3 — Why microtasks first?

Why drain the whole microtask queue before taking the next task — what would break with a fairer "one microtask, one task" alternation?

<details>
<summary>Answer</summary>

Promise code relies on all its follow-up work running before unrelated tasks interleave (Idea #7) — a `.then` chain updating shared state must not have a click handler run between its links. Alternation would make promise ordering depend on what else the page is doing. The price of the guarantee is that microtasks can starve tasks.

</details>

### Q4 — Clicks during a freeze

During a 2-second synchronous loop the user clicks a button five times. Are the clicks lost, and when does their handler run?

<details>
<summary>Answer</summary>

Not lost — each click is filed as a task while the loop blocks the stack (Ideas #2, #4). When the long task ends, the handlers run one per loop turn, back to back. That burst of late clicks is exactly what the sandbox freeze demo in the deep dive shows.

</details>

### Q5 — Before the first await

In an async function, is the code *before* the first `await` synchronous or asynchronous — and how would you prove it in one snippet?

<details>
<summary>Answer</summary>

Synchronous — it runs in place during the call (Idea #9). Proof shape: log inside the function before `await`, log after the call; the inside log prints first (W4). Only at the first `await` does the function suspend and the rest become a microtask.

</details>

### Q6 — Already settled

`p` is a promise that settled an hour ago. When does `p.then(fn)` run `fn`?

<details>
<summary>Answer</summary>

At the next microtask checkpoint — after the current run of the stack, never synchronously (Idea #8). Settlement decides *that* `fn` runs and with what value; the queue decides *when*. Callers can therefore rely on `.then` never running mid-way through their own code.

</details>

### Q7 — The infinite sticky note

A microtask queues another microtask, which queues another, forever. What happens to the page, and why can't two timers do the same damage to each other?

<details>
<summary>Answer</summary>

The checkpoint drains to empty, so the drain never ends: no task, no click, no paint — the page is dead (Idea #7, starvation). Timers can't do this to each other because tasks run one per loop turn; a timer that re-arms itself always lets everything else have a turn in between.

</details>

### Q8 — The Node extras

Name the two Node-only scheduling tools from the Variants table and the one ordering rule that is safe in every environment.

<details>
<summary>Answer</summary>

`process.nextTick` (its own queue, drained even before promise microtasks) and `setImmediate` (a task in a later loop phase whose order against `setTimeout(fn, 0)` is observably not fixed) — Idea #11. The portable rule: synchronous code, then the full microtask drain, then tasks.

</details>

### Q9 — Chunking with the wrong queue

B3 yields between chunks with `setTimeout`. Why does replacing it with `queueMicrotask` silently destroy the whole point?

<details>
<summary>Answer</summary>

Yielding means *ending the task* so the loop can serve others (Ideas #6, #7, #10). A microtask runs in the same checkpoint — chunk queues chunk, the drain runs them all back to back, and no click or paint fits anywhere. Task = breathe; microtask = don't.

</details>

### Q10 — What if tasks came first?

Thought experiment: the loop takes the next task *before* draining microtasks. Which guarantee from this topic dies, and name one snippet from this doc set whose output would change.

<details>
<summary>Answer</summary>

The guarantee that promise follow-ups run before unrelated work (Idea #7) — `.then` chains could be interleaved with any waiting task. W2 flips to `C A B`: the timer task would beat the promise microtask.

</details>

## Explain It Back

### E1 — The new teammate

Explain the event loop to a new teammate in two minutes, starting from the kitchen analogy, out loud.

<details>
<summary>Rubric — what a complete answer covers</summary>

- One cook / one call stack; a dish is cooked start to finish (run to completion, Idea #1)
- The oven counts the timer down — the host, not JavaScript (Idea #3)
- Tickets wait on the rail — the task queue, served one per cycle when the cook is free (Idea #4)
- Sticky notes all cleared after every dish — the microtask drain, promise callbacks (Ideas #6, #7)
- One trap stated: delay is a minimum (Idea #5)
- Where the analogy lies — at least one break named (nothing interrupts the cook; notes added mid-sweep still get cleared)

</details>

### E2 — The late timer

An interviewer shows you a 100 ms timer that fired at 600 ms and asks: "Why, and is this a bug in the browser?" Answer out loud.

<details>
<summary>Rubric — what a complete answer covers</summary>

- Not a bug: a delay is when the callback becomes eligible, not when it runs (Idea #5)
- The callback waited for the stack — something ran for ~600 ms (Idea #2)
- Mechanism named: host fires → task queue → runs on empty stack (Ideas #3, #4)
- Mentions the other delayers for completeness: queue backlog, clamping, background throttling (Idea #11)
- Closes with the fix direction: find and split the long task, or measure with `Date.now()` instead of counting ticks

</details>

### E3 — The sceptic

"Single-threaded is a design flaw — JavaScript should just run things in parallel." Argue both sides for a minute, then give your verdict.

<details>
<summary>Rubric — what a complete answer covers</summary>

- The cost, honestly: one long task freezes input and paint; CPU work doesn't fit the model (Ideas #2, #10)
- The gain: run to completion means no locks, no data races; every handler sees consistent state (Idea #1)
- Escape hatches exist and are deliberate: hosts do I/O off-thread, Workers add real threads without shared state (Idea #3)
- Verdict takes a side and names the condition (e.g. the model wins for UI and I/O-bound servers; CPU-bound work belongs off the main thread)

</details>
