

#include "actrformat.h"
#include "actrtime.h"
#include "actrui.h"
#include "actrprng.h"
#include "actrmergesortmutate.h"

#define PART_COUNT 60

struct MyState
{
    struct ActrVector *updates;
    struct ActrVector *uiParts;
    struct ActrVector *scramble;
    int countDown;
    struct ActrUIControlButton * restartHandle;
    int index;
};

struct MyState *state;

void dbg(int u);
[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_ui_init();

    state = actr_malloc(sizeof(struct MyState));
    state->updates = actr_vector_init(8, 8);
    state->uiParts = actr_vector_init(8, 8);
    state->scramble = actr_vector_init(8, 8);
    state->restartHandle = actr_ui_button(20, 20, 100, 25, "Restart");
    state->countDown = -1;
    state->index = 0;
}
void generateControls()
{
    for (int i = 0; i < state->uiParts->count; i++)
    {
        actr_ui_remove_control(state->uiParts->head[i]);
    }
    state->uiParts->count = 0;

    struct ActrVector *step = state->updates->head[state->index];
    int fgcolor = actr_pack_bytes(100, 100, 255, 100);
    int bgcolor = actr_pack_bytes(100, 100, 255, 25);
    
    int width = actrState->canvasSize.w / PART_COUNT;
    
    for (int i = 0; i < step->count; i++)
    {
        int val = (int)step->head[i];
        struct ActrUIControlContainer * container = actr_ui_container(5 + i * (width), actrState->canvasSize.h - val - 5, width - 2, val);
        container->control.backgroundColor = bgcolor;
        container->control.foregroundColor = fgcolor;
        actr_vector_add(state->uiParts, container);
    }
}

[[clang::export_name("actr_resize")]]
void actr_resize()
{
    actr_ui_invalidate(); // required
}

[[clang::export_name("actr_pointer_tap")]]
void actr_pointer_tap(int x, int y)
{
    struct ActrUIControlButton * button = (struct ActrUIControlButton *)actr_ui_tap(x, y);
    if (button == state->restartHandle)
    {

        state->countDown = 1;
        state->index = 0;
        for (int i = 0; i < state->uiParts->count; i++)
        {
            actr_ui_remove_control(state->uiParts->head[i]);
        }
        state->uiParts->count = 0;

        for (int i = 0; i < state->updates->count; i++)
        {
            actr_vector_free(state->updates->head[i]);
        }
        state->updates->count = 0;

        state->scramble = actr_vector_init(8, 8);
        for (int i = 0; i < PART_COUNT; i++)
        {
            int val = actr_prng() * (actrState->canvasSize.h * 0.9f);
            // actr_vector_add(state->scramble, (void*)val);
            actr_vector_add(state->scramble, (void *)val);
        }
        // actr_vector_add(state->updates, actr_vector_slice(state->scramble, 0, 0));
        actr_merge_sort_mutate(state->scramble, 0, state->scramble->count - 1, 0, state->updates);
        actr_vector_add(state->updates, state->scramble);
        generateControls();
    }
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    actr_ui_draw(delta);
    if (state->countDown == -1)
    {
        return;
    }
    state->countDown--;
    if (state->countDown == 0)
    {

        if (state->index >= state->updates->count)
        {
            state->index = 0;
            state->countDown = 60;
            return;
        }
        else
        {
            state->countDown = 5;
        }

        generateControls();
        // actrdebug(999999);
        // actrdebug(step->count);
        state->index++;
    }
}
