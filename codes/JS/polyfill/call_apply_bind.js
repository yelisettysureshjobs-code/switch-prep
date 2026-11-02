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

Function.prototype.myApply = function (obj = {}, args = []) {
  if (typeof this !== "function") {
    throw new Error("Not callable");
  }

  if (!Array.isArray(args)) {
    throw new TypeError("Arguments must be an array");
  }

  obj = obj ?? {};

  const key = Symbol("fn");
  obj[key] = this;

  try {
    return obj[key](...args);
  } finally {
    delete obj[key];
  }
};
sayHello.myApply(user, ["NRT"]);

const bindedHello = sayHello.bind(user);
bindedHello("NRT");

Function.prototype.myBind = function (obj = {}, ...args) {
  if (typeof this !== "function") {
    throw new TypeError("Not callable");
  }

  const fn = this;

  return function (...newArgs) {
    const key = Symbol("fn");
    obj[key] = fn;

    try {
      return obj[key](...args, ...newArgs);
    } finally {
      delete obj[key];
    }
  };
};

const bindedHello1 = sayHello.myBind(user);
bindedHello1("CPT");
