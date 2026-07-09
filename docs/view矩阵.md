# 图形学中的 view 矩阵
---

当把世界坐标转化为视图坐标的时候, 我们可以利用线性代数中的基变换矩阵。假设视图空间中的位置为 $\mathbf{\bar{p}}$ ，世界空间的位置为 $\mathbf{p}$。现在我们知道摄像机的三个正交基在 **世界空间** 下的坐标，设为 $\mathbf{x, y, z}$ , 那么现在的 $\mathbf{\bar{p}}$ 是视图空间下的语言所表示的坐标，现在要用我们世界空间的语言去表达它，则设基变换矩阵：
$$
\mathbf{P} = \left[
\begin{matrix}
~~|&~|&~~|~~ \\
~~\mathbf{x} & ~\mathbf{y} & \mathbf{z} \\
~~|&~|&~~|~~ 
\end{matrix}
\right]\\
~\\
~\\
\mathbf{P} \mathbf{\bar{p}} = \mathbf{p}
$$
则显然, 如果我们要把我们世界坐标语言，转化为视图空间的坐标呢？
那么就是:
$$
\mathbf{\bar{p}} = \mathbf{P}^{-1} \mathbf{p} \\
~ \\
由于~\mathbf{P}~是正交矩阵, 则~\mathbf{P}^{-1} = \mathbf{P}^\mathrm{T} \\
~ \\
 \mathbf{\bar{p}} = \mathbf{P}^\mathrm{T} \mathbf{p}
$$
当然这里只是描述了旋转与缩放, 别忘了我们的位移，根据相对运动，一个物体向摄像机移动等同于摄像机向前移动。因此物体需要加上我们摄像机的反向位移，最终得到 `LookAt` 矩阵：
$$
\mathrm{LookAt} = \left[
\begin{matrix}
~R_x&R_y&R_z&0~\\ 
~U_x&U_y&U_z&0~\\
~D_x&D_y&D_z&0~ \\
~0&0&0&1~
\end{matrix}
\right] 
*
\left[
\begin{matrix}
~1&0&0&-P_x~\\ 
~0&1&0&-P_y~\\
~0&0&1&-P_z~ \\
~0&0&0&1~
\end{matrix}
\right]
$$