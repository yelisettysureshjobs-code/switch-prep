"use strict";
// const ulListener = document.getElementById("parent");

// ulListener.addEventListener("click", (e) => {
//   const btn = e.target.closest("[data-action]");

//   if (btn) {
//     console.log(btn.dataset.action);
//   }
// });

// const overLay = document.getElementById("overlay");
// const modal = document.getElementById("modal");

// overLay.addEventListener("click", (e) => {
//   console.log("event seen");
//   if (e.target === overLay) console.log("modal closed");
// });
// document.addEventListener("keydown", (e) => {
//   console.log("key event seen", e.key);
//   if (e.key === "Escape") console.log("modal closed");
// });

// const user = {
//   name: "Asha",
//   greet() {
//     console.log("hi, " + (this === undefined ? "nobody" : this.name));
//   },
// };

// // const run = user.greet;
// // const box = { name: "Box", run };
// // box.run();

// function greet(greeting, mark = ".") {
//   console.log(greeting + ", " + this.name + mark);
// }
// const asha = { name: "Asha" };
// const ravi = { name: "Ravi" };

// // greet.call(asha, "hi", "!");
// // greet.apply(ravi, ["hello", "?"]);

// // const greetAsha = greet.bind(asha, "hey");
// // greetAsha();

// class Animal {
//   constructor(name) {
//     this.name = name;
//   }
// }

// class Dog extends Animal {
//   constructor(name, breed) {
//     // 1. Call parent constructor with parameters
//     super(name);

//     // 2. Now you can use 'this'
//     this.breed = breed;
//   }
// }

// const myDog = new Dog("Rex", "German Shepherd");
// console.log(myDog.name); // Output: Rex

// const userProfile = {
//   username: "Dev123",
//   regularFunc: function () {
//     (function () {
//       console.log(this.username);
//     })(); // "Dev123" (points to userProfile object)
//   },
//   arrowFunc: () => {
//     console.log(this.username); // undefined (points to the global window/module context)
//   },
// };
// userProfile.regularFunc()

function sayHello(place) {
  console.log(`${this.name} is ${this.age} years old, and is from ${place}`);
}

const user = {
  name: "suresh",
  age: "23",
};

sayHello.call(user, "NRT");

Function.prototype.myCall = function (object = {}, ...args) {
  console.log(this, object, ...args);
  if (typeof this !== "function") {
    throw new Error("Not Callable");
  }
  object.func = this;
  object.func(...args);
};
sayHello.myCall(user, "NRT");

sayHello.apply(user, ["NRT"]);

Function.prototype.myApply = function (object = {}, ...args) {
  console.log(this, object, ...args);
  if (typeof this !== "function") {
    throw new Error("Not Callable");
  }
  if (!Array.isArray(args)) {
    throw new Error("TypeError: CreateListFromArrayLike called on non-object");
  }
  object.func = this;
  object.func(...args);
};
sayHello.myApply(user, ["NRT"]);

const bindedHello = sayHello.bind(user);
bindedHello("NRT");

Function.prototype.myBind = function (object = {}, ...args1) {
  console.log(this, object, ...args1);
  if (typeof this !== "function") {
    throw new Error("Not Callable");
  }
  object.func = this;
  return function (...args2) {
    object.func(...args1, ...args2);
  };
};

const bindedHello1 = sayHello.myBind(user);
bindedHello1("CPT");
