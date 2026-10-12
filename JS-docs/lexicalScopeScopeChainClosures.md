
# JavaScript Lexical Scope, Scope Chain & Closures

## 1. Scope

**Scope** is the region of code where a variable can be accessed.

JavaScript has three kinds of scope:

- **Global scope** → variables declared outside any function or block
- **Function scope** → variables declared inside a function (`var`, `let`, `const`, parameters)
- **Block scope** → `let` / `const` declared inside `{ }` (an `if`, a `for`, or a plain block)

```js
var g = 1;              // global scope

function outer() {
    var f = 2;          // function scope

    if (true) {
        let b = 3;      // block scope
        var v = 4;      // function scope (var ignores blocks)
    }

    console.log(v);     // 4
    console.log(b);     // ReferenceError
}
```

> `var` is function-scoped. `let` and `const` are block-scoped.

## 2. Lexical Scope

**Lexical scope** means the scope of a variable is decided by **where the code is written**, not by where or how a function is called.

```js
const name = "global";

function print() {
    console.log(name);
}

function run() {
    const name = "local";
    print();
}

run(); // "global"
```

`print` is written in the global scope, so it looks for `name` in the global scope. It does not matter that it was called from inside `run`.

> A function's scope is fixed at the place it is defined. Calling it from somewhere else does not change what it can see.

This is also called **static scope**. The opposite, where the caller decides what a function can see, is called dynamic scope. JavaScript does not use dynamic scope for variables.

## 3. Scope Chain

Every Lexical Environment has two parts:

- its own variables
- a reference to its **outer** (parent) Lexical Environment

When JavaScript needs a variable, it looks in the current scope first. If the variable is not there, it follows the outer reference and looks in the parent, then the parent's parent, until it reaches the global scope. This path is the **scope chain**.

```js
const a = 1;

function outer() {
    const b = 2;

    function inner() {
        const c = 3;
        console.log(a, b, c); // 1 2 3
    }

    inner();
}

outer();
```

Conceptually:

```text
inner scope            outer scope            global scope
┌───────────┐          ┌───────────┐          ┌───────────┐
│ c → 3     │ ──────▶  │ b → 2     │ ──────▶  │ a → 1     │ ──────▶ null
│           │  outer   │ inner → fn│  outer   │ outer → fn│
└───────────┘          └───────────┘          └───────────┘
```

Looking up `a` from `inner`:

```text
inner scope  → not found
outer scope  → not found
global scope → found (1)
```

Rules of the scope chain:

- Lookup goes **outward only**. An outer scope can never see variables of an inner scope.
- Lookup **stops at the first match**.
- If the global scope does not have it either, a `ReferenceError` is thrown.
- The chain is built from where functions are **written** (lexical scope), not from the call stack.

### Shadowing

If an inner scope declares a variable with the same name as an outer one, the inner one is found first and hides the outer one.

```js
const x = 1;

function test() {
    const x = 2;        // shadows the global x
    console.log(x);     // 2
}

test();
console.log(x);         // 1
```

## 4. Closures

A **closure** is a function together with the scope it was created in. The function keeps a reference to its outer Lexical Environment, so those variables stay alive as long as the function does, even after the outer function has returned.

```js
function outer() {
    let count = 0;

    return function inner() {
        count++;
        return count;
    };
}

const fn = outer();     // outer has finished
fn(); // 1
fn(); // 2
```

`outer`'s execution context is gone, but its Lexical Environment is not destroyed because `inner` still points to it through the scope chain.

Two rules explain almost every closure question:

1. **Closures capture variables, not values.** The function sees the variable's current value when it runs, not the value at the time it was created.
2. **Every call of the outer function creates a fresh scope.** Functions created in different calls do not share variables. Functions created in the same call do.

```js
let x = 1;
const f = () => x;
x = 2;
f(); // 2, not 1  (rule 1)
```

## 5. Counter Factory

```js
function createCounter(start = 0) {
    let count = start;          // lives in this call's scope

    return function () {
        count += 1;             // reads and writes the captured variable
        return count;
    };
}

const a = createCounter();
const b = createCounter(10);

a(); // 1
a(); // 2
b(); // 11  → independent of a
```

- `createCounter()` returns, but `count` is not destroyed because the inner function still references it.
- `a` and `b` came from separate calls, so each has its own `count` (rule 2).
- `a()` returns 1 and then 2 because it changes the same variable each time, not a copy (rule 1).

Conceptually:

```text
a → function ──▶ scope { count → 2 }
b → function ──▶ scope { count → 11 }
```

Several functions created in the same call share one scope:

```js
function createCounter() {
    let count = 0;

    return {
        increment: () => ++count,
        decrement: () => --count,
        reset: () => { count = 0; },
        value: () => count,
    };
}
```

All four methods see the same `count`.

## 6. `once()`

`once(fn)` returns a wrapper that runs `fn` only the first time and returns the cached result afterwards. The closure holds two pieces of state: whether it has run, and the result.

```js
function once(fn) {
    let called = false;
    let result;

    return function (...args) {
        if (!called) {
            called = true;
            result = fn.apply(this, args);  // forward `this` and arguments
            fn = null;                      // release the reference for GC
        }
        return result;
    };
}

const init = once(() => {
    console.log("initializing...");
    return 42;
});

init(); // logs "initializing...", returns 42
init(); // returns 42, no log
```

Details to remember:

- **`fn.apply(this, args)`** → the wrapper must be a regular function, not an arrow function, so `this` is preserved when it is used as a method.
- **`called = true` before calling `fn`** → guards against re-entrant calls, where `fn` calls the wrapper itself.
- **`fn = null`** → without it, the closure keeps `fn` and everything `fn` closes over alive forever.
- **A separate `called` flag** → checking `result === undefined` instead would break when `fn` really returns `undefined`.
- **If `fn` throws** → this version counts the throwing call as the one call. To allow a retry, set `called = true` only after `fn` succeeds.

Real uses: one-time initialisation (database connection, config load), and making sure an event handler or a payment submit fires only once.

## 7. Private State

Closure variables cannot be reached from outside the function. This gives true privacy.

```js
function createBankAccount(initial) {
    let balance = initial;      // private

    return {
        deposit(amount) {
            if (amount <= 0) throw new Error("Invalid amount");
            balance += amount;
        },
        withdraw(amount) {
            if (amount > balance) throw new Error("Insufficient funds");
            balance -= amount;
        },
        getBalance: () => balance,
    };
}

const acct = createBankAccount(100);
acct.deposit(50);
acct.getBalance(); // 150
acct.balance;      // undefined
```

The only way to change `balance` is through the methods, so the validation cannot be bypassed.

### Module Pattern

The same idea with an immediately invoked function (IIFE), giving a single instance:

```js
const idGenerator = (function () {
    let lastId = 0;

    return {
        next: () => ++lastId,
    };
})();

idGenerator.next(); // 1
idGenerator.next(); // 2
```

### Closure vs `#private` Fields

| | Closure | `#private` field |
|---|---|---|
| Privacy | True | True |
| Methods | Recreated for every instance (more memory) | Shared on the prototype |
| Works without classes | Yes | No |

Closures suit factories and modules. `#private` fields suit classes with many instances.

## 8. Closures in Loops

### The Problem

```js
for (var i = 0; i < 3; i++) {
    setTimeout(() => console.log(i), 0);
}
// 3, 3, 3
```

`var` is function-scoped, so there is only **one** `i` for the whole loop. All three callbacks capture that same variable (rule 1). They run after the loop has finished, when `i` is 3.

```text
callback 1 ──┐
callback 2 ──┼──▶ scope { i → 3 }
callback 3 ──┘
```

### Fix 1: `let`

```js
for (let i = 0; i < 3; i++) {
    setTimeout(() => console.log(i), 0);
}
// 0, 1, 2
```

`let` in a `for` header creates a **new `i` for each iteration**, copying the previous value forward. Each callback captures a different variable.

```text
callback 1 ──▶ scope { i → 0 }
callback 2 ──▶ scope { i → 1 }
callback 3 ──▶ scope { i → 2 }
```

### Fix 2: IIFE (before ES6)

```js
for (var i = 0; i < 3; i++) {
    (function (j) {
        setTimeout(() => console.log(j), 0);
    })(i);
}
// 0, 1, 2
```

Each call creates a new scope with its own `j` (rule 2), copying `i` at that moment.

### Fix 3: Pass the Value as an Argument

```js
for (var i = 0; i < 3; i++) {
    setTimeout(console.log, 0, i);
}
// 0, 1, 2
```

Arguments are evaluated immediately, so nothing is captured. `console.log.bind(null, i)` works the same way.

### Stale Closures

The modern form of this bug: a callback captures a variable from an old scope and never sees updates.

```js
function Counter() {
    const [count, setCount] = useState(0);

    useEffect(() => {
        const id = setInterval(() => {
            console.log(count);     // always 0
        }, 1000);
        return () => clearInterval(id);
    }, []);                         // effect runs once, captures first render's count
}
```

Each render creates a new `count`. The interval callback was created in the first render, so it keeps seeing that render's `count`.

## 9. Pitfalls

- **Memory** → a closure keeps its scope alive. A long-lived callback (event listener, timer) that closes over a large object prevents it from being garbage collected. Remove listeners and clear timers.
- **Mutation is visible** → variables are captured, not values, so a change made after the closure is created is seen by the closure.
- **Loops with `var`** → all callbacks share one variable. Use `let`.

## Key Takeaways

1. **Scope** → the region of code where a variable can be accessed: global, function, or block.
2. **Lexical scope** → scope is decided by where code is **written**, not where a function is called.
3. **Scope chain** → each Lexical Environment has a reference to its outer one; lookup goes outward until the global scope, then throws a `ReferenceError`.
4. **Shadowing** → an inner variable with the same name hides the outer one.
5. **Closure** → a function plus the scope it was created in; the scope stays alive as long as the function does.
6. Closures capture **variables, not values**.
7. Every call of the outer function creates a **fresh scope**.
8. **Counter factory** → each call gives an independent counter; methods from the same call share state.
9. **`once()`** → a `called` flag and a cached `result` held in a closure.
10. **Private state** → closure variables are unreachable from outside; the module pattern uses an IIFE for a single instance.
11. **Loops** → `var` gives one shared variable (prints 3, 3, 3); `let` gives a new variable per iteration (prints 0, 1, 2).
12. **Stale closure** → a callback keeps seeing an old variable because it was created in an old scope.

## Practice

Implement these from scratch, in order:

1. `once(fn)`
2. `memoize(fn)` → the same pattern as `once`, with a `Map` cache keyed by arguments
3. `debounce(fn, delay)` → the closure holds a timer id
