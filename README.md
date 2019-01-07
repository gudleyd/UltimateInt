# UltimateInt
Just one more C++ BigInt implementation, but this one is unique (because it's mine)

Will or already in use in my next projects

Multiplication time here:
a = length of the first number
b = length of the second

| a*b   | 1       | 10      | 100     | 1e3     | 1e4     | 5*1e4   | 1e5    |
|-------|---------|---------|---------|---------|---------|---------|--------|
| 1     | 0.000s  | 0.000s  | 0.000s  | 0.000s  | 0.000s  | 0.0005s | 0.001s |
| 10    | 0.000s  | 0.000s  | 0.000s  | 0.000s  | 0.000s  | 0.0005s | 0.002s |
| 100   | 0.000s  | 0.000s  | 0.000s  | 0.000s  | 0.0003s | 0.002s  | 0.004s |
| 1e3   | 0.000s  | 0.000s  | 0.000s  | 0.0002s | 0.003s  | 0.015s  | 0.026s |
| 1e4   | 0.000s  | 0.000s  | 0.0003s | 0.003s  | 0.027s  | 0.132s  | 0.261s |
| 5*1e4 | 0.0007s | 0.0005s | 0.002s  | 0.015s  | 0.132s  | 0.558s  | 0.920s |
| 1e5   | 0.0001s | 0.002s  | 0.004s  | 0.026s  | 0.261s  | 0.920s  | 2.001s |

Division is about 200 times slower
