## llvm-objdump 定位某个函数
使用：
```powershell
llvm-objdump -d --symbol=<函数名> your.exe
```
这样就可以只看该符号的汇编代码。