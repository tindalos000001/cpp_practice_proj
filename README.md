# IDE
build in cpp23 and c23,and use clang compiler, the code formater is clang-format,
## VSCode
if build this project in VSCode,use c_cpp_properties.json like this
```json
{
    "configurations": [
        {
            "name": "clang",
            "includePath": [
                "${workspaceFolder}/Source",
                "I:/programme/environment/Msys/ucrt64/include/c++/13.2.0" // use your cpp standard library path
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "compilerPath": "I:/programme/environment/Msys/ucrt64/bin/clang++.exe", // use your compiler path
            "cStandard": "c23",
            "cppStandard": "c++23",
            "intelliSenseMode": "windows-clang-x64"
        }
    ],
    "version": 4
}
```
After buiding project, launch and debug executable with lldb,you can use lanch.json and tasks.json listed subsequently

```json
# lanch.json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Debug",
            "program": "${workspaceFolder}/build/bin/test.exe",
            "args": [],
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "compile"
        }
    ]
}
```
compile task is in tasks.json, this configuration is written for msys environment, you can use other clang toolchain instead, cmake,ninja and clang compiler are necessary
```json
# tasks.json
{
    "tasks": [
    
        {
            
            "type": "shell",
            "label": "cmakefile",
            "command": "cmake",
            "args": [
                "-G",
                "Ninja",
                ".",
                "-B",
                "build", 
                "-DCMKAE_CXX_COMPILER=I:/programme/environment/Msys/ucrt64/bin/clang++.exe", // use your clang++ compiler path
                "-DCMKAE_C_COMPILER=I:/programme/environment/Msys/ucrt64/bin/clang.exe", // use your clang compiler path
                
            ]
        },
        {
            
            "type": "shell",
            "label": "install",
            "command": "ninja",
            "args": [
                "-C",
                "build",
                "install"
            ],
            "dependsOn":"cmakefile"
        },
        {
            "type": "shell",
            "label": "makefile",
            "command": "ninja",
            "args": [
                "-C",
                "build"
            ],
            "dependsOn":[
                "install",
            ]
        },
        {
            "label": "compile",
            "dependsOn": [
                "makefile",
            ],
        }
    ],
    "version": "2.0.0"
}
```
