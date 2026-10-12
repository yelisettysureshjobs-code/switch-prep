// const ulListener = document.getElementById("parent");

// ulListener.addEventListener("click", (e) => {
//   const btn = e.target.closest("[data-action]");

//   if (btn) {
//     console.log(btn.dataset.action);
//   }
// });

const overLay = document.getElementById("overlay");
const modal = document.getElementById("modal");

overLay.addEventListener("click", (e) => {
  console.log("event seen");
  if (e.target === overLay) console.log("modal closed");
});
document.addEventListener("keydown", (e) => {
  console.log("key event seen", e.key);
  if (e.key === "Escape") console.log("modal closed");
});
