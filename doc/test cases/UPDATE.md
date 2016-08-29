### UPDATE
#### UPDATE 语法定义：
```
UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
```
##### test:
| ID    | Name   | Address |
|-------|--------|---------|
| 12121 | Wilson |         |
| 12120 | Gates  | BeiJing |

这张名为test的表会在以下每个测试前当作预置表格。
其中ID为主键，Name存在空值约束。
##### case 1:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id = 12138 WHERE Name = 'Wilson' 
```
expect:
```
成功为Name为'Wilson'的人更新值为12138的id。
```

| ID    | Name   | Address |
|-------|--------|---------|
| 12138 | Wilson |         |
| 12120 | Gates  | BeiJing |

##### case 2:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id = 12580, Address = 'NanJing'
WHERE Name = 'Wilson'
```
expect:
```
成功为Name为'Wilson'的人更新值为'NanJing'的Address和值为12580的id。
```

| ID    | Name   | Address |
|-------|--------|---------|
| 12580 | Wilson | NanJing |
| 12120 | Gates  | BeiJing |

##### case 3:
preset:
```
预置名为test的表。
```
input:
```
UPDATE SET id = 12121 WHERE Name = 'Wilson' 
```
expect:
```
缺少表名参数。
```
##### case 4:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test id = 12121 WHERE Name = 'Wilson' 
```
expect:
```
语法错误。
```
##### case 5:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id WHERE Name = 'Wilson' 
```
expect:
```
缺少id参数。
```
##### case 6:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET Work = 'Police' WHERE Name = 'Wilson' 
```
expect:
```
test表中不存在'Work'列。
```
##### case 7:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id = 12121 WHERE Work = 'Police'
```
expect:
```
test表中不存在'Work'列。
```
##### case 8:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id = 12138 WHERE Name = 'Waston'
```
expect:
```
不存在Name为'Waston'的数据。
```
##### case 9:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id = 9283751092385
WHERE Name = 'Wilson'
```
expect:
```
输入的id参数超出规定限制。
```
##### case 10:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET Name = NULL WHERE id = 12120
```
expect:
```
Name不能为NULL。
```
##### case 11:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id = 12120 WHERE Name = 'Wilson'
```
expect:
```
主键必须唯一。
```
##### case 12:
preset:
```
预置名为test的表。
```
input:
```
UPDATE test SET id = '12120' WHERE Name = 'Wilson'
```
expect:
```
输入参数类型错误。
```
