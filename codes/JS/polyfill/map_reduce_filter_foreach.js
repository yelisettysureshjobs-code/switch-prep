Array.prototype.myMap = function (fun) {
  let ans = [];
  for (let i = 0; i < this.length; i++) {
    ans.push(fun(this[i], i, this));
  }
  return ans;
};
Array.prototype.myFilter = function (fun) {
  let ans = [];
  for (let i = 0; i < this.length; i++) {
    if (fun(this[i])) {
      ans.push(this[i], i, this);
    }
  }
  return ans;
};

Array.prototype.myReduce = function (fun, initialValue) {
  let i = 0;
  let a;

  if (arguments.length > 1) {
    a = initialValue;
  } else {
    a = this[0];
    i = 1;
  }

  for (; i < this.length; i++) {
    a = fun(a, this[i], i, this);
  }

  return a;
};

Array.prototype.myForEach = function (fun) {
  for (let i = 0; i < this.length; i++) {
    fun(this[i], i, this);
  }
};

const a = [1, 2, 3];
console.log(a.myMap((val) => val * 5));
console.log(a.myFilter((val) => val % 2 === 1));
console.log(a.myForEach((val) => val * 2));
console.log(
  [...a, ...a, ...a].myReduce(function (acc, ch) {
    if (!acc[ch]) {
      acc[ch] = 0;
    }

    acc[ch] += 1;

    return acc;
  }, {}),
);
console.log(
  [...a].myReduce(function (sum, cur) {
    return sum + cur;
  }, 100),
);
