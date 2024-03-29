# ZJU-DIP-2022

## Intro
This repo is specially for Zhejiang University undergraduates in aid of their learning in Digital Image Processsing course.
Relating to the Digital Image Processsing course in 2022, here are 6 folders including 6 assignments of this course in total. 
If you are not in this university, you can also find the code you need in the realm of fundamental DIP.
<div align=center>
<img src='https://raw.github.com/Meteors27/ZJU-DIP-2022/master/cliff.bmp' width=25% height=25%><img src='https://raw.github.com/Meteors27/ZJU-DIP-2022/master/ProcessedImages/VisEnhance_cliff.bmp' width=25% height=25%><img src='https://raw.github.com/Meteors27/ZJU-DIP-2022/master/ProcessedImages/equalization_cliff.bmp' width=25% height=25%>
</div>

## Notice
If you want to copy `bmp` file reading & writing, please check [Assignment6](https://github.com/Meteors27/ZJU-DIP-2022/tree/master/Assignment6) rather than [Assignment1](https://github.com/Meteors27/ZJU-DIP-2022/tree/master/Assignment1). 

The files `bmp.h` and `bmp.cpp` in every assignment folder include the reading and writing part of `bmp` files. 

## Included code in each assignment folder
1. How to read and write `bmp` files
2. Binarize (OSTU algorithm), dialate, erode, opening and closing
3. Visibility enhance and histogram equalize
4. Simple geometric transformation including rotate (using bilinear interpolation), scale, sheer and mirror. 
5. Image mean filtering and Laplacian image enhancement
6. Bilateral filter

## Something you must know
If you are learning DIP course in ZJU, you may probably find reading and writing `bmp` files the most challenging part. 
However, due to the laziness of the author, the earlier assignment code has known bugs in the io of `bmp` files. (especially in [Assignment1](https://github.com/Meteors27/ZJU-DIP-2022/tree/master/Assignment1) and [Assignment2](https://github.com/Meteors27/ZJU-DIP-2022/tree/master/Assignment2))

As a result, please **avoid using them** as your code. They may bring about errors. 

So if you want to check how to read and write a `bmp` file correctly, please check the latest version ([Assignment6](https://github.com/Meteors27/ZJU-DIP-2022/tree/master/Assignment6)). 
Actually as long as `bmp.h` and `bmp.cpp` are what not in the early assignments, the author is pretty sure that `bmp.h` and `bmp.cpp` is so correct that it must meet the demand of DIP course in ZJU.
