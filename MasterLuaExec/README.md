# ImGui-D3D11Hook
D3D11Hook 使用ImGui进行绘制 支持x64/x86

编译会生成一个 DLL 文件，用注入工具即可注入游戏。

如果注入后没有效果，请检查游戏是否为 D3D11 或尝试关闭 Bandicam、Fraps 等帧率显示的软件（这些软件会进行 D3D 的 hook，进而导致我们自己的 Hook 失效）
