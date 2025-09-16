# 标题
&nbsp;&nbsp;&nbsp;&nbsp;这是一个跟随恋恋风辰([博客地址](https://llfc.club/home)、[B站地址](https://space.bilibili.com/271469206))学习用Qt写一个相册的项目。目前完成了album内相册的创建和打开，图片的展示和切换，剩余未完成的是以幻灯片形式播放图片的相关功能。
## 开发环境
+ 系统版本：Ubuntu 18.04.4 LTS
+ Qt版本：Qt 5.9.5、Qt Creator 4.5.2
## 亮点
### 线程分离
    继承QThread重写了run()方法，在加载文件夹时避免了主线程的阻塞。具体见ProTreeThread和OpenTreeThread。
### 针对Ubuntu的优化
    开发过程中发现QFileDialog的`setFileMode(QFileDialog::Directory)`和`setOption(QFileDialog::ShowDirsOnly)`组合并没有达到期望的效果，我期望的是向导页浏览本地目录时只展示文件夹不展示文件，而这个模式在Ubuntu下还是会显示文件，官方文档也说了 `The name of a directory. Both files and directories are displayed. However, the native Windows file dialog does not support displaying files in the directory chooser`。针对该情况做了如下优化：
```cpp
QFileDialog file_dialog;
file_dialog.setFileMode(QFileDialog::Directory);
// 仅在Linux上强制使用Qt对话框
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
// Linux下默认的对话框模式设置QFileDialog::ShowDirsOnly不生效
    file_dialog.setOption(QFileDialog::DontUseNativeDialog, true);
#endif
file_dialog.setOption(QFileDialog::ShowDirsOnly);
```
### 图片动态变化大小
&nbsp;&nbsp;&nbsp;&nbsp;原代码在PicShow相关的布局和尺寸的获取有些问题，导致即使重写了resizeevent()方法，初次显示图片时图片会把已经全屏的主界面撑大，同时在拉伸主界面时图片所在的布局只能放大不能缩小。对此我做了一些改动。
- 新建QWidget作为label和四个弹簧的父widget

![designer界面](./assets/screenShot_1.PNG)
- 设置布局gridlayout
  - 对PicShow的三个子widget从左往右重命名为widget, widget_2, widget_3。widget_2布局设置为QGridLayout，修改布局的拉伸因子stretch，数字越大优先级越高，行为0, 1, 0，列为0, 1, 0，如图

![修改拉伸因子](./assets/screenShot_2.PNG)
- 修改子label的尺寸调整策略
  - 创建时这五个原件默认sizepolicy都是expanding，这会出现图片能放大但不能缩小的问题，我查到的解决方式是把label的sizepolicy修改为ignored。最终效果如下(为凸显效果，对左widget和右widget_3添加了临时的边框)：

![取消全屏](./assets/screenShot_3.PNG)

![打开图片后保持比例占据label的全部](./assets/screenShot_4.PNG)

![放大且全屏](./assets/screenShot_5.PNG)

![缩小时图片跟着变化](./assets/screenShot_6.PNG)