# 景区导览系统后端服务部分

**尽量多用数据结构和算法，尽量不用std库**



说明：

- 需要配置的库：
  - httplib.h在/DataStructureServer中，手动添加一下路径即可。教程：https://blog.csdn.net/HandsomeHong/article/details/114157201
  - json解析：附加包含库.DataStructureServer\include\single_include
- 在这个实现中
  - Place其实是Point的子集。个人感觉更好的设计应该是Place为Point的子类，但这样太过复杂，用到的C++知识太多了，且必要性也没有那么大。鉴于这是数据结构的大作业，就先用一种简单粗暴的方法实现
    - 如果能力足够想写继承那种方式，肯定是更好（）有人想写记得群里说一下
  - 这个方案中，Place和Point是两套系统，Place只用于线性表，Point只用于图。Point的placeId用来标识它是否是个景点（如果是，那么这个值是它再Place中的id。如果不是，为-1），从而把两者联系起来。
    - 因为图和线性表同时开始，图这边先编点假的placeId，最后合在一起的时候再改成真的。
    - Place：所有的景点
    - Point：所有的景点+拐点
  - 如果不测试http部分，直接把main函数中的http相关语句注释掉就可以了。



本人能力有限，设计过程中可能会有考虑不太好的地方，如果函数的参数类型和返回类型不太妥当随时修改，但记得群里说一下（涉及到http.cpp的设计）
