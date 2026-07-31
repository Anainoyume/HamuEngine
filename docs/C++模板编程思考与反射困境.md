就不写的太正式了，想到什么写什么。

首先是，我们在理解 C++模板元编程 上面，我们先得知道所谓模板实例化，就是一个模板函数，我给他传入 100 个不同的函数，这叫做实例化了 100 个函数模板。这本质上和我们手写这 100 个函数没区别，或者说重载这 100 个函数区别不大。先打好这层认知。

然后就是我们必须时时刻刻思考，什么东西是我们提前知道的，什么东西是我们编程的时候就知道类型是什么，什么东西我们编程的时候就可以直接写死代码，硬编码。
啥意思呢，就拿经典的 EnTT 的注册机制来说吧。它可以注册不同类型的 Component 和 System 对吧。显然我们不知道运行的时候一个 EntityID 上，会有哪些不同的 Component，这就是为什么如果我们遍历 EntityID 的所有组件，只能拿到指针和类型 ID，而拿不到类型本身了。

所谓类型这个东西，它在运行的时候是完全不存在的，类型这个东西只存在于我们写代码的时候。

但是什么我们是知道的呢 ? 我们知道一个 Component 需要用哪些 System 去执行，去操作。那么这个时候我们其实是可以类似硬编码 `Bind<Component,System>()`，这本质上和我们直接写一个叫 `Bind_Componet_System()` 的函数没区别，我们写代码的时候就知道了它们的关系，并且运行的时候完全不会改变。

紧接着，为什么说纯静态类型做不了类似于 IMGUI的类型编辑器绘制 的逻辑 ? 我们简单做一个思想实验，首先：我们在运行的时候，点击一个 EntityID，它上面有哪些组件，我们知道吗？不知道，因此我们只能拿到一个 ID，运行时 ID，这个时候没有类型了，ID 作为一个运行时比较的凭证。也做不了静态的函数类型分发和函数重载了。

我们遍历，拿到这个 ID，如果我们现在要做不同类型的组件绘制不同的编辑器UI，我们只能在运行期去写大量 if-else，或者 switch，或者我们配置一个表，让 id 映射到对应的函数。但不管如果，我们没办法只用这个 ID 分发到不同函数。

现在问题来了，id 和 type 的映射关系，我们在编程的时候是不是就能确定 ? 对，所以我们可以用宏，或者模板，来 “硬编码” 这段逻辑。但必须在运行时走一次判断，映射到对应的 type。

那么我们就会写大量的代码，例如一个统一的 `Draw(int id, void* ptr)` 函数，我内部写一大堆 if-else，例如 id=1 对应 transform，那我就强转 ptr 为 transform 类型，然后传入调用。对吧 ?

然后我就会写 100 个 if-else 分支，100 个这样类型的 Draw 函数。这个时候我们就可以用模板了，还记得一开始说的吗 ? 模板就是用来干这个的, 本质没有区别。模板模板，它就真的只是一个代码的模板，编译期运算是后面赋予它一层更具强大的能力。因为你意识到确实有一些东西，编程的时候就能确定，编译器也这样认为，于是编译的时候可以帮你提前算好，何乐而不为呢 ? 

那么做这个 if-else 的手段有很多，但其实你确实可以直接写一个函数如下：
```cpp
using draw_func_type = void(*)(void*);

std::array<draw_func_type, meta_id::count> __draw_register;

// 如果我们手写的话
__draw_register[meta_id::transform] = [](void* ptr) {
    auto& value = *static_cast<transform*>(ptr);

    // 写任意你想绘制的内容
};

// 全局 draw 的调用
void draw(int id, void* ptr) noexcept {
    auto func = __draw_register[id];
    if (func == nullptr) return;
    func(ptr);
}
```
然后我们现在可以做一个宏：
```cpp
#define REGISTER_DRAW(type, body)                   \
__draw_register[meta_id::type] = [](void* ptr) {    \
    auto& value = *static_cast<type*>(ptr);         \
    body                                            \
}

// 这样一来我们就可以写：
REGISTER_DRAW(transform, {
    auto& pos = value.position;
    ImGui::DragFloat("x", &pos.x);
    ImGui::DragFloat("y", &pos.y);
    ImGui::DragFloat("z", &pos.z);
});
```
看，逻辑是不是一下子变得非常优雅。