# Promises — Practice

> **Track:** JS · **Type:** concept · **Scheduled:** Week 3 · Wed · **Runtime:** Node v24 · **Generated:** 2026-10-24

## How to Use This File

Every answer is collapsed — predict (or build) first, open after. Warm-Ups and Self-Checks are
from memory; Build Its are from a blank file, no lookups; Explain It Backs are said out loud.
Every snippet's shown output is from a real run on Node v24. Log what you attempted in
`prep/notes.md` in your own words — the method and what "done" means are in
[how-to-study.md](../../../prep/how-to-study.md).

## Warm-Up

### W1 — Two subscribers

```js
const p = Promise.resolve("A");
p.then(function first(value) {
  console.log(1, value);
});
p.then(function second(value) {
  console.log(2, value);
});
console.log("start");
```

<details>
<summary>Output and why</summary>

```text
start
1 A
2 A
```

Reactions never run synchronously, so `start` is first (#2's deferral rule). Both `.then`s
are attached to the *same* promise `p` — two independent subscribers, not a chain — so both
receive `"A"` (#1, #3).

</details>

### W2 — What flows down

```js
Promise.resolve(2)
  .then(function a(n) {
    return n + 1;
  })
  .then(function b(n) {
    console.log("b got", n);
  })
  .then(function c(n) {
    console.log("c got", n);
  });
```

<details>
<summary>Output and why</summary>

```text
b got 3
c got undefined
```

Each link's input is the previous callback's *return value* (#3, #4). `a` returns 3, so `b`
gets 3 — but `b` returns nothing, so `c` gets `undefined`. A chain only carries what you
explicitly return.

</details>

### W3 — The error lane

```js
Promise.reject(new Error("x"))
  .then(function a() {
    console.log("a");
  })
  .catch(function b() {
    console.log("b");
  })
  .then(function c() {
    console.log("c");
  })
  .catch(function d() {
    console.log("d");
  });
```

<details>
<summary>Output and why</summary>

```text
b
c
```

The rejection skips `a` (no rejection handler) and lands on `b`, the nearest handler (#5).
`b` returns normally, so the chain is *recovered* — fulfilled — and `c` runs (#6). `d` never
runs: there is no rejection left to catch.

</details>

### W4 — What `finally` touches

```js
Promise.resolve("v")
  .finally(function f() {
    return "swap";
  })
  .then(function t(value) {
    console.log("value:", value);
  })
  .finally(function g() {
    console.log("done");
  });
```

<details>
<summary>Output and why</summary>

```text
value: v
done
```

`finally` passes the incoming result straight through: `f`'s `"swap"` is ignored, so `t`
still sees `"v"` (#8). `g` runs either way — that is the whole job of `finally`.

</details>

### W5 — Siblings can't catch each other

```js
Promise.resolve("fine")
  .then(
    function ok() {
      throw new Error("broke in ok");
    },
    function bad(err) {
      console.log("bad:", err.message);
    }
  )
  .then(
    function next() {
      console.log("next");
    },
    function rescue(err) {
      console.log("rescue:", err.message);
    }
  );
```

<details>
<summary>Output and why</summary>

```text
rescue: broke in ok
```

`bad` watches the *incoming* promise, which is fulfilled — so it never runs. `ok`'s throw
rejects the first `.then`'s *output* promise (#4), and the next link's rejection handler
(`rescue`) is the one that sees it (#7). `next` is skipped for the same reason `bad` was.

</details>

### W6 — `await` and the order of things

```js
async function f() {
  console.log("f start");
  await null;
  console.log("f after await");
}
f();
console.log("script end");
```

<details>
<summary>Output and why</summary>

```text
f start
script end
f after await
```

An async function runs synchronously until its first `await` (#2's logic in `await` form),
then suspends *itself only* and returns control to the caller (#10). The remainder runs as a
microtask after the script ends — the queue mechanics are the
[Event Loop docs](../eventLoop/brief.md)' topic.

</details>

## Spot It

### S1 — The immortal spinner

Your page shows a spinner while loading a profile. QA reports: when the API errors, the error
toast appears correctly — but the spinner never goes away. The happy path hides it fine. The
code hides the spinner in the final `.then` of the chain. What is going on, in one sentence?

<details>
<summary>What it is</summary>

The hide-spinner call lives on the success lane only: when the chain rejects, every plain
`.then` is skipped on the way to the `.catch` (#5), so the hide never runs. Cleanup that must
happen on both lanes belongs in `.finally` (#8).

</details>

### S2 — The instant `undefined`

A teammate's `loadSettings()` "sometimes returns before the network call finishes" — callers
get `undefined` immediately, yet the settings do arrive in the network tab. What single
missing word do you expect to find in `loadSettings`?

<details>
<summary>What it is</summary>

`return`. The function starts a `.then` chain but does not return it — a broken chain (#9).
Callers get `undefined` synchronously, cannot await the result, and will never see its
errors. Fix: `return fetch(…).then(…)`.

</details>

### S3 — Only the first message (decoy)

A chat widget should update its badge on every incoming message. A teammate wrapped the socket
in a promise — `new Promise(resolve => socket.on("message", resolve))` — and awaits it in a
loop that re-renders. The badge updates exactly once, then never again. Which concept was
misapplied?

<details>
<summary>What it is</summary>

This is not a promise bug — it is the wrong tool. A promise settles exactly once (#1); the
second `resolve` call is ignored, and re-awaiting the same promise just returns the first
message again. A stream of occurrences wants an event subscription (the `EventEmitter`
pattern — on the plan in Week 5), not a promise.

</details>

### S4 — The stubborn timer (decoy)

A teammate reads your code and asks: "You scheduled `setTimeout(poll, 0)` *before* this
`.then`, so why does the `.then` callback run first? Is the promise jumping the queue — is
that a promise bug?" What do you tell them?

<details>
<summary>What it is</summary>

Nothing here is a promise bug — it is queue ordering: promise reactions are microtasks and
drain before the task queue where timer callbacks wait. The full why lives in the
[Event Loop docs](../eventLoop/brief.md), not in this topic.

</details>

### S5 — The slow profile page

A profile page awaits three API calls — avatar, posts, friends — each measured at ~600 ms on
its own, yet the page takes ~1.8 s. None of the three needs the others' data. What is the
diagnosis, and roughly what should the time become?

<details>
<summary>What it is</summary>

Sequential awaits on independent work: each promise is *created* only after the previous one
settles, so the latencies add up (#11). Create all three first (or `Promise.all` them) and
the cost becomes the maximum, ~600 ms — a 3× win for moving two lines.

</details>

### S6 — The crash behind the try/catch

A Node service crashed with an unhandled rejection — yet the offending call site reads
`try { sendReport(data); } catch (err) { log(err); }`. The author insists "it's wrapped, it
can't crash". Why did it?

<details>
<summary>What it is</summary>

`sendReport` is async, and the call is neither awaited nor chained — the returned promise
floats (#9). `try`/`catch` only catches what *throws during the call itself*; the rejection
happens later, on a promise nobody holds. Fix: `await sendReport(data)` inside the `try`
(#10), or `.catch` the returned promise.

</details>

## Build It

The real scheduled follow-ons have their own slots and are not duplicated here: "autocomplete —
debounce, cancellation, keyboard nav" (Week 4 · Tue) and "polyfills — `Promise.all`, `race`,
`allSettled`, `any`" (Week 5 · Mon). The drills below are unscheduled reps for the ideas those
builds will lean on.

### B1 — `delay(ms)`

*Unscheduled drill · Mode: from memory*

**Task.** Write `delay(ms)`: it returns a promise that fulfils (with no value) after `ms`
milliseconds. Then prove it chains: wait 200 ms, log, wait 100 more, log.
Expected: `delay(200).then(…)` logs `after 200 ms`, then 100 ms later logs `after 100 more`.

<details>
<summary>Hint 1 — where to look</summary>

Which API already waits, and which tool turns a callback API into a promise?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

This is the legitimate use of `new Promise`: promisifying `setTimeout` — and chaining the
second wait means *returning* the second `delay(…)` from the first `.then`.

</details>

<details>
<summary>Solution</summary>

**Approach.** The executor starts the timer and hands `resolve` to it as the callback.
Returning `delay(100)` from the first `.then` makes the next link adopt it — the chain waits.

```js
function delay(ms) {
  return new Promise(function run(resolve) {
    setTimeout(resolve, ms);
  });
}

delay(200)
  .then(function first() {
    console.log("after 200 ms");
    return delay(100);
  })
  .then(function second() {
    console.log("after 100 more");
  });
```

```text
after 200 ms
after 100 more
```

**Edge cases handled.** Chained delays actually accumulate (run shows both lines in order);
no value is passed — fulfilling with `undefined` is deliberate.

**Uses ideas:** #2, #3, #4

</details>

### B2 — `runSequentially` vs `runInParallel`

*Unscheduled drill · Mode: from memory*

**Task.** Given an array of *task functions* (each returns a promise when called), write
`runSequentially(tasks)` — runs them one after another — and `runInParallel(tasks)` — starts
them all at once. Both resolve to the array of results **in task order**. Demonstrate the
timing difference with three tasks of 200/150/50 ms: sequential ≈ the sum, parallel ≈ the
maximum.

<details>
<summary>Hint 1 — where to look</summary>

Why does the input hold *functions* rather than promises? What would already be true if you
were handed promises?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Work starts at *creation* (#11): sequential means "don't call the next task until the previous
result is in"; parallel means "call them all, then collect".

</details>

<details>
<summary>Solution</summary>

**Approach.** Sequential is `await` inside `for…of` — creation waits for the previous settle.
Parallel calls every task via `map` (all created now), then lets `Promise.all` collect in
order.

```js
function makeTask(name, ms) {
  return function task() {
    return new Promise(function run(resolve) {
      setTimeout(function done() {
        resolve(name);
      }, ms);
    });
  };
}

async function runSequentially(tasks) {
  const results = [];
  for (const task of tasks) {
    results.push(await task());
  }
  return results;
}

function runInParallel(tasks) {
  return Promise.all(
    tasks.map(function start(task) {
      return task();
    })
  );
}

const tasks = [makeTask("a", 200), makeTask("b", 150), makeTask("c", 50)];

async function main() {
  let start = Date.now();
  const seq = await runSequentially(tasks);
  console.log("sequential:", seq, "~" + Math.round((Date.now() - start) / 100) * 100, "ms");
  start = Date.now();
  const par = await runInParallel(tasks);
  console.log("parallel:  ", par, "~" + Math.round((Date.now() - start) / 100) * 100, "ms");
}
main();
```

```text
sequential: [ 'a', 'b', 'c' ] ~400 ms
parallel:   [ 'a', 'b', 'c' ] ~200 ms
```

(Milliseconds are rounded in code because raw timings wobble a few ms between runs.)

**Edge cases handled.** Results stay in task order even though `c` finishes first (that
ordering is `Promise.all`'s guarantee, and the sequential loop's by construction); an empty
array resolves to `[]` in both.

**Uses ideas:** #10, #11

</details>

### B3 — `retry(fn, attempts)`

*Unscheduled drill · Mode: from memory*

**Task.** Write `retry(fn, attempts)`: call `fn()` (it returns a promise); on rejection try
again, up to `attempts` total tries; if all fail, reject with the last error. A `flaky()` that
fails twice then succeeds should log two retries and then the success.

<details>
<summary>Hint 1 — where to look</summary>

Which link of a chain is allowed to *decide* between recovering and staying on the error lane?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Recovery is returning from `.catch` (#6) — and returning `retry(fn, attempts - 1)` makes the
chain adopt the next attempt (#4). Out of attempts → re-throw.

</details>

<details>
<summary>Solution</summary>

**Approach.** The whole machine is one `.catch`: re-throw when out of attempts, otherwise
return the recursive call — adoption flattens the retries into one promise the caller holds.

```js
function retry(fn, attempts) {
  return fn().catch(function again(err) {
    if (attempts <= 1) {
      throw err;
    }
    console.log("failed, retrying:", err.message);
    return retry(fn, attempts - 1);
  });
}

let calls = 0;
function flaky() {
  calls = calls + 1;
  if (calls < 3) {
    return Promise.reject(new Error("attempt " + calls + " failed"));
  }
  return Promise.resolve("worked on attempt " + calls);
}

retry(flaky, 5).then(function show(result) {
  console.log(result);
});
```

```text
failed, retrying: attempt 1 failed
failed, retrying: attempt 2 failed
worked on attempt 3
```

**Edge cases handled.** Success on a later attempt (run shown); exhausted attempts re-throw
the last error (`attempts <= 1` branch — verified by calling with `retry(flaky, 2)` against a
3-fail `flaky`); the chain is returned, so nothing floats.

**Uses ideas:** #4, #5, #6, #9

</details>

### B4 — Fix your own `myQueueMicrotask`

*Unscheduled drill · Mode: from memory*

**Task.** Your `codes/JS/practice/eventLoop.js` has
`async function myQueueMicrotask(fun) { Promise.resolve().then(fun); }` — and the deep dive
shows an error inside `fun` crashes the process even when the caller attached `.catch`.
Rewrite it (from memory, without opening the file) so `myQueueMicrotask(boom).catch(…)`
actually catches, while still running `fun` as a microtask. Do **not** edit the original file
— that log line is yours to write after a cold attempt.

<details>
<summary>Hint 1 — where to look</summary>

Follow the promise that `.then(fun)` creates. Who is holding it?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

The caller can only catch what the function *returns* — connect the inner chain to the
returned promise (#9).

</details>

<details>
<summary>Solution</summary>

**Approach.** Return the chain; the `async` keyword was doing nothing and can go. (Returning
it from an `async` function, or `await`ing it, would be equally correct — adoption either
way.)

```js
function myQueueMicrotask(fun) {
  return Promise.resolve().then(fun);
}

myQueueMicrotask(function boom() {
  throw new Error("boom");
}).catch(function rescue(err) {
  console.log("caught:", err.message);
});
console.log("scheduled");
```

```text
scheduled
caught: boom
```

**Edge cases handled.** `fun`'s throw now rejects the *returned* promise, so the caller's
`.catch` fires instead of a process crash (compare the crash run in
[deep-dive.md](deep-dive.md) → Misconceptions & Traps); `fun` still runs as a microtask —
`scheduled` prints first.

**Uses ideas:** #2, #4, #9

</details>

### B5 — `withTimeout(promise, ms)`

*Unscheduled drill · Mode: from memory*

**Task.** Pending is silent — so build the guard rail: `withTimeout(promise, ms)` resolves or
rejects with `promise`'s own result if it settles within `ms`, otherwise rejects with
`Error("timed out after <ms> ms")`. Reuse B1's `delay`. A 50 ms task under a 200 ms limit
passes through; a 500 ms task under the same limit rejects with the timeout error.

<details>
<summary>Hint 1 — where to look</summary>

You cannot stop the slow promise — but can you refuse to keep waiting for it? What already
exists for "whichever settles first wins"?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Race the real promise against a delay whose `.then` *throws* — the first settler decides the
outcome (#4: throw → reject).

</details>

<details>
<summary>Solution</summary>

**Approach.** `delay(ms).then(throw)` is a promise that rejects at the deadline;
`Promise.race` adopts whichever of the two settles first. The slow promise is not cancelled —
only no longer waited for (the limit the deep dive names; real cancellation is Week 4's
`AbortController`).

```js
function delay(ms) {
  return new Promise(function run(resolve) {
    setTimeout(resolve, ms);
  });
}

function withTimeout(promise, ms) {
  const timer = delay(ms).then(function expire() {
    throw new Error("timed out after " + ms + " ms");
  });
  return Promise.race([promise, timer]);
}

const fast = delay(50).then(function done() {
  return "fast result";
});
const slow = delay(500).then(function done() {
  return "slow result";
});

withTimeout(fast, 200).then(function show(value) {
  console.log("fast:", value);
});
withTimeout(slow, 200).catch(function rescue(err) {
  console.log("slow:", err.message);
});
```

```text
fast: fast result
slow: timed out after 200 ms
```

**Edge cases handled.** Both lanes exercised in the run (fast passes through, slow times
out); the fast path carries the real value, not a race artifact; output order was identical
across three runs.

**Uses ideas:** #1, #4, #11

</details>

## Self-Check Questions

### Q1 — One settle

Why may a promise settle only once — what downstream guarantee would break otherwise?

<details>
<summary>Answer</summary>

Every consumer of a promise trusts that "the result" is a single, final thing: reactions run
once with one value, chains transform one input (#1). If a promise could re-settle, every
handler would need re-run semantics and "current value as of when?" bookkeeping — which is
exactly what *events* are for, a different tool.

</details>

### Q2 — When the kitchen starts

`const p = new Promise(function executor(resolve) { console.log("go"); resolve(1); });` —
when does `"go"` print, and why is that the right design?

<details>
<summary>Answer</summary>

Immediately — before `new Promise` returns (#2). The executor is where the work is *started*
(timer set, request sent), and starting must be synchronous or you could not know your
request had even begun. Only the reactions are deferred.

</details>

### Q3 — Fork vs chain

What is the difference between `p.then(a); p.then(b);` and `p.then(a).then(b)`?

<details>
<summary>Answer</summary>

The first is a fork: two independent subscribers to `p`, both receiving `p`'s value (W1).
The second is a pipeline: `b` subscribes to the *new* promise `.then(a)` returned, so `b`
receives `a`'s return value (#3). Fork when both need the original; chain when the second
step depends on the first.

</details>

### Q4 — Three fates

A `.then` callback can do exactly three kinds of thing. Name them, and what each does to the
next promise.

<details>
<summary>Answer</summary>

Return a plain value → next promise fulfils with it; throw → next promise rejects with it;
return a promise (or any thenable) → next promise adopts it, waiting and copying its result
(#4). Adoption is why chains of dependent async steps stay flat.

</details>

### Q5 — Where a rejection goes

A chain has four `.then`s and one `.catch` at the end. The second `.then`'s callback throws.
What runs, what is skipped, and why doesn't the error just explode?

<details>
<summary>Answer</summary>

The third and fourth `.then`s are skipped — a rejection travels to the *nearest* rejection
handler (#5) — and the `.catch` runs with the thrown error. It doesn't explode because the
throw was converted into a rejected promise (#4), and rejection is a value moving down the
chain, not an unwinding stack.

</details>

### Q6 — Two shapes of catch

When would you use `.then(ok, bad)` instead of `.then(ok).catch(bad)` — and what does the
two-argument form *not* catch?

<details>
<summary>Answer</summary>

Use it when you want to handle the *incoming* rejection without catching `ok`'s own failures
— the two arguments are siblings watching the same incoming promise, so `bad` cannot see what
`ok` throws (#7). The chained `.catch` watches the output, so it catches both. Default to the
chained form.

</details>

### Q7 — What `finally` is for

What does a `.finally` callback receive, what happens to its return value, and what is the
one way it can change the chain's outcome?

<details>
<summary>Answer</summary>

It receives nothing and its return value is ignored — the incoming result passes through
(#8). It exists for cleanup that must happen on both lanes (hide the spinner, release the
lock). The one exception: if it throws or returns a rejecting promise, that failure replaces
the passing result — cleanup failures must not be swallowed.

</details>

### Q8 — The float

Define a floating promise, and explain why marking the surrounding function `async` does not
stop the float.

<details>
<summary>Answer</summary>

A promise created but neither returned, awaited, nor given a rejection handler — nobody holds
it, so its rejection is unhandled (#9). `async` only wires the function's *own* returns,
throws and awaits into the returned promise; an inner chain you never connect (your
`myQueueMicrotask` case) stays floating regardless.

</details>

### Q9 — What `await` really does

What does an async function give its caller, and what are the two things `await p` does with
`p`?

<details>
<summary>Answer</summary>

The caller always gets a promise — immediately, often still pending (#10). `await p` suspends
only that function until `p` settles, then either hands back the unwrapped fulfilment value
or *throws* the rejection reason — which is why `try`/`catch` works on async code.

</details>

### Q10 — The cost of the second `await`

`const a = await getA(); const b = await getB();` — the calls are independent, each ~300 ms.
What does this cost, why, and what is the fix?

<details>
<summary>Answer</summary>

~600 ms: `getB()` is not even *called* — its work not started — until `a` arrives, because
work starts at creation and `await` sits between the creations (#11). Fix: create first
(`const pa = getA(); const pb = getB();`), await after — or `Promise.all` — for ~300 ms.

</details>

## Explain It Back

### E1 — The two-minute teach

Explain promises to a new teammate in two minutes, starting from the pizza-receipt story and
ending at code they could write.

<details>
<summary>Rubric — a complete answer covers</summary>

- The receipt: an object you hold *now* for a result that arrives *later*, and you attach
  instructions to it rather than waiting (#1).
- Settles once, fulfilled or rejected — and instructions never interrupt running code (#1,
  the deferral rule).
- `.then` returns a *new* promise: chains are pipelines, return/throw/return-a-promise decide
  each link (#3, #4).
- Errors travel: skip to the nearest handler, and a handler that returns normally *recovers*
  the chain (#5, #6).
- `async`/`await` is the same thing written straight-down; rejections become throws (#10).
- Where the analogy breaks: a dropped receipt still gets you pizza; a dropped promise loses
  its errors (#9).

</details>

### E2 — "Why not just callbacks?"

Your interviewer says: "`setTimeout` and callbacks already let you run code later. What do
promises actually add?" Answer out loud.

<details>
<summary>Rubric — a complete answer covers</summary>

- A callback defers *code*; a promise is a *value* you can store, pass, and attach to after
  the fact (#1).
- Composition: chains and `Promise.all` replace nesting; dependent steps stay flat through
  adoption (#3, #4, #11).
- An error lane: rejections travel to a handler instead of being lost inside someone else's
  stack (#5) — with callbacks, every level needs its own error argument.
- The consistency guarantee: reactions are never synchronous, so ordering is predictable
  whether the result took 0 ms or 10 s (the Level 1 lesson).
- Honest limit: one-shot only — recurring occurrences still want events/callbacks.

</details>

### E3 — Defend the diff

In review, you changed a teammate's
`return new Promise((resolve) => { api.get(url).then(resolve); })` to `return api.get(url)`,
and separately turned their two stacked awaits into a `Promise.all`. They ask why. Answer
both, out loud.

<details>
<summary>Rubric — a complete answer covers</summary>

- The wrapper adds nothing: `api.get` already returns a promise; returning it directly lets
  the chain adopt it (#4).
- The wrapper *loses rejections*: the inner `.then` chain floats, the outer promise hangs
  pending forever on error — the crash run in the deep dive (#9).
- `new Promise` is for promisifying callback APIs only (B1), not for wrapping promises.
- The awaits: both requests were independent, and work starts at creation (#11) — stacked
  awaits made latencies add; `Promise.all` makes them overlap and still fails loudly if
  either fails.
- Trade-off named: fail-fast is wanted here; if partial success mattered, `allSettled`
  (Week 5) is the variant.

</details>
