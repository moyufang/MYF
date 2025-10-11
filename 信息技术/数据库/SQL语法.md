SQL语言分类
```sql
-- 数据定义语言（DDL）
CREATE, ALTER, DROP

-- 数据查询语言（DQL）  
SELECT, WHERE, GROUP BY, HAVING, OVER

-- 数据操作语言（DML）  
INSERT, UPDATE, DELETE

-- 数据控制语言（DCL）
GRANT, REVOKE

-- 事务控制语言（TCL）
COMMIT, ROLLBACK, SAVEPOINT
```
### 数据类型详解

数值类型
- `INT` / `INTEGER`：整数
- `DECIMAL(p, s)`：精确小数（p=总位数，s=小数位数）
- `FLOAT` / `REAL`：浮点数
- `NUMERIC`：数值类型

字符串类型
- `CHAR(n)`：定长字符串
- `VARCHAR(n)`：变长字符串
- `TEXT`：长文本

日期时间类型
- `DATE`：日期（YYYY-MM-DD）
- `TIME`：时间（HH:MM:SS）
- `DATETIME`：日期时间
- `TIMESTAMP`：时间戳

## 数据定义语言（DDL）

创建数据库
```sql
CREATE DATABASE company;
CREATE DATABASE school CHARACTER SET utf8mb4;
```

创建表（CREATE TABLE）
```sql
CREATE TABLE employees (
    -- 列定义
    emp_id INT PRIMARY KEY,
    emp_name VARCHAR(50) NOT NULL,
    birth_date DATE,
    salary DECIMAL(10,2),
    dept_id INT,
    
    -- 表级约束
    FOREIGN KEY (dept_id) REFERENCES departments(dept_id)
);
```

约束（Constraints）
```sql
--列级约束
CREATE TABLE students (
    student_id INT PRIMARY KEY,
    student_name VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE,
    age INT CHECK (age >= 0 AND age <= 150),
    class_id INT REFERENCES classes(class_id)
);

-- 表级约束
CREATE TABLE orders (
    order_id INT,
    product_id INT,
    quantity INT,
    PRIMARY KEY (order_id, product_id),
    FOREIGN KEY (product_id) REFERENCES products(product_id),
    CHECK (quantity > 0)
);
```

修改表结构（ALTER TABLE）
```sql
-- 添加列
ALTER TABLE employees ADD COLUMN phone VARCHAR(15);

-- 修改列
ALTER TABLE employees ALTER COLUMN salary SET DEFAULT 0;

-- 删除列
ALTER TABLE employees DROP COLUMN phone;

-- 添加约束
ALTER TABLE employees ADD CONSTRAINT fk_dept 
FOREIGN KEY (dept_id) REFERENCES departments(dept_id);

ALTER TABLE ADD CONSTRAINT CHECK quantity < 10;

-- 添加列约束
ALTER TABLE employees
ALTER COLUMN quantity SET DEFAULT,
ALTER COLUMN id DROP NOT NULL;
```

删除表（DROP TABLE）
```sql
DROP TABLE employees;          -- 直接删除
DROP TABLE employees CASCADE;  -- 级联删除相关对象
DROP TABLE IF EXISTS employees; -- 安全删除
```
### 约束类型

```sql
PRIMARY KEY
FOREIGN KEY

CHECK
UNIQUE
DEFAULT
NOT NULL

--自动生成唯一的数字序列，不同数据库有不同语法
-- SQL Server
CREATE TABLE employees (
    id INT IDENTITY(1,1) PRIMARY KEY,
    name VARCHAR(50)
);

-- MySQL
CREATE TABLE employees (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50)
);

-- PostgreSQL
CREATE TABLE employees (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50)
);

```

### 数据查询语言（DQL） 
### 基本查询（SELECT）

SELECT语句基本结构
```sql
SELECT [DISTINCT] column1, column2, ...
FROM table_name
[WHERE condition]
[ORDER BY column1 [ASC|DESC], ...]
[LIMIT number];
```

选择特定列
```sql
-- 选择所有列
SELECT * FROM employees;

-- 选择特定列
SELECT emp_id, emp_name, salary FROM employees;

-- 使用DISTINCT去重
SELECT DISTINCT dept_id FROM employees;

--列别名
SELECT 
    emp_id AS "员工编号",
    emp_name AS "员工姓名",
    salary * 12 AS "年薪"
FROM employees;

--表达式和计算
SELECT 
    product_name,
    unit_price,
    quantity,
    unit_price * quantity AS total_amount
FROM order_details;
```

### WHERE子句（条件筛选）

比较运算符
```sql
-- 等于
SELECT * FROM employees WHERE salary = 5000;

-- 不等于
SELECT * FROM employees WHERE salary <> 5000;

-- 大于、小于
SELECT * FROM employees WHERE salary > 5000;
SELECT * FROM employees WHERE salary < 5000;

-- 范围
SELECT * FROM employees WHERE salary BETWEEN 3000 AND 8000;
```

逻辑运算符
```sql
-- AND
SELECT * FROM employees 
WHERE salary > 5000 AND dept_id = 10;

-- OR  
SELECT * FROM employees
WHERE dept_id = 10 OR dept_id = 20;

-- NOT
SELECT * FROM employees 
WHERE NOT dept_id = 10;
```

模糊匹配（LIKE）
```sql
-- % 匹配任意多个字符
SELECT * FROM employees WHERE emp_name LIKE '张%';

-- _ 匹配单个字符
SELECT * FROM employees WHERE emp_name LIKE '张_';

-- 包含特定字符
SELECT * FROM employees WHERE emp_name LIKE '%明%';
```

IN 和 NULL 判断
```sql
-- IN 运算符
SELECT * FROM employees 
WHERE dept_id IN (10, 20, 30);

-- NULL 判断
SELECT * FROM employees 
WHERE phone IS NULL;

SELECT * FROM employees
WHERE phone IS NOT NULL;
```

### 聚合函数

常用聚合函数
```sql
-- 计数
SELECT COUNT(*) FROM employees;
SELECT COUNT(DISTINCT dept_id) FROM employees;

-- 求和、平均、方差、标准差
SELECT
SUM(salary),
AVG(salary),
VARIANCE(salary),
STDDEV(salary) FROM employees;

-- 最大值、最小值
SELECT MAX(salary), MIN(salary) FROM employees;
```

`GROUP BY` 以及 `HAVING`
```sql
-- 筛选平均工资大于5000且人数大于10的部门
SELECT 
    department,
    AVG(salary) AS avg_salary
FROM employees
GROUP BY department
HAVING AVG(salary) > 5000 AND COUNT(*) > 10;
```

`OVER`
（未完待续）

### 数据操作语言（DML）

插入数据（INSERT）
```sql
-- 插入完整行
INSERT INTO employees 
VALUES (1, '张三', '1990-01-01', 5000.00, 10);

-- 插入指定列
INSERT INTO employees (emp_id, emp_name, salary)
VALUES (2, '李四', 6000.00);

-- 插入多行
INSERT INTO employees VALUES
(3, '王五', '1992-05-15', 5500.00, 20),
(4, '赵六', '1991-08-20', 7000.00, 10);
```

更新数据（UPDATE）
```sql
-- 更新所有行
UPDATE employees SET salary = salary * 1.1;

-- 条件更新
UPDATE employees 
SET salary = salary * 1.2 
WHERE dept_id = 10;

-- 更新多个列
UPDATE employees
SET salary = 8000.00, dept_id = 30
WHERE emp_id = 1;
```

删除数据（DELETE）
```sql
-- 删除所有行
DELETE FROM employees;

-- 条件删除
DELETE FROM employees 
WHERE salary < 3000;

-- 清空表（更高效）
TRUNCATE TABLE employees;
```

### 排序和限制（ORDER BY & LIMIT）

排序
```sql
-- 单列排序
SELECT * FROM employees 
ORDER BY salary DESC;

-- 多列排序
SELECT * FROM employees
ORDER BY dept_id ASC, salary DESC;

-- 按表达式排序
SELECT * FROM employees
ORDER BY salary * 12 DESC;
```

限制结果集
```sql
-- 限制返回行数
SELECT * FROM employees 
LIMIT 10;

-- 分页查询
SELECT * FROM employees 
LIMIT 10 OFFSET 20;  -- 跳过20行，取10行

-- 等价写法
SELECT * FROM employees 
LIMIT 20, 10;  -- MySQL语法：从第20行开始取10行
```


### 完整的学生管理系统示例
```sql
-- 创建数据库
CREATE DATABASE school_management;
USE school_management;

-- 创建学生表
CREATE TABLE students (
    student_id INT PRIMARY KEY,
    student_name VARCHAR(50) NOT NULL,
    gender CHAR(1) CHECK (gender IN ('M', 'F')),
    birth_date DATE,
    major VARCHAR(100),
    enrollment_date DATE DEFAULT CURRENT_DATE
);

-- 创建课程表
CREATE TABLE courses (
    course_id INT PRIMARY KEY,
    course_name VARCHAR(100) NOT NULL,
    credits INT CHECK (credits > 0),
    instructor VARCHAR(50)
);

-- 插入数据
INSERT INTO students VALUES
(1, '张三', 'M', '2000-05-15', '计算机科学', '2020-09-01'),
(2, '李四', 'F', '2001-02-20', '软件工程', '2020-09-01'),
(3, '王五', 'M', '1999-11-08', '数据科学', '2019-09-01');

-- 基础查询练习
SELECT * FROM students WHERE gender = 'M';
SELECT student_name, major FROM students ORDER BY enrollment_date DESC;
SELECT COUNT(*) AS total_students FROM students;
```