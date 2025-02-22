Security Test Benchmark for Computer Architectures
========================

This project aims to provide a generic test benchmark to
systematically evaluate the security features
supported by a processor hardware.

Copyright (c) 2017-2023 [Wei Song](mailto:wsong83@gmail.com) at
the Institute of Information Engineering,
Chinese Academy of Sciences.

The software can be distributed under GNU General Public License version 3.
See [LICENSE](LICENSE.md) for more details.

### Usage (Linux variants)

* Make the C++ `run-test` helper program

~~~bash
  make
~~~

* Run the test bench in exhaustive mode with a report

~~~bash
  ./run-test exhausted-run
~~~

* Run the test bench in fast mode with a report

~~~bash
  ./run-test fast-run
~~~

* Extra functions provided by the `run-test` helper

~~~bash
Usage: ./run-test [OPTION]
Run Script for the Security Test Benchmark.

Possible parameters:
  help          Show this help information.
  continue      Continue a previous test by reading the results.json file first.
  debug         Stop testing on the first unexpected exit status.
  make-only     Make the test cases without running them.
  no-make       Due to make the test cases as they are made aleady.
  fast-run      Only run the test case that their requirement runs successfully, and then generate a report.
  exhausted-run Run all tests until the total test case is exhausted, and then generate a report.
  print-trace   Print the trace log of the attacked target.
~~~

* Extra targets provided by the `Makefile` script

~~~text
dump        Generate a dissambly dump for all test cases.
prep        Generate the preprocessed C++ files (resolving all macros) for all test cases.
clean       Clean up the test environment for a fresh test.
~~~

### Usage (Windows MSVC)

We assume MSVC 2019 or later is installed.
The test bench currently relies on GNU Make is made available through the winget:

~~~bash
  winget install gnuwin32.make
~~~

Assuming the test bench is run in a Windows PowerShell lunched by MSVC
(Suggested by Microsoft, a shell must be lunched through MSVC),
the GNU Make needs to be added to the PATH variable:

~~~bash
  $Env:PATH += ";C:\Program Files (x86)\GnuWin32\bin"
~~~

Once this is done, we try to make the Makefile works on Windows as well.
For the obvious reasons, some functions are unavailable but we are working on them.


### Content of the repo:
- **acc**: test cases related to memory access control.
- **cfi**: test cases related to control flow integrity.
- **cpi**: test cases related to code pointer integrity.
- **lib**: the architecture-specific support library.
- **mss**: test cases related to memory spatial safety.
- **mts**: test cases related to memory temporal safety.
- **script**: helper scripts of the test suite.
- LICENSE.md: license declaration.
- Makefile: auto make script.
- README.md: this readme file.
- configure.json: the configuration parameters recorded in JSON.

### Test results

The results of individual test cases are recorded in a generated file `results.json`.
The meaning of the result integer:

* 0: the test case passed and supposedly a vulnerability is expoited.
* [1-15]: A predefined error number is returned.
* [16-31]: An expected exception is catched and a predefined error number is then returned.
* [32-63]: A run-time parameter (`n-32`) is found and returned by the test case.
* [256-511]: An exception is raised but uncatched with the signal number as `n-256`.
* 1024: Test case directly failed due to failure in prerequisite cases.
* -1: configuration error which needs to be fixed.
* other: Something is wrong and need investigation.

### Contributors

In the alphabet order:

- Boya Li
- Ciyan Ouyang
- Da Xie
- Hao Ma
- Jiameng Ying
- Sihao Shen
- Zhuxin Yang
- Wei Song

### References:

1. 沈思豪, 解达, 宋威. [跨平台内存安全测试集设计](http://dx.doi.org/10.15888/j.cnki.csa.008840). 计算机系统应用, 2022, 31(10).
1. Wei Song, Jiameng Ying, Sihao Shen, Boya Li, Hao Ma, Peng Liu.
   _A comprehensive and cross-platform test suite for memory safety -- Towards an open framework for testing processor hardware supported security extensions_.
   [arXiv:2111.14072](https://arxiv.org/abs/2111.14072), November 2021.
1. Jianping Zhu, Wei Song, Ziyuan Zhu, Jiameng Ying, Boya Li, Bibo Tu, Gang Shi, Rui Hou, and Dan Meng.
   _[CPU security benchmark](https://wsong83.github.io/publication/comparch/secarch2018.pdf)_.
   In Proceedings of the 1st Workshop on Security-Oriented Designs of Computer Architectures and Processors,
   Toronto, ON, Canada, pp. 8–14, 2018.
