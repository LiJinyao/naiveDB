
###SELECT
####语法语义

```
SELECT 列名称 FROM 表名称
```
以及
```
SELECT * FROM 表名称
```
提示：星号（*）是选取所有列的快捷方式。

"StudentsInfo"表

|Id|Name|Age|Address|
|------| ------| ------| ------| 
|01|李华|17|北京理工大学|
|02|张三|18|北京理工大学|
|03|李四|17|北京理工大学|
|04|王五|19|北京理工大学|





####Case 1:
Input:
```
SELECT Name FROM StudentsInfo
```
Except:
```
从StudentsInfo表中将获取Name列的数据
```
| Name  |
|-------|
| 李华  |
| 张三  |
| 李四  |
| 王五  |


####Case 2:
Input :
```
SELECT Name,Age From StudentsInfo
```
Except:
```
从StudentsInfo表中将获取Name列的数据
```


####Case 3:
Input:
```
SELECT * FROM StudentsInfo
```
Except:
```
从StudentsInfo中获取表中的所有数据
```
|Id|Name|Age|Address|
|------| ------| ------| ------| 
|01|李华|17|北京理工大学|
|02|张三|18|北京理工大学|
|03|李四|17|北京理工大学|
|04|王五|19|北京理工大学|



####Case 4:
Input:
```
SELECT * FROM
````
Except:
```
语法错误，参数不完整（缺少表名）
```

####Case 5：
Input:
```
S
