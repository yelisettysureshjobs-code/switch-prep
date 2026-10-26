# Event Loop — Call Stack, Task Queue & Microtask Queue — Deep Dive

> **Track:** JS · **Type:** concept · **Scheduled:** Week 3 · Mon · **Runtime:** Node v22 · **Generated:** 2026-10-19

## Why It Exists

Prerequisite: the call stack — see [Execution Context, Lexical Environment & Hoisting](../../../JS-docs/lexicalEnvExecContextHoisting.md). This topic needs one idea from it: function calls stack up, and only the function on top is running.

You ask for a timer in 100 ms, then do half a second of work. When does the timer fire?

```js
console.log("start");
const begin = Date.now();
setTimeout(function onTimer() {
  console.log("timer done, waited at least 500 ms:", Date.now() - begin >= 500);
}, 100);
while (Date.now() - begin < 500) {}
console.log("loop finished");
```

```text
start
loop finished
timer done, waited at least 500 ms: true
```

A timer set for 100 ms fired after more than 500 — not because timers are broken, but because JavaScript was busy. *(Idea 2)* While any function is running, **nothing** else runs in JavaScript: not another callback, not a click handler, not a screen update. Keeping the running code busy is called **blocking**, and it is why a heavy loop freezes a whole page.

The event loop is the mechanism that decides what runs next once the current work finishes. Its rules are the difference between guessing at output order and deriving it — and they carry every piece of async work coming up on the plan: Wednesday's promises, Week 4's debounce and the autocomplete build, Week 5's `Promise.all` polyfills.

## Level 1 — Intuition

The kitchen story — one cook, an order rail, an oven that rings — is in [brief.md](brief.md); here is what it maps to.

*(Idea 1)* JavaScript has **one call stack**: one cook. A function that starts gets cooked start to finish — it is never paused halfway so something else can cut in. This is called **run to completion**.

*(Idea 3)* So who counts down a timer while JavaScript is busy running your code? Not JavaScript. `setTimeout` hands the countdown to the **host** — the browser or Node, the kitchen *around* the cook. The host watches the clock, and when time is up it does not barge in: it files your function — the **callback** — into a waiting line, as a **task**.

```js
console.log("start");
setTimeout(function onTimer() {
  console.log("timer");
}, 0);
console.log("end");
```

```text
start
end
timer
```

Even at `0` ms, `timer` prints last. The zero was honoured — the host filed `onTimer` into the line immediately. But the line is only read when the cook is free:

```text
                ┌────────────────┐
  running code  │   call stack   │ ◀── one callback runs here at a time,
                └────────────────┘     start to finish
                        ▲
                        │ stack empty? take the next task
                ┌────────────────┐
                │   task queue   │ onTimer waits in line here
                └────────────────┘
                        ▲
                        │ "time's up" — callback joins the line
                ┌────────────────┐
                │    the host    │ counts the 0 ms down, outside JS
                └────────────────┘
```

**Recap:** One call stack, so one thing runs at a time, to completion. Timers and events are counted down by the host; their callbacks wait in a line and run only when the stack is empty.

**Check yourself (C1):**  The timer above was 0 ms. Why did `end` still print first?

<details>
<summary>Answer</summary>

`0` says when `onTimer` becomes *eligible*, and the host filed it into the task queue right away — but a task is only moved onto the call stack when the stack is empty, and the script itself was still running. `end` is part of the script; `timer` had to wait for the script to finish.

</details>

## Level 2 — How It Actually Works

The loop in "event loop" is exactly this cycle: run the stack until it is empty, take **one** task from the **task queue**, run it to completion, repeat. *(Idea 4)* A callback never interrupts running code — it enters the stack only when the stack is empty. That is why `setTimeout(fn, 0)` never runs `fn` immediately.

```text
prints: start end timer

step  runs                    task     prints  why now
1     console.log("start")    -        start   synchronous: runs in place
2     setTimeout(onTimer, 0)  onTimer  -       host files onTimer as a task at once
3     console.log("end")      onTimer  end     the script is still on the stack
4     script ends             -        timer   stack empty: the loop takes the next task
```

*(Idea 5)* Now re-read the snippet in Why It Exists: the delay is a **minimum**, never a promise of when. `100` means "eligible from 100 ms on"; the callback still waits for the stack to empty and for its turn in the queue. If it were otherwise — if the host could fire the callback *at* 100 ms — it would have to interrupt the running loop, and run to completion would be gone: any line of your code could be preempted by any other.

> **Primer — promises, just enough for this topic** (Wednesday's slot covers them properly):
> a **promise** is an object that stands for a value that will exist later.
> `Promise.resolve("x")` makes one whose value is already there.
> `p.then(fn)` registers `fn` to be called with that value, later.
> Chaining and error handling belong to the [promises topic](../promises/brief.md).

Extend the running example with one promise line:

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

`promise` beat `timer` — even though the `setTimeout` line came first. *(Idea 6)* Promise callbacks do not join the task queue. They go to a second, higher-priority line: the **microtask queue**. `queueMicrotask(fn)` puts a plain function there too. Each entry in it is a **microtask**.

```text
prints: start end promise timer

step  runs                      micro      task     prints   why now
1     console.log("start")      -          -        start    synchronous: runs in place
2     setTimeout(onTimer, 0)    -          onTimer  -        host files a task
3     then(onPromise)           onPromise  onTimer  -        promise callback → microtask
4     console.log("end")        onPromise  onTimer  end      still inside the script
5     script ends               -          onTimer  promise  stack empty: microtasks first
6     next task                 -          -        timer    only now the timer callback
```

**Recap:** The loop rule — stack empties, then one task. A delay is a minimum. Promise callbacks wait in a second queue, the microtask queue, which the loop serves before the task queue.

**Check yourself (C2):** In the extended example, move the `Promise.resolve().then(...)` lines *above* the `setTimeout` lines. Does the output change?

<details>
<summary>Answer</summary>

No — still `start end promise timer`. The order of the *scheduling calls* only decides when each callback joins its queue; what decides the running order is which **queue** each lands in. `onPromise` is a microtask and `onTimer` is a task, and microtasks run first regardless of who was queued first.

</details>

## The Analogy, Mapped

| In the story                                           | In reality                                     | Idea # |
| ------------------------------------------------------ | ---------------------------------------------- | ------ |
| One cook, one dish at a time, start to finish          | One call stack; run to completion              | 1      |
| The cook never looks up mid-dish                       | Running code blocks everything else            | 2      |
| The oven counts down and rings                         | The host times the timer, outside JS           | 3      |
| The order rail of tickets                              | The task queue; one ticket per cycle           | 4      |
| Sticky notes, all cleared after every single dish      | The microtask queue, drained after each task   | 6, 7   |
| The waiter collects plates only between dishes         | Rendering happens between tasks                | 10     |

Where the analogy breaks:

- A real cook glances up when the oven rings. JavaScript cannot — the ring only files a ticket; the running function finishes untouched.
- A real cook would stop clearing sticky notes that keep arriving. The microtask queue is drained even of notes added *during* the drain — see Level 3.
- A real kitchen's waiter takes every finished plate. The browser may skip a paint opportunity; between-tasks is a *chance* to render, not a guarantee.
- The rail holds food orders only. The task queue holds every kind of callback — timers, clicks, network events — from several sources.

## Level 3 — The Formal Picture

The precise vocabulary: the **event loop**, **tasks** and the **microtask checkpoint** are defined in the HTML Standard ("event loop processing model" — the standard, not ECMAScript, owns them). ECMAScript itself defines **Jobs**; a promise reaction is a Job, and the host runs promise Jobs as microtasks. MDN's "Microtask guide" is the readable version of both.

*(Idea 7)* The rule the checkpoint enforces: **after the script, and after every task, ALL microtasks run before the next task** — including microtasks queued during the drain itself.

```js
setTimeout(function onTimer1() {
  console.log("timer 1");
}, 0);
setTimeout(function onTimer2() {
  console.log("timer 2");
}, 0);
Promise.resolve().then(function onPromise1() {
  console.log("promise 1");
  Promise.resolve().then(function onPromise2() {
    console.log("promise 2");
  });
});
console.log("script end");
```

```text
prints: script end  promise 1  promise 2  timer 1  timer 2
legend: P1/P2 = onPromise1/2 (microtasks) · T1/T2 = onTimer1/2 (tasks)

step  runs                        micro  task   prints      why now
1     setTimeout(onTimer1, 0)     -      T1     -           task filed
2     setTimeout(onTimer2, 0)     -      T1 T2  -           same-delay tasks keep order
3     then(onPromise1)            P1     T1 T2  -           promise callback → microtask
4     console.log("script end")   P1     T1 T2  script end  still inside the script
5     script ends → checkpoint    -      T1 T2  promise 1   microtasks drain before tasks
6     onPromise1 queues P2        P2     T1 T2  -           queued DURING the drain
7     the same drain continues    -      T1 T2  promise 2   the drain runs until empty
8     next task                   -      T2     timer 1     one task per loop turn
9     checkpoint (empty) → next   -      -      timer 2     the next task only now
```

`promise 2` was not even queued when the drain began — it still ran before any timer. If the rule were "drain only what was queued so far", promise code could never rely on all its follow-up work finishing before unrelated tasks ran in between. The cost of the rule is **starvation**: a microtask that keeps queueing microtasks holds the task queue — and the page — hostage:

```js
let count = 0;
function queueAgain() {
  if (count < 3) {
    count++;
    console.log("microtask", count);
    Promise.resolve().then(queueAgain);
  }
}
setTimeout(function onTimer() {
  console.log("timer");
}, 0);
Promise.resolve().then(queueAgain);
```

```text
microtask 1
microtask 2
microtask 3
timer
```

Three generations of microtasks ran before one task. Make that condition `count < Infinity` and `timer` never runs. Tasks cannot do this to each other — the loop takes exactly one per turn.

*(Idea 8)* A `.then` callback never runs synchronously — even when the promise is **already resolved** at the moment `.then` is called:

```js
const p = Promise.resolve("ready");
p.then(function onReady(value) {
  console.log("got", value);
});
console.log("script end");
```

```text
script end
got ready
```

The guarantee is deliberate: a promise callback always runs on an empty stack, never mid-way through someone else's code. Without it, `p.then(fn)` would run `fn` sometimes now, sometimes later — and every caller would have to defend against both.

*(Idea 9)* `await` is this same machinery wearing nicer clothes. An async function runs **synchronously up to its first `await`**; at the `await` it suspends, and everything after it resumes **as a microtask**:

```js
async function main() {
  console.log("before await");
  await null;
  console.log("after await");
}
main();
console.log("script end");
```

```text
before await
script end
after await
```

`main()` is an ordinary call — `before await` prints in place. `await null` wraps `null` in a resolved promise and suspends `main`; the rest of the function behaves exactly like a `.then` callback on it. Only `main` is suspended — the script carries on, which is why `script end` comes second.

*(Idea 10)* One more queue-like thing matters in a browser: painting the screen. The browser's loop may **update the rendering between tasks** — never in the middle of one, and only after the microtask checkpoint. At 60 frames per second the budget is `1000 ms ÷ 60 ≈ 16.7 ms` per frame; a task (or a microtask drain) longer than that is skipped frames, and a 2-second one is a frozen page:

```html
<!-- Not run — paste into HTML_JS_Sandbox to see it -->
<button id="count">count</button> <span id="n">0</span>
<button id="freeze">freeze 2 s</button>
<script>
  let n = 0;
  document.getElementById("count").addEventListener("click", function onCount() {
    n++;
    document.getElementById("n").textContent = n;
  });
  document.getElementById("freeze").addEventListener("click", function onFreeze() {
    const begin = Date.now();
    while (Date.now() - begin < 2000) {}
  });
</script>
```

Expected, not observed here: after clicking *freeze 2 s*, clicks on *count* do nothing for two seconds — then all land at once, because every click was filed as a task and waited for the stack.

**Recap:** After every task the checkpoint drains the whole microtask queue, even entries added mid-drain. Promise callbacks are never synchronous. `await` = run to the first `await` synchronously, resume as a microtask. The browser paints between tasks, after the checkpoint.

**Check yourself (C3):** In the drain-rule example, add a third nested `Promise.resolve().then(onPromise3)` inside `onPromise2`. Does `onPromise3` run before or after `timer 1`?

<details>
<summary>Answer</summary>

Before. It is queued during the same microtask checkpoint, and the drain runs until the microtask queue is empty — however many generations that takes. Only then does the loop take the next task.

</details>

## Variants & Environment Differences

*(Idea 11)* The two-queue model and every output above hold in both browsers and Node. What differs is at the edges:

| Variant or environment            | What changes                                                                                                 | Since / where                        |
| --------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------ |
| Node: `process.nextTick`          | A third, Node-only queue that drains *before* promise microtasks                                             | All modern Node — Node docs          |
| Node: `setImmediate`              | A task scheduled into a later phase of Node's loop; its order vs `setTimeout(fn, 0)` at script top level is **not fixed** — this run observed both orders across 10 executions of the same file | Node — do not build on this ordering |
| `queueMicrotask`                  | Available in browsers and in Node 11 and later                                                               | WHATWG / Node 11+                    |
| Browser: nested timer clamping    | Timers nested more than 5 levels deep with a delay under 4 ms are clamped to 4 ms                            | HTML Standard, timer steps           |
| Browser: background tabs          | Timers in hidden tabs are throttled, commonly to one per second (recalled — verify)                          | Browser behaviour                    |
| Browser: `requestAnimationFrame`  | Runs with "update the rendering" — neither a task nor a microtask; belongs to Week 6's rendering topic       | HTML Standard                        |
| Modules vs scripts, strict mode   | Nothing changes for scheduling — queue semantics are identical                                               | —                                    |

The `process.nextTick` row, shown (this file was run whole):

```js
Promise.resolve().then(function onPromise() {
  console.log("promise");
});
process.nextTick(function onTick() {
  console.log("nextTick");
});
console.log("sync");
```

```text
sync
nextTick
promise
```

The portable rule — the only ordering worth memorising — is: **synchronous code, then microtasks, then tasks.** Orderings between task *sources* are environment details.

## Compared With What You Know

You have already written task-queue consumers without naming them: every handler in [DOM Events — Capture, Bubble & Delegation](../../../JS-docs/domEventsCaptureBubbleDelegation.md) runs as a task when its event fires.

**`setTimeout(fn, 0)` vs `Promise.resolve().then(fn)`** — both say "run `fn` later"; they are not interchangeable:

| Aspect                        | `setTimeout(fn, 0)`                   | `Promise.resolve().then(fn)`              |
| ----------------------------- | ------------------------------------- | ----------------------------------------- |
| Queue it joins                | Task queue                            | Microtask queue                           |
| Runs                          | A later loop turn                     | At the current checkpoint, before any task |
| Clicks / paint can go first   | Yes — the page gets a turn            | No — nothing runs in between              |
| Can starve the page           | No — one task per turn                | Yes — a self-queueing chain holds the loop |

Tell them apart by intent: "later, and let the page breathe" is a task; "immediately after this work, before anything else sneaks in" is a microtask.

**`async`/`await` vs actual multithreading** — `await` looks like "do this in the background", and it is not:

| Aspect               | `async`/`await`                                   | A second thread (Web Worker)         |
| -------------------- | ------------------------------------------------- | ------------------------------------ |
| Threads created      | None — same single thread                         | One real one                         |
| A CPU-heavy loop     | Still blocks everything while it runs             | Runs in parallel, page stays live    |
| What `await` frees   | The thread, but only *between* suspension points  | Nothing to free — truly concurrent   |
| Sharing data         | Everything in scope                               | Messages only                        |

Tell them apart: `async` *reorders* work on the one thread; a Worker *adds* a thread. (The run that proves the left column is in Misconceptions & Traps.)

**`setInterval(fn, ms)` vs a recursive `setTimeout`** — both repeat; they promise different things:

| Aspect             | `setInterval(fn, 1000)`                                  | `setTimeout` re-armed at the end of `fn`      |
| ------------------ | -------------------------------------------------------- | --------------------------------------------- |
| Schedule           | On the clock — every second, ready or not                | A fixed gap *after* each run finishes         |
| Overlapping work   | A slow async job from tick N can still be in flight at N+1 | Impossible — next timer is set after the work |
| Under load         | Keeps the average rate                                   | Drifts — gaps stretch, rate drops             |

Tell them apart: an interval is a metronome; a recursive timeout is rest between reps. Polling a server wants the second.

## Where It's Used

1. **A Node server handling thousands of connections on one thread** — the event loop is Node's whole concurrency story: callbacks interleave while I/O waits in the host, with no threads and no locks. **Documented** — [About Node.js](https://nodejs.org/en/about) (opened for this doc): "asynchronous event-driven JavaScript runtime … in contrast to today's more common concurrency model, in which OS threads are employed." The flip side: one synchronous handler blocks *every* user — see Spot It S4.
2. **The Week 4 autocomplete** ("autocomplete — debounce, cancellation, keyboard nav") — debounce is nothing but Ideas 3–5: re-arm a host timer on every keystroke so the callback only survives the last one. **Illustrative.**
3. **The spinner that never shows** — code sets a spinner visible, then parses a huge JSON synchronously; the paint between tasks never gets its turn, so users see a freeze and then the finished page, never the spinner. **Illustrative.**
4. **Chunked work to keep a list responsive** — processing 10,000 rows in slices with a timer between slices, so clicks and paints interleave (built in Practice B3; the Week 6 virtualized list is the rendering-side cousin). **Illustrative.**
5. **Every DOM event handler you have written** — a click files the handler as a task; two rapid clicks during a long task both wait, then run back to back. **Illustrative**, and the reason delegation (your events doc) costs nothing extra: one task either way.

Nothing in the learner's own files uses timers or promises yet — the Week 4 builds will be the first; no honest in-repo example exists, so none is invented.

## Misconceptions & Traps

*No event-loop code exists in the learner's files yet (checked `codes/JS/`, `HTML_JS_Sandbox/`, the React sandbox), so there are no personal findings to open with. These are the traps the interview questions are built from.*

**"`setTimeout(fn, 0)` runs `fn` immediately."**
*Why it seems right* — zero means zero; what else would it mean?
*Why it is wrong* — the Level 1 snippet: `start end timer`, with `timer` last despite the 0.
*The correct model* — `0` is when the callback becomes *eligible*. It still joins the task queue and still waits for the stack to empty. `setTimeout(fn, 0)` means "as soon as you're free", never "now".

**"A 100 ms delay means it runs at 100 ms."**
*Why it seems right* — for a page doing nothing, it looks exactly like that.
*Why it is wrong* — the Why It Exists snippet: a 100 ms timer observed waiting ≥ 500 ms behind a busy loop.
*The correct model* — every delay is a minimum. Actual fire time = max(delay, whenever the stack and the queue ahead of it clear). Clamping and tab throttling (Variants) only push it later.

**"`.then` runs as soon as the promise is resolved."**
*Why it seems right* — the value of `Promise.resolve("ready")` exists before `.then` is even called; there is nothing to wait for.
*Why it is wrong* — the Level 3 snippet: `script end` prints before `got ready`.
*The correct model* — resolution decides *that* the callback will run, the microtask queue decides *when*: after the current run of the stack, at the checkpoint. A promise callback is never synchronous, so callers never face the sometimes-now-sometimes-later trap.

**"`await` pauses the whole program."**
*Why it seems right* — inside the async function, reading top to bottom, execution really does stop at the `await` line.
*Why it is wrong* — the Level 3 `await` snippet: `script end` prints while `main` is suspended.
*The correct model* — `await` suspends *one function* and frees the thread. The rest of that function is re-queued as a microtask. Everything outside carries on.

**"Microtasks are just faster tasks — same thing, higher priority."**
*Why it seems right* — both are queues of callbacks the loop serves.
*Why it is wrong* — the starvation run in Level 3: three generations of self-queueing microtasks (`microtask 1 2 3`) all printed before one waiting task (`timer`). Tasks can never do that to each other.
*The correct model* — tasks run one per loop turn, so they cannot lock each other out. The microtask drain runs to empty, including entries queued mid-drain, so microtasks *can* starve tasks — and the paint. Different queue, different contract.

**"Wrapping CPU work in a promise makes it non-blocking."**
*Why it seems right* — promises are "async", and async things don't block… surely.
*Why it is wrong* —

```js
const begin = Date.now();
setTimeout(function onTimer() {
  console.log("0 ms timer fired after at least 300 ms:", Date.now() - begin >= 300);
}, 0);
Promise.resolve().then(function heavy() {
  while (Date.now() - begin < 300) {}
  console.log("heavy work finished");
});
```

```text
heavy work finished
0 ms timer fired after at least 300 ms: true
```

*The correct model* — a promise changes *when* the work starts (at the checkpoint), not *where* it runs. The loop ran `heavy` to completion on the one thread; the 0 ms timer waited 300 ms behind it. Only the host (I/O, timers) or another thread (a Web Worker) does work off the main thread.

## Limits & Trade-offs

The event loop is not a choice — it is simply how JavaScript runs — so the question is what its model costs and the standard ways around each cost:

- **One long task freezes everything** — input, rendering, other callbacks. The ways out: slice the work and yield between slices with a timer (Practice B3), hand it to a Web Worker (a real second thread — beyond this plan's depth, one line is enough in interviews), or do it on the server.
- **"Async" code can still block** — a microtask chain or a heavy `.then` body holds the loop exactly like sync code. Yielding means *ending the current task*, not merely using promises.
- **Timers are imprecise by design** — minimum delays, clamping, background throttling. Schedule with timers, but *measure* with `Date.now()`; a clock built by accumulating `setTimeout(…, 1000)` ticks drifts.
- **Ordering guarantees are narrow** — rely on: synchronous code, then the full microtask drain, then one task. Orderings between task sources (`setImmediate` vs timers) are environment details that observably vary run to run.

Verdict: the single-threaded loop trades raw parallelism for a model with no locks and no data races, where every function runs atomically. That trade is why JavaScript is easy to reason about in the small — and why its one hard rule, never block the loop, has no exceptions.

## Interview Notes

The question opens one of two ways: a predict-the-output snippet mixing `console.log`, `setTimeout` and promises (exactly the plan's own wording for this slot — "predict output of 6 tricky snippets"), or the open "explain the event loop". For the snippet form, narrate queue by queue, not line by line — interviewers score the *why now*, not the final ordering alone.

The usual follow-up ladder, each with the key idea of the reply:

- "Why does the promise beat the timer?" — two queues; the microtask checkpoint drains fully after the script and after every task.
- "Where does `await` fit?" — sync until the first `await`, the rest resumes as a microtask; only that function suspends.
- "Your page froze during a computation — fix it." — nothing runs mid-task, paint included; chunk the work and yield with a timer, or move it off the thread entirely.
- "Any difference in Node?" — `process.nextTick` ahead of microtasks, `setImmediate` exists; the portable rule is sync → microtasks → tasks.

At 2 years of experience, product-company rounds (the roles in `prep/README.md`) stop there — libuv phase internals and spec quotation are not expected; deriving outputs with reasons is.

Said out loud in under two minutes: one thread, one call stack, run to completion — so nothing ever interrupts running code. Timers and I/O live in the host; finished callbacks wait as tasks, promise callbacks as microtasks. The loop: empty the stack, drain all microtasks, take one task, repeat — the browser may paint between tasks. `await` suspends one function and resumes it as a microtask. And the trap that catches everyone: a delay is a minimum, so a 100 ms timer behind a 500 ms loop fires at 500.
