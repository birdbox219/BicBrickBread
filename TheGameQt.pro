QT += core gui widgets

TEMPLATE = app
TARGET = TheGameQt

CONFIG += c++17

INCLUDEPATH += . \
              header \
              Neural_Network/Include

# GUI Sources
SOURCES += GUI/main.cpp \
           GUI/mainwindow.cpp \
           GUI/gamewindow.cpp

HEADERS += GUI/mainwindow.h \
           GUI/gamewindow.h

# Game Logic Sources (Recursively added mostly manually or via patterns in IDE, but qmake needs specific files usually)
# We add the known files from the compilation command:

SOURCES += Refrence/XO_Classes.cpp \
           Games/4x4_Tic_Tac_Toe/4by4_XO.cpp \
           Games/Four_in_a_row/four.cpp \
           Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.cpp \
           Games/Memory_Tic_Tac_Toe/Memory_Tic_Tac_Toe.cpp \
           Games/Obstacles_Tic_Tac_Toe/Obstacles_Tic_Tac_Toe.cpp \
           Games/PyramidXO/PyramidXO.cpp \
           Games/SUS/SUS.cpp \
           Games/Ultimate_Tic_Tac_Toe/Ultimate.cpp \
           Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.cpp \
           Games/XO_inf/XO_inf.cpp \
           Games/XO_num/xo_num.cpp \
           Games/anti_XO/Anti_XO.cpp \
           Games/diamond_XO/dia_XO.cpp \
           Neural_Network/Source/Layer.cpp \
           Neural_Network/Source/Matrix.cpp \
           Neural_Network/Source/NeuralNetwork.cpp

HEADERS += header/AI.h \
           header/BoardGame_Classes.h \
           header/Custom_UI.h \
           header/XO_Classes.h \
           Games/4x4_Tic_Tac_Toe/4by4_XO.h \
           Games/Four_in_a_row/four.h \
           Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.h \
           Games/Memory_Tic_Tac_Toe/Memory_Tic_Tac_Toe.h \
           Games/Obstacles_Tic_Tac_Toe/Obstacles_Tic_Tac_Toe.h \
           Games/PyramidXO/PyramidXO.h \
           Games/SUS/SUS.h \
           Games/Ultimate_Tic_Tac_Toe/Ultimate.h \
           Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.h \
           Games/XO_inf/XO_inf.h \
           Games/XO_num/xo_num.h \
           Games/anti_XO/Anti_XO.h \
           Games/diamond_XO/dia_XO.h
