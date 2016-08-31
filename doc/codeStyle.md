# naiveDB编码规范规范

------

##命名空间
顶级命名空间为`naiveDB`。各模块在顶级命名空间下定义自己的命名空间，如：`naiveDB::parser`。
##变量名
采用驼峰命名法。
###Good
```
int someInt = 1;
```
###Bad
```
int some_int = 1;
```
##常量名
采用大写。
###Good
```
const int SOME_INT = 1;
```
###Bad
```
const int some_int = 1;
const int someInt = 1;
```
##大括号
大括号都不换行。
###Good
```
namespace naiveDB { 
// ...
}
```
###Bad
```
namespace naiveDB
{ 
// ...
}
```
##using
不允许对标准库使用`using namespace`。
###Good
```
using std::cout;
cout << "hello world.\n";
```
Also good
```
std::cout << "hello world.\n";
```
###Bad
```
using namespace std;
cout << "hello world.\n";
```