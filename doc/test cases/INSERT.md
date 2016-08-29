
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

```
//下列用例对CREATE.md中定义的表test进行操作，其中id为主键，name为非空键。
```

##### Case 1
input:
```
INSERT INTO test VALUES (1, '张三', '北京市海淀区北京理工大学')
```
expect:
```
成功插入一条记录，'id'为1，'name'为'张三'，'address'为'北京市海淀区北京理工大学'。
```

| id | name | address |
| ------| ------ | ------ |
| 1 | 张三 | 北京市海淀区北京理工大学 |

##### Case 2
input:
```
INSERT INTO test VALUES (1, '北京市海淀区北京理工大学')
```
expect:
```
语法错误。参数数量不匹配。
```


##### Case 3
input:
```
INSERT INTO test VALUES (2, '李四', '')
```
expect:
```
成功插入一条记录，'id'为2，'name'为'李四'，'address'为''。
```

| id | name | address |
| ------| ------ | ------ |
| 1 | 张三 | 北京市海淀区北京理工大学 |
| 2 | 李四 |  |

##### Case 4
input:
```
INSERT INTO test VALUES ( , '李四', '北京市海淀区北京理工大学')
```
expect:
```
报错。主键不能为空。
```

##### Case 5
input:
```
INSERT INTO test VALUES (1, '王五', '北京市海淀区北京理工大学')
```
expect:
```
报错。主键不能重复。
```

##### Case 6
input:
```
INSERT INTO test VALUES (3, '一只特立独行的猪', '北京市大兴区京新养殖场')
```
expect:
```
报错。'name'格式不符合要求。
```

##### Case 7
input:
```
INSERT INTO test VALUES (3, 王五, 北京市海淀区北京理工大学)
```
expect:
```
报错。'name','address'格式不符合要求。
```

##### Case 8
input:
```
INSERT INTO test VALUES (3, '王五', '北京市海淀区北京理工大学', '男')
```
expect:
```
语法错误。参数数量不匹配。
```

##### Case 9
input:
```
INSERT INTO test VALUES (3, '', '北京市海淀区北京理工大学')
```
expect:
```
报错。'name'不能为空。
```

##### Case 10
input:
```
INSERT INTO test VALUES ('3', '王五', '北京市海淀区北京理工大学')
```
expect:
```
报错。'id'格式不符合要求。
```

##### Case 11
input:
```
INSERT INTO test (id, name, address) VALUES (3, '王五', '北京市昌平区')
```
expect:
```
成功向指定列中插入数据，'id'为3，'name'为'王五'，'address'为'北京市昌平区'。
```

| id | name | address |
| ------| ------ | ------ |
| 1 | 张三 | 北京市海淀区北京理工大学 |
| 2 | 李四 |  |
| 3 | 王五 | 北京市昌平区 |

##### Case 12
input:
```
INSERT INTO test (id, name, address) VALUES ( , '王五', '')
```
expect:
```
报错。主键不能为空。
```

##### Case 13
input:
```
INSERT INTO test (id, name, address) VALUES (3, '王五', '')
```
expect:
```
报错。主键不能重复。
```

##### Case 14
input:
```
INSERT INTO test (id, name, address) VALUES (3, '', '北京市朝阳区')
```
expect:
```
报错。'name'不能为空。
```

##### Case 15
input:
```
INSERT INTO test (id, name) VALUES (4, '赵六')
```
expect:
```
成功向指定列中插入数据，'id'为4，'name'为'赵六'。
```

| id | name | address |
| ------| ------ | ------ |
| 1 | 张三 | 北京市海淀区北京理工大学 |
| 2 | 李四 |  |
| 3 | 王五 | 北京市昌平区 |
| 4 | 赵六 |  |

##### Case 16
input:
```
INSERT INTO test (name, address) VALUES ('小明', '北京市海淀区北京理工大学')
```
expect:
```
报错。无法插入记录：缺少主键。
```

##### Case 17
input:
```
INSERT INTO test (id, address) VALUES (5, '北京市海淀区北京理工大学')
```
expect:
```
报错。无法插入记录：'name'不能为空。
```
