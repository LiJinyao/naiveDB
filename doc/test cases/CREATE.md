
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
```
成功创建一个表名为test的表，字段包含int型的id，长度为4的字符型name，长度为100的字符型address。
```
##### Case 2
input:
```
CREATE TABLE test ()
```
expect:
```
报错。不允许创建空字段的表。
```
##### Case 3
input:
```
CREATE TABLE test
```
expect:
```
语法错误。参数不完整。
```
##### Case 4
input:
```
CREATE TABLE
```
expect:
```
语法错误。参数不完整。
```
##### Case 5
input:
```
CREATE test
```
expect:
```
语法错误。
```
