#include <Elementary.h>

enum BUTTON{
   BUTTON_COPY              = 0,
   BUTTON_PASTE             = 1,
   BUTTON_SELECT_ALL        = 2,
   BUTTON_CUT               = 3,
   BUTTON_SET               = 4,
   BUTTON_GET               = 5,
   BUTTON_ONLY_NUMBERS_9    = 6,
   BUTTON_PASSWORD          = 7,

   BUTTON_ALL               = BUTTON_PASSWORD+1,
};

char* BUTTON_STR[BUTTON_ALL] ={
   "COPY",
   "PASTE",
   "SELECT_ALL",
   "CUT",
   "SET",
   "GET",
   "ONLY_NUMBERS_9",
   "PASSWORD",
};

enum BUTTON_STATE{
   BUTTON_STATE_MODE              = 0,
   BUTTON_STATE_AUTO_CAPITAL      = 1, 
   BUTTON_STATE_SCROLL            = 2,
   BUTTON_STATE_EDITABLE          = 3,
   BUTTON_STATE_SINGLE_LINE       = 4,
   BUTTON_STATE_WARP              = 5,
   BUTTON_STATE_ICON_VISIBLE      = 6,
   BUTTON_STATE_ALL               = BUTTON_STATE_ICON_VISIBLE + 1,
};


char* BUTTON_STATE_STR[BUTTON_STATE_ALL] ={
   "MODE",
   "AUTO_CAPITAL",
   "SCROLL",
   "EDITABLE",
   "SINGLE_LINE",
   "WARP",
   "ICON_VISIBLE",
};

typedef struct _APP
{
   Evas_Object *win, *box, *img, *boxHor, *boxHor2;
   Eo *btn[BUTTON_ALL];
   Eo *btnState[BUTTON_STATE_ALL];
   char * str;
} APP;
APP *app;

void
image_blur(Evas_Object *img)
{
    unsigned char *img_src = evas_object_image_data_get(img, EINA_TRUE);

    int w;
    int h;
    evas_object_image_size_get(img, &w, &h);
    int blur_size = 4;
    int x;
    int y;
    int xx;
    int yy;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            int avg_color[3] = {0, 0, 0};
            int blur_pixel_cnt = 0;

            for (xx = x; (xx < x + blur_size) && (xx < w); xx++) {
                for (yy = y; (yy < y + blur_size) && (yy < h); yy++) {
                    int idx = (yy * w * 4) + (xx * 4);
                    avg_color[0] += img_src[idx + 0];
                    avg_color[1] += img_src[idx + 1];
                    avg_color[2] += img_src[idx + 2];
                    ++blur_pixel_cnt;
                }
            }
            avg_color[0] /= blur_pixel_cnt;
            avg_color[1] /= blur_pixel_cnt;
            avg_color[2] /= blur_pixel_cnt;

            for (xx = x; (xx < x + blur_size) && (xx < w); xx++) {
                for (yy = y; (yy < y + blur_size) && (yy < h); yy++) {
                    int idx = (yy * w * 4) + (xx * 4);
                    img_src[idx + 0] = avg_color[0];
                    img_src[idx + 1] = avg_color[1];
                    img_src[idx + 2] = avg_color[2];
                }
            }
        }
    }
    evas_object_image_data_update_add(img, 0, 0, w, h);
}




static void _btn_clicked(void *data, Eo *obj, void *eventInfo){
   if (obj == app->btn[BUTTON_GET]){
     image_blur(app->img);
   }  else if (obj == app->btn[BUTTON_SET]){
     
   }else if (obj == app->btn[BUTTON_COPY]){
     
   } else if (obj == app->btn[BUTTON_SELECT_ALL]){
     
   } else if (obj == app->btn[BUTTON_PASTE]){
     
   } else if (obj == app->btn[BUTTON_PASSWORD]){
     
   } else if (obj == app->btn[BUTTON_CUT]){
     
   } else if (obj == app->btn[BUTTON_ONLY_NUMBERS_9]){
     
   } 
}

static void _btn_state_clicked(void *data, Eo *obj, void *eventInfo)
{
   if (obj == app->btnState[BUTTON_STATE_MODE])
   {
     
   } else if (obj == app->btnState[BUTTON_STATE_AUTO_CAPITAL]){
     
   } else if (obj == app->btnState[BUTTON_STATE_SCROLL]){
     
   } else if (obj == app->btnState[BUTTON_STATE_EDITABLE]){
     
   } else if (obj == app->btnState[BUTTON_STATE_SINGLE_LINE]){
     
      
   } else if (obj == app->btnState[BUTTON_STATE_WARP]){
     
   }
   else if (obj == app->btnState[BUTTON_STATE_WARP]){
     
   }
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
   app->img =  evas_object_image_filled_add(app->box);
    evas_object_image_file_set(app->img, "4.png", NULL);


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

   for(int i = 0 ; i < BUTTON_ALL ; i++){
      app->btn[i] = elm_button_add(app->boxHor);
      evas_object_smart_callback_add(app->btn[i], "clicked", _btn_clicked, NULL);\
      elm_object_text_set(app->btn[i], BUTTON_STR[i]);
      elm_box_pack_end(app->boxHor, app->btn[i]);
      evas_object_show(app->btn[i]);
   }

   for(int i = 0 ; i < BUTTON_STATE_ALL ; i++){
      app->btnState[i] = elm_button_add(app->boxHor2);
      evas_object_smart_callback_add(app->btnState[i], "clicked", _btn_state_clicked, NULL);
      elm_object_text_set(app->btnState[i], BUTTON_STATE_STR[i]);
      elm_box_pack_end(app->boxHor2, app->btnState[i]);
      evas_object_show(app->btnState[i]);
   }
   

   evas_object_size_hint_weight_set(app->img, EVAS_HINT_EXPAND, 0.1);
   evas_object_size_hint_align_set(app->img, EVAS_HINT_FILL, EVAS_HINT_FILL);

   evas_object_show(app->win);
   
   elm_run();

   return 0;
}
ELM_MAIN()
