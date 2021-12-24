#include "../header/GameObject.h"
#include "../header/Logger.h"
#include "../header/AssetManager.h"
#include "../header/GPUBase.h"
#include "../header/MemUtils.h"
#include "../header/TxtHandler.h"

char **static_messages;
char **animated_messages;

int main() {
    Font *fnt;
    Message *msg;
    Dialog *dlg_anim, *dlg_static;
    Color bg_color = {20, 70, 120};
    MEM_INIT_HEAP_3()
    gpub_init_screen(320, 240, GPUB_MODE_NTSC, GPUB_FPS_60);
    gpub_set_bg_color(&bg_color);
    CDR_INIT()
    fnt = txt_fnt_init("FONT.TIM", 8, 8, 2);
    CDR_CLOSE()
    msg = txt_msg_init(fnt, 50, 200, "Static Message Works", 0);
    static_messages[0] = "Static Message Works";
    animated_messages[0] = "Animated Message works!";
//    dlg_static = txt_dlg_init(static_messages, 1, fnt, 0, 50, 100);
//    dlg_anim = txt_dlg_init(animated_messages, 1, fnt, 5, 50, 150);

    while (1) {
        gpub_clear_display();
        FntPrint("Fonts test project");
//        txt_msg_draw(msg);
//        txt_dlg_draw(dlg_static);
//        txt_dlg_draw(dlg_anim);
//        txt_dlg_tick(dlg_anim);
        gpub_display();
    }
}

