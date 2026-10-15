# `this` in Every Context — Brief

> **Track:** JS · **Type:** concept · **Scheduled:** Week 2 · Mon · **Runtime:** Node v22 · **Generated:** 2026-10-13

## Explain It Like I'm 5

There is a little note that says: "Bring **me** my shoes."

Asha picks up the note and reads it out loud. Everyone knows "me" means Asha, so they bring Asha's shoes. Then Ravi takes the note and reads it. Now "me" means Ravi — same note, same words, different shoes.

One day the note is lying on the floor and a robot reads it. Nobody is holding it. "Bring me my shoes" — but who is "me"? Nobody. The robot is stuck.

So mum makes a special note. She crosses out "me" and writes "Asha" in ink. Now it does not matter who holds it.

In real terms: `this` is the word "me" inside a function — it means whichever object the function is being called on, and an arrow function is the note written in ink.

## What It Is & Where It Shows Up

`this` is a value every regular function receives automatically on each call, decided by **how the function is called** rather than where it is written.

It is what lets one function serve many objects: a method is stored once and works for whichever object it is called on. The cost is that the connection exists only during a call made the right way. Someone who expects `this` to behave like a variable gets methods that work when called directly and break — often silently — the moment they are passed anywhere.

Where it shows up:

- **Callbacks** — passing `obj.method` to a timer, an array method or an event listener.
- **Classes** — any method used as a handler.
- **Wrappers** — helpers such as `once`, `debounce` or a logger must forward `this` to stay usable on methods.
- **Interviews** — nearly always "what does this print?", then "how would you fix it?".

## Glossary

| Term             | In plain words                                                      | Precise definition                                                                                   |
| ---------------- | ------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------- |
| `this`           | The word "me" inside a function                                     | A value bound for each call of a regular function, determined by the form of the call                |
| Regular function | A function written with `function` or as a method                   | Any function that is not an arrow function; it gets its own `this` on every call                     |
| Method call      | Calling a function through an object, with a dot                    | A call of the form `obj.fn()`; `this` is `obj`                                                       |
| Plain call       | Calling a function by itself                                        | A call of the form `fn()`, with no object, no `new` and no binding                                   |
| Strict mode      | The careful version of the language that turns quiet mistakes into errors | The mode enabled by `"use strict"`, and always on in classes and ES modules                    |
| Sloppy mode      | The old, forgiving version of the language                          | Non-strict code; a plain call gets the global object as `this`                                       |
| Global object    | The one big object that holds everything shared                     | `window` in browsers, `global` in Node; `globalThis` everywhere                                      |
| Arrow function   | The short function written with `=>`                                | A function with no `this` binding of its own; `this` inside it resolves in the enclosing scope       |
| Explicit binding | Telling a function who "me" is, by hand                             | Setting `this` with `call`, `apply` or `bind`                                                        |
| Bound function   | A function with "me" fixed for good                                 | The function returned by `bind`; its `this` cannot be overridden except by `new`                     |
| Lost `this`      | A method that has forgotten its object                              | A method reference used as a value, then invoked by a plain call                                     |

## Core Ideas

| #   | Idea                    | Level          | In one line                                                                    | Taught in                                         | Tested by       |
| --- | ----------------------- | -------------- | ------------------------------------------------------------------------------ | ------------------------------------------------- | --------------- |
| 1   | Call-site rule          | `basic`        | `this` is decided by how a regular function is called, not where it is written | [Deep Dive](deep-dive.md) → Level 1               | W2, Q1, E1      |
| 2   | Method call             | `basic`        | In `obj.fn()`, `this` is the object directly left of the final dot             | [Deep Dive](deep-dive.md) → Level 1               | W1, B3          |
| 3   | Plain call              | `basic`        | `fn()` gets `undefined` in strict mode, the global object in sloppy mode       | [Deep Dive](deep-dive.md) → Level 2               | W3, Q2          |
| 4   | Top level               | `basic`        | Outside functions it depends on the file: `module.exports`, `undefined` or `window` | [Deep Dive](deep-dive.md) → Level 2          | Q3              |
| 5   | Losing `this`           | `intermediate` | Reading a method out of its object drops the object                            | [Deep Dive](deep-dive.md) → Level 1               | W2, S1, B1, E3  |
| 6   | `new`                   | `intermediate` | With `new`, `this` is the freshly created object                               | [Deep Dive](deep-dive.md) → Level 2               | Q4, B3          |
| 7   | Arrow functions         | `intermediate` | No `this` of their own; they use the surrounding one, permanently              | [Deep Dive](deep-dive.md) → Level 2               | W3, W4, B2      |
| 8   | Arrow as object method  | `intermediate` | An object literal is not a scope, so the arrow sees the outer `this`           | [Deep Dive](deep-dive.md) → Level 2               | W4, S2, Q5, E2  |
| 9   | Classes                 | `intermediate` | Always strict; no `this` before `super()`                                      | [Deep Dive](deep-dive.md) → Level 2               | W5, Q6, B1      |
| 10  | Explicit binding        | `intermediate` | `call` / `apply` set `this` for one call; `bind` fixes it for good             | [Deep Dive](deep-dive.md) → Level 2               | W2, B2          |
| 11  | Callbacks               | `intermediate` | Whoever calls the callback chooses its `this`                                  | [Deep Dive](deep-dive.md) → Level 2               | S1, Q7          |
| 12  | Precedence              | `advanced`     | `new`, then `bind`, then `call`/`apply`, then the dot, then the default        | [Deep Dive](deep-dive.md) → Level 3               | W2, Q8, C3      |
| 13  | The three fixes         | `advanced`     | Arrow wrapper, `bind`, arrow class field — each with a cost                    | [Deep Dive](deep-dive.md) → Limits & Trade-offs   | S6, B1, Q9      |

## Know This Cold

**The rules.** For a regular function, check in this order and stop at the first match:

1. Called with `new` → `this` is the new object.
2. Made with `bind` → `this` is the bound object, for good.
3. Called with `call` / `apply` → `this` is the object passed.
4. Called through a dot, `obj.fn()` → `this` is `obj`.
5. Called on its own, `fn()` → `undefined` in strict mode, the global object in sloppy mode.

For an arrow function, skip all five: `this` is whatever it is in the code surrounding the arrow.

And the one consequence that causes most bugs: **a method passed as a value has lost its object.**

**The canonical snippet.**

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

## Costs & Limits

| What it costs                                   | When it bites                                                                    |
| ----------------------------------------------- | -------------------------------------------------------------------------------- |
| A method cannot be passed around as it is       | Every timer, listener, array callback and promise handler                        |
| Silent failure                                  | Sloppy mode, or when the function lands on another object — no error, wrong data |
| `bind` or an arrow field: one function per instance | Many instances; counted in the Deep Dive — shared method `true`, fixed ones `false` |
| An arrow wrapper: a new function on every pass  | Removing a listener later, or comparing references                                |

Hard limits: an arrow function's `this` cannot be set by anything, and an arrow cannot be called with `new`. A bound function's `this` cannot be changed again. In a class that `extends`, `this` does not exist before `super()`.

## Connections

**Prerequisites**

- Execution context — [Execution Context, Lexical Environment & Hoisting](../../../JS-docs/lexicalEnvExecContextHoisting.md)
- Lexical scope — [Lexical Scope, Scope Chain & Closures](../../../JS-docs/lexicalScopeScopeChainClosures.md)
- Strict mode (no doc yet) — an 8-line primer is in the Deep Dive, Level 2

**Confusables**

- Lexical scope — a variable follows where the code is written; `this` follows how the function is called.
- Closures — an arrow "remembering" `this` is ordinary scope lookup, not a second mechanism.
- `event.currentTarget` — always the element the listener is on; `this` matches it only in a regular function listener.

**Unlocks**

- call / apply / bind, written by hand — Week 2 · Wed
- Prototypes, the prototype chain and `class` desugared — Week 2 · Fri
- Polyfills: `call`, `apply`, `bind`, `debounce`, `throttle` — Week 4 · Wed

## Mastery Checklist

What "done" means for the track is in [how-to-study.md](../../../prep/how-to-study.md). For this topic:

- [ ] You can state the five rules in order, and the arrow exception, without notes
- [ ] You can predict the output of an unseen snippet and name the rule behind each line, without notes
- [ ] You can write the canonical snippet from a blank file and explain all three outputs, without notes
- [ ] You can say what changes in sloppy mode, and why that is worse, without notes
- [ ] You can fix a lost `this` three ways and give one cost of each, without notes
- [ ] You can explain why an arrow is wrong for an object method and right for a callback inside one, without notes
- [ ] You can explain it to someone else in two minutes, starting from the note that says "bring me my shoes"

## 2-Minute Revision

- `this` = "me". Regular function: set **per call**, by the call's form. Arrow: none of its own, uses the surrounding one.
- Order: `new` → `bind` → `call`/`apply` → dot → default.
- Default: `undefined` in strict mode (classes and modules are always strict), global object in sloppy mode.
- Dot rule: only the object directly left of the **final** dot.
- `const f = obj.method; f()` and `doLater(obj.method)` are both plain calls — the object is gone.
- Callbacks: the caller chooses `this`. `forEach(fn, thisArg)`, DOM listener → the element, Node timer → `Timeout`.
- Fixes: `() => obj.method()` · `this.m = this.m.bind(this)` · `m = () => { … }` in a class.
- Trap 1: an arrow as an object method sees the outer `this`, not the object.
- Trap 2: a wrong `this` often does not throw — it reads `undefined` or writes to the wrong object.
- `bind` cannot be re-bound; `call` cannot touch an arrow; `new` beats `bind`.
