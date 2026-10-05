
# JavaScript Execution Context, Lexical Environment & Hoisting

## 1. Lexical Environment

**Lexical** means related to the structure or placement of code.

A **Lexical Environment** is the environment associated with a particular scope. It keeps track of the variables, functions, and other identifiers available in that scope and helps JavaScript determine what an identifier refers to.

## 2. Execution Context

An **Execution Context** is the environment created by JavaScript to execute a piece of code. It contains the information required to execute that code, such as variables, functions, and `this`.

When a JavaScript program starts, a **Global Execution Context (GEC)** is created. In a browser, the global object is commonly `window`.

> Not every line gets a new execution context. A new Function Execution Context is created when a function is called.

## 3. Phases of an Execution Context

An execution context can be understood in two phases:

### Creation / Setup Phase

JavaScript prepares the execution context and processes declarations:

- `var` → binding is created and initialized with `undefined`
- `let` / `const` → binding is created but remains uninitialized
- Function declarations → function object is created and made available

Example:

```js
console.log(a); // undefined
console.log(add(2, 3)); // 5

var a = 10;

function add(x, y) {
    return x + y;
}


Conceptually during creation:

```text
a   → undefined
add → Function Object
```

### Execution Phase

JavaScript executes the code line by line, assigns values, and calls functions.

```text
a = 10
add(2, 3) → 5
```

## 4. Function Execution Context

Whenever a function is called, JavaScript creates a new **Function Execution Context** for that function.

```js
function add(a, b) {
    let result = a + b;
    return result;
}

add(10, 20);
```

Conceptually:

```text
Global Execution Context
        │
        │ add(10, 20)
        ▼
Function Execution Context
        │
        ├── a → 10
        ├── b → 20
        └── result → 30
```

The function execution context also goes through its own creation/setup and execution phases. Once the function finishes, its execution context is completed.

## 5. Hoisting

**Hoisting** is a term used to describe the behavior where JavaScript processes declarations during the creation/setup phase before normal execution begins.

It does **not** mean JavaScript physically moves the code to the top.

### `var`

```js
console.log(x); // undefined

var x = 10;
```

During creation:

```text
x → undefined
```

So `x` can be accessed before its declaration, but its value is `undefined`.

### `let` / `const`

```js
console.log(x); // ReferenceError

let x = 10;
```

The binding is created but remains uninitialized until execution reaches the declaration. The period between entering the scope and reaching the declaration is called the **Temporal Dead Zone (TDZ)**.

## 6. Function Declaration vs Function Expression

### Function Declaration

```js
add(2, 3); // 5

function add(a, b) {
    return a + b;
}
```

During creation:

```text
add → Function Object
```

Therefore, the function can be called before its declaration.

### Function Expression with `var`

```js
add(2, 3); // TypeError

var add = function(a, b) {
    return a + b;
};
```

During creation:

```text
add → undefined
```

So when `add()` is called, JavaScript is effectively trying to call `undefined`, resulting in a `TypeError`.

## Key Takeaways

1. **Lexical Environment** → keeps track of identifiers available within a particular scope.
2. **Execution Context** → environment created by JavaScript to execute code.
3. The program starts with a **Global Execution Context**.
4. A **Function Execution Context** is created whenever a function is called.
5. Execution contexts can be understood as having a **creation/setup phase** and an **execution phase**.
6. `var` → initialized with `undefined`.
7. `let` / `const` → created but uninitialized until their declaration is reached; accessing them before that causes a `ReferenceError` due to the **TDZ**.
8. Function declarations → function object is available during setup, so they can be called before their declaration.
9. **Hoisting does not physically move code**; it describes the behavior resulting from how declarations are processed before execution.
