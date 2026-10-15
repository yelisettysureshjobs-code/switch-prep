# `this` in Every Context — Practice

> **Track:** JS · **Type:** concept · **Scheduled:** Week 2 · Mon · **Runtime:** Node v22 · **Generated:** 2026-10-13

## How to Use This File

Every output, hint, answer and rubric below is collapsed — commit to your own answer before opening one. Warm-Up and Spot It are done in your head; Build It is typed from a blank file, from memory; Explain It Back is said out loud. The method for the track is in [how-to-study.md](../../../prep/how-to-study.md). Log what you got wrong in `prep/notes.md`, in your own words.

## Warm-Up

For each snippet: say what it prints, and which rule decides `this` on each call.

### W1 — Two objects, one method

```js
"use strict";
const user = {
    name: "Asha",
    address: {
        city: "Pune",
        show() {
            console.log(this.name, this.city);
        },
    },
};
user.address.show();
```

<details>
<summary>Output and why</summary>

```text
undefined Pune
```

Only the object directly left of the final dot counts (Idea 2). That is `user.address`, which has a `city` but no `name`. `user` is not involved, and nothing throws because `this` is still an object.

</details>

### W2 — Borrowed

```js
"use strict";
const a = {
    name: "a",
    who() {
        return this.name;
    },
};
const b = { name: "b" };

b.who = a.who;
console.log(b.who());

b.whoBound = a.who.bind(a);
console.log(b.whoBound());
```

<details>
<summary>Output and why</summary>

```text
b
a
```

`b.who()` is a dot call, so `this` is `b` — where the function was written does not matter (Ideas 1, 5). `whoBound` was fixed to `a` with `bind`, and `bind` ranks above the dot (Ideas 10, 12).

</details>

### W3 — A function that returns a function

```js
"use strict";
const counter = {
    count: 5,
    regular() {
        return function () {
            return this;
        };
    },
    arrow() {
        return () => this.count;
    },
};
console.log(counter.regular()());
console.log(counter.arrow()());
```

<details>
<summary>Output and why</summary>

```text
undefined
5
```

`counter.regular()` returns a regular function, and the second `()` calls it on its own: a plain call, so `undefined` in strict mode (Idea 3). `counter.arrow()` runs with `this` as `counter`, and the arrow it returns has no `this` of its own, so it uses that one (Idea 7).

</details>

### W4 — Made elsewhere

```js
"use strict";
function make() {
    return {
        name: "inner",
        get: () => this.name,
    };
}
const made = make.call({ name: "outer" });
console.log(made.get());
```

<details>
<summary>Output and why</summary>

```text
outer
```

The arrow is written inside `make`, and the object literal around it is not a scope (Idea 8). So its `this` is `make`'s `this`, which `call` set to the `outer` object (Idea 10). The dot in `made.get()` has no effect on an arrow (Idea 7).

</details>

### W5 — Tax

```js
class Cart {
    constructor() {
        this.tax = 2;
        this.prices = [10, 20];
    }
    totals() {
        return this.prices.map(function (p) {
            return p * this.tax;
        });
    }
}
try {
    console.log(new Cart().totals());
} catch (e) {
    console.log(e.constructor.name + ": " + e.message);
}
```

<details>
<summary>Output and why</summary>

```text
TypeError: Cannot read properties of undefined (reading 'tax')
```

`totals` has the right `this`, but the callback is a regular function that `map` calls on its own. There is no `"use strict"` line, yet `this` is `undefined` rather than the global object, because class code is always strict (Idea 9). An arrow callback, or `map(fn, this)`, fixes it.

</details>

## Spot It

For each scenario: within 60 seconds, name what is going on. No code.

### S1 — The clock that never moves

A `Clock` class has a `tick()` method that does `this.seconds++`. Its `start()` method calls `setInterval(this.tick, 1000)`. In the browser nothing throws, but `clock.seconds` stays at `0` forever.

<details>
<summary>Answer</summary>

Lost `this` in a callback (Ideas 5, 11). `this.tick` is read out of the object and handed over bare; the timer calls it with its own `this`, so `seconds++` runs against the wrong object and silently produces `NaN` there. Fix: `setInterval(() => this.tick(), 1000)` or bind it.

</details>

### S2 — The config object

A teammate writes `const api = { base: "https://x.dev", url: () => this.base + "/users" }`. `api.url()` returns `"undefined/users"`.

<details>
<summary>Answer</summary>

An arrow used as an object method (Idea 8). The object literal is not a scope, so the arrow's `this` is the surrounding one, not `api`. Use method syntax: `url() { return this.base + "/users"; }`.

</details>

### S3 — Three threes

A loop `for (var i = 0; i < 3; i++) { setTimeout(function () { console.log(i); }, 0); }` prints `3 3 3`. A teammate says "it's a `this` problem, use an arrow function".

<details>
<summary>Answer</summary>

Decoy — this is not about `this` at all; the callback never uses it. It is a closure over one shared `var` (see your closures notes, "Closures in Loops"). An arrow changes nothing; `let` does.

</details>

### S4 — The list that cannot find its item

One click listener is attached to a `ul`, written as a regular function. It reads `this.dataset.id` to find which `li` was clicked and always gets `undefined`.

<details>
<summary>Answer</summary>

Decoy — `this` is behaving exactly as designed: in a regular listener it is the element the listener is attached to, the `ul`, the same as `event.currentTarget`. The bug is using it where `event.target.closest("li")` is needed. This is target vs currentTarget, from your DOM events notes.

</details>

### S5 — The wrong name

A function `print()` logs a variable `name`. It is called from inside `run()`, which has its own local `name`, yet it prints the global one. A teammate suggests `print.call(run)`.

<details>
<summary>Answer</summary>

Decoy — lexical scope. A variable is found by where the function is written, and no call form can change that. `call` sets `this`, which `print` does not use.

</details>

### S6 — The listener that will not leave

A component adds a listener with `el.addEventListener("click", this.onClick.bind(this))` and later calls `el.removeEventListener("click", this.onClick.bind(this))`. The handler keeps firing.

<details>
<summary>Answer</summary>

The cost of a fix, not the rule itself (Idea 13). Every `bind` call returns a new function, so the one being removed is not the one that was added. Bind once in the constructor, or use an arrow class field, and pass that same reference both times.

</details>

## Build It

Nothing on the plan is scheduled around this topic except its two follow-ons — call / apply / bind (Week 2 · Wed) and prototypes (Week 2 · Fri) — which get their own docs. These three tasks exercise what those will build on.

### B1 — Keep the stopwatch running

*Not on the plan · Mode: from memory*

**Task.** A `Stopwatch` class has `seconds = 0` and a `tick()` method that adds one. A helper `runTwice(fn)` calls `fn()` two times. Passing `sw.tick` to `runTwice` throws. Make it work in **three different ways** without changing `runTwice`, and for each way say what it costs. After using all three once each, `sw.seconds` must be `6`.

<details>
<summary>Hint 1 — where to look</summary>

Inside `runTwice`, what does the call `fn()` look like? Which of the four call forms is it?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Either hand over a function that still makes a dot call itself, or hand over a function whose `this` can no longer be changed — there are two ways to create the second kind.

</details>

<details>
<summary>Solution</summary>

**Approach.** `fn()` is a plain call, so the method must not depend on the call for its `this`. An arrow wrapper keeps the dot call; `bind` fixes `this` once per instance; an arrow class field never had its own `this`.

```js
"use strict";
class Stopwatch {
    constructor() {
        this.seconds = 0;
        this.tickBound = this.tick.bind(this);
    }
    tick() {
        this.seconds++;
    }
    tickField = () => {
        this.seconds++;
    };
}

function runTwice(fn) {
    fn();
    fn();
}

const sw = new Stopwatch();
try {
    runTwice(sw.tick);
} catch (e) {
    console.log("broken: " + e.message);
}
runTwice(() => sw.tick());
runTwice(sw.tickBound);
runTwice(sw.tickField);
console.log(sw.seconds);
```

```text
broken: Cannot read properties of undefined (reading 'seconds')
6
```

**Costs.** Arrow wrapper: a new function each time it is passed, so no stable reference. `bind`: one extra function per instance. Class field: one function per instance instead of one shared method. The comparison table is in [Deep Dive](deep-dive.md) → Limits & Trade-offs.

**Edge cases handled.** The unfixed call is shown failing first, so the fixes are measured against a real failure.

**Uses ideas:** #5, #9, #13

</details>

### B2 — `logCalls(fn)`

*Not on the plan · Mode: from memory*

**Task.** Write `logCalls(fn)`. It returns a new function that prints `calling <name> with <args>` and then behaves exactly like `fn` — same arguments, same return value. It must keep working when the result is put back on an object as a method:

```js
account.deposit = logCalls(account.deposit);
account.deposit(50);    // logs, then returns the new balance: 150
```

and also for a plain function: `logCalls(function add(a, b) { return a + b; })(2, 3)` returns `5`.

<details>
<summary>Hint 1 — where to look</summary>

When `account.deposit(50)` runs, which function actually receives `this` from the dot — the original `deposit`, or your wrapper?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

The wrapper receives `this`, so it must be a kind of function that has one, and it must pass it along when it calls the original.

</details>

<details>
<summary>Solution</summary>

**Approach.** The wrapper is a regular function, so the dot call gives it `this`; it forwards both `this` and the arguments with `apply`. This is the same line as in your `once()` helper.

```js
"use strict";
function logCalls(fn) {
    return function (...args) {
        console.log("calling " + fn.name + " with " + args.join(", "));
        return fn.apply(this, args);
    };
}

const account = {
    balance: 100,
    deposit(amount) {
        this.balance += amount;
        return this.balance;
    },
};
account.deposit = logCalls(account.deposit);

console.log(account.deposit(50));
console.log(account.balance);

const add = logCalls(function add(a, b) {
    return a + b;
});
console.log(add(2, 3));
```

```text
calling deposit with 50
150
150
calling add with 2, 3
5
```

**Edge cases handled.** Used as a method (`this` forwarded); used as a plain function (`this` is `undefined` and unused); return value passed through.

**The tempting wrong version.** `return (...args) => fn(...args);` — an arrow has no `this` to forward, and `fn(...)` is a plain call. Run against the same `account`, it throws `TypeError: Cannot read properties of undefined (reading 'balance')`.

**Uses ideas:** #7, #10

</details>

### B3 — A chainable calculator

*Not on the plan · Mode: from memory*

**Task.** Write a class `Calc` so that these work:

```js
new Calc().add(5).multiply(2).value();              // 10
new Calc(10).multiply(3).add(1).add(1).value();     // 32
```

<details>
<summary>Hint 1 — where to look</summary>

In `x.add(5).multiply(2)`, what is left of the second dot? What must `add` hand back for `multiply` to receive the same calculator?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Each method that should be chainable returns the object it was called on.

</details>

<details>
<summary>Solution</summary>

**Approach.** In a dot call `this` is the calculator, so returning `this` puts the same calculator left of the next dot.

```js
"use strict";
class Calc {
    constructor(start = 0) {
        this.total = start;
    }
    add(n) {
        this.total += n;
        return this;
    }
    multiply(n) {
        this.total *= n;
        return this;
    }
    value() {
        return this.total;
    }
}

console.log(new Calc().add(5).multiply(2).value());
console.log(new Calc(10).multiply(3).add(1).add(1).value());
const c = new Calc(1);
console.log(c.add(1) === c);
```

```text
10
32
true
```

**Edge cases handled.** Default start value; a chain of more than two calls; the returned object is the same instance, not a copy.

**Uses ideas:** #2, #6

</details>

## Self-Check Questions

### Q1 — When is `this` decided?

For a regular function, at what moment is `this` decided, and by what?

<details>
<summary>Answer</summary>

At the start of each call, by the form of that call — not when the function is written. That is why the same function can see a different `this` on every call, and why moving it between objects changes the result.

</details>

### Q2 — Strict vs sloppy

What is `this` in a plain call `fn()` in strict mode and in sloppy mode, and which of the two is more dangerous?

<details>
<summary>Answer</summary>

`undefined` in strict mode, the global object in sloppy mode. Sloppy is more dangerous because nothing fails: reads return `undefined` and writes create global variables, so the bug surfaces far from its cause.

</details>

### Q3 — Top level

What is `this` at the top of a Node CommonJS file, and at the top of an ES module?

<details>
<summary>Answer</summary>

`module.exports` in CommonJS and `undefined` in an ES module. Because the answer depends on the environment, top-level `this` is not worth relying on — `globalThis` is the portable name for the global object.

</details>

### Q4 — Forgetting `new`

A constructor function `User(name)` sets `this.name = name`. What happens if it is called without `new` in strict mode, and why?

<details>
<summary>Answer</summary>

It throws a `TypeError`, because without `new` the call is a plain call and `this` is `undefined`. `new` is what creates the object and passes it in as `this`. In sloppy mode it would instead set a global `name`.

</details>

### Q5 — Why not an arrow for a method?

Why does an arrow function written as a property of an object literal not see that object as `this`?

<details>
<summary>Answer</summary>

An arrow takes `this` from the nearest enclosing scope that has one, and an object literal's braces are not a scope. So it uses whatever `this` is outside the object, regardless of how it is later called.

</details>

### Q6 — Classes

Give two ways `this` behaves in a class that differ from a plain sloppy-mode function.

<details>
<summary>Answer</summary>

Class code is always strict, so a detached method gets `undefined` and throws instead of silently using the global object. And in a class that `extends` another, `this` cannot be touched before `super()` runs, because the parent constructor is what creates the object.

</details>

### Q7 — Who decides for a callback?

You pass `obj.method` to some library function. What will `this` be inside `method`?

<details>
<summary>Answer</summary>

You cannot know from your own code — the library makes the call, so the library decides. Array methods give `undefined` unless you pass `thisArg`, DOM listeners give the element, Node timers give a `Timeout` object. The one certainty is that it is no longer `obj`.

</details>

### Q8 — Precedence

Rank these from strongest to weakest: the dot, `bind`, `new`, a plain call, `call`. Where do arrow functions fit?

<details>
<summary>Answer</summary>

`new`, then `bind`, then `call`/`apply`, then the dot, then the plain-call default. Arrows are not in the ranking: they have no `this` binding, so none of these can set it.

</details>

### Q9 — The price of a class-field arrow

What does `handler = () => { … }` in a class cost compared with a normal method, and when is it worth it?

<details>
<summary>Answer</summary>

A normal method exists once, on the prototype, shared by every instance; an arrow field is created again for each instance. It is worth it for methods that are always passed as callbacks, because the reference is stable and `this` cannot be lost.

</details>

### Q10 — What if `this` were lexical everywhere?

If every function took `this` from where it was written, as arrows do, what would stop working?

<details>
<summary>Answer</summary>

Sharing one function between objects. A method on a prototype is written once, outside any particular instance; it serves every instance only because each call supplies a different `this`. Borrowing a method with `call` would stop working for the same reason.

</details>

## Explain It Back

### E1 — Two minutes, for a new teammate

Explain `this` in two minutes to someone who knows variables and functions but has never used `this`. Start from the note that says "bring me my shoes".

<details>
<summary>Rubric</summary>

- "Me" means whoever holds the note: `this` is set per call, not when the function is written.
- The dot: the object left of it is `this`.
- A bare call has nobody: `undefined` in strict mode.
- Passing a method somewhere else drops the object — the most common bug.
- Arrow functions have no `this` and use the surrounding one.
- One sentence on fixes: arrow wrapper or `bind`.

</details>

### E2 — "Why didn't you just use an arrow function?"

You wrote an object with normal method syntax. Your interviewer asks why you did not use arrow functions for the methods, since they are shorter.

<details>
<summary>Rubric</summary>

- An arrow has no `this` of its own.
- An object literal is not a scope, so the arrow would see the outer `this`, not the object.
- The dot in `obj.method()` cannot fix it — nothing can set an arrow's `this`.
- Arrows are right for callbacks *inside* methods, where sharing the method's `this` is what you want.
- In a class, an arrow field does work, at the cost of one function per instance.

</details>

### E3 — Walk through the output

Take the running example from the Deep Dive (`user.greet()`, then `greet()`, then `[1].forEach(user.greet)`) and narrate each line as you would in an interview: the rule first, then the output.

<details>
<summary>Rubric</summary>

- States the call-site rule before any output.
- Line 1: dot call, `this` is `user`.
- Line 2: the function was copied out, the call is bare, `this` is `undefined` in strict mode.
- Line 3: the dot only *found* the function; `forEach` makes a plain call later.
- Mentions what sloppy mode would do differently.
- Offers a fix without being asked.

</details>
