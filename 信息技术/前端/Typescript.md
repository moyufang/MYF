##### 基本语法
```ts
let helloWorld:string = "Hello World";

interface User {
  name: string;
  id: number;
};
const user:User = {name:"Hayes", id:0};

class UserAccount{
  name:string;
  id:number;
  constructor(name:string, id:number){
    this.name = name;
    this.id = id;
  }
};
//符合接口规范
const user2:User = new UserAccount("Murphy", 1);

function get_admin_user():User{
	//...
}

```

##### 组合类型
```ts

//处理多种类型
function getLength(obj: string | string[]) {
  //判断类型是什么用
  //typeof s === "string"
  return obj.length;
}

//可将类型声明为枚举类型
type my_bool = true | false;
type WindowStates = "open" | "closed" | "minimized";
type LockStates = "locked" | "unlocked";
type PositiveOddNumbersUnderTen = 1 | 3 | 5 | 7 | 9;

//类型取别名
type StringArray = Array<string>;
type NumberArray = Array<number>;
type ObjectWithNameArray = Array<{ name: string }>;

//泛型
interface Backpack<Type> {
  add: (obj: Type) => void;
  get: () => Type;
}
```
