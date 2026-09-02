# How to Study Each Track

Three tracks, three different methods. Getting this wrong is the main way people lose months.

---

## DSA — learn the pattern, then struggle

Struggle-first only works when you have something to struggle *with*. You cannot derive the monotonic stack from staring at a histogram problem — you'll burn 90 minutes, learn nothing but that you're bad at it, and quit in week 3.

### Per new pattern

1. **Learn it cold, ~30 min.** Striver's video or article for that topic. Just the concept and one worked example.
2. **2 problems with the solution open.** Not copying — reading, closing it, then typing from scratch. This is calibration, not cheating.
3. **The rest solo**, on a timer.

Roughly **20% learning, 80% struggling.**

### The 25-minute rule

The whole discipline lives here:

| Time | What you're allowed |
|---|---|
| 0–25 min | Solo. No help. Stuck means stuck — sit in it, that's where the learning is |
| 25 min, no approach | Read **only the pattern name or one hint**. Not the code. 15 more minutes |
| 40 min | Read the full solution. Understand it → **close the tab** → blank file → write it from nothing |
| After | Log it in `notes.md` with a `!` |

### Two rules that decide whether any of this sticks

- **Never "understand it and move on."** Reading a solution and nodding feels exactly like learning and is not. If you didn't type it from a blank file, you didn't learn it.
- **Re-solve at day 3 and day 14, cold.** That's what the daily Rev block is for. A problem you solved once is a problem you don't know.

### What "solved" means

Not "it passed". Solved means: you can state the trigger, write it from blank in under 20 minutes, and explain the complexity without thinking. Anything less goes back in the queue.

---

## Machine coding — you know more than you think

You build React daily. You are not missing the skill; you're missing **the format and the rubric.** That's a one-week fix, not a three-month one.

### What they actually score, roughly in order

1. **A working core feature** — a half-finished app with beautiful architecture fails. Ship the main flow first.
2. **Component decomposition** — sensible boundaries, no 400-line component
3. **State design** — where state lives, what's derived vs stored
4. **Edge states** — loading, empty, error. Most candidates skip these; it's the cheapest score on the board.
5. **Accessibility basics** — keyboard nav, focus management, semantic HTML
6. **Communication** — narrating what you're doing and why

### The 90-minute budget

| Minutes | Phase |
|---|---|
| 0–10 | Clarify requirements, sketch component tree **out loud** |
| 10–65 | Build the core happy path. Nothing else. |
| 65–80 | Edge states, polish, obvious a11y |
| 80–90 | Walk them through it, name what you'd do with more time |

### Approach

**Code-first, always.** Read the rubric above once, then just build. First 3 builds untimed to find your natural pace; everything after that on a strict timer.

The failure mode is never knowledge — it's time. People over-engineer the first 20 minutes and never finish. **Finishing an ugly working app beats an elegant unfinished one, every single time.**

### Rules

- No component libraries. Build the primitive yourself; that's what's being tested.
- Say your plan out loud before typing. Silent coding scores badly even when the code is right.
- If you're behind at the 65-minute mark, cut scope and say so explicitly. Naming the tradeoff scores; running out of time silently does not.

---

## System design — learn-first, no exceptions

The only track where struggling first is pure waste. You cannot derive SSR-vs-CSR tradeoffs, CDN behaviour, or cache invalidation from first principles under pressure. It's vocabulary and accumulated patterns.

### Learn a framework before any content

**RADIO** — the skeleton that means you're never staring at a blank whiteboard:

| | |
|---|---|
| **R**equirements | Functional + non-functional. Ask, don't assume. Scope it down out loud. |
| **A**rchitecture | Components, data flow, module boundaries |
| **D**ata model | What state exists, where it lives, who owns it |
| **I**nterface | API contracts + component props/events |
| **O**ptimizations | Perf, a11y, error states, edge cases |

Having a skeleton is most of the battle — the real failure mode is a blank whiteboard and a rambling five minutes.

### Per topic

1. Read it
2. **Answer out loud, timed, alone** — 40 minutes, no notes
3. Compare against a reference answer
4. Write down what you missed

Step 2 is the one people skip and it's the only one that matters. **Talking is the skill being tested, and it's completely separate from knowing the material.** Record yourself once in Month 3. It's unpleasant, and it's the fastest correction you'll get all year.

### Don't start before Month 3

You'd be memorising vocabulary with nothing to attach it to, and you'd forget it before it's tested. The plan sequences it deliberately: by Month 3 you've built enough that the concepts have somewhere to land.

---

## Time ratio

**Months 1–2:** ~60% DSA · ~30% JS/React fundamentals · ~10% learning the machine-coding format
**Months 3–4:** ~45% DSA · ~25% project/machine coding · ~20% system design · ~10% React depth
**Month 5+:** DSA drops to maintenance; the hours move to output and interviews

---

## Resources

Recalled from memory — verify they still exist and match before committing time:

| Track | Resource |
|---|---|
| DSA | Striver's A2Z on takeuforward.org — videos + articles per topic; this plan follows its ordering |
| Machine coding | frontendeval.com (free prompts) · GreatFrontEnd · Chirag Goel on YouTube for worked rounds |
| Frontend system design | GreatFrontEnd's system design section (source of RADIO) · Akshay Saini's Namaste Frontend System Design (paid, structured) |
| HLD basics | ByteByteGo / Alex Xu — one level deep only, you're not interviewing as a backend engineer |
| JS fundamentals | javascript.info for depth · Akshay Saini's Namaste JavaScript for the event loop and closures |

**Pick one resource per track and finish it.** Collecting resources feels like progress and is the most comfortable way to waste a month.

---

## The one habit that matters most

Every problem, every build, every design answer produces **one line in `notes.md`**: what it was, which pattern, what the trigger was, and whether you failed it.

That file is what makes Month 5's weak-pattern sweep possible, and it's the difference between 200 problems solved and 200 problems *known*. Everything else in this repo is scaffolding around that habit.
