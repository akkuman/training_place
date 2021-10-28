一个简单的动态 TLS 回调示例

[xmake.lua](./xmake.lua) 比较需要关注的是

```lua
if is_plat("windows") then 
    if is_mode("release") then
        add_cxflags("-MD") 
    elseif is_mode("debug") then
        add_cxflags("-MDd") 
    end
end
```

需要改成 `MD` 动态编译，静态编译会无法达到代码的预期，并且不能开启优化

## Ref

- https://github.com/kevinalmansa/TLS_Examples/blob/master/TLS_Examples/TLS_Dynamic/TLS_Dynamic.cpp