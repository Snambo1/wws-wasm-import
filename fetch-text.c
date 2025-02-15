#include "actrwasm.h"
#include "actrfetch.h"
#include "actrcanvas.h"
#include "actrasync.h"
#include "actrjson.h"

#define WAIT_TIME 60 * 5
// fetch text example

int asyncHandle = 0;
char *text;
int wait = WAIT_TIME;

[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_heap_string(&text, "loading will begin shortly...");
}

[[clang::export_name("actr_async_result")]]
void actr_async_result(int handle, enum AsyncResult result)
{
    if (handle == asyncHandle)
    {
        asyncHandle = -1;
        switch (result)
        {
        case AsyncResultSuccess:
            actr_free(text);
            text = actr_json_get_string(1, "test");
            break;
        case AsyncResultFailure:
            actr_heap_string(&text, "fetch error, retrying...");
            break;
        case AsyncResultAPIError:
            actr_heap_string(&text, "api error, retrying...");
            break;
        default:
            actr_heap_string(&text, "impossible error");
        }
    }
}

[[clang::export_name("actr_step")]]
void actr_step(float delta)
{

    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, 9999, 9999);

    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(10, 20, text);

    wait--;

    if (wait > 0)
    {
        return;
    }
    wait = WAIT_TIME;
    if (asyncHandle == 0)
    {
        actr_heap_string(&text, "Fetching...");
        asyncHandle = actr_fetch_text("https://mrnathanstiles.github.io/test.txt", 1, "test");
        if (asyncHandle < 1)
        {
            // this would happen if the url is invalid or rejected
            // this will happen if actr_async_result is not exported
            actr_heap_string(&text, "url error, retrying...");
            asyncHandle = 0;
        }
    } else if (asyncHandle == -1) {
        actr_heap_string(&text, "Reloading...");
        asyncHandle = 0;
    }
}