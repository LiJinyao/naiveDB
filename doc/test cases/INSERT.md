
### INSERT
#### 语法定义：
##### 插入新的行：
```
INSERT INTO 表名称 VALUES (值1, 值2,....)
```
##### 在指定的列中插入数据：
```
INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....)
```
##### Case 1
input:
```
INSERT INTO test VALUES (1, '张三', '北京市海淀区北京理工大学')
```
expect:
```
成功插入一条记录，id为1，name为张三，address为北京市海淀区北京理工大学。
```
