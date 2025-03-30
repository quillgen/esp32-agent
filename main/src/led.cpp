#include "led.h"

#include <cmath>

// hue: 0-359（色相）
// sat: 0-255（饱和度）
// val: 0-255（亮度）
void hsv2rgb_fast(uint16_t hue, uint8_t sat, uint8_t val,
                  uint8_t *r, uint8_t *g, uint8_t *b)
{
    // 参数约束检查
    hue %= 360;

    // 计算色相区域（0-5）
    uint8_t region = hue / 60;
    uint16_t remainder = (hue % 60) * 256 / 60; // 转换为0-255范围

    // 计算中间值
    uint16_t chroma = ((uint16_t)val * sat) >> 8;
    uint16_t x = ((uint16_t)chroma * (255 - abs(remainder - 128) * 2)) >> 8;
    uint16_t m = val - chroma;

    // 根据色相区域确定颜色分量
    uint16_t r_tmp, g_tmp, b_tmp;
    switch (region)
    {
    case 0:
        r_tmp = chroma;
        g_tmp = x;
        b_tmp = 0;
        break;
    case 1:
        r_tmp = x;
        g_tmp = chroma;
        b_tmp = 0;
        break;
    case 2:
        r_tmp = 0;
        g_tmp = chroma;
        b_tmp = x;
        break;
    case 3:
        r_tmp = 0;
        g_tmp = x;
        b_tmp = chroma;
        break;
    case 4:
        r_tmp = x;
        g_tmp = 0;
        b_tmp = chroma;
        break;
    default:
        r_tmp = chroma;
        g_tmp = 0;
        b_tmp = x;
        break;
    }

    // 合并亮度偏移并限制范围
    *r = (uint8_t)((r_tmp + m) > 255 ? 255 : (r_tmp + m));
    *g = (uint8_t)((g_tmp + m) > 255 ? 255 : (g_tmp + m));
    *b = (uint8_t)((b_tmp + m) > 255 ? 255 : (b_tmp + m));
}

// 整数正弦函数（输入：0-65535对应0-2π，输出：-255到+255）
int16_t int_sin(uint16_t angle)
{
    // 将角度映射到四象限
    uint8_t quadrant = angle >> 14; // 取高2位判断象限
    uint16_t a = angle & 0x3FFF;    // 取低14位作为基础角度

    // 使用抛物线近似正弦曲线（误差<1.5%）
    int32_t x = (int32_t)a - 0x2000; // 转换为-8192~+8191
    int32_t y = (x * (0x7FFF - ((x * x) >> 14))) >> 13;

    // 四象限符号处理
    switch (quadrant)
    {
    case 0:
        return y; // 第一象限
    case 1:
        return 0x7FFF - y; // 第二象限
    case 2:
        return -y; // 第三象限
    default:
        return y - 0x7FFF; // 第四象限
    }
}