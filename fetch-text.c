#include "actrwasm.h"
#include "actrfetch.h"
#include "actrmap.h"
#include "actrcanvas.h"
#include "actrasync.h"

// fetch text example

int asyncHandle = 0;
char * text = "loading will begin shortly...";

[[clang::export_name("actr_async_result")]]
void actr_async_result(int handle, int success)
{
    if (handle == asyncHandle) {
        if (success == 1) {
            text = actr_map_get_string(1, "test");
        } else {
            // this will happen if the fetch request fails
            asyncHandle = 0;
            text = "fetch error, retrying...";
        }
    }
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    if (asyncHandle == 0)
    {
        text = "loading...";
        asyncHandle = actr_fetch_text("https://mrnathanstiles.github.io/test.txt", 1, "test");
        if (asyncHandle < 1) {
            // this would happen if the url is invalid or rejected
            // this will happen if actr_async_result is not exported
            text = "url error, retrying...";
            asyncHandle = 0;
        }
    }


    actr_canvas2d_fillStyle(0, 0, 0, 100);
    actr_canvas2d_fillRect(-10, -10, 9999, 9999);

    actr_canvas2d_fillStyle(255, 255, 255, 100);
    actr_canvas2d_fillText(10, 20, text, strlen(text));

}