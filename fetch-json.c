#include "actrwasm.h"
#include "actrfetch.h"
#include "actrmap.h"
#include "actrcanvas.h"
#include "actrasync.h"
#include "actrjson.h"
#include "actrui.h"
#include "actrlog.h"
#include "actrformat.h"

#define MAP_MAIN 1
#define MAP_PATH "fetch.json.example.clickCount"
// fetch json example

enum MyState
{
    MyStateLoadingMap,
    MyStateMapLoaded
};

struct MyData
{
    int asyncHandle;
    char *text;
    int state;
    struct ActrUIButton *button;
};

struct MyData *data;

extern void logsz(int val);
extern void logsuc(int val);
extern void loghan(int val);
extern void loghansuc(int val, int suc);

[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_ui_init();
    data = actr_malloc(sizeof(struct MyData));
    data->state = MyStateLoadingMap;
    data->text = "loading...";
    data->asyncHandle = actr_json_load(MAP_MAIN);
    data->button = actr_ui_button(20, 20, 200, 50, actr_heap_string("loading..."));
}

void asyncsuc();
void asyncfail();
void asyncapierr();

void reformatButton()
{
    actr_free(data->button->text);
    int tapCount = actr_json_get_int(MAP_MAIN, MAP_PATH);
    struct ActrFormatState *format = actr_format("Click Count: %s");
    actr_format_int(format, tapCount);
    data->button->text = actr_format_close(format);
}
[[clang::export_name("actr_tap")]]
void actr_tap(int x, int y)
{
    int identity = actr_ui_tap(x, y);
    if (identity == data->button->identity)
    {
        int tapCount = actr_json_get_int(MAP_MAIN, MAP_PATH);
        tapCount++;
        actr_json_set_int(MAP_MAIN, MAP_PATH, tapCount);
        reformatButton();
        actr_json_store(MAP_MAIN);
    }
}
[[clang::export_name("actr_move")]]
void actr_move(int x, int y)
{
    actr_ui_move(x, y);
}
[[clang::export_name("actr_async_result")]]
void actr_async_result(int handle, enum AsyncResult success)
{
    if (handle == data->asyncHandle)
    {
        switch (success)
        {
        case AsyncResultSuccess:
            reformatButton();
            break;
        case AsyncResultFailure:
            // only json_load will fail this way if nothing exists to load
            actr_json_set_int(MAP_MAIN, MAP_PATH, 0);
            reformatButton();
            break;
        case AsyncResultAPIError:
            // json_store/load/delete will fail this way if no user/not logged in or connection failure
            asyncapierr();
            break;
        }
    }
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    actr_ui_draw(delta);
}
