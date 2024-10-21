# The Qt's dll
To install the Qt's dll, input this in the powershell
```shell
d:\qt\6.7.2\mingw_64\bin\windeployqt.exe ./build/QLink.exe
```

In all the `CMakeLists.txt`, change the CMAKE_PREFIX_PATH to the Qt's mingw install path, for example
```cmake
list(APPEND CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64/")
```

# About Collider
the Hero's collider is on feet
![](res\images\README\Hero_colldier.png)
the Boxes' collider is the whole one
![](\res\images\README\Box_collider.png)

# About Item
### generate place
the item generate in the red
![](res\images\README\Item_place.png)
