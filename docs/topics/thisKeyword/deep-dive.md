# `this` in Every Context — Deep Dive

> **Track:** JS · **Type:** concept · **Scheduled:** Week 2 · Mon · **Runtime:** Node v22 · **Generated:** 2026-10-13

## Why It Exists

You already know how variables work: a function sees the variables around the place it was **written** (see [Lexical Scope, Scope Chain & Closures](../../../JS-docs/lexicalScopeScopeChainClosures.md)). So it is natural to expect `this` to work the same way — "`this` is the object the function was written inside". Run this, the running example for the whole doc set:

```js
"use strict";

const user = {
    name: "Asha",
    greet() {
        console.log("hi, " + (this === undefined ? "nobody" : this.name));
    },
};

user.greet();               // 1. called through the object
const greet = user.greet;
greet();                    // 2. same function, called on its own
[1].forEach(user.greet);    // 3. same function, handed to someone else
```

```text
hi, Asha
hi, nobody
hi, nobody
```

One function, written once, inside one object — and it gives two different answers. Nothing about where it was written changed between line 1 and line 2. What changed is **how it was called**.

That is the whole reason `this` exists: it lets one function work on different objects without being rewritten for each. `greet` is stored once and can serve a thousand users, because each call tells it which user it is working for. The price is that the function has to be told every time — and when nobody tells it, it has nobody.

## Level 1 — Intuition

The story is in [Brief](brief.md) → Explain It Like I'm 5: a note that says "bring **me** my shoes". The word "me" is not about who wrote the note. It means whoever is holding the note when it is read out.

**`this` is the word "me"** *(Idea 1)*. Inside a regular function, `this` is not fixed when the function is written. It is filled in fresh on every call, by looking at how that call was made. The same function can have a different `this` each time it runs.

**The dot hands over the note** *(Idea 2)*. In `user.greet()`, look at what is immediately left of the dot at the moment of the call: `user`. That object is "holding the note", so inside `greet`, `this` is `user`. Only the last dot counts:

```text
user.greet()            this = user
user.address.show()     this = user.address     (not user)
         ^
         whatever is directly left of the final dot
```

**No dot, nobody holding it** *(Idea 5)*. `const greet = user.greet` copies the function out of the object. It does not copy any connection back to `user` — a function does not remember which object it was read from. So `greet()` is a call with nothing left of a dot, and `this` is nobody.

Line 3 is the same thing in disguise. `[1].forEach(user.greet)` reads the function out of `user` and hands the bare function to `forEach`. Later, somewhere inside `forEach`, it is called on its own. The dot in `user.greet` was used to **find** the function, not to **call** it.

```text
user.greet()                  found AND called through user     → this = user
const greet = user.greet      found through user ...
greet()                       ... called on its own             → this = nobody
[1].forEach(user.greet)       found through user ...
                              ... called later by forEach       → this = nobody
```

**Recap:** For a regular function, `this` is decided at each call, by what is directly left of the dot in that call.
Reading a method out of its object — into a variable or as an argument — drops the object, and the later call has no `this`.

**Check yourself (C1):** `const run = user.greet; const box = { name: "Box", run }; box.run();` — what is printed, and why is it not "hi, Asha"?

<details>
<summary>Answer</summary>

It prints `hi, Box`. The function was written inside `user`, but the call is `box.run()`, so the object left of the dot is `box`. Where the function came from plays no part.

</details>

## Level 2 — How It Actually Works

Every call of a regular function falls into one of four kinds. Each kind sets `this` differently. Arrow functions stand outside this system and come after.

```text
how the function is called     example                this is
-----------------------------  ---------------------  ---------------------------------
with new                       new User("Asha")       the brand-new object
with call / apply / bind       greet.call(asha)       the object you passed
through a dot                  user.greet()           the object left of the dot
on its own                     greet()                undefined (strict) / global (sloppy)
```

**A short primer on strict mode** (needed for the last row). JavaScript has two modes. **Sloppy mode** is the old default, kept so that old web pages do not break. **Strict mode** turns several silent mistakes into errors. A file or function opts in with the line `"use strict";` at its top. Code inside a `class` and code in an ES module is always strict, with no opt-in needed — so most code written today is strict. The examples in this doc use `"use strict";` unless they say otherwise.

**Plain call** *(Idea 3)*. When a function is called on its own, strict mode leaves `this` as `undefined`. Sloppy mode substitutes the global object instead:

```js
function show() {
    console.log(this === globalThis);
}
show();
```

```text
true
```

Add `"use strict";` as the first line and print `this` directly, and the output is `undefined`. The sloppy behaviour is the dangerous one: `this.name = "x"` inside a plain call silently creates a global variable instead of failing. Strict mode fails loudly, which is why an accidental plain call usually shows up as `TypeError: Cannot read properties of undefined`.

**Top level** *(Idea 4)*. Outside any function there is no call to inspect, so the answer depends on what kind of file is running. In a Node CommonJS file:

```js
console.log(this === module.exports);
console.log(this === globalThis);
```

```text
true
false
```

The same `console.log(this)` in an ES module (a `.mjs` file) prints `undefined`. The browser case is in Variants & Environment Differences. In practice top-level `this` is not something to rely on; reach for `globalThis` when you mean the global object.

**`new`** *(Idea 6)*. Calling a function with `new` creates an empty object, runs the function with `this` set to that object, and returns it:

```js
"use strict";
function User(name) {
    this.name = name;
}
const a = new User("Asha");
console.log(a.name);
try {
    User("Ravi");
} catch (e) {
    console.log(e.constructor.name + ": " + e.message);
}
```

```text
Asha
TypeError: Cannot set properties of undefined (setting 'name')
```

The second call forgot `new`, so it is a plain call and `this` is `undefined`. In sloppy mode it would not throw — it would set a global `name` and return nothing.

**Explicit binding** *(Idea 10)*. `call`, `apply` and `bind` let you state `this` by hand. `call` and `apply` run the function immediately; `bind` returns a new function with `this` fixed for good. Building them by hand is Week 2 · Wed — here they are only used.

```js
"use strict";
function greet(greeting, mark) {
    console.log(greeting + ", " + this.name + mark);
}
const asha = { name: "Asha" };
const ravi = { name: "Ravi" };

greet.call(asha, "hi", "!");
greet.apply(ravi, ["hello", "?"]);

const greetAsha = greet.bind(asha, "hey");
greetAsha(".");
greetAsha.call(ravi, ".");          // call cannot undo bind
greetAsha.bind(ravi)(".");          // a second bind cannot either

const arrow = () => typeof this;
console.log(arrow.call(asha) === arrow());   // call cannot touch an arrow
```

```text
hi, Asha!
hello, Ravi?
hey, Asha.
hey, Asha.
hey, Asha.
true
```

Once a function is bound, later attempts to change `this` with `call` or another `bind` are ignored.

**Arrow functions** *(Idea 7)*. An arrow function does not take part in any of the four rules, because it has no `this` of its own. When code inside an arrow says `this`, JavaScript looks it up the same way it looks up a variable: in the surrounding code, at the place the arrow was written. That is why arrows are the usual fix for a callback inside a method:

```js
"use strict";
const team = {
    name: "Core",
    members: ["Asha", "Ravi"],
    listRegular() {
        this.members.forEach(function (m) {
            console.log(m + " -> " + (this === undefined ? "no this" : this.name));
        });
    },
    listArrow() {
        this.members.forEach((m) => {
            console.log(m + " -> " + this.name);
        });
    },
};
team.listRegular();
team.listArrow();
```

```text
Asha -> no this
Ravi -> no this
Asha -> Core
Ravi -> Core
```

Both methods are called as `team.list…()`, so inside each method `this` is `team`. The regular callback is then called on its own by `forEach`, and gets its own, empty `this`. The arrow callback has none of its own and uses the method's.

**An arrow as an object method** *(Idea 8)*. The same rule cuts the other way. The braces of an object literal are not a scope — only functions (and modules, and class bodies) are. So an arrow written as a property looks straight past the object to whatever `this` is outside it:

```js
"use strict";
const user = {
    name: "Asha",
    greet: () => {
        console.log(this === module.exports, this.name);
    },
};
user.greet();
```

```text
true undefined
```

The call is `user.greet()`, with a dot — and it makes no difference. The arrow's `this` is the file's top-level `this`.

**Classes** *(Idea 9)*. A class method is a regular function, so all four rules apply. Two things are specific to classes. First, class code is always strict, so a detached method gets `undefined`, never the global object:

```js
class User {
    constructor(name) {
        this.name = name;
    }
    greet() {
        console.log("hi, " + this.name);
    }
}
const u = new User("Asha");
u.greet();
const greet = u.greet;
try {
    greet();
} catch (e) {
    console.log(e.constructor.name + ": " + e.message);
}
```

```text
hi, Asha
TypeError: Cannot read properties of undefined (reading 'name')
```

Second, in a class that `extends` another, `this` does not exist until `super()` has run, because it is the parent constructor that creates the object. Touching `this` first throws `ReferenceError: Must call super constructor in derived class before accessing 'this' or returning from derived constructor`.

**Callbacks** *(Idea 11)*. When you pass a function to someone else, **they** make the call, so they choose `this`. Different callers choose differently:

```js
"use strict";
const user = {
    name: "Asha",
    greet() {
        console.log(this === undefined ? "undefined" : this.constructor.name);
    },
};
[1].forEach(user.greet);
[1].forEach(user.greet, user);
setTimeout(user.greet, 0);
```

```text
undefined
Object
Timeout
```

`forEach` makes a plain call unless you pass its optional second argument, `thisArg`. Node's `setTimeout` calls the callback with `this` set to its own `Timeout` object. A DOM event listener is called with `this` set to the element the listener is attached to — you already noted this in [DOM Events](../../../JS-docs/domEventsCaptureBubbleDelegation.md) as `currentTarget`. The lesson is not to memorize each caller: it is that a bare method handed to anyone else has lost its object, and what it gets instead is up to them.

**Recap:** A regular function's `this` comes from the call: `new`, explicit binding, the dot, or — failing all three — `undefined` in strict mode.
An arrow function has no `this` and borrows the one from the code around it, which nothing can change afterwards.

**Check yourself (C2):** In `listRegular` above, the callback is written inside a method of `team`. Why is its `this` not `team`?

<details>
<summary>Answer</summary>

Because it is a regular function, and a regular function's `this` comes from how it is called, not where it is written. `forEach` calls it on its own — no dot, no `new`, no binding — so in strict mode `this` is `undefined`. Being written inside a method would matter only for an arrow.

</details>

## The Analogy, Mapped

| In the story                                                    | In reality                                             | Idea # |
| --------------------------------------------------------------- | ------------------------------------------------------ | ------ |
| The word "me" on the note                                       | `this`                                                 | 1      |
| "Me" means whoever holds the note when it is read               | `this` is set on each call, by how the call is made    | 1      |
| Asha holds the note and reads it                                | `user.greet()` — the object left of the dot            | 2      |
| The note is read while lying on the floor                       | A plain call, `greet()` — `this` is `undefined`        | 3, 5   |
| Passing the note to a friend to read later                      | Passing a method as a callback                         | 5, 11  |
| A name tag glued onto the note                                  | `bind`                                                 | 10     |
| A note with the name written in ink instead of the word "me"    | An arrow function                                      | 7      |

**Where the analogy breaks**

- A note on the floor has no "me". In sloppy mode JavaScript does not leave `this` empty: it quietly fills in the global object (Idea 3).
- The story has no `new`. Nothing in it corresponds to a brand-new object being made for the call (Idea 6).
- "Written in ink" suggests an arrow stores a name. It stores nothing — it looks `this` up in the surrounding code, like a variable. If that surrounding function runs again with a different `this`, a new arrow created in that run sees the new one.
- A glued name tag can be peeled off. A bound function cannot be re-bound; only `new` gets past it (Level 3).

## Level 3 — The Formal Picture

**The precise model.** In the language specification, every running function has an execution context (see [Execution Context, Lexical Environment & Hoisting](../../../JS-docs/lexicalEnvExecContextHoisting.md)), and a regular function's context records a **this binding** — a value fixed at the moment the call starts and unchanged for the rest of that call. An arrow function's context records none; a `this` inside it is resolved through the chain of enclosing environments until one that has a this binding is found. Defined in ECMAScript under function environment records; MDN's page on `this` describes the same model in plainer terms.

What decides the binding of a regular call is the **form of the call expression**, not the function:

```js
"use strict";
const user = {
    name: "Asha",
    greet() {
        console.log(this === undefined ? "undefined" : this.name);
    },
};
user.greet();
(user.greet)();
(0, user.greet)();
const { greet } = user;
greet();
```

```text
Asha
Asha
undefined
undefined
```

`user.greet` evaluates to a **reference** — the pair "property `greet` of object `user`" — and calling a reference uses its object as `this`. Plain parentheses keep the reference intact. Anything that turns the reference into an ordinary value — the comma operator, an assignment, destructuring, passing it as an argument — throws the object half away, and what remains is just a function.

**Precedence** *(Idea 12)*. When more than one rule could apply, the stronger one wins:

```text
1. new            new Fn()                 the new object — beats even bind
2. bind           bound()                  the bound object — call/apply cannot override
3. call / apply   fn.call(obj)             obj
4. the dot        obj.fn()                 obj
5. default        fn()                     undefined (strict) / global object (sloppy)

arrow functions: none of the above applies — this comes from the surrounding code
```

```js
"use strict";
function User(name) {
    this.name = name;
}
const asha = { name: "Asha" };
const BoundUser = User.bind(asha);

const made = new BoundUser("Ravi");
console.log(made.name, asha.name);      // new beat bind

const obj = {
    name: "obj",
    show() {
        console.log(this.name);
    },
};
obj.show.call(asha);                     // call beat the dot
```

```text
Ravi Asha
Asha
```

`new BoundUser` built a fresh object and ignored the bound `asha`, which was left untouched.

**Two details at the edges of the rules.** In sloppy mode a primitive passed as `this` is wrapped in an object (`fn.call(5)` sees a `Number` object) and `null` or `undefined` become the global object; strict mode passes whatever you give it, unchanged. And `this` inside an arrow is fixed per **creation** of the arrow, not per program: each time the enclosing function runs, the arrows created in that run see that run's `this`.

**Recap:** A regular call's `this` is fixed when the call starts, from the form of the call expression: `new`, then `bind`, then `call`/`apply`, then the dot, then the default.
An arrow has no binding and resolves `this` outward through its enclosing code, so no call form can affect it.

**Check yourself (C3):** `const f = user.greet.bind({ name: "Ravi" }); const holder = { name: "Box", f }; holder.f();` — which name is used, and which two rules were competing?

<details>
<summary>Answer</summary>

`Ravi`. The dot rule would give `holder`, and `bind` gives the bound object; `bind` ranks above the dot, so the bound object wins. Only calling `f` with `new` would get past the binding.

</details>

## Variants & Environment Differences

| Variant or environment            | What changes                                                                                      | Since / where                              |
| --------------------------------- | ------------------------------------------------------------------------------------------------- | ------------------------------------------ |
| Sloppy mode                       | A plain call gets the global object instead of `undefined`                                        | Any script or function without `"use strict"` |
| Top level, Node CommonJS          | `this` is `module.exports`                                                                        | Observed, Node v22                         |
| Top level, ES module              | `this` is `undefined`                                                                             | Observed, Node v22 (`.mjs`)                |
| Top level, browser classic script | `this` is expected to be `window`                                                                 | Not run — browser only                     |
| `setTimeout` callback             | Node: `this` is the `Timeout` object (observed). Browser: expected to be `window`, even in strict code | Not run in a browser                  |
| DOM listener (regular function)   | `this` is the element the listener is attached to; with an arrow it is the outer `this`           | Browser only — MDN, `addEventListener`     |
| Arrow functions                   | No own `this`                                                                                     | ES2015                                     |
| Arrow class fields                | `handler = () => { … }` creates one arrow per instance with `this` fixed to it                    | ES2022                                     |
| `globalThis`                      | One name for the global object in every environment                                               | ES2020                                     |

To see the browser rows yourself, paste this into `HTML_JS_Sandbox/index.js`:

```js
// Not run — paste into HTML_JS_Sandbox to see it
console.log(this === window);
setTimeout(function () {
    console.log(this === window);
}, 0);
document.addEventListener("click", function (e) {
    console.log(this === e.currentTarget);
});
```

Expected, not observed: `true` twice on load, then `true` on each click.

## Compared With What You Know

**Lexical scope** — which you have already written up.

| Aspect                     | Variables (lexical scope)                 | `this` in a regular function            |
| -------------------------- | ----------------------------------------- | --------------------------------------- |
| Decided by                 | Where the function is written             | How the function is called              |
| Decided when               | Once, when the code is written            | Again on every call                     |
| Can the caller change it?  | No                                        | Yes — dot, `call`, `apply`, `bind`, `new` |
| Survives being passed around | Yes — that is what a closure is         | No — the object is dropped              |

Tell them apart: a variable follows the **code**, `this` follows the **call** — except in an arrow function, where `this` follows the code too.

**Closures.**

| Aspect             | Closure                                         | Arrow function and `this`                        |
| ------------------ | ----------------------------------------------- | ------------------------------------------------ |
| What is remembered | Variables of the enclosing scope                | Nothing new — `this` is looked up the same way   |
| Works with         | Any function                                    | Arrows only                                      |

Tell them apart: an arrow "remembering" `this` is not a second mechanism — it is ordinary scope lookup applied to `this`, which regular functions opt out of by having their own.

**`event.currentTarget`.**

| Aspect                       | `event.currentTarget`           | `this` in the listener                |
| ---------------------------- | ------------------------------- | ------------------------------------- |
| Regular function listener    | The element the listener is on  | The same element                      |
| Arrow function listener      | The element the listener is on  | The outer `this` — not the element    |

Tell them apart: `currentTarget` is always right; `this` is right only when the listener is a regular function.

## Where It's Used

- **Your own `once()` helper.** `Documented` — in [Lexical Scope, Scope Chain & Closures](../../../JS-docs/lexicalScopeScopeChainClosures.md) the wrapper is a regular function that calls `fn.apply(this, args)`. That line exists so that `obj.method = once(obj.method)` still works: the wrapper receives `this` from the dot and forwards it. An arrow wrapper would lose it (see Build It, B2 in [Practice](practice.md)).
- **Your `Counter` class.** `Documented` — `codes/JS/closures.js` uses `this.#count` in every method. It works because each call is `count.add()`, through the dot. Passing `count.add` to a timer or an event handler would break it.
- **DOM event listeners.** `Documented` — MDN, `EventTarget.addEventListener`, section on the value of `this` within the handler: a regular function gets the element, an arrow does not.
- **Class-based UI handlers.** `Illustrative` — any class whose method is passed as a handler (`button.addEventListener("click", this.onClick)`) hits the lost-`this` problem; this is why older React class components bound their handlers in the constructor `(recalled — verify)`.
- **Chainable APIs.** `Illustrative` — a method that ends with `return this` lets calls be chained (`calc.add(5).multiply(2)`); the dot rule makes each link receive the same object.

## Misconceptions & Traps

No finding from your own code: `codes/JS/closures.js` uses `this` correctly throughout.

**"`this` is the object the function was defined in."**
*Why it seems right* — it holds every time you call the method the normal way.
*Why it is wrong* — move the function to another object and it follows the new one:

```js
"use strict";
const user = {
    name: "Asha",
    greet() {
        console.log("hi, " + this.name);
    },
};
const btn = { onClick: null };
btn.onClick = user.greet;
btn.onClick();
```

```text
hi, undefined
```

*The correct model* — `this` is `btn`, the object left of the dot in the call, and `btn` has no `name`. Note that nothing throws: a wrong `this` that is still an object fails silently.

**"An arrow function as a method is just shorter syntax."**
*Why it seems right* — arrows are shorter everywhere else.
*Why it is wrong* — the `user.greet()` example under Idea 8 printed `true undefined`: the arrow never saw `user`.
*The correct model* — an object literal is not a scope, so the arrow takes `this` from outside the object. Use method syntax for methods, arrows for callbacks inside them.

**"A callback written inside a method can use the method's `this`."**
*Why it seems right* — it can use the method's variables, so why not its `this`.
*Why it is wrong* — `listRegular` printed `no this` for every member.
*The correct model* — variables are found by where the code is written; a regular function's `this` is set by its own call. Only an arrow callback shares the method's `this`.

**"`bind` can be changed later with `call` or another `bind`."**
*Why it seems right* — `call` overrides the dot, so it looks like the strongest tool.
*Why it is wrong* — `greetAsha.call(ravi, ".")` and `greetAsha.bind(ravi)(".")` both printed `hey, Asha.`
*The correct model* — a bound function has its `this` fixed permanently. The only thing that ignores it is `new`.

**"`call` can give an arrow function a `this`."**
*Why it seems right* — it works on every other function.
*Why it is wrong* — an arrow created inside a call with one `this` keeps it whatever is tried later:

```js
"use strict";
function outer() {
    const inner = () => this.name;
    return inner;
}
const fromAsha = outer.call({ name: "Asha" });
console.log(fromAsha());
console.log(fromAsha.call({ name: "Ravi" }));
```

```text
Asha
Asha
```

*The correct model* — the arrow has no this binding to set. `call` still runs it and passes arguments; the `this` argument is simply unused.

**"Without `this`, I get an error — so a missing error means `this` is right."**
*Why it seems right* — in strict code a lost `this` usually throws `Cannot read properties of undefined`.
*Why it is wrong* — it throws only when `this` is `undefined`. In sloppy mode, or when the function lands on some other object (the `btn` example, Node's `Timeout`), you read `undefined` properties or write onto the wrong object with no error at all.
*The correct model* — check what `this` is, not whether the code throws.

## Limits & Trade-offs

`this` is not a choice — it comes with every regular function. The real choice is how to keep it when a method is passed around *(Idea 13)*:

```js
"use strict";
class Timer {
    constructor() {
        this.ticks = 0;
        this.tickBound = this.tick.bind(this);      // fix 2
    }
    tick() {
        this.ticks++;
    }
    tickField = () => {                             // fix 3
        this.ticks++;
    };
}
const t = new Timer();
const run = (fn) => fn();

run(() => t.tick());        // fix 1: arrow wrapper
run(t.tickBound);           // fix 2: bind
run(t.tickField);           // fix 3: arrow class field
console.log(t.ticks);

const other = new Timer();
console.log(t.tick === other.tick);             // shared on the prototype
console.log(t.tickField === other.tickField);   // one copy per instance
console.log(t.tickBound === other.tickBound);   // one copy per instance
```

```text
3
true
false
false
```

| Fix                 | Where the work happens      | Cost                                                                 | Prefer it when                                           |
| ------------------- | --------------------------- | -------------------------------------------------------------------- | -------------------------------------------------------- |
| Arrow wrapper       | At the place it is passed   | A new function on every pass, so it cannot be removed by reference later | One-off callbacks; you also want to pass arguments     |
| `bind` in constructor | Once per instance         | One extra function per instance; one line per method                 | You need a stable reference, e.g. to remove a listener   |
| Arrow class field   | Once per instance           | One function per instance instead of one shared on the prototype; not visible to subclasses through `super` | The method is always used as a callback |

Use a plain method when it is only ever called through the dot — it is shared by every instance and costs nothing. Fix `this` only for methods that are handed to someone else, and prefer a stable reference (`bind` or a class field) whenever the same function must later be removed or compared.

## Interview Notes

- **How it opens.** Almost always a snippet: "what does this print?" — a method pulled into a variable, a `setTimeout` inside a method, or an arrow used as a method. Less often: "explain `this`" or "difference between arrow and regular functions".
- **Say the rule before the answer.** "For a regular function `this` depends on the call site, so I look at how it is called." Naming the rule first earns the credit even if you slip on the output.
- **Follow-up: "how would you fix it?"** Give all three fixes and one cost of each (table above).
- **Follow-up: "what if this were sloppy mode?"** A plain call gets the global object, so no error — the bug is silent.
- **Follow-up: "can you change `this` of an arrow / a bound function?"** No for both; `new` on a bound function is the single exception, and arrows cannot be used with `new` at all.
- **Follow-up: "implement `bind`."** That is the next topic on the plan (Week 2 · Wed); the key idea is returning a function that calls the original with `apply`.
- **Depth for the target roles.** The four call rules, arrows, lost `this` and the fixes, strict vs sloppy. The reference-vs-value detail in Level 3 is beyond the usual ask — use it only if pushed on `(0, obj.fn)()`.
- **Two-minute version.** "`this` is a hidden argument. For regular functions it is set per call: `new` gives a new object, `call`/`apply`/`bind` give what I pass, a dot gives the object before it, and a bare call gives `undefined` in strict mode. Arrows have no `this` and use the surrounding one. So methods lose `this` when passed as callbacks, and I fix that with an arrow wrapper, `bind`, or an arrow class field."
