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
