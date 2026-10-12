
# DOM Events: Capture vs Bubble, Delegation, preventDefault vs stopPropagation

## 1. Event Propagation

When you click a button, the browser does not fire the event only on the button. The event travels through the DOM tree in **three phases**:

1. **Capture phase** → from `window` down to the target's parent
2. **Target phase** → on the element that was actually clicked
3. **Bubble phase** → from the target's parent back up to `window`

```text
            CAPTURE (1)                    BUBBLE (3)
window ──────────┐                 ┌──────────▶ window
  document ──────┤                 ├────▶ document
    <html> ──────┤                 ├──▶ <html>
      <body> ────┤                 ├─▶ <body>
        <div> ───┤                 ├▶ <div>
          <button> ── TARGET (2) ─▶
```

Every event that propagates goes **down first, then up**. The same element can be visited twice: once on the way down, once on the way up.

```js
const div = document.querySelector("div");
const button = document.querySelector("button");

div.addEventListener("click", () => console.log("div capture"), true);
div.addEventListener("click", () => console.log("div bubble"));
button.addEventListener("click", () => console.log("button"));

// Clicking the button logs:
// div capture
// button
// div bubble
```

> Not every event bubbles. `click`, `input`, `keydown`, `submit` do. `focus`, `blur`, `mouseenter`, `mouseleave` do not (use `focusin`/`focusout` and `mouseover`/`mouseout` instead, which do).

## 2. Capture vs Bubble Listeners

The third argument of `addEventListener` decides which phase the listener runs in:

```js
el.addEventListener("click", handler);                  // bubble phase (default)
el.addEventListener("click", handler, true);            // capture phase
el.addEventListener("click", handler, { capture: true }); // same, options form
```

Rules:

- The default is **bubble**. Almost all real-world listeners use it.
- On the **target element itself**, capture and bubble listeners both run during the target phase, in the order they were registered.
- A capture listener on an ancestor runs **before** the target's listeners; a bubble listener on an ancestor runs **after**.
- `removeEventListener` must match the phase: a listener added with `true` can only be removed with `true`.

### `event.target` vs `event.currentTarget`

```js
div.addEventListener("click", (e) => {
    console.log(e.target);        // <button> → the element that was clicked
    console.log(e.currentTarget); // <div>    → the element this listener is attached to
});
```

- **`target`** → where the event originated. Same in every listener along the path.
- **`currentTarget`** → the element currently handling the event. Changes at every step. Equal to `this` in a regular (non-arrow) handler.

This distinction is what makes delegation possible.

## 3. Event Delegation

**Delegation** means attaching **one listener to a common ancestor** instead of one listener per child, and using `event.target` to find out which child was interacted with. It works because events bubble up to the ancestor.

### The Problem

```js
// 1000 list items → 1000 listeners
document.querySelectorAll("li").forEach((li) => {
    li.addEventListener("click", () => console.log(li.textContent));
});
```

- Memory: one handler function per element.
- Worse: items added to the list **later** have no listener at all.

### The Fix

```js
const list = document.querySelector("ul");

list.addEventListener("click", (e) => {
    const li = e.target.closest("li");      // walk up from the clicked node
    if (!li || !list.contains(li)) return;  // click was not on an item
    console.log(li.textContent);
});
```

- **One** listener, no matter how many items.
- Items added later work automatically — the listener is on the parent, which never changes.

```text
                ul  ◀── one listener here
       ┌────────┼────────┐
      li       li       li ... (any number, added any time)
       ▲
     click ──bubbles up──▶ handled at ul, e.target tells us which li
```

### Why `closest()` and not `e.target` directly

The clicked node may be **inside** the item, not the item itself:

```html
<li><span class="icon">★</span> Item 1</li>
```

A click on the star makes `e.target` the `<span>`, not the `<li>`. `e.target.closest("li")` walks up from the span until it finds the `li` (or returns `null` if there is none).

The `list.contains(li)` check guards against `closest` escaping past the list when the click landed outside any item but a matching `li` exists higher in the tree.

### Delegation with multiple actions

Use `data-*` attributes to route:

```html
<div id="toolbar">
    <button data-action="save">Save</button>
    <button data-action="delete">Delete</button>
</div>
```

```js
const actions = {
    save: () => console.log("saving..."),
    delete: () => console.log("deleting..."),
};

toolbar.addEventListener("click", (e) => {
    const btn = e.target.closest("[data-action]");
    if (btn) actions[btn.dataset.action]?.();
});
```

### When delegation does not work

- Events that **do not bubble** (`focus`, `blur` → use `focusin`/`focusout`; `mouseenter`/`mouseleave` → use `mouseover`/`mouseout`).
- When a child calls `stopPropagation()` — the event never reaches the ancestor (see below).

## 4. preventDefault vs stopPropagation

These two are confused constantly. They do completely different things:

| | `preventDefault()` | `stopPropagation()` |
|---|---|---|
| Stops | The **browser's default action** | The event from **travelling further** |
| Propagation continues? | **Yes** — all listeners still run | **No** — ancestors never see it |
| Default action still happens? | **No** | **Yes** |
| Example | Link does not navigate, form does not submit | Parent's click handler never fires |

### `preventDefault()`

Cancels what the browser would do on its own:

```js
link.addEventListener("click", (e) => {
    e.preventDefault();         // link does NOT navigate
    // ...but the event still bubbles to ancestors
});

form.addEventListener("submit", (e) => {
    e.preventDefault();         // page does NOT reload
    // validate and send with fetch instead
});
```

Common default actions: link navigation, form submission, checkbox toggling, text input on `keydown`, context menu on right-click.

- Only works if the event is **cancelable** (`e.cancelable === true`).
- A listener registered with `{ passive: true }` **cannot** call `preventDefault` — the browser ignores it and warns. Scroll/touch listeners are often passive by default for performance.

### `stopPropagation()`

Stops the event from visiting any further elements:

```js
button.addEventListener("click", (e) => {
    e.stopPropagation();        // div's handler below will NOT run
});

div.addEventListener("click", () => {
    console.log("never logged when the button is clicked");
});
```

The default action is **not** affected — a link still navigates even if propagation is stopped.

### `stopImmediatePropagation()`

`stopPropagation()` still lets **other listeners on the same element** run. `stopImmediatePropagation()` stops those too:

```js
button.addEventListener("click", (e) => {
    e.stopImmediatePropagation();
    console.log("first");
});
button.addEventListener("click", () => {
    console.log("never runs");  // same element, but blocked
});
```

### They are independent

You can call both when you want neither the default action nor any ancestor handlers:

```js
el.addEventListener("click", (e) => {
    e.preventDefault();
    e.stopPropagation();
});
```

> Think of it as two separate questions: *"Should the browser do its built-in thing?"* → `preventDefault`. *"Should other elements hear about this?"* → `stopPropagation`.

## 5. Modal Close on Outside Click

A classic pattern that uses everything above:

```js
const overlay = document.querySelector(".overlay");
const modal = document.querySelector(".modal");

overlay.addEventListener("click", () => closeModal());

modal.addEventListener("click", (e) => {
    e.stopPropagation();        // clicks inside the modal must not bubble to the overlay
});
```

A cleaner version without `stopPropagation` — check the target instead:

```js
overlay.addEventListener("click", (e) => {
    if (e.target === overlay) closeModal();   // only direct clicks on the backdrop
});
```

The second version is preferred: `stopPropagation` silently breaks **any** ancestor that relies on bubbling (analytics click-tracking on `document`, other delegated handlers), which is a frequent source of "my listener never fires" bugs.

## 6. Pitfalls

- **`stopPropagation` breaks delegation and analytics** → any `document`-level listener (click tracking, close-on-outside-click, delegated handlers) goes blind for that subtree. Prefer checking `e.target` over stopping propagation.
- **`e.target` may be a child of the element you care about** → always use `closest()` in delegated handlers.
- **Passive listeners cannot `preventDefault`** → `{ passive: true }` (the default for `touchstart`/`touchmove`/`wheel` in some browsers) makes `preventDefault` a no-op.
- **`focus` and `blur` do not bubble** → delegation needs `focusin`/`focusout`.
- **Forgetting to remove listeners** → a long-lived listener closes over its scope and keeps it alive. Remove listeners on teardown, or use `{ once: true }` for one-shot handlers, or an `AbortController`:

```js
const ctrl = new AbortController();
el.addEventListener("click", handler, { signal: ctrl.signal });
// later, removes every listener registered with this signal:
ctrl.abort();
```

- **Returning `false` from an `addEventListener` handler does nothing.** It only works in old-style `onclick="..."` HTML attributes. Use `preventDefault()` explicitly.

## Key Takeaways

1. **Three phases** → capture (window → target), target, bubble (target → window). Down first, then up.
2. `addEventListener(..., true)` runs in the **capture** phase; the default is **bubble**.
3. **`target`** → where the event started (fixed). **`currentTarget`** → the element whose listener is running now (changes along the path).
4. **Delegation** → one listener on an ancestor + `e.target.closest(selector)`. Scales to any number of children and covers elements added later.
5. Delegation needs bubbling → `focus`/`blur`/`mouseenter`/`mouseleave` don't bubble; use `focusin`/`focusout`/`mouseover`/`mouseout`.
6. **`preventDefault()`** → cancels the browser's default action; propagation continues.
7. **`stopPropagation()`** → stops the event travelling; the default action still happens.
8. **`stopImmediatePropagation()`** → also stops remaining listeners on the same element.
9. Prefer **checking `e.target`** over `stopPropagation` — stopping propagation breaks unrelated ancestor listeners.
10. `{ once: true }` for one-shot listeners, `AbortController` for bulk cleanup, `{ passive: true }` disables `preventDefault`.

## Practice

Implement these from scratch, in order:

1. A delegated todo list → one listener on the `<ul>` handles complete/delete buttons inside each `<li>` (use `data-action` + `closest`).
2. A modal that closes on outside click and on `Escape` — without using `stopPropagation`.
3. A logger that attaches capture **and** bubble listeners to `document`, `body`, and a button, and prints the full propagation order of one click.
