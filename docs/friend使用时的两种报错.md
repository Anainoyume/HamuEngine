看下面的情况：

```cpp
struct VertexAttribute
{
    GLint count;
    GLenum type;
    GLboolean normalized = GL_FALSE;

    static size_t GetSizeOfType(GLenum type) {
        switch (type) {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
            case GL_INT: return sizeof(GLint);
            case GL_BYTE: return sizeof(GLbyte);
            default: return 0;
        }
    }

    size_t GetSize() const { return count * GetSizeOfType(type); }
};
```

我现在要在这个结构体里加一个 `ostream& operator<<` 的重载。

我应该写：
```cpp
friend std::ostream& operator<<(std::ostream& os, const VertexAttribute& attribute);
```

现在有两种报错：
1. 我写了下面这种情况:
   ```cpp
     std::ostream& operator<<(std::ostream& os, const VertexAttribute& attribute);
   ```
   报错为：
   > Overloaded 'operator<<' must be a binary operator (has 3 parameters)clang(operator_overload_must_be)

   这里提示我们 `<<` 是一个二元运算符，但我们传入了三个参数，为什么？因为当我们不写 `friend` 的时候，编译器会把这个函数当成类成员函数，而类成员函数都有一个隐藏参数 `this`，所以这里报错了。
   而使用 `friend` 标记的函数，则为全局函数，第一个参数固定为 `ostream&` ，所以没问题。
   
2. 来看第二种情况:
   ```cpp
     friend std::ostream& operator<<(std::ostream& os, const VertexAttribute& attribute) const;
   ```
   报错为：
   > Non-member function cannot have 'const' qualifier (fix available)clang(invalid_qualified_function_type)

   本意是好的，告诉它我不会修改类成员变量，但我们知道加了 `friend` 之后，这个函数是全局函数而不是类成员函数了，你都不在类中？保证什么类的成员变量不变的？所以报错该函数不为类成员函数。

---

这两种报错都很有趣，也值得推敲理解 C++ 的语法设计，所以作此记录。