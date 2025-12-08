## Compiling the Project

### For VS Code Users

1.  Make sure you have a C++ compiler like g++ installed and configured.
2.  You can use the following command to compile the project. Replace `<path-to-g++>` with the actual path to your g++ executable and `<project-directory>` with the absolute path to the project directory.

    ```bash
    g++ -o TheGame.exe TheGame.cpp Refrence/XO_Classes.cpp Games/4x4_Tic_Tac_Toe/4by4_XO.cpp Games/Four_in_a_row/four.cpp Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.cpp Games/Memory_Tic_Tac_Toe/Memory_Tic_Tac_Toe.cpp Games/Obstacles_Tic_Tac_Toe/Obstacles_Tic_Tac_Toe.cpp Games/PyramidXO/PyramidXO.cpp Games/SUS/SUS.cpp Games/Ultimate_Tic_Tac_Toe/Ultimate.cpp Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.cpp Games/XO_inf/XO_inf.cpp Games/XO_num/xo_num.cpp Games/anti_XO/Anti_XO.cpp Games/diamond_XO/dia_XO.cpp Neural_Network/Source/Layer.cpp Neural_Network/Source/Matrix.cpp Neural_Network/Source/NeuralNetwork.cpp -I.
    ```

### Using VS Code Tasks

Alternatively, you can create a `tasks.json` file in your `.vscode` directory with the following content. This will allow you to build the project using the `Terminal > Run Build Task` command in VS Code.

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "<path-to-g++>/g++.exe",
            "args": [
                "-Wall",
                "-Wextra",
                "-g3",
                "${workspaceFolder}/XO_Demo.cpp",
                "${workspaceFolder}/XO_Classes.cpp",
                "-o",
                "${workspaceFolder}/output/XO_Demo.exe"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ],
            "detail": "compiler: <path-to-g++>/g++.exe"
        }
    ]
}
```
Make sure to replace `<path-to-g++>` with the path to your g++ executable in the `command` and `detail` fields.
