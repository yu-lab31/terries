#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _WIN32
    #define RENDER_API __declspec(dllexport)
#else
    #define RENDER_API
#endif


typedef struct {
    float x;
    float y;
    float z;
} Vector3;


/**
 * @brief 设置网格数据
 * @param vertices 顶点数组指针
 * @param vertexCount 顶点的总个数
 * @param indices 三角形索引数组指针
 * @param indexCount 索引的总个数
 */
RENDER_API void SetMesh(Vector3* vertices, int vertexCount, int* indices, int indexCount);

/**
 * @brief 顶点处理 利用 MVP 矩阵将 3D 顶点转换为 2D 屏幕坐标并缓存
 * @param mvpMatrix 4x4 矩阵 长度为16的float数组
 * @param screenWidth 屏幕总宽
 * @param screenHeight 屏幕总高
 */
RENDER_API void ProcessVertices(float* mvpMatrix, int screenWidth, int screenHeight);

/**
 * @brief 光栅化 遍历三角形 包围盒计算 Z-Buffer深度测试 并写入颜色
 * @param colorBuffer 颜色缓冲首地址 (Bgra32 格式，每个元素 4 字节)
 * @param zBuffer 深度缓冲首地址 (长度等于 screenWidth * screenHeight)
 * @param screenWidth 屏幕总宽
 * @param screenHeight 屏幕总高
 * @param minX 当前线程被分配的区块左边缘
 * @param minY 当前线程被分配的区块上边缘
 * @param maxX 当前线程被分配的区块右边缘
 * @param maxY 当前线程被分配的区块下边缘
 */
RENDER_API void RasterizeTile(
    uint32_t* colorBuffer, float* zBuffer, 
    int screenWidth, int screenHeight, 
    int minX, int minY, int maxX, int maxY);

#ifdef __cplusplus
}
#endif