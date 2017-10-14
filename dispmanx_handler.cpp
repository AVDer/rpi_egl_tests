#include "dispmanx_handler.h"

#include <cassert>
#include <iostream>

void DispmanxHandler::init()
{
    int32_t success = 0;
    uint32_t screen_width;
    uint32_t screen_height;
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;
    bcm_host_init();
    // create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */,
                                        &screen_width,
                                        &screen_height);
    assert(success >= 0);
    dst_rect.x = 0;
    dst_rect.height = screen_height;
    dst_rect.y = 0;
    dst_rect.width = screen_width;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = screen_width << 16;
    src_rect.height = screen_height << 16;
    dispman_display = vc_dispmanx_display_open(0 /* LCD */);
    dispman_update = vc_dispmanx_update_start(0);
    dispman_display_ = dispman_display;
    dispman_element =
        vc_dispmanx_element_add(dispman_update, dispman_display,
                                0 /*layer*/, &dst_rect, 0 /*src*/,
                                &src_rect, DISPMANX_PROTECTION_NONE,
                                0 /*alpha*/, 0 /*clamp*/, static_cast<DISPMANX_TRANSFORM_T>(0) /*transform*/);
    // Build an EGL_DISPMANX_WINDOW_T from the Dispmanx window
    native_window_.element = dispman_element;
    native_window_.width = screen_width;
    native_window_.height = screen_height;
    vc_dispmanx_update_submit_sync(dispman_update);
    assert(vc_dispmanx_element_remove(dispman_update, dispman_element) == 0);
    std::cout << "Got a Dispmanx window\n";
}