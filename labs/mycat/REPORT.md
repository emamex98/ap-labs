# Permformance Report

**Passing a single file as arguemnt:**
| File size | ./cat | ./mycat |
|-|:-:|:-:|
| 5 bytes | 0.000062 sec. | 0.000046 sec. |
| 500 bytes | 0.000067 sec. | 0.000968 sec. |
| 50000 bytes | 0.000840 sec. | 0.204660 sec. |

**Passing two files of equal size as arguemnts:**
| File size | ./cat | ./mycat |
|-|:-:|:-:|
| 5 bytes | 0.000081 sec. | 0.000107 sec. |
| 500 bytes | 0.000086 sec. | 0.001699 sec. |
| 50000 bytes | 0.001672 sec. | 0.341554 sec. |
