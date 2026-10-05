// 1.Requirements
/*
1.user should be able to select the rating
2. when user hovers on the star, the stars up to that point should be filled
3. the no of stars should be customisable
*/
// CHANGED: removed the "color should be dynamic" requirement, colors are fixed.

import { useState } from "react";

const FILLED_COLOR = "#FFD700";
const EMPTY_COLOR = "#212120";

// CHANGED: Star is now purely presentational. The click/hover handlers moved
// to a <button> in StarRating, so the svg no longer needs index or callbacks.
// CHANGED: color is set once via `style.color` and both fill and stroke read
// it through currentColor (before, the same value was passed three ways).
// CHANGED: an empty star is an outline (fill="none") instead of a solid shape.
const Star = ({ filled }) => (
  <svg
    className="h-5 w-5"
    style={{ color: filled ? FILLED_COLOR : EMPTY_COLOR }}
    viewBox="0 0 24 24"
    fill={filled ? "currentColor" : "none"}
    xmlns="http://www.w3.org/2000/svg"
    // CHANGED: the button carries the accessible label, so hide the svg itself
    aria-hidden="true"
  >
    <path
      d="M12 2.5L14.93 8.43L21.5 9.38L16.75 13.99L17.87 20.5L12 17.43L6.13 20.5L7.25 13.99L2.5 9.38L9.07 8.43L12 2.5Z"
      stroke="currentColor"
      strokeWidth="1.8"
      strokeLinejoin="round"
    />
  </svg>
);

// CHANGED: new props.
// - value / onChange: lets a parent control and read the rating
// - defaultValue: starting rating when the parent does not control it
// - readOnly: display-only mode
// - className: lets the parent style/position the wrapper
// CHANGED: noOfStars defaults to 5 (the old default of 0 rendered nothing).
const StarRating = ({
  noOfStars = 5,
  value,
  defaultValue = 0,
  onChange,
  readOnly = false,
  className = "",
}) => {
  const [internalValue, setInternalValue] = useState(defaultValue);
  const [hoveredStar, setHoveredStar] = useState(0);

  // CHANGED: if the parent passes `value` we use it, otherwise our own state.
  const isControlled = value !== undefined;
  const selectedStar = isControlled ? value : internalValue;

  // CHANGED: sanitise the count. `new Array(noOfStars)` threw a RangeError for
  // negatives/decimals and produced a single star for the string "5".
  const count = Math.max(0, Math.floor(Number(noOfStars)) || 0);

  // CHANGED: hover takes precedence over the selection, so hovering a lower
  // star previews the lower rating. Before, the two were OR-ed together and
  // the selected stars always stayed lit.
  const activeStar = hoveredStar || selectedStar;

  const handleSelect = (starValue) => {
    // CHANGED: clicking the already selected star clears the rating.
    const nextValue = starValue === selectedStar ? 0 : starValue;
    if (!isControlled) setInternalValue(nextValue);
    onChange?.(nextValue);
    // Drop the hover preview so the result of the click shows immediately
    // (otherwise a cleared rating still looks filled until the mouse leaves).
    setHoveredStar(0);
  };

  return (
    // CHANGED: removed `w-full min-h-screen` and the centering classes. Page
    // layout now lives in App.jsx so this component can be dropped anywhere.
    // CHANGED: one onMouseLeave on the container instead of one per star, so
    // the preview does not reset while moving between stars.
    <div
      role="group"
      aria-label="Rating"
      className={`inline-flex ${className}`}
      onMouseLeave={() => setHoveredStar(0)}
    >
      {Array.from({ length: count }, (_, index) => {
        // CHANGED: stars are 1-based (1..count) and 0 means "no rating".
        // Before, the 0-based index was compared with `<=` against a state
        // whose "nothing" value was also 0, so the first star was always
        // filled and a zero rating was impossible.
        const starValue = index + 1;
        return (
          // CHANGED: each star is a real <button>, so it is focusable, works
          // with Enter/Space, and is announced by screen readers. Focus/blur
          // mirror hover so keyboard users get the same preview.
          <button
            type="button"
            key={starValue}
            className={`p-0 bg-transparent border-0 ${
              readOnly ? "cursor-default" : "cursor-pointer"
            }`}
            aria-label={`Rate ${starValue} out of ${count}`}
            aria-pressed={starValue === selectedStar}
            disabled={readOnly}
            onClick={() => handleSelect(starValue)}
            onMouseEnter={() => setHoveredStar(starValue)}
            onFocus={() => setHoveredStar(starValue)}
            onBlur={() => setHoveredStar(0)}
          >
            <Star filled={starValue <= activeStar} />
          </button>
        );
      })}
    </div>
  );
};

export default StarRating;
