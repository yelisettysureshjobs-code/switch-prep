function debounce(fn, delay) {
  let timer;
  const debounced = function (...args) {
    clearTimeout(timer);
    timer = setTimeout(() => fn.apply(this, args), delay);
  };
  debounced.cancel = () => clearTimeout(timer);
  return debounced;
}

function throttle(fn, delay) {
  let lastTime = Date.now();
  return function (...args) {
    if (Date.now() - lastTime >= delay) {
      lastTime = Date.now();
      fn.apply(this, args);
    }
  };
}

const newFn = debounce(() => {
  console.log("hello");
}, 100);
console.log(newFn());
