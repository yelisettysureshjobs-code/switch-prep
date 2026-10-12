// Requirements
// 1. user should be able to render the accordions
// 2. each accordion should be toggle to open and close (initially single open then multi-open)
// 3. user can provide open accordions list , default open accordions and sepecify if the items are read only
// 4.

// ============================================================================
// INTERVIEW PREP: Possible requirements for the Accordion question
// ============================================================================
//
// --- Core (almost always expected) ---
// 1. Render a list of accordion items from a data array ({ key, label, description }).
// 2. Clicking a header toggles that item open/closed.
// 3. Visual indicator of open state (arrow/chevron that flips).
// 4. Only the header is clickable; the panel shows/hides.
//
// --- Expansion behavior (the main variation point) ---
// 5. Single-open mode — opening one closes the others (classic accordion).
// 6. Multi-open mode — any number can be open simultaneously.
// 7. A prop to switch between the two (allowMultiple / type="single" | "multiple").
// 8. In single mode: can the open item be closed by clicking it again, or must
//    one always stay open (`collapsible` prop — a real distinction in Radix etc.)?
//
// --- State API (what separates mid-level from junior) ---
// 9.  Uncontrolled mode — defaultOpenKeys, component owns state.
// 10. Controlled mode — openKeys + onChange(nextKeys), parent owns state.
// 11. Support both in one component (detect whether openKeys is passed).
// 12. onChange payload design — next keys array, or (key, isOpen).
//
// --- Item-level features ---
// 13. disabled — per item and/or whole list; grayed out, not clickable.
// 14. readOnly — looks normal but ignores interaction (different from disabled).
// 15. Custom content: description as arbitrary JSX/children, not just a string.
// 16. Custom header rendering (renderHeader render prop or icon slot).
// 17. Nested accordions (accordion inside a panel).
//
// --- Accessibility (expected at mid+ level) ---
// 18. WAI-ARIA accordion pattern: header is a <button> inside a heading,
//     aria-expanded, aria-controls; panel has role="region" + aria-labelledby.
// 19. Keyboard support: Enter/Space to toggle; Up/Down arrows move focus between
//     headers; Home/End jump to first/last.
// 20. Focus states visible; disabled items focusable-but-inert with aria-disabled
//     (preferred by the spec) or skipped.
//
// --- UX / polish (stretch goals) ---
// 21. Smooth expand/collapse animation (CSS grid-rows or max-height transition).
// 22. Responsive width (no hardcoded pixel widths).
// 23. Lazy-render panel content vs keep-mounted (matters if content is expensive).
// 24. "Expand all / Collapse all" buttons.
// 25. Deep-linking: open a section based on URL hash; update hash on toggle.
// 26. Async content: fetch panel data on first open, with loading state.
// 27. Persist open state to localStorage.
//
// --- Non-functional ---
// 28. Clean component decomposition (Accordion + AccordionItem).
// 29. PropTypes/TypeScript types for the items shape.
// 30. Edge cases: empty items, duplicate keys, keys in openKeys that don't exist.
//
// ============================================================================
// INTERVIEW PREP: Clarifying questions to ask the interviewer
// (ask ~5-8 up front, grouped so it sounds structured; minimum: 1, 4, 8, 11, 12)
// ============================================================================
//
// --- Behavior ---
// 1. Should this be single-open or multi-open? Or support both via a prop?
// 2. In single-open mode, can the user collapse the open item, or must one
//    always remain open?
// 3. Should any items be open by default on first render?
//
// --- API / state ownership ---
// 4. Controlled, uncontrolled, or both? i.e., does the parent ever need to
//    programmatically open/close items?
// 5. What shape is the data — can I assume { key, label, description }?
//    Are keys guaranteed unique?
// 6. Can panel content be arbitrary JSX, or is plain text fine?
// 7. Do you need an onChange callback, and what should it receive?
//
// --- Features / scope ---
// 8. Do I need disabled or read-only items — per item, whole list, or both?
// 9. Is expand/collapse animation in scope, or is instant toggle fine?
// 10. Should I handle async content loading inside panels, or is data local?
//
// --- Quality bar ---
// 11. How much to prioritize accessibility — full WAI-ARIA pattern with keyboard
//     navigation, or basic ARIA attributes?
// 12. Styling: polish or functional-with-basic-styles? Any library (Tailwind) allowed?
// 13. TypeScript or JavaScript? Prop validation wanted?
// 14. How many items should this scale to — any need for virtualization?
//
// --- Process ---
// 15. What's the time box — more features, or fewer features done thoroughly?
//
// Strategy: ask the big scope forks (expansion mode, controlled/uncontrolled,
// disabled, a11y, styling depth), then state assumptions out loud for the rest
// and write them as a comment at the top of the file.

// Improvements
// --- Bugs / correctness ---
// 1. Missing `key` prop on <AccordionRow /> in the .map() — React will warn and
//    reconciliation can misbehave. Use key={item.key}.
// 2. `const [localItems] = useState(items)` snapshots items on first render —
//    if the parent passes new items later, the list never updates. Just use
//    `items` directly; it doesn't need to be state at all.
// 3. Same problem for `openItemsKeys`: useState(openItemsKeys) only reads the
//    prop once, so the component is neither properly controlled nor properly
//    "defaultOpen". Decide on a controlled/uncontrolled API:
//      - `defaultOpenKeys` -> internal state (uncontrolled)
//      - `openKeys` + `onChange` -> parent owns state (controlled)
// 4. `onChange()` is called with no arguments — the parent gets no information.
//    Call it with the next open-keys array, e.g. onChange(nextKeys).
// 5. Use strict inequality `!==` instead of `!=` in the filter.
// 6. The requirement says "initially single open then multi-open" — there is no
//    mode for single-open. Add a prop like `allowMultiple` (or `type="single|multiple"`)
//    and in single mode replace the array instead of appending.
//
// --- State / logic cleanups ---
// 7. Use the functional updater to avoid stale-closure issues:
//    setOpenItemsKeys(prev => prev.includes(key) ? prev.filter(k => k !== key) : [...prev, key]).
//    The `[...localOpenItemsKeys].filter(...)` spread before filter is redundant —
//    filter already returns a new array.
// 8. `disabled` is global only; per-item disabling (item.disabled) would be more
//    flexible, falling back to the list-level prop.
// 9. "read only" from the requirements isn't implemented — read-only should still
//    look normal (not grayed-out like disabled) but ignore clicks; consider a
//    separate `readOnly` prop instead of reusing `disabled`.
//
// --- Accessibility ---
// 10. Follow the WAI-ARIA accordion pattern: the header button should have
//     aria-expanded={open} and aria-controls pointing at the panel's id; the
//     panel should have role="region" and aria-labelledby the button's id.
// 11. The whole row (header + panel) being one <button> is invalid semantics —
//     the panel content (description) lives inside the button, so any links or
//     interactive content in it would be nested inside a button (invalid HTML)
//     and text can't be selected. Make only the header a <button> and render the
//     panel as a sibling div.
// 12. The arrow glyph should be aria-hidden (it's decorative); state is already
//     conveyed by aria-expanded.
//
// --- Styling / UX ---
// 13. Hardcoded w-[600px] breaks on small screens — use w-full with a max-width
//     on the container, or let the parent control width.
// 14. Fixed h-[70px] when closed will clip long labels; let content size itself.
// 15. `hidden={!open}` still renders the content (fine) but consider a CSS
//     transition (grid-rows / max-height) for smooth open/close animation.
// 16. overflow-y-scroll always shows a scrollbar track; overflow-y-auto is nicer.
// 17. mb-4 on the header creates a gap even when closed; apply spacing only when open.
//
// --- API / code quality ---
// 18. `onClick = undefined` default + `if (onClick)` guard can be simplified with
//     optional chaining: onClick?.(id).
// 19. Add PropTypes or migrate to TypeScript to document the items shape
//     ({ key, label, description, disabled? }).
// 20. Naming: the list component is `Accordion` and the row is `AccordionRow`,
//     but the comment tree says "Accordion list / Accordion" — align names
//     (e.g. Accordion + AccordionItem) and prop names (id vs key is inconsistent).
// 21. Stale requirements comment (point 4 is empty) — finish or remove it.

import { useState } from "react";

// component tree
// Accordion list (list , values , default , disabled , onChange)
//      |
//   Accordion (label , description , open , onClick ,disabled )

const AccordionRow = ({
  label = "",
  description = "",
  id = "",
  open = false,
  onClick = undefined,
  disabled = false,
}) => {
  return (
    <button
      className={`flex flex-col w-[600px]  px-5 ${open ? "h-fit bg-blue-400 text-white" : "h-[70px] text-gray-500"} border border-solid items-start justify-between py-5
        rounded-lg shadow-sm`}
      disabled={disabled}
      onClick={() => {
        if (onClick) {
          onClick(id);
        }
      }}
    >
      <div className="flex items-center justify-between w-full gap-x-4 mb-4">
        <span className="text-lg font-semibold">{label}</span>
        {open ? "\u2191" : "\u2193"}
      </div>
      <div className="text-left " hidden={!open}>
        {description}
      </div>
    </button>
  );
};

const Accordion = ({
  items = [],
  openItemsKeys = [],
  disabled = false,
  onChange = undefined,
}) => {
  const [localItems] = useState(items);
  const [localOpenItemsKeys, setOpenItemsKeys] = useState(openItemsKeys);

  const handleClick = (key) => {
    if (onChange) onChange();
    if (localOpenItemsKeys.includes(key)) {
      setOpenItemsKeys(
        [...localOpenItemsKeys].filter((openKey) => openKey != key),
      );
    } else {
      setOpenItemsKeys([...localOpenItemsKeys, key]);
    }
  };
  return (
    <div className="flex flex-col gap-y-2 h-fit overflow-y-scroll items-center justify-center py-10">
      {localItems.map((item) => {
        return (
          <AccordionRow
            description={item.description}
            label={item.label}
            open={localOpenItemsKeys.includes(item.key)}
            disabled={disabled}
            onClick={handleClick}
            id={item.key}
          />
        );
      })}
    </div>
  );
};

export default Accordion;
