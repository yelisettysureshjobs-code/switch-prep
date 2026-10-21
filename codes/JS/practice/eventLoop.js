// async function processChuks(items, size, onDone) {
//   for (let index = 0; index < items.length; index++) {
//     const element = items[index];
//     console.log(element + 1);
//     if ((index + 1) % size === 0) {
//       await new Promise((resolve) => setTimeout(resolve, 0));
//     }
//   }
//   onDone();
// }

// let arr = new Array(50).fill(0).map((v, i) => i);
// const timer = setInterval(() => console.log("heart beat"), 0);
// processChuks(arr, 10, () => {
//   console.log("done");
//   clearInterval(timer);
// });

// myQueueMicrotask

async function myQueueMicrotask(fun) {
  Promise.resolve().then(fun);
}

setTimeout(() => console.log("task"), 0);
myQueueMicrotask(() => console.log("micro task"));
console.log("sync");
