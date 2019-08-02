#include "efl_color_converter.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

//https://gist.github.com/yoggy/8999625
#define min_f(a, b, c)  (fminf(a, fminf(b, c)))
#define max_f(a, b, c)  (fmaxf(a, fmaxf(b, c)))

void hsv_to_rgb(HSV24 hsv, RGBA32 *rgba)
{
    float h = hsv.h * 2.0f;   // 0-360
    float s = hsv.s / 255.0f; // 0.0-1.0
    float v = hsv.v / 255.0f; // 0.0-1.0

    float r, g, b; // 0.0-1.0

    int hi = (int)(h / 60.0f) % 6;
    float f = (h / 60.0f) - hi;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (hi)
    {
    case 0:
        r = v, g = t, b = p;
        break;
    case 1:
        r = q, g = v, b = p;
        break;
    case 2:
        r = p, g = v, b = t;
        break;
    case 3:
        r = p, g = q, b = v;
        break;
    case 4:
        r = t, g = p, b = v;
        break;
    case 5:
        r = v, g = p, b = q;
        break;
    }

    rgba->r = (unsigned char)(r * 255); // rgba->r : 0-255
    rgba->g = (unsigned char)(g * 255); // rgba->r : 0-255
    rgba->b = (unsigned char)(b * 255); // rgba->r : 0-255
    rgba->a = 0xff;
}

void rgb_to_hsv(RGBA32 rgba, HSV24 *hsv)
{
    float r = rgba.r / 255.0f;
    float g = rgba.g / 255.0f;
    float b = rgba.b / 255.0f;

    float h, s, v; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

    float max = max_f(r, g, b);
    float min = min_f(r, g, b);

    v = max;

    if (max == 0.0f)
    {
        s = 0;
        h = 0;
    }
    else if (max - min == 0.0f)
    {
        s = 0;
        h = 0;
    }
    else
    {
        s = (max - min) / max;

        if (max == r)
        {
            h = 60 * ((g - b) / (max - min)) + 0;
        }
        else if (max == g)
        {
            h = 60 * ((b - r) / (max - min)) + 120;
        }
        else
        {
            h = 60 * ((r - g) / (max - min)) + 240;
        }
    }

    if (h < 0)
        h += 360.0f;

    hsv->h = (unsigned char)(h / 2);   // HSV24->h : 0-180
    hsv->s = (unsigned char)(s * 255); // HSV24->s : 0-255
    hsv->v = (unsigned char)(v * 255); // HSV24->v : 0-255
}

int efl_color_buffer_convert(
    void *src,
    void *des,
    unsigned int src_len,
    unsigned int des_len,
    EFL_COLORSPACE src_colorspace,
    EFL_COLORSPACE des_colorspace)
{
    if (src_colorspace == des_colorspace)
    {
        memcpy(des, src, src_len);
    }
    else if (src_colorspace == EFL_COLORSPACE_RGBA32)
    {
        size_t items_count = src_len / sizeof(RGBA32);
        for (int i = 0; i < items_count; i++)
        {
            RGBA32 *rgba = (RGBA32 *)src;
            HSV24 *hsv = (HSV24 *)des;
            rgb_to_hsv(*rgba, hsv);
            src += sizeof(RGBA32);
            des += sizeof(HSV24);
        }
    }
    else if (src_colorspace == EFL_COLORSPACE_HSV24)
    {
        size_t items_count = src_len / sizeof(HSV24);
        for (int i = 0; i < items_count; i++)
        {
            RGBA32 *rgba = (RGBA32 *)des;
            HSV24 *hsv = (HSV24 *)src;
            hsv_to_rgb(*hsv, rgba);
            des += sizeof(RGBA32);
            src += sizeof(HSV24);
        }
    }
    return 0;
}