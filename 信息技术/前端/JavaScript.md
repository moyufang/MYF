
### 基本使用

##### 引入
```html
<script src=""></script>
```
##### debug输出 注释
```js
var num=10;
console.log("Hello! I have %d apples", num)
```
##### 变量声明
有 `var`、`let` 两种，`var` 声明的变量是函数级作用域的，`let` 则是块级作用域的，什么都不带的是全局变量
```js
var x = 1, y = 2;
if (x > y){
	let z = 3;
}
console.log(z) //报错
```
js中有 7 种基本数据类型：number,boolean, string, undefined,bigint,symbol，通过 typeof 可以输出数据类型
```js
let arr = [1, true, 1.2, 'sfs', 's', [234], null, undefined,BigInt(2), Symbol('de')]
//数据类型是['number', 'boolean', 'number', 'string', 'string', 'object', 'object', 'undefined', 'bigint', 'symbol']
for (let x in arr)console.log(`The datatype of ${x}th obj in arr is ${typeof arr[x]}`)
```
JavaScript 中整数、浮点数均用 `float64` 表示，所以绝对值最大的安全整数是 $2^{53}-1$，对更大的整数，可以用 `BigInt`，声明方法如下
```js
var bi1 = 9223372036854775807n;
var bi2 = BigInt(2342);
var bi3 = BigInt("0x7fffffffffffffff")
```

##### 数组的声明和特性
```js
var ar1 = new Array(10).fill(null) //一个大小为 10 的数组，默认值是 undefined，然后填充为 null
var ar2 = [1, 2, "0", null, undefined, [2, 3, 4]]

// 索引超出范围会导致数组大小自动调整:
let arr = ['A', 'B', 'C']; arr[5] = 'x';
console.log(arr, arr.length); // arr变为['A', 'B', 'C', undefined, undefined, 'x']，长度变为 6

// 数组和字符串的切片均使用 slice
arr.slice(1, 3)
```
##### 对象
```js
var user={
	name:"sdf",
	age:12,
	"married":null
}
console.log(user.name, user["name"])
console.log(user["abc"]) //未定义属性返回 undefined
user.hasOwnProperty("name"); //判断对象是否有该属性
```
对象可以当字典用，但要求键全部为字符串，可以用 `Map` 或 `Set` 代替
```js
let m = Map();
m.set('Adam', 67);
m.get('Adam');
```

javascript 中万物皆对象，对象皆有属性，在浏览器中，默认的顶层对象是 `window`，所有在脚本中声明的变量（包括基本数据结构、对象、函数）均可以通过 `window.<name>` 来访问
```js
let x = 123;
window.alert()
window.console.log(x)
```
命名空间（即作用域）也可以通过对象来实现
```js
let my_app = {};
my_app.func = function(x){ return x*x; };
my_app.app_var = "234"
console.log(my_app.func(2), my_app.app_var, my_app['app_var']);
```
##### 字符串
```js
var 中文 = "\u4e2d\u6587"
var abc = "\x61\x62\x63"
var multi = `
这里是多行文本
abcdefg
1234567
` //使用反引号声明多行字符串
var `这里是模版字符串 ${中文} ${abc} ${multi}` //使用反引号声明嵌入字符串
```
注意，字符串是不可变的，如果对字符串的某个索引值赋值，不会引发语法错误但没有任何效果。
`toLowerCase(),toUpperCase,toString()` 等函数会创建一个新字符串。
##### 分支
```js
if (){ }
else if (){ }
else{ }
```
#### 循环
```js
var a = {name:"Jony",age:170}
var b = ["a", a, "BB", "CCC"]
for (let x in a){ console.log(x, a[x]); } //x 是对象键
for (let x in b){ console.log(x, b[x]); } //x 是数组索引
for (i=0; i < 4; ++i){ console.log(i, b[i]); 

//迭代键值对
var obj = {'1':'a', '2':'b', '3':'c'}
for (let value of Object.keys(obj))
for (let key of Object.keys(obj))
for (let [key,value] of Object.entries(obj));
```

解构赋值
```js
let a = [1, 2, [3, 4]];
let [x, y, [z, w]] = x; //对数组，两边结构须一致
let [t,,[s,]] = x //但可以通过省略变量名来提取部分值

let user = {
	name:"Alice",
	age:20,
	address:{
		city:"Guangzhou",
		street:"Baiyun",
		email:"example@.com.cn"
	}
}
let {name, address:{city,email}, alive=True} = user;
	//对于对象，解构赋值可以将值赋给与键值同名的变量
	//如果键不存在则值被赋值为 undifine, 但是可以通过=指定键不存在时的默认值
console.log(name, city, email) //此例中, address 不是变量名的一部分
```
#### 展开、剩余操作符与

展开操作符`...` 用于展开可迭代对象（如数组、字符串、Map、Set等）或对象。
```javascript
// 展开数组
const arr1 = [1, 2, 3];
const arr2 = [4, 5, 6];
const combined = [...arr1, ...arr2]; // [1, 2, 3, 4, 5, 6]

// 函数调用传参
const numbers = [1, 2, 3];
Math.max(...numbers); // 等同于 Math.max(1, 2, 3)

// 展开对象，靠右属性会覆盖靠左的属性
// ES2018+ 支持
const defaults = { theme: 'light', fontSize: 16 };
const userSettings = { fontSize: 14 };
const finalSettings = { ...defaults, ...userSettings }; // { theme: 'light', fontSize: 14 }
```

剩余操作符仍然是 `...`，用于表示剩余的元素或参数。
```javascript
//数组解构
const [first, second, ...rest] = [1, 2, 3, 4, 5];

//函数传参
function sum(first, ...rest) {
  return rest.reduce((acc, num) => acc + num, first);
}

//对象解构
const { a, b, ...rest } = { a: 1, b: 2, c: 3, d: 4 };
```
### 函数

函数中有一个装有所有传参的默认对象 `arguments`，它的索引从 0 到 `arguments.length-1`，值是传参的值，注意 `arguments` 与函数声明中的默认传参无关，只与实际传参有关。
`...rest_parameter` 是只能放在函数末尾、装有剩余参数的数组。
```js
function foo(a, b, ...rest){
	for (let i in arguments){
		//console.log(`argu[${i}]=${arguments[i]}`)
	}
	console.log('a = ' + a);
    console.log('b = ' + b);
    console.log(rest);
}

foo(1, 2, 3, 4, 5);
// 结果:
// a = 1
// b = 2
// Array [ 3, 4, 5 ]

foo(1);
// 结果:
// a = 1
// b = undefined
// Array []
```

js 的函数定义有一个特点，会先扫描一便函数将所有声明生效，然后再根据执行顺序依次赋值
```js
function foo(){
	var x = "y:"+y; // 输出"y:undefined"
	var y = "text";
	console.log(x)
}
```
这种奇怪的特性可以通过用 `let` 替换 `var` 而避免。

在默认情况下 js 函数声明后，会被挂在声明它的作用域上，当函数被调用时 `this` 指代了该作用域
```js
let x = 1;
var y = 2;
function f(){ console.log(this.x, this.y); }
	//结果：undefined 2
	//this 在此相当于 window
let user = {
	name:"Bob",
	method:function(){ console.log(this.name); } //这里 this 相当于 user
}
f();
user.method();
```
从对象角度上看 `let` 和 `var` 区别是，`let` 声明的变量不会绑定在对象上，`var` 会，这也是为什么 `var` 变量在赋值前可以被访问为 `undefined`。
`call` 和 `apply` 方法可以在调用函数时指定 `this`，后者通过数组传参
```js
var base = 2;
function sum(...arr){
	let ret = this.base;
	for (let x of arr){ ret+=x;}
	return ret;
}
console.log(sum(2, 3, 4)); //11
console.log(sum.call({base:3}, 2, 3, 4)); //12
let arr = [];
for (let i=1;i<=10;++i){arr.push(i);}
console.log(sum.apply({base:3}, arr);) //58
console.log(sum(...arr)) //57
```

##### `=>` 函数

`=>` 函数与匿名函数不同的一点在于 `this` 不再挂在声明它的作用域上，而是调用它作用域上，即使使用 `call` 和 `apply` 也无法将 `this` 绑定在特定对象上
```js
base = 1;
let fn = x => (x+this.base);
console.log(fn.call({base:432}, 2)); //3
```
##### 标签函数
一种通过嵌入式字符串向函数传参的方法，例子如下
```js
const email = "test@example.com";
const password = 'hello123';
function sql(strings, ...exps) {
	//在下面的例子中
	//strings是["SELECT * FROM users WHERE email=", " AND password=", ""]
	//exps是["test@example.com", "hello123"]
    console.log(`SQL: ${strings.join('?')}`);
    console.log(`SQL parameters: ${JSON.stringify(exps)}`);
    return {name: '小明',age: 20};
}
const result = sql`SELECT * FROM users WHERE email=${email} AND password=${password}`;
console.log(JSON.stringify(result));

```
##### 生成器
声明生成器的语法是 `function*`
```js
    fn = (x) => (console.log("Call from fn at", x), x);
    function* g(){
        for(let i = 0; i < 10; ++i) yield fn(i);
    }
    let gg = g();
    gg.next(); gg.next(); gg.next();
```

### 标准对象

##### RegExp
正则表达式的声明有两种
```js
let target_str = "abcdefg ab234"
let re1 = /^[\d\w]+$/
let re2 = new RegEpr("^[\\d\\w]$")

re1.test(target_str) //只要匹配上至少一个，就返回True
re2.exec(target_str) //返回一个装有所有匹配上的数组
target_str.split(/"\s+"/) //按正则表达式分割字符串，返回数组
```


### 面向对象

```js
class Student{
	constructor(name){
		this.name = name;
	}
	hello(){
		console.log("Hello! I am "+this.name+"!");
	}
}
class PrimaryStudent extends Student{
	constructor(name, grade){
		super(name);
		this.grade = grade;
	}
}
```


### 浏览器

一个 html 界面相当于由 DOM 节点组成的 DOM 树，`document` 默认为这个页面的根，浏览器中按 F12 显示的控制台中 的 Elements 项就是 DOM 树的结构（作为根的 `document` 没显示）

在 DOM 节点中有以下得到内部子节点方法
```js
getElementById() //返回 DOM 节点
getElementsByClassName() //返回可以像数组一样索引的 HTMLCollection 对象
getElementsByTagName() //返回可以像数组一样索引的 HTMLCollection 对象
```

### 后端交互

##### CORS

浏览器 不允许直接通过 `file://` 导入其它资源，解决方法是搭一个支持 CORS 的本地服务器（），浏览器能够获取该服务器上的资源。

##### Promise

Promise 是 ES6 标准后用来处理异步响应的一个强大工具。

Promise 对象被创建时，接受一个有两个参数（一般命名为`resolve`和 `reject`，它们均被期望是一个仅由一个参数的函数）的函数 func，它会被立刻执行。

注意到 func 的函数体中可以调用 `resolve` 和 `reject`，它俩一般是在某异步函数中被调用，也就是说有可能 func 的函数体执行完了 `resolve` 和 `reject` 都还没被调用。

Promise 对象只有三个状态
- `pending`：`resolve` 和 `reject` 被调用前的状态
-  `fulfilled`：一旦 `resolve` 被调用，就会陷入这个状态
- `rejected`：一旦 `reject` 被调用，就会陷入这个状态
Promise 对象一旦进入 `fulfilled` 或 `rejected` 状态，它的状态就不再改变。

Promise 对象有个方法 `.then(resolve_then[,reject_then])`，被调用时根据 Promise 对象的状态产生以下行为
- `pending`：暂时不产生任何效果，直到该 Promise 对象的状态改变时才产生效果。这种情况下 then 的调用相当于一个异步函数的调用。
- `fulfilled`：调用 `resolve_then(solo_argu_resolve)`，其中 `solo_argu_resolve` 是 `resolve` 在 `func` 中被调用时传进去的参数，它在调用 `resolve_then` 前就已经确定。然后根据 `resolve_then` 的 return 情况返回一个的 Promise 对象。
- `rejected`：
	- 如果 `reject_then` 不为 `undefined` 则调用 `reject_then(solo_argu_reject)`，`solo_argu_reject` 类似于 `solo_argu_resolve`。然后根据 `reject_then` 的 return 情况返回一个的 Promise 对象。
	- 如果为 `undefined` 则返回一个 状态为 `rejected` 且能传递 `solo_argu_reject` 的 Promise 对象。
在上述描述中调用 `resolve_then` 或 `reject_then` 的情况下
- 如果`resolve_then` 或 `reject_then` 返回了一个不是 Promise 对象的对象 `ret`（没有返回相等于 `ret===undefined`），则 `then` 会返回一个将 `ret` 作为 `solo_argu_resolve` 或 `solo_argu_reject` 的状态为 `fulfilled` 的 Promise 对象。
- 否则`resolve_then` 或 `reject_then` 返回了一个 Promise 对象，`then` 会将这个 Promise 对象直接返回。
也就是说，不管怎样，`then` 会返回一个 Promise 对象。

Promise 对象还有个方法 `.catch(reject_then)`，功能类似于 `.then(undefined, reject_then)` 

### 事件响应

事件响应函数可传入事件对象 `event`，`event.target` 始终指向响应元素。
