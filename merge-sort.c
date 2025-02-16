

#include "actrformat.h"
#include "actrtime.h"
#include "actrui.h"
#include "actrprng.h"
#include "actrmergesort.h"
#define PART_COUNT 40

struct MyState
{
    struct ActrVector *updates;
    struct ActrVector *uiParts;
    struct ActrVector *scramble;
    int countDown;
    int restartHandle;
    int index;
};

struct MyState *state;

[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_ui_init();

    state = actr_malloc(sizeof(struct MyState));
    state->updates = actr_vector_init(8, 8);
    state->uiParts = actr_vector_init(8, 8);
    state->scramble = actr_vector_init(8, 8);
    state->restartHandle = actr_ui_button(20, 20, 100, 25, "Restart")->identity;
    state->countDown = -1;
    state->index = 0;
}

void actrdebug(int h);
[[clang::export_name("actr_pointer_tap")]]
void actr_pointer_tap(int x, int y)
{
    int id = actr_ui_tap(x, y);
    if (id == state->restartHandle)
    {
        
        state->countDown = 1;
        state->index = 0;
        for (int i = 0; i < state->uiParts->count; i++)
        {
            actr_ui_remove_control((int)state->uiParts->head[i]);
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
            int val = actr_prng() * 100;
            // actr_vector_add(state->scramble, (void*)val);
            actr_vector_add(state->scramble, (void*)val);
        }
        actr_vector_add(state->updates, state->scramble);
        actr_vector_add(state->updates, actr_merge_sort(actr_vector_slice(state->scramble, 0, 0), 0));
        
    }
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    actr_ui_draw(delta);
    if (state->countDown == -1) {
        return;
    }
    state->countDown--;
    if (state->countDown == 0) {
        if (state->index >= state->updates->count) {
            return;
            state->index = 0;
        }
        state->countDown = 1;
        for (int i = 0; i < state->uiParts->count; i++)
        {
            actr_ui_remove_control((int)state->uiParts->head[i]);
        }
        state->uiParts->count = 0;

        struct ActrVector *step = state->updates->head[state->index];
        int color = actr_pack_bytes(255,255,255,100);
        for (int i = 0; i < step->count; i++)
        {
            int val = (int)step->head[i];
            actrdebug(val);

            actr_vector_add(state->uiParts, (void*)actr_ui_container(10 + i * 20, 100, 15, val, color, color)->identity);
        }
        // actrdebug(999999);
        // actrdebug(step->count);
        state->index++;
    }
}
