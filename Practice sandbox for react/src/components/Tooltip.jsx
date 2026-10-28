import { useState, useRef } from "react";
import { createPortal } from "react-dom";

function Tooltip({ text, children }) {
  const [isVisible, setIsVisible] = useState(false);
  const [position, setPosition] = useState({
    top: 0,
    left: 0,
  });

  const triggerRef = useRef(null);

  const handleMouseEnter = () => {
    const rect = triggerRef.current.getBoundingClientRect();

    setPosition({
      top: rect.top - 8,
      left: rect.left + rect.width / 2,
    });

    setIsVisible(true);
  };

  return (
    <span
      ref={triggerRef}
      onMouseEnter={handleMouseEnter}
      onMouseLeave={() => setIsVisible(false)}
    >
      {children}

      {isVisible &&
        createPortal(
          <div
            className="fixed z-50 -translate-x-1/2 -translate-y-full rounded-md bg-gray-900 px-3 py-2 text-sm text-white shadow-lg"
            style={{
              top: position.top,
              left: position.left,
            }}
          >
            {text}
          </div>,
          document.body,
        )}
    </span>
  );
}

export default Tooltip;
