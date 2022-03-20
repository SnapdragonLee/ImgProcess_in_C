# ImgProcess_in_C

用纯 C 语言完成的一套图像处理流程。包括 bmp 的文件 IO、rgb数据的处理、图像直方图均衡、图像灰度线性拉伸等等。



## 1 读取 BMP 图像

用 C 语言读取 BMP 图像难度不大，整体要搞定这种文件的文件头，以及信息头、调色板等内容。暂时完成了 24bit 与 8bit 的读取、互转与存取。



主要的结构可以查看以下定义的结构体：

```c
typedef struct bmpFileHeader {
    unsigned short bFType;        // File type (0x4d42 19778)
    unsigned short bFSize_1;      // File size in bytes _1 (2-3 Bytes)
    unsigned short bFSize_2;      // File size in bytes _2 (4-5 Bytes)
    unsigned short bFReserved1;   // Reserved (6-7 Bytes)
    unsigned short bFReserved2;   // Reserved (8-9 Bytes)
    unsigned short bFOffBits_1;   // Offset to image data _1  (10-11 Bytes)
    unsigned short bFOffBits_2;   // Offset to image data _2  (12-13 Bytes)
    unsigned int bFSize;          // File size
    unsigned int bFOffBits;       // Offset to image data
} bmpFileHeader;

typedef struct bmpInfoHeader {
    unsigned int bISize;          // Info header size (14-17 Bytes)
    int bIWidth;                  // Width  (18-21 Bytes)
    int bIHeight;                 // Height  (22-25 Bytes)
    unsigned short bIPlanes;      // Number of colour planes (26-27 Bytes)
    unsigned short bIBitCount;    // Bits per pixel (28-29 Bytes)
    unsigned int bICompression;   // Compression type (30-33 Bytes)
    unsigned int bISizeImage;     // Image size (34-37 Bytes)
    int bIXPelsPerMeter;          // XPixels per meter (38-41 Bytes)
    int bIYPelsPerMeter;          // YPixels per meter (42-45 Bytes)
    unsigned int bIClrUsed;       // Number of colours (46-49 Bytes)
    unsigned int bIClrImportant;  // Important colours (50-53 Bytes)
} bmpInfoHeader;

typedef struct bmpPixelInfo {
    unsigned char rgbBlue;        // B  (0-255)
    unsigned char rgbGreen;       // G  (0-255)
    unsigned char rgbRed;         // R  (0-255)
    unsigned char rgbReserved;    // Reserved
} bmpPixelInfo;
```

由于 C 语言的 struct 结构体大小计算的限制，`bFSize` 与 `bFOffBits`被我拆开成了 2 个部分，并使用位运算合成的方式，这样可以减少文件 IO 函数使用的时间，做到一定的优化。

这里要注意一个问题，就是 BMP 文件读取的时候，由于格式要求 32 位完整大小，所以遇到 `图像宽度*位宽` 非 32 的倍数时，要在该行末尾补 0，直到其数据长度被 32 整除。



![lena2](example/lena2.bmp)

<div align = "center">lena 24bit origin</div>



另外，由于 24-bit 的真彩 BMP 文件不会有调色板的问题，因此在 24bit 转换到 8bit 图像的时候需要加入调色板的信息，我这里直接加入了全灰度色板 (0-255)，如有需要可以选择更改 `bmpFileIO/bmpBoard.profile`，注意要用二进制打开该文件。

![lena2-Gray24](example/lena2-Gray24.bmp)

<div align = "center">lena 24bit gray</div>



![lena2-Gray8](example/lena2-Gray8.bmp)

<div align = "center">lena 8bit gray</div>





## 直方图均衡

图像直方图均衡，对于背景和前景都太亮或者太暗的图像非常有用，这种方法可以带来灰度图图像中曝光过度或者曝光不足照片中更好的细节。这种方法的一个缺点是它对处理的数据不加选择，它可能会增加背景噪声的对比度并且降低有用信号的对比度。

累积分布函数 cdf，定义为： $cdf(v) = \Sigma_i cdf(i), i < v$ .

新的映射 h(v)，定义为： $h(v) = round(\frac{cdf(v)-cdf_{min}}{cdf_{max}-cdf_{min}})*(L-1)$ .



![lena2-HistogramEqualization8](example/lena2-HistogramEqualization8.bmp)

<div align = "center">lena 8bit HistogramEqualization</div>

此外，该方法不适合应用于 RGB 图像，因为 RGB 图像的三个维度数据之间有联系，直接分别对 r、g、b 三个通道直接进行直方图均衡，会导致颜色的失真。



![lena2-HistogramEqualization24](example/lena2-HistogramEqualization24.bmp)

<div align = "center">lena 24bit HistogramEqualization</div>



## 线性变换

图像的线性变换，要求为有一个分段函数建立在各个通道上，并将原先的通道值通过新的函数映射成新的。这里做了一个简化，程序可以接收一个序列包含在中括号 `[]` 内，序列的内容为分段函数的拐点，一次最多可以接受 20 个拐点（必须包含 0 和 255的点，且需要从小到大排列）。



例如输入序列：`[[0,0],[100,40],[150, 100],[200,250],[255,255]]` 

则可以得到如下灰度图结果：

![lena2-Linea Transform8](example/lena2-Linea%20Transform8.bmp)

<div align = "center">lena 8bit LineaTransform</div>

当然，也可以得到如下真彩图结果：

![lena2-LineaTransform24](example/lena2-LineaTransform24.bmp)

<div align = "center">lena 24bit LineaTransform</div>



## 项目结构

`settings/` 存储图像的信息头，颜色板，各种参数信息，不建议更改。

`processAPI/` 封装的 API 函数，可以根据 `\base` 目录下的任意底层函数进行调用，可以自行更改。

`bmpFileIO/` bmp 24bit 8bit 图像的互转（包括颜色板）、输入输出等，不建议更改。

`base/` 图像底层的函数，`base/transferFunc.h` 为颜色空间转换底层函数，`base/algoFunc.h` 为各种图像处理算法的底层函数。

`example/` 为各种调试图片输出，可根据命名查看处理后的结果。



当开始测试的时候，注意在 `main.c` 的算法区域填入自己封装的 API，用来测试相关的性能和正确性。



## Run

Compile the C file:

```bash
cmake ./CMakeLists.txt
make
```



Then run with args[], filled with filename:

```bash
./ImgProcess_in_C example/lena2.bmp
```