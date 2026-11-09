const p1 = new Promise((resolve, reject) => {
  setTimeout(() => {
    reject("p1 resolved");
  }, 3000);
});
const p2 = new Promise((resolve, reject) => {
  setTimeout(() => {
    // resolve("p2 resolved");
    reject("p2 rejected");
  }, 2000);
});
const p3 = new Promise((resolve, reject) => {
  setTimeout(() => {
    reject("p3 resolved");
  }, 1000);
});

Promise.myAll = function (promises) {
  return new Promise((resolve, reject) => {
    if (!Array.isArray(promises)) {
      throw new Error("promises is not iterable");
    }
    const result = [];
    const n = promises.length;
    let completed = 0;
    if (n == 0) {
      return resolve(result);
    }
    promises.forEach(async (promise, idx) => {
      try {
        const res = await promise;
        result[idx] = res;
        completed++;

        if (completed == n) {
          resolve(result);
        }
      } catch (error) {
        reject(error);
      }
    });
  });
};
Promise.myAllSettled = function (promises) {
  return new Promise((resolve, reject) => {
    if (!Array.isArray(promises)) {
      throw new Error("promises is not iterable");
    }
    const n = promises.length;
    let completed = 0;

    const result = [];
    if (n == 0) {
      return resolve(result);
    }
    promises.forEach(async (promise, idx) => {
      try {
        const res = await promise;
        result[idx] = { status: "resolved", res };
        completed++;
      } catch (error) {
        result[idx] = { status: "rejected", error };
        completed++;
      } finally {
        if (completed == n) {
          return resolve(result);
        }
      }
    });
  });
};
Promise.myRace = function (promises) {
  return new Promise((resolve, reject) => {
    if (!Array.isArray(promises)) {
      throw new Error("promises is not iterable");
    }
    const n = promises.length;

    if (n == 0) {
      return resolve({});
    }
    promises.forEach(async (promise, idx) => {
      try {
        const res = await promise;
        return resolve(res);
      } catch (error) {
        return reject(error);
      }
    });
  });
};
Promise.myAny = function (promises) {
  return new Promise((resolve, reject) => {
    if (!Array.isArray(promises)) {
      throw new Error("promises is not iterable");
    }
    const n = promises.length;
    let completed = 0;

    const result = [];
    if (n == 0) {
      return resolve(result);
    }
    promises.forEach(async (promise, idx) => {
      try {
        const res = await promise;
        completed++;
        return resolve(res);
      } catch (error) {
        result[idx] = { status: "rejected", error };
        completed++;
      } finally {
        if (completed == n) {
          return reject(new AggregateError(result, "All promises rejected"));
        }
      }
    });
  });
};

// Promise.myAll([p1, p2, p3])
// Promise.myAllSettled([p1, p2, p3])
// Promise.myRace([p1, p2, p3])
Promise.myAny([p1, p2, p3])
  .then((res) => {
    console.log(res);
  })
  .catch((err) => {
    console.error(err);
  });
