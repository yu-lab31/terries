using System.Numerics;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Render
{
    public partial class MainWindow : Window
    {
        private const int RenderWidth = 800;
        private const int RenderHeight = 600;

        private WriteableBitmap _colorBuffer;
        private float[] _zBuffer;

        private float _rotationAngle = 0f;


        private readonly Vector3[] Cube = new Vector3[]{
            new Vector3(-1,  1, -1),
            new Vector3(-1, -1, -1),
            new Vector3( 1, -1, -1),
            new Vector3( 1,  1, -1),
            new Vector3(-1,  1,  1),
            new Vector3(-1, -1,  1),
            new Vector3( 1, -1,  1),
            new Vector3( 1,  1,  1)
        };
        private readonly int[] CubeIndices = new int[]{
            0,1,2, 0,2,3,
            7,6,5, 7,5,4,
            4,5,1, 4,1,0,
            3,2,6, 3,6,7,
            4,0,3, 4,3,7,
            1,5,6, 1,6,2
        };
        private readonly uint[] Colors = new uint[]{
            0xFFFF0000, 0xFF880000, // 红色
            0xFF00FF00, 0xFF008800, // 绿色
            0xFF0000FF, 0xFF000088, // 蓝色
            0xFFFFFF00, 0xFF888800, // 黄色
            0xFFFF00FF, 0xFF880088, // 紫色
            0xFF00FFFF, 0xFF008888  // 青色
        };


        public MainWindow()
        {
            InitializeComponent();

            _colorBuffer = new WriteableBitmap(RenderWidth, RenderHeight, 96, 96, PixelFormats.Bgra32, null);
            ScreenImage.Source = _colorBuffer;

            _zBuffer = new float[RenderWidth * RenderHeight];
            CompositionTarget.Rendering += OnRender;
        }

        private unsafe void OnRender(object? sender, EventArgs e)
        {
            _rotationAngle += 0.02f;
            _colorBuffer.Lock();
            IntPtr pColor = _colorBuffer.BackBuffer;
            uint* ptrColor = (uint*)pColor.ToPointer();

            //清理颜色缓冲 和 Z-Buffer缓冲
            for (int i = 0; i < RenderWidth * RenderHeight; i++)
            {
                ptrColor[i] = 0xFF1E1E1E;
            }
            Array.Fill(_zBuffer, float.MaxValue);

            var modelMatrix = Matrix4x4.CreateRotationX(_rotationAngle * 0.8f) * Matrix4x4.CreateRotationY(_rotationAngle);
            var viewMatrix = Matrix4x4.CreateTranslation(0, 0, -5);
            var projectionMatrix = Matrix4x4.CreatePerspectiveFieldOfView((float)(Math.PI / 4), (float)RenderWidth / RenderHeight, 0.1f, 100f);
            var mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;

            Vector3[] screenVertices = new Vector3[Cube.Length];
            for (int i = 0; i < Cube.Length; i++)
            {
                Vector4 clipCoords = Vector4.Transform(new Vector4(Cube[i], 1.0f), mvpMatrix);

                Vector3 ndcCoords = new Vector3(
                    clipCoords.X / clipCoords.W,
                    clipCoords.Y / clipCoords.W,
                    clipCoords.Z / clipCoords.W);

                screenVertices[i] = new Vector3(
                    (ndcCoords.X + 1.0f) * 0.5f * RenderWidth,
                    (1.0f - ndcCoords.Y) * 0.5f * RenderHeight,
                    ndcCoords.Z
                );
            }
            int tilesX = 4;
            int tilesY = 4;
            int tileW = RenderWidth / tilesX;
            int tileH = RenderHeight / tilesY;

            fixed (float* ptrZBuffer = _zBuffer)
            {
                IntPtr zBufferIntPtr = (IntPtr)ptrZBuffer;
                Parallel.For(0, tilesX * tilesY, i =>
                {
                    int tx = i % tilesX;
                    int ty = i / tilesX;
                    int minX = tx * tileW;
                    int minY = ty * tileH;
                    int maxX = minX + tileW;
                    int maxY = minY + tileH;

                    float* threadLocalPtrZ = (float*)zBufferIntPtr;

                    //遍历12个三角形
                    for (int tri = 0; tri < 12; tri++)
                    {
                        Vector3 v0 = screenVertices[CubeIndices[tri * 3]];
                        Vector3 v1 = screenVertices[CubeIndices[tri * 3 + 1]];
                        Vector3 v2 = screenVertices[CubeIndices[tri * 3 + 2]];
                        uint color = Colors[tri];
                        DrawTriangle3D(ptrColor, threadLocalPtrZ, RenderWidth, minX, minY, maxX, maxY, v0, v1, v2, color);
                    }
                });
            }

            _colorBuffer.AddDirtyRect(new Int32Rect(0, 0, RenderWidth, RenderHeight));
            _colorBuffer.Unlock();
        }

        private unsafe void DrawTriangle3D(
            uint* colorBuffer, float* zBuffer, int screenWidth,
            int minX, int minY, int maxX, int maxY,
            Vector3 v0, Vector3 v1, Vector3 v2, uint color)
        {
            int bboxMinX = (int)Math.Max(minX, Math.Min(v0.X, Math.Min(v1.X, v2.X)));
            int bboxMinY = (int)Math.Max(minY, Math.Min(v0.Y, Math.Min(v1.Y, v2.Y)));
            int bboxMaxX = (int)Math.Min(maxX, Math.Max(v0.X, Math.Max(v1.X, v2.X)) + 1);
            int bboxMaxY = (int)Math.Min(maxY, Math.Max(v0.Y, Math.Max(v1.Y, v2.Y)) + 1);

            for (int y = bboxMinY; y < bboxMaxY; y++)
            {
                for (int x = bboxMinX; x < bboxMaxX; x++)
                {
                    float px = x + 0.5f;
                    float py = y + 0.5f;

                    float w0 = EdgeFunction(v1, v2, px, py);
                    float w1 = EdgeFunction(v2, v0, px, py);
                    float w2 = EdgeFunction(v0, v1, px, py);

                    //计算整个三角形的面积
                    float area = w0 + w1 + w2;
                    if (area == 0) continue;

                    // 判断是否在三角形内
                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    {
                        //归一化重心坐标
                        w0 /= area;
                        w1 /= area;
                        w2 /= area;

                        //计算出当前像素点的 Z 深度
                        float pixelZ = w0 * v0.Z + w1 * v1.Z + w2 * v2.Z;

                        int index = y * screenWidth + x;

                        if (pixelZ < zBuffer[index])
                        {
                            zBuffer[index] = pixelZ;
                            colorBuffer[index] = color;
                        }
                    }
                }
            }
        }
        private float EdgeFunction(Vector3 a, Vector3 b, float px, float py)
        {
            return (b.X - a.X) * (py - a.Y) - (b.Y - a.Y) * (px - a.X);
        }
    }
}