#include <Elementary.h>
#include "efl_color_converter.h"

enum BUTTON
{
   BUTTON_INC_SATURATION = 0,
   BUTTON_DEC_SATURATION = 1,
   BUTTON_INC_VALUE = 2,
   BUTTON_DEC_VALUE = 3,
   BUTTON_RESET = 4,
   BUTTON_ALL = BUTTON_RESET + 1,
};

char *BUTTON_STR[BUTTON_ALL] = {
    "INC SATURATION",
    "DEC SATURATION",
    "INC VALUE",
    "DEC VALUE",
    "RESET",
};

typedef struct _APP
{
   Evas_Object *win, *box, *img, *boxHor, *boxHor2;
   Eo *btn[BUTTON_ALL];
   char *str;
   HSV24 *hsv_data;
   HSV24 *hsv_data_tmp;
   size_t hsv_data_len;
   RGBA32 *img_src;
   size_t img_src_len;
   int img_w, img_h;
} APP;
APP *app;

void hsv_update(Evas_Object *img, int sat, int value)
{
   if (!app->img_src)
   {
      app->img_src = evas_object_image_data_get(img, EINA_TRUE);
      evas_object_image_size_get(img, &app->img_w, &app->img_h);
      app->img_src_len = app->img_w * app->img_h * 4;
   }
   if (!app->hsv_data)
   {
      app->hsv_data_len = app->img_w * app->img_h * 3;
      app->hsv_data = malloc(app->hsv_data_len);
      app->hsv_data_tmp = malloc(app->hsv_data_len);
      efl_color_buffer_convert(app->img_src, app->hsv_data, app->img_src_len, app->hsv_data_len, EFL_COLORSPACE_RGBA32, EFL_COLORSPACE_HSV24);
   }

   memcpy(app->hsv_data_tmp, app->hsv_data, app->hsv_data_len);

   if(sat != 0 || value != 0)
   for (int i = 0; i < app->hsv_data_len / 3; i++)
   {
      int new_s = app->hsv_data_tmp[i].s + sat;
      if (new_s > 255)
         new_s = 255;
      else if (new_s < 0)
         new_s = 0;
      app->hsv_data_tmp[i].s = new_s; 

      int new_v = app->hsv_data_tmp[i].v + value;
      if (new_v > 255)
         new_v = 255;
      else if (new_v < 0)
         new_v = 0;
      app->hsv_data_tmp[i].v = new_v; 
   }

   efl_color_buffer_convert(app->hsv_data_tmp, app->img_src, app->hsv_data_len, app->img_src_len, EFL_COLORSPACE_HSV24, EFL_COLORSPACE_RGBA32);
   evas_object_image_data_update_add(img, 0, 0, app->img_w, app->img_h);
}

static void _btn_clicked(void *data, Eo *obj, void *eventInfo)
{
   static int sat = 0,value = 0;
   if (obj == app->btn[BUTTON_INC_SATURATION])
   {
      sat+=25;
   }
   else if (obj == app->btn[BUTTON_DEC_SATURATION])
   {
      sat-=25;
   }
   else if (obj == app->btn[BUTTON_INC_VALUE])
   {
      value+=25;
   }
   else if (obj == app->btn[BUTTON_DEC_VALUE])
   {
      value-=25;
   }
   else if (obj == app->btn[BUTTON_RESET])
   {
      value = 0;
      sat = 0;
   }
   hsv_update(app->img, sat, value);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   app = calloc(sizeof(APP), 1);

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   app->win = elm_win_util_standard_add("Main", "App");
   elm_win_autodel_set(app->win, EINA_TRUE);

   app->box = elm_box_add(app->win);
   app->boxHor = elm_box_add(app->box);
   app->boxHor2 = elm_box_add(app->box);
   app->img = evas_object_image_filled_add(app->box);
   evas_object_image_file_set(app->img, "1.jpeg", NULL);

   elm_box_horizontal_set(app->boxHor, EINA_TRUE);
   elm_box_horizontal_set(app->boxHor2, EINA_TRUE);

   evas_object_size_hint_weight_set(app->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(app->box, EVAS_HINT_FILL, EVAS_HINT_FILL);

   evas_object_show(app->img);
   evas_object_show(app->box);
   evas_object_show(app->boxHor);
   evas_object_show(app->boxHor2);

   elm_box_pack_end(app->box, app->img);
   elm_box_pack_end(app->box, app->boxHor);
   elm_box_pack_end(app->box, app->boxHor2);

   elm_win_resize_object_add(app->win, app->box);
   evas_object_resize(app->win, 320, 480);

   for (int i = 0; i < BUTTON_ALL; i++)
   {
      app->btn[i] = elm_button_add(app->boxHor);
      evas_object_smart_callback_add(app->btn[i], "clicked", _btn_clicked, NULL);
      elm_object_text_set(app->btn[i], BUTTON_STR[i]);
      elm_box_pack_end(app->boxHor, app->btn[i]);
      evas_object_show(app->btn[i]);
   }

   evas_object_size_hint_weight_set(app->img, EVAS_HINT_EXPAND, 0.1);
   evas_object_size_hint_align_set(app->img, EVAS_HINT_FILL, EVAS_HINT_FILL);

   evas_object_show(app->win);

   elm_run();

   return 0;
}
ELM_MAIN()
