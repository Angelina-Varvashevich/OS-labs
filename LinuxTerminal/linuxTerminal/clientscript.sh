#!/bin/bash

c++ -fPIC -c Number/Number.cpp -o Number/Number.o
ar rc Number/libNumber.a Number/Number.o
c++ -fPIC -c Vector/Vector.cpp -o Vector/Vector.o -I Number
c++ -shared -o Vector/libVector.so Vector/Vector.o Number/libNumber.a
c++ Client/main.cpp -I Vector Vector/libVector.so -I Number Number/libNumber.a
./a.out
