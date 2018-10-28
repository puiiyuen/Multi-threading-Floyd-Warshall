# Multi-threading-Floyd-Warshall
Multi-threading Floyd-Warshall All Pairs Shortest Path with C/C++
# Introduction
1. A shortest path of matrix program (FW algorithm) includes multi-threading and single-threading function
2. Default number of threads is the number of vertices you input
3. Only run on Unix-like system
# Tips
1. Too much threads could cause the program be killed, because of lack of memory
2. If the number of vertices is too high(e.g.10000), comment out the single-threading part of program then compile the program and run. 
3. When the number vertices is lower than 50, it will print the shortest path matrix. You can modify it by yourself
