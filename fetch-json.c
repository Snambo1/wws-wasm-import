#include "actrwasm.h"
#include "actrfetch.h"
#include "actrmap.h"
#include "actrcanvas.h"
#include "actrasync.h"
#include "actrjson.h"

// fetch json example

int asyncHandle = 0;
char * text = "loading will begin shortly...";

[[clang::export_name("actr_async_result")]]
void actr_async_result(int handle, int success)
{
    if (handle == asyncHandle) {
        if (success == 1) {
            text = actr_json_get_string(1, "MyObject.one");
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
        asyncHandle = actr_fetch_json("https://mrnathanstiles.github.io/test.json", 1);
        if (asyncHandle < 1) {
            // this would happen if the url is invalid or rejected
            // this will happen if actr_async_result is not exported
            text = "url error, retrying...";
            asyncHandle = 0;
        }
    }


    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, 9999, 9999);

    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(10, 20, text);

}