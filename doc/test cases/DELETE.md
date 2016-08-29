###DELETE 
####语法定义： 
####删除一个表
DELETE FROM 表名称（删除表里面所有信息但是保留表的结构，属性和索引）
####删除表中值等于某个条件的所有行
DELETE FROM 表名称 WHERE 列名称 = 值


####preset:
Person

| Lastname | FirstName | Address      | City     | Age |
|----------|-----------|--------------|----------|-----|
| Gates    | Bill      | xuanwumen 10 | Beijing  | 37  |
| Wilson   | Fred      | zhongshan 23 | Nanjing  | 48  |
| Andy     | Brown     | nanjing 51   | Shanghai | 25  |


#####Case 1:

preset: 
预置一个名为person的表

input:

```	
DELETE FROM person
```

expect:
```
成功删除一个表名为person的表
```

#####Case 2:

input:

```	
DELETE FROM person
```

expect:

```
报错，报错类型为无所要删除的表
```


#####Case 3:

preset: 
预置一个名为person的表

input:

```
DELETE FROM person WHERE LastName = "Wilson"
```

expect:
| Lastname | Firstname | Address      | City     | Age |
|----------|-----------|--------------|----------|-----|
| Gates    | Bill      | xuanwumen 10 | Beijing  | 37  |
| Andy     | Brown     | nanjing      | Shanghai | 25  |

#####Case 4:

preset: 
预置一个名为person的表

input:

```
DELETE FROM person WHERE LastName = "Eric"
```

expect:

```
报错，报错类型为无所要删除的行。
```

#####Case 5:

preset: 
预置一个名为person的表

input:

```
DELETE FROM "person"
```

expect:
```
报错，报错类型为语句语法错误
```

#####Case 6:

preset: 
预置一个名为person的表

input:

```
DELETE FROM Lastname = hehe
```

expect:
```
报错，报错类型为语句语法错误
```

#####Case 7:

preset: 
预置一个名为person的表

input:

```
DELETE FROM person WHERE Sex = "Male"
```
expect:

```
报错，表格缺乏所要删除的参数
```

#####Case 8:

preset: 
预置一个名为person的表

input:

```
DELETE person
```
expect:

```
报错，缺少关键字FROM
```

#####Case 9:

preset: 
预置一个名为person的表

input:

```
DELETE FROM person LastName = "Brown"
```
expect:

```
报错，缺少关键字where
```
