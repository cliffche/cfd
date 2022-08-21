**2022.7.26**

这个一个CFD软件开发的记录,从最简单的流动开始.

已经完成

0. Jacobi迭代

1. Tridiagonal Matrix Algorithm

2. Couette 流 

3. 准一维喷管问题

   

本项目依赖第三方线性代数库armadillo,可以通过vcpkg来安装,也可以通过其他方法安装,后续会移除对该库的依赖.

```shell
vcpkg install armadillo
vcpkg integrate install
```



This is a record of CFD software development, starting from the simplest flow.

The flow cases has been completed

0. Jacobi iteration

1. Tridiagonal Matrix Algorithm

2. Couette Flow

3. Quasi-one-dimensional nozzle problem

   

This project relies on the third-party linear algebra library armadillo, which can be installed through vcpkg or through other methods, and the dependency on this library will be removed later.

```
vcpkg install armadillo
vcpkg integrate install
```

**2022.8.1**

使用oop重构了代码,路径位于\cliff_cfd\cfd_oop

Refactored the code library by oop. The path is \cfd\v1

 
