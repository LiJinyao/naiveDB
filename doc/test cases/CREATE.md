
### CREATE TABLE
#### 语法定义：
```
CREATE TABLE 表名称
(
列名称1 数据类型,
列名称2 数据类型,
列名称3 数据类型,
....
)
```
##### Case 1
创建表测试。

input:
```
CREATE TABLE test
(
id int,
name char(4),
address char(100)
)
```
expect:

成功创建一个表名为test的表，字段包含int型的id，长度为4的字符型name，长度为100的字符型address。

| id | name | address |
|----|------|---------|
|    |      |         |

##### Case 2
`PRIMARY KEY`测试。

input:
```
CREATE TABLE test
(
id int PRIMARY KEY,
name char(4),
address char(100)
)
```
expect:

`PRIMARY KEY`限制的字段不能为空。
成功创建一个表名为test的表，字段包含int型的id且为主键，长度为4的字符型name，长度为100的字符型address。

| id | name | address |
|----|------|---------|
|    |      |         |
##### Case 3
`NOT NULL`测试。

input:
```
CREATE TABLE test
(
id int PRIMARY KEY,
name char(4) NOT NULL,
address char(100)
)
```
expect:

`PRIMARY KEY`和`NOT NULL`限制的字段都不能为空。
成功创建一个表名为test的表，字段包含int型的id且为主键，长度为4的字符型name且不能为空，长度为100的字符型address。

| id | name | address |
|----|------|---------|
|    |      |         |

##### Case 4
input:
```
CREATE TABLE test ()
```
expect:
```
参数错误。不允许创建空字段的表。
```
##### Case 5
input:
```
CREATE TABLE test
```
expect:
```
语法错误。参数不完整。
```
##### Case 6
input:
```
CREATE TABLE
```
expect:
```
语法错误。参数不完整。
```
##### Case 7
input:
```
CREATE test
```
expect:
```
语法错误。
```
##### Case 8
preset:
```
预置名为test的表。
```
input:
```
CREATE TABLE test (id int)
```
expect:
```
命名冲突错误，不能创建相同名称的表。
```
##### Case 9
input:
```
CREATE DATABASE database
```
expect:
```
创建了一个名为database的数据库。
```
##### Case 10
preset:
```
预置名为database的数据库。
```
input:
```
CREATE DATABASE database
```
expect:
```
命名冲突错误，不能创建相同名称的数据库。
```
