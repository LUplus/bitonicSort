# bitonicSort
Coding Test of GAPS Lab

## 问题说明
给出分成 m 段的 n 个浮点数，输入数据已按段号有序，但每段内部无序。
用 C/C++ 编写一个分段双调排序(Bitonic sort)函数，对每一段内部的浮点数进行排序，但不要改变段间的位置。 

### 1 接口方式
```C++
void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int n, int m); 
```
- data 包含需要分段排序的 n 个 float 值（由下面的 seg_id 可知是表示多段的数据）
- seg_id 给出 data 中 n 个元素各自所在的段编号
- seg_start 共有 m+1 个元素，前 m 个分别给出 0..m-1 共 m 个段的起始位置，seg_start[m] = n
- n 表示 data 中包含 n 个数据
- m 表示 data 分为 m 段数据

由题意得，m <= n，因为 data 某一段可能有多于 1 个数据，这种情况下：m < n

seg_id 中的元素保证单调不下降，即对任意的 i < j，seg_id[i] <= seg_id[j]
seg_id 所有元素均在 0 到 m-1 范围内。（因为是段 id，m 段就是0..m-1） 

### 2 输入输出
输入
```C++
float data[5] = {0.8, 0.2, 0.4, 0.6, 0.5};
int seg_id[5] = {0, 0, 1, 1, 1};
int seg_start[3] = {0, 2, 5};
int n = 5;
int m = 2;
```

输出
```C++
float data[5] = {0.2, 0.8, 0.4, 0.5, 0.6};
```

### 3 其他要求

#### 3.1 注意
1. 必须使用双调排序算法进行排序。 
2. 可以直接使用从网上下载的双调排序代码，但须注明出处。 

#### 3.2 加分挑战（非必需）
1. 不递归：segmentedBitonicSort 函数及其所调用的任何其他函数都不得直接或间接地进行递归。 
2. 不调用函数：segmentedBitonicSort 不调用除标准库函数外的任何其他函数。 
3. 内存高效：segmentedBitonicSort 及其所调用的任何其他函数都不得进行动态内存分配，包括malloc、new和静态定义的STL容器。 
4. 可并行：segmentedBitonicSort 涉及到的所有时间复杂度O(n)以上的代码都写在for循环中，而且每个这样的for循环内部的循环顺序可以任意改变，不影响程序结果。注：自己测试时可以用rand()决定循环顺序。 
5. 不需内存：segmentedBitonicSort 不调用任何函数（包括C/C++标准库函数）， 不使用全局变量，所有局部变量都是int、float或指针类型，C++程序不使用new关键字。 
6. 绝对鲁棒：在输入数据中包含 NaN 时（例如```sqrt(-1.0)```)，保证除NaN以外的数据正确排序，NaN的个数保持不变。 

## 结果提交
### 1 算法描述
#### 1.1 基本双调排序
要求输入数组满足双调性。二分该数组，比较两数组中相同下标的元素，通过交换，使后一数组的元素总大于前者中相应元素。

容易证明，上述比较完成后，前一数组的最大值≤后一数组的最小值。

之后，对两个子数组递归调用双调排序，直至数组大小为1。

### 2.1 分治法构造双调数组
双调排序需要输入数组满足双调性。换言之，可被分割成两个单调数组。
1. 首先不断二分目标数组，直至其大小为1或2，此时数组一定是单调的。
2. 两个这样的数组，连接在一起满足双调性。对其进行双调排序，输出数组满足单调性。
3. 通过bottom-up的方式不断合并两数组，对合并后的数组，相邻两两间单调性应当相反，如图中的蓝/绿色网络。合并的方式即双调排序。

【图片】
如图，每列蓝绿色排序网络即一层Divide()调用。
每列网络内部又有多列红色小网络，即逐层向下的BitonicSort()调用。
两函数的完整代码见naive.cpp。

### 3 n!=2^k的双调排序
根据[1][2]，对于规模为p的数组，2^n < p < 2^(n+1)，只需比较前p-2^n对元素即可。

根据0-1原理，向一个双调递减序列后添1，其仍然保持双调递减（即1-0-1序列）。在进行该排序网络的比较后，b、c中至少有一个是“双调清洁的”。

递归比较网络，最终可以得到有序序列。

注意到对最后两个1的比较不可能发生交换，因而它们是多余的，只需比较前p-2^n对数据。

