# Promises — Deep Dive

> **Track:** JS · **Type:** concept · **Scheduled:** Week 3 · Wed · **Runtime:** Node v24 · **Generated:** 2026-10-24

## Why It Exists

You already know, from the event loop, that slow work is handed to the host and its callback
runs later ([Event Loop deep dive](../eventLoop/deep-dive.md) → Level 2). Here is the problem
that knowledge leaves open: **an async result cannot be returned.**

```js
let pizza;
setTimeout(function deliver() {
  pizza = "margherita";
}, 0);
console.log("dinner is:", pizza);
```

```text
dinner is: undefined
```

The log runs while the stack is still busy with the script; `deliver` has not run yet. So the
only way to *use* an async result is to push your next step *into* the callback — and the next
step after that into *its* callback. Three dependent steps become three levels of nesting, each
level needing its own error handling, because a `throw` inside `deliver` would land in nobody's
`try/catch` — the code that scheduled it finished long ago.

A promise is the fix: instead of handing your next step to the slow function, the slow function
hands **you an object** that stands for the future result. You attach your next steps to that
object, flat, one after another, and errors travel along the same object instead of getting
lost. The rest of this doc is how that object actually behaves.

## Level 1 — Intuition

The story is the pizza order from the [brief](brief.md) → Explain It Like I'm 5: you order,
and you immediately get a **receipt**. The receipt is not the pizza — it is a thing you can
hold *now* that stands for food arriving *later*. And you can leave instructions on it: "when
it arrives, slice it."

```js
const order = Promise.resolve("pizza");
order.then(function slice(food) {
  console.log("slicing the", food);
});
console.log("setting the table");
```

```text
setting the table
slicing the pizza
```

Two things to see, and both are the whole topic in miniature:

```text
            you                         the receipt (order)
             │                                 │
   order ────┼────────────────────────────────▶│  holds: "pizza" (already there!)
             │                                 │
   .then ────┼── "slice it when it arrives" ──▶│  instruction stored
             │                                 │
   set the table (your own work continues)     │
             │                                 │
   stack empty ────────────────────────────────▶ slice runs, gets "pizza"
```

First: getting the receipt and leaving an instruction takes no time — your own work
("setting the table") continues immediately. Second: even though the pizza was ready the
moment we ordered (`Promise.resolve` makes a receipt whose value is already there), `slice`
still waited its turn. An instruction on a receipt **never** interrupts what you are doing.

**Recap:** a promise is an object you hold now for a value that arrives later; you attach
instructions to it with `.then`. Instructions run after your current code finishes, never
during it.

**Check yourself (C1):** in the snippet above, replace `Promise.resolve("pizza")` with a
promise that takes 2 seconds. Which of the two printed lines changes, and which stays?

<details>
<summary>Answer</summary>

"setting the table" is unchanged — it never depended on the pizza. Only "slicing the pizza"
moves 2 seconds later. That is the point of the receipt: the code that does not need the
value never waits for it.

</details>

## Level 2 — How It Actually Works

### The object is a tiny state machine

*(Idea 1)* A promise is always in exactly one of three states: **pending** (the pizza is not
here yet), **fulfilled** (it arrived, and the promise holds the value), or **rejected** (it is
not coming, and the promise holds the reason — normally an `Error`). Moving from pending to
either result is called **settling**, and it happens **exactly once**. Whoever settles it
first wins; every later attempt is silently ignored:

```js
const p = new Promise(function executor(resolve, reject) {
  resolve("first");
  resolve("second");
  reject(new Error("too late"));
});
p.then(function show(value) {
  console.log("settled with:", value);
});
```

```text
settled with: first
```

If it were otherwise — if a promise could change its mind — nothing downstream could trust it:
every handler would have to ask "which value, and as of when?". One order, one outcome, is the
contract everything else is built on.

*(Idea 2)* The function you pass to `new Promise` is the **executor** — the kitchen. It runs
**synchronously, immediately**, before `new Promise` even returns. Only the *reactions* — the
instructions left with `.then` — are deferred:

```js
console.log("before");
const p = new Promise(function executor(resolve) {
  console.log("inside the executor");
  resolve("done");
});
p.then(function react(value) {
  console.log("react:", value);
});
console.log("after");
```

```text
before
inside the executor
after
react: done
```

The executor is where the slow work is *started* (the timer set, the request sent) — and
starting work is synchronous. If the executor were deferred too, you could not know when your
request had even begun.

### Chaining: every `.then` makes a new promise

*(Idea 3)* `.then` does not change the promise it is called on — it **returns a brand-new
promise** representing "the original result, after my callback has transformed it":

```js
const p = Promise.resolve(1);
const q = p.then(function same(value) {
  return value;
});
console.log(p === q);
```

```text
false
```

So a chain is a **pipeline**: each link takes the previous link's output as its input. This is
the running example in full — order it, slice it, share it, with a fallback and a cleanup:

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

Each arrow below is a *different* promise — five links, five promises, made before any of them
has settled:

```text
order ─then(slice)─▶ p1 ─then(share)─▶ p2
   p2 ─catch(fallback)─▶ p3 ─finally(cleanUp)─▶ p4 ─then(eat)─▶ p5
```

*(Idea 4)* What decides each new promise's fate? **The callback's own outcome** — one of
exactly three things:

| The callback…               | …makes the next promise                  |
| --------------------------- | ----------------------------------------- |
| returns a plain value       | fulfilled with that value                 |
| throws                      | rejected with what was thrown             |
| returns a promise           | **adopt** it — wait, then copy its result |

```js
Promise.resolve(1)
  .then(function double(n) {
    return n * 2;
  })
  .then(function adopt(n) {
    return Promise.resolve(n + 10);
  })
  .then(function report(n) {
    console.log("value:", n);
    throw new Error("fail");
  })
  .then(function skipped() {
    console.log("never runs");
  })
  .catch(function handle(err) {
    console.log("caught:", err.message);
  });
```

```text
value: 12
caught: fail
```

The third row is the quiet superpower: returning a promise does **not** give the next link a
promise-in-a-promise — the chain waits and passes the *unwrapped* value through. This
**flattening** is why dependent async steps stay flat instead of nesting. If it were
otherwise, every chained request would hand you `Promise<Promise<…>>` and you would be back
to the nesting this topic exists to remove. (`Promise<Promise<…>>` is written here as prose —
in code you never see it, precisely because of adoption.)

### The error path: same chain, other lane

*(Idea 5)* Now break the running example — same chain, rejected start:

```js
const order = Promise.reject(new Error("oven broke"));

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
making sandwiches instead
cleaning the table
eating: sandwiches
```

Trace it link by link — the reason column is the lesson:

```text
prints: setting the table · making sandwiches instead · cleaning the table
        · eating: sandwiches

link               incoming state          what happens                outgoing state
.then(slice)       rejected (oven broke)   no rejection handler: SKIP  rejected (same)
.then(share)       rejected (oven broke)   skip again                  rejected (same)
.catch(fallback)   rejected (oven broke)   runs; returns "sandwiches"  fulfilled
.finally(cleanUp)  fulfilled (sandwiches)  runs; return value ignored  fulfilled (same)
.then(eat)         fulfilled (sandwiches)  runs; logs the meal         fulfilled (undef)
```

A rejection **skips every link that has no rejection handler** and lands on the nearest one
that does. It does not explode, and it does not vanish — it travels. If it reaches the end of
the chain unhandled, the environment steps in (see Variants below — in Node that is a crash).

*(Idea 6)* And look at what `.catch` did to the chain's state: it ran, returned normally, and
the chain came out **fulfilled**. `.catch` is not a terminator — it is **recovery**. Handling
the error means the problem is dealt with; downstream links get the fallback value and carry
on:

```js
Promise.reject(new Error("first try failed"))
  .catch(function recover(err) {
    console.log("recovering from:", err.message);
    return "fallback value";
  })
  .then(function continueOn(value) {
    console.log("chain continues with:", value);
  });
```

```text
recovering from: first try failed
chain continues with: fallback value
```

If it were otherwise — if `.catch` ended the chain — you could never *fix* an error (retry,
substitute a default, show a cached result); you could only report it. To stay on the error
lane deliberately, re-`throw` inside the `.catch`.

**Recap:** a promise settles once; the executor runs now, reactions run later. Each `.then`
returns a new promise whose fate is decided by the callback: return → fulfil, throw → reject,
return a promise → adopt its result. Rejections skip to the nearest rejection handler, and a
handler that returns normally puts the chain back on the success lane.

**Check yourself (C2):** in the rejected run above, move the `.catch(fallback)` to the very
end of the chain, after `.then(eat)`. What prints now, and what does `eat` receive?

<details>
<summary>Answer</summary>

Only `setting the table`, `cleaning the table`, and `making sandwiches instead` print — in
that order. `slice`, `share` and `eat` are all skipped: the rejection travels past every one
of them (none has a rejection handler) to the `.catch` at the end. `eat` receives nothing
because it never runs. `finally` still runs on the way past — it runs either way. One trap in
one move: recovery only helps the links *after* it.

</details>

## The Analogy, Mapped

| In the story                                  | In reality                                     | Idea # |
| --------------------------------------------- | ---------------------------------------------- | ------ |
| Ordering a pizza and getting a receipt        | Calling an async function, getting a promise   | 1      |
| The kitchen starts cooking as you order       | The executor runs synchronously                | 2      |
| The receipt: not food, but stands for it      | The promise object: not the value, holds state | 1      |
| "When it arrives: slice, then share"          | A `.then` chain — each step a new receipt      | 3, 4   |
| "If it never comes, make sandwiches"          | `.catch` — recovery, and dinner still happens  | 5, 6   |
| "Either way, clean the table"                 | `.finally` — runs on both lanes                | 8      |
| The pizza arrives once; no second delivery    | A promise settles exactly once                 | 1      |

**Where the analogy breaks:**

- A receipt is passive — you check it. A promise *calls you*: the reaction is scheduled the
  moment it settles; nobody polls.
- You can lose a receipt and still get the pizza by waiting at the counter. A promise you drop
  (a **floating promise**) takes its errors down with it — there is no counter (Idea 9).
- Real kitchens update you ("10 minutes left"). A promise has no progress — pending carries no
  information at all, which is one of its real limits (see Limits & Trade-offs).
- Receipts can be refunded or cancelled. A plain promise cannot be cancelled from outside —
  cancellation needs extra machinery (`AbortController`, coming with the Week 4 autocomplete).

## Level 3 — The Formal Picture

The precise vocabulary, and the three pieces Level 2 postponed. The terms below are the ones
the ECMAScript specification uses (it defines `Promise` objects, their fulfilled/rejected/
pending states, and the job queue their reactions run on); MDN's promise reference uses the
same names.

- A promise's reactions run as **microtasks**: when a promise settles, each registered
  reaction is queued, and the queue drains whenever the call stack empties — before any timer
  or click gets a turn. That ordering is the previous topic, not this one — see the
  [Event Loop deep dive](../eventLoop/deep-dive.md) → Level 2 and Level 3. What matters here
  is one consequence, and it held even for the already-settled `Promise.resolve("pizza")` in
  Level 1: **a reaction never, ever runs synchronously.**
- **Adoption** (Idea 4's third row) is specified for any **thenable** — any object with a
  `.then` method — not just real promises. That is how promises from different libraries (or
  an `await` on a third-party "promise-like") interoperate.

*(Idea 7)* `.catch(f)` is nothing special — it is literally shorthand for
`.then(undefined, f)`: a link with no success handler and a rejection handler. Which exposes a
real difference between the two ways of writing error handling. The two arguments of one
`.then` are **siblings** — they watch the *incoming* promise, so the second argument cannot
catch what the first one throws. A `.catch` *after* the `.then` watches the `.then`'s
*output* — so it catches both:

```js
Promise.resolve("ok")
  .then(
    function work() {
      throw new Error("thrown inside work");
    },
    function sibling(err) {
      console.log("second argument saw:", err.message);
    }
  )
  .catch(function outer(err) {
    console.log("outer catch saw:", err.message);
  });
```

```text
outer catch saw: thrown inside work
```

`sibling` never runs — the incoming promise was *fulfilled*. The throw inside `work` rejects
the `.then`'s output promise, and only the downstream `.catch` sees it. Default to
`.then(ok).catch(bad)`; reach for the two-argument form only when you specifically want to
handle the incoming rejection *without* catching your own success handler's errors.

*(Idea 8)* `.finally(f)` (ES2018) is the cleanup lane: `f` runs whether the chain is on the
success or the error lane, receives **no argument** (it is not supposed to know), and its
return value is **ignored** — the incoming result passes straight through:

```js
Promise.resolve("the value")
  .finally(function tidy() {
    console.log("tidy runs");
    return "this is ignored";
  })
  .then(function next(value) {
    console.log("still:", value);
  });
```

```text
tidy runs
still: the value
```

It is an observer, not a transformer — hide the spinner, close the connection, release the
lock. The one exception: if `f` itself throws (or returns a promise that rejects), *that*
rejection replaces the passing result — a cleanup step is allowed to fail, and its failure
must not be swallowed.

### async/await: the same chain, written downwards

*(Idea 10)* `async`/`await` (ES2017) adds no new machinery — it is syntax over exactly the
chains above. Three rules cover all of it:

1. An `async` function **always returns a promise** — whatever you `return` fulfils it,
   whatever you `throw` rejects it (Idea 4, wearing syntax).
2. `await p` pauses **only this async function** and unwraps `p`: the function's remainder
   becomes, in effect, the `.then` reaction of `p`. The caller and the rest of the program
   keep running.
3. If `p` rejects, `await p` **throws** that reason — which is what lets ordinary
   `try`/`catch` work on async code (it is `.catch`, wearing syntax).

```js
async function main() {
  try {
    const value = await Promise.reject(new Error("nope"));
    console.log("never reached:", value);
  } catch (err) {
    console.log("caught in try/catch:", err.message);
  }
  return "finished";
}
console.log("before the call");
const result = main();
console.log("main returned:", result);
```

```text
before the call
main returned: Promise { <pending> }
caught in try/catch: nope
```

All three rules in one run: the caller got a still-pending promise back (rules 1 and 2 — the
call did not block), and the rejection surfaced as a throwable inside the function (rule 3).
And rule 2's "only this function":

```js
async function slow() {
  await new Promise(function wait(resolve) {
    setTimeout(resolve, 100);
  });
  console.log("slow is done");
}
slow();
console.log("the rest of the program keeps running");
```

```text
the rest of the program keeps running
slow is done
```

*(Idea 11)* The sharpest consequence of `await`, and Wednesday's second bullet: **sequential
vs parallel is decided when the promise is created, not when it is awaited.** A promise starts
its work at creation (Idea 2 — the executor runs immediately). `await` only decides when *you
wait*. So two awaits with the creations in between run one-after-another; create both first
and the waits overlap:

```js
function step(name, ms) {
  return new Promise(function run(resolve) {
    setTimeout(function done() {
      resolve(name);
    }, ms);
  });
}

async function sequential() {
  const start = Date.now();
  const a = await step("a", 300);
  const b = await step("b", 300);
  const rounded = Math.round((Date.now() - start) / 100) * 100;
  console.log("sequential:", a, b, "~" + rounded, "ms");
}

async function parallel() {
  const start = Date.now();
  const pa = step("a", 300);
  const pb = step("b", 300);
  const a = await pa;
  const b = await pb;
  const rounded = Math.round((Date.now() - start) / 100) * 100;
  console.log("parallel:  ", a, b, "~" + rounded, "ms");
}

sequential().then(parallel);
```

```text
sequential: a b ~600 ms
parallel:   a b ~300 ms
```

(The milliseconds are rounded in the code because raw timings wobble by a few ms between
runs; the shape — two waits vs the longest single wait — is stable, and this output was
identical across three runs.) Sequential cost is the **sum** of the latencies; parallel cost
is the **maximum**. Use sequential only when step B needs step A's result. For a *list* of
independent promises, `Promise.all` is the standard way to await them together — with one
sharp edge: it is **fail-fast**, rejecting the moment any input rejects:

```js
function step(name, ms) {
  return new Promise(function run(resolve) {
    setTimeout(function done() {
      resolve(name);
    }, ms);
  });
}

Promise.all([step("one", 100), step("two", 50), step("three", 10)])
  .then(function show(values) {
    console.log("all of them:", values);
  });

Promise.all([step("one", 100), Promise.reject(new Error("bad")), step("three", 10)])
  .then(function show(values) {
    console.log("never runs:", values);
  })
  .catch(function rescue(err) {
    console.log("all rejected:", err.message);
  });
```

```text
all rejected: bad
all of them: [ 'one', 'two', 'three' ]
```

(The rejected `all` prints first because it settles immediately; the fulfilled one waits for
its slowest member — the order of those two lines is itself Idea 11.) Writing `Promise.all`
and its siblings yourself is Week 5 Monday's own topic; here you only need to *choose* it.

**Recap:** `.catch` is `.then(undefined, f)`, and the two-argument form watches the incoming
promise while a chained `.catch` watches the output. `finally` observes and passes through.
`async` functions return promises, `await` unwraps and throws rejections, and work starts at
creation — so start everything you can before you await anything.

**Check yourself (C3):** `const a = await getA(); const b = await getB(a);` — can these two
awaits be parallelised the way `sequential` was rewritten into `parallel`?

<details>
<summary>Answer</summary>

No — `getB(a)` needs `a` as its input, so its promise cannot be *created* until `getA()` has
settled. Sequential awaits are wrong only when the steps are independent. The test is always
dataflow: does the second creation need the first result? If yes, sequential is correct, not
a mistake.

</details>

## Variants & Environment Differences

| Variant or environment            | What changes                                                                                  | Since / where                     |
| --------------------------------- | --------------------------------------------------------------------------------------------- | --------------------------------- |
| `async`/`await` syntax            | Chains written as straight-line code; same machinery underneath                               | ES2017                            |
| `.finally`                        | Not available before; cleanup needed a duplicated handler on both lanes                        | ES2018                            |
| Top-level `await`                 | `await` outside any function — in ES modules only, never in plain scripts                      | ES2022 (modules)                  |
| Unhandled rejection — Node        | Crashes the process (observed on Node v24: `Error: boom`, exit 1); default since Node 15 (recalled — verify) | Node                              |
| Unhandled rejection — browsers    | No crash: a console error, plus an `unhandledrejection` event on `window`                      | All current browsers (per MDN)    |
| `queueMicrotask(fn)` vs `.then`   | Same queue, no promise created — the cleaner tool when you want *only* the scheduling          | See [Event Loop](../eventLoop/deep-dive.md) → Level 3 |

## Compared With What You Know

Two things promises get mistaken for — both already in your repo.

**`setTimeout(fn, 0)` — "I'll just defer it with a timer"** (you have used both in
`codes/JS/practice/eventLoop.js`):

| Aspect                   | `setTimeout(fn, 0)`                          | `promise.then(fn)`                                |
| ------------------------ | --------------------------------------------- | -------------------------------------------------- |
| What it defers *to*      | The task queue — after render, clicks, I/O    | The microtask queue — before any of those          |
| Carries a value?         | No — `fn` gets nothing                        | Yes — `fn` receives the settled value              |
| Carries errors?          | No — a throw inside `fn` is on its own        | Yes — rejection travels the chain (Idea 5)         |
| Composable?              | No — nesting is the only sequencing           | Yes — chaining, `await`, `Promise.all`             |

Tell them apart: a timer is *scheduling only*; a promise is scheduling **plus** a value and an
error lane. (The queue-ordering half of this story lives in the
[Event Loop docs](../eventLoop/brief.md), not here.)

**Events / `EventEmitter`** (your Week 5 polyfill list has an `EventEmitter` on it):

| Aspect              | An event (`emitter.on`, DOM events)        | A promise                                     |
| ------------------- | ------------------------------------------ | ---------------------------------------------- |
| How many times      | Fires zero to many times                   | Settles exactly once (Idea 1)                  |
| Subscribing late    | Missed it — past events are gone           | Still works — the reaction runs with the value |
| Stands for          | A *stream* of occurrences                  | One *result*                                   |

Tell them apart: ask "can this happen again?" A click can; a response to one request cannot.
Many occurrences → event. One result → promise.

## Where It's Used

- **`fetch`** — the browser's (and Node's) HTTP API returns a promise of the response; every
  loading state you have ever built sits on Ideas 3–6. *Documented* — MDN's `fetch` reference.
- **`fs/promises` and friends in Node** — Node ships promise versions of its core callback
  APIs, precisely because chains compose and callbacks do not. *Documented* — Node.js API docs.
- **Dynamic `import()`** — code-splitting works by `import("…")` returning a promise of the
  module; it is how `React.lazy` loads a chunk on demand. *Documented* — ECMAScript / MDN.
- **Your own code** — `codes/JS/practice/eventLoop.js` uses `Promise.resolve().then(fun)` to
  schedule a microtask: a real use of Idea 2 + the microtask rule (and the file also stars in
  Misconceptions below).
- **The Week 4 autocomplete** — a debounced `fetch` per keystroke, where dropping stale
  responses is the hard part; it is Ideas 9 and 11 under interview conditions. *Illustrative*
  (scheduled: "autocomplete — debounce, cancellation, keyboard nav", Week 4 · Tue).

## Misconceptions & Traps

**"Wrapping it in an `async` function makes the error catchable from outside."** — from your
own `codes/JS/practice/eventLoop.js`:

*Why it seems right:* an `async` function returns a promise, and a rejected promise can be
caught — so surely whatever goes wrong inside comes out through that promise.

*Why it is wrong:*

```js
// the helper from codes/JS/practice/eventLoop.js
async function myQueueMicrotask(fun) {
  Promise.resolve().then(fun);
}

myQueueMicrotask(function boom() {
  throw new Error("boom");
}).catch(function rescue() {
  console.log("caught it");
});
```

```text
Error: boom
```

The `.catch` never runs — the process crashes with an unhandled rejection instead (that
`Error: boom` is the crash, not a log).

*The correct model:* the returned promise only carries what the function `return`s, `throw`s
or **awaits**. `Promise.resolve().then(fun)` is created and *dropped* — a **floating
promise** *(Idea 9)*. Its rejection belongs to it alone, and nobody is holding it. One word
fixes it: `return Promise.resolve().then(fun)` (or `await` it). The same bug in chain form —
the forgotten `return` — is the most common promise bug in review:

```js
function loadAndFormat() {
  Promise.resolve("data").then(function format(d) {
    return d.toUpperCase();
  });
}
console.log("returned:", loadAndFormat());
```

```text
returned: undefined
```

The caller gets `undefined` instantly, and can neither wait for the result nor see its errors.
If a function does async work, **return the chain**.

---

**"A settled promise's `.then` runs immediately."**

*Why it seems right:* the value is already there — what is there to wait for?

*Why it is wrong:*

```js
const done = Promise.resolve("ready");
done.then(function react(value) {
  console.log("react:", value);
});
console.log("after subscribing");
```

```text
after subscribing
react: ready
```

*The correct model:* reactions are *always* queued as microtasks, settled or not — "never
synchronous" has no exceptions. The guarantee is about *consistency*: code after the `.then`
always runs first, whether the promise took zero seconds or ten. Why the queue drains when it
does is the [Event Loop docs](../eventLoop/deep-dive.md)' story.

---

**"`forEach(async …)` waits for the awaits inside."**

*Why it seems right:* there is an `await` right there in the callback, and `await` waits.

*Why it is wrong:*

```js
const names = ["asha", "ravi"];
async function shout(name) {
  return name.toUpperCase();
}
async function main() {
  const out = [];
  names.forEach(async function collect(name) {
    out.push(await shout(name));
  });
  console.log("after forEach:", out);
}
main();
```

```text
after forEach: []
```

*The correct model:* `await` pauses only its own function (Idea 10) — here, each `collect`.
`forEach` neither awaits its callback's returned promises nor returns them; they float
(Idea 9). Use `for…of` with `await` for sequential work, or `Promise.all(names.map(…))` for
parallel (Idea 11).

---

**"To return a promise, I should build one with `new Promise`."**

*Why it seems right:* the constructor is how promises are made — wrapping feels explicit and
safe.

*Why it is wrong:*

```js
// WRONG — wraps a promise in another promise and loses its rejection
function fetchUser() {
  return Promise.reject(new Error("db down"));
}
function getUser() {
  return new Promise(function executor(resolve) {
    fetchUser().then(function pass(user) {
      resolve(user);
    });
  });
}
getUser()
  .then(function show(user) {
    console.log("user:", user);
  })
  .catch(function rescue(err) {
    console.log("caught:", err.message);
  });
```

```text
Error: db down
```

Another crash: `fetchUser`'s rejection lives on the inner, floating chain; the outer promise
just stays pending forever, so `rescue` never fires.

*The correct model:* `new Promise` is for **promisifying** — wrapping an API that does *not*
speak promises (like `setTimeout`). If you already have a promise, return it: the one-line
`getUser` is `return fetchUser();`, and rejection flows correctly through adoption (Idea 4).

---

**"`await` blocks the program — it's synchronous again."**

*Why it seems right:* inside the function, the next line really does not run until the value
is there; it reads exactly like blocking code.

*Why it is wrong:* the `slow()` run in Level 3 — "the rest of the program keeps running"
printed before `slow is done`.

*The correct model:* `await` suspends *one function* and returns control to the caller
(Idea 10). Everything else — other handlers, other requests, rendering in a browser — carries
on. It is cooperative waiting, not blocking. (CPU-heavy work inside an async function still
freezes everything, though — `await` moves *when* code runs, never *where*; the single thread
is the [Event Loop docs](../eventLoop/brief.md)' first lesson.)

---

**"`.catch` ends the chain."**

*Why it seems right:* in `try`/`catch` thinking, `catch` is where control flow *ends up* —
the aftermath.

*Why it is wrong:* the `recover` run in Level 2 — `chain continues with: fallback value`.

*The correct model:* `.catch` is a transformer like any other link (Idea 6): return → back on
the success lane, re-throw → stay on the error lane. That choice — recover or re-throw — is a
*decision*, and interviewers probe whether you know you are making it.

## Limits & Trade-offs

- **One value, then done.** A promise cannot represent a stream — progress updates, repeated
  clicks, a socket's messages. Forcing those into promises means a new promise per occurrence
  and missed events in between; use events (or an `EventEmitter`, Week 5) instead.
- **No cancellation built in.** Once created, the work runs; you can ignore the result but not
  stop the request. Real cancellation pairs promises with `AbortController` — which the Week 4
  autocomplete build needs for stale responses, and where it is taught.
- **Pending is silent.** There is no "how long so far?", no timeout, no partial result. A
  timeout is something you build (a race between the work and a timer — a B-item in
  [practice](practice.md)).
- **Each link is an allocation and a microtask hop.** A chain of N `.then`s creates N promise
  objects — counted, not measured: one per call, by Idea 3. Irrelevant at request scale
  (a handful per user action), real in a hot loop at tens of thousands per second.
- **Fail-fast `Promise.all`.** One rejection loses all the other results. When partial success
  is useful, `Promise.allSettled` exists — written and compared properly in Week 5's slot.

Verdict: for "one async result, possibly dependent on other async results", promises — almost
always as `async`/`await` — are simply the platform's answer, and every alternative is worse.
The judgement that remains is at the edges: streams of occurrences are events, cancellation
needs an `AbortController`, and parallelism is yours to choose at creation time (Idea 11).

## Interview Notes

- **The opener** is rarely "what is a promise" at 2 YOE — it is a predict-the-output snippet
  mixing `.then` chains with a timer (this topic plus the event loop), or "this code has a
  bug, find it" on a forgotten `return`. The plan's own phrasing for the slot — "chaining,
  error propagation, `finally`, common footguns" — is almost a transcript of the round.
- **Standard follow-up ladder:** what does `.then` return? (a new promise — Idea 3) → what if
  the callback throws? (the returned promise rejects — Idea 4) → difference between
  `.then(ok, bad)` and `.then(ok).catch(bad)`? (sibling vs downstream — Idea 7) → how do you
  run three calls in parallel and fail if any fails? (`Promise.all` — Idea 11) → and if you
  want the successes anyway? (`allSettled` — name it and say you would reach for it; writing
  it is Week 5).
- **The senior tell** is unprompted error-path thinking: mentioning what happens to the chain
  *after* the `.catch`, spotting a floating promise, asking whether two awaits are actually
  dependent before serialising them.
- **The two-minute version, out loud:** a promise is an object standing for one future result;
  it settles once, fulfilled or rejected. `.then` returns a *new* promise, so chains are
  pipelines: return a value and it flows on, throw and it switches to the error lane, return a
  promise and the chain waits for it. Rejections skip to the nearest handler, and a handler
  that returns recovers the chain. `async`/`await` is the same thing written straight-down —
  the function returns a promise, `await` unwraps and throws rejections. And because work
  starts at creation, you choose parallel vs sequential by *where you create*, not where you
  await — then finish with the pizza receipt if they want it ELI5.
