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
双调排序要求输入数组满足双调性。二分该数组，循环比较两数组中相同下标的元素，通过交换，使后者总大于前者（假设是升序排列）。

容易证明，上述比较完成后，前一数组的最大值≤后一数组的最小值。

之后，对两个子数组递归调用双调排序，直至数组大小为1。

#### 1.2 分治法构造双调数组
双调排序要求输入数组满足双调性，因此在排序前需对目标数组进行构造。
1. 首先不断二分目标数组，直至其大小为1或2，这些小数组一定是单调的。
2. 两个这样的数组，连接在一起满足双调性。对其进行双调排序，输出数组又满足单调性。
3. 通过Bottom-Up的方式不断合并两数组，对合并后的数组，相邻两两间单调性应当相反，如图中的蓝/绿色网络。合并的方式即1.1中所述双调排序。

![](https://github.com/LUplus/bitonicSort/blob/master/pic/sortNetwork.png)

如图，每列蓝绿色排序网络是一层Divide()调用，即1.2中的一次分治。
```C++
void divide(float *arr, int len, bool order)
{
  if (len > 1)
  {
    int mid = len >> 1;
    divide(arr, mid, !order);
    divide(arr + mid, len - mid, order);
    bitonicSort(arr, len, order);
  }
}
```
每列网络内部又有多列红色小网络，即一次1.1中逐层向下的BitonicSort()调用。
```C++
void bitonicSort(float *arr, int len, bool order)
{
  if (len > 1)
  {
    int mid = greatest_powerOfTwo_lessThan(len);
    int cmpTimes = len - mid;

    for (int i = 0; i < cmpTimes; i++)
      if ((arr[i] > arr[mid + i]) == order)
        swap(arr[i], arr[mid + i]);

    bitonicSort(arr, mid, order);
    bitonicSort(arr + mid, len - mid, order);
  }
}
```
完整代码见naive.cpp。

#### 1.3 n!=2^k的双调排序
根据[1][2]，对于规模为p的数组，2^n < p < 2^(n+1)，只需比较前p-2^n对元素即可。

![](https://github.com/LUplus/bitonicSort/blob/master/pic/non_2_power.jpg)

如图，向一个双调递减序列（即1-0-1序列）后添1，其仍然保持双调递减。将数组在理论上补全到2^(n+1)，进行图示排序网络的比较后，b、c中至少有一个是“双调清洁的”，即全0或全1。递归比较网络，最终可以得到有序的0-1序列。根据0-1原理，这一性质对其他任意序列也成立。

注意到对最后两个1的比较不可能发生交换，因而它们是多余的，只需比较前p-2^n对数据。对应代码为刚才BitonicSort()中的
```C++
int cmpTimes = len - mid;
```
一句。

### 2 加分挑战
#### 2.1 已完成
1. 内存高效：未使用动态内存和STL容器。（为节省时间，测试并行性时使用了std::vector等）
2. 可并行：限于时间，没有编写并行程序，而是打乱了所有时间复杂度O(n)的循环，模拟并行效果（包括数组各段和段内的执行顺序）。<br>
此时得到的排序结果仍然正确，完整代码见parallel.cpp。
```C++
std::vector<int> segment;
for (int i = 0; i < m; i++)
{
  segment.push_back(i);
}
std::random_shuffle(segment.begin(), segment.end());

// Each segment
for (auto iter = segment.begin(); iter != segment.end(); iter++)
{
  int k = *iter;
  divide(data + seg_start[k], seg_start[k + 1] - seg_start[k], ASCEND_ORDER);
}
```

3. 绝对鲁棒：题目没有要求NaN的稳定性（位置不变），故将NaN作为一个有实际值的数参与排序，此处将其作为Inf。这导致所有的NaN最终排列在数组末尾。完整代码见NaN_robustness.cpp。
```C++
bool left = isnan(arr[i]), right = isnan(arr[mid + i]);
if (left && right) {
  continue;
}
else if (left) {
  if ((INFINITY > arr[mid + i]) == order)
    swap(arr[i], arr[mid + i]);
}
else if (right) {
  if ((arr[i] > INFINITY) == order)
    swap(arr[i], arr[mid + i]);
}
else {
  if ((arr[i] > arr[mid + i]) == order)
    swap(arr[i], arr[mid + i]);
}
```

#### 2.2 已尝试
1. 不递归：
BintonicSort()已改写为非递归形式。Divide()未改写成功。<br>
Divide()的原始是Top-Down的形式，不断二分数组，容易导致底层产生许多大小为奇数的子数组。非递归形式是Bottom-Up的形式，子数组规模大多是2^n，只有最右面的一个可能未填满。因为其他的程序执行流程没有变化，推测是数组划分的差异导致非递归的divide()存在bug。<br>
完整代码见no_recursion.cpp。
2. 不调用函数：
Divide()需要递归，必然产生调用。
3. 不需内存：
无全局变量，局部变量全部为int、float或指针类型，无动态内存分配。但存在Divide()函数在栈上的开销。

### 3 源码说明
**naive.cpp:** 分段双调排序，实现了递归的divide()和bitnomicSort()函数

**NaN_robustness.cpp:** 在naive.cpp的基础上加入了对NaN的处理

**noRecursion.cpp:** 在NaN_robustness.cpp的基础上将bitnomicSort()改写为非递归版本

**parallel.cpp:** 打乱了O(n)循环的执行次序，验证并行性

**timeTest.cpp:** 测试排序代码的时间开销

### 4 测试数据
C++标准库rand()产生的伪随机数，随机数种子为默认的0x01。

手动进行了针对各版本代码、数据规模从1到50000，包含2^(n+1), 2^(n-1)等corner cases的测试，排序结果均与STL qsort()输出一致。

### 5 性能分析
#### 5.1 时间复杂度
理论上是n(logn)^2，如果n个线程并行执行，则下降到(logn)^2。

![](https://github.com/LUplus/bitonicSort/blob/master/pic/timeStatistics.jpg)

如图，横轴为输入规模，纵轴为排序时间，取10次平均值。
程序开销在低数量级时呈线性，尚未反映出cache-miss等问题的影响。限于机能和时间，未进行进一步测试。

#### 5.2 空间复杂度
原址排序，仅在栈上有O(1)的数组索引等临时变量。

### 6 测试起止时间
**6.28**

**11:00-14:00** 查找双调排序资料

**15:00-16:30** 自主实现初版代码

**16:30-23:00** Debug不顺，参考了别人的实现[3][4]

**6.29**

**7:30-11:00** 完成加分挑战

**12:00-16:00** 撰写报告

![](https://github.com/LUplus/bitonicSort/blob/master/pic/git_commit_history.png)

共计约用14.5小时实现算法，4小时梳理结果。

### 7 参考资料
[1] 算法导论（原书**第二版**）第27章 排序网络

[2] n!=2^k的双调排序网络 https://blog.csdn.net/ljiabin/article/details/8630627

[3] 分段双调排序算法 http://www.huangjunqin.com/2017/08/19/segmentedBitonicSort/

[4] 双调排序 - GAPS编程测试 https://github.com/Shuai-Xie/BitonicSort
