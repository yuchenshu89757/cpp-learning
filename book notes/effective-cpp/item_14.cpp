Think carefully about copying behavior in resource-managing classes

在资源管理类中小心copying行为

1.复制RAII(resource acquisition is initialization)对象时，必须一并复制它所管理的

资源，所以资源的copying行为决定了RAII对象的copying行为。

2.普遍常见的RAII class copying行为是：抑制copying、施行引用计数法。不过其他行为

也都可能被实现

3. auto_ptr和shared_ptr对象都属于RAII对象
