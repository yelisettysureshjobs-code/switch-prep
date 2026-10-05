// const counterFactory = () => {
//   let count = 0;
//   return {
//     add: (val) => (count = count + val),
//     delete: (val) => countcount + val,
//     show: () => console.log(count),
//   };
// };

// let counter = counterFactory();

// counter.add(5);
// counter.add(5);
// counter.show();

// let counter1 = counterFactory();

// counter1.add(5);
// counter1.add(10);
// counter1.show();

class Counter{
    #count
    constructor(){
        this.#count=0;
    }
    add(){
        this.#count++;
    }
    delete(){
        this.#count--;
    }
    show(){
        console.log(this.#count)
    }
}

let count=new Counter();
count.add()
count.add()
count.add()
count.add()
count.add()
count.show()
