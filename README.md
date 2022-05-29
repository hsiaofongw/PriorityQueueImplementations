# 优先队列实现演示

定义定义了关于类型 `T` 的下列操作的数据结构为类型 `T` 的优先队列：

- `T top(this) const` 获得队列最前的元素；
- `void pop(this)` 移除队列最前的元素；
- `void insert(this, const T& element)` 将一个元素入列；
- `void clear(this)` 清空；
- `size_t size(this) const` 取队列长度；
- `bool empty(this) const` 空性判定；

## 堆

在当前版本，此项目已经实现了基于堆的优先队列。文件: `heap.hpp`.
