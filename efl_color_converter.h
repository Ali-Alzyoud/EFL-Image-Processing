typedef enum {
   EFL_COLORSPACE_RGBA32            = 0,
   EFL_COLORSPACE_HSV24               = 1,
} EFL_COLORSPACE;

typedef struct _RGBA32
{
    unsigned char r, g, b, a;
} RGBA32;

typedef struct _HSV24
{
    unsigned char h, s, v;
} HSV24;

int efl_color_buffer_convert(
   void * src,
   void * des,
   unsigned int src_len,
   unsigned int des_len,
   EFL_COLORSPACE src_colorspace,
   EFL_COLORSPACE des_colorspace);