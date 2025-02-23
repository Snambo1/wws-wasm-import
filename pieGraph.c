#include "actrwasm.h"
#include "actrui.h"
#include "actrlog.h"
#include "actralloc.h"

#define DIST 100
#define XSTART 800
#define YSTART 200

long long parseInt(char *s){
    long long n=strlen(s);
    long long buffer=0;
    for (long long i=n-1,j=1;i>=0;i++,j*=10)
        buffer+=j*(s[i]-'0');
    return buffer;

}

int draw=0;
double parseFloat(char *s){
    long long n=strlen(s);
    double buffer=0;
    double idx=0;
    for (long long i=0;i<n;i++)
        if (s[i]=='.'){
            idx=i;
            break;
        }
    if (idx==0)
        idx=1.0;
    else
        idx=10.0*idx;
    for (long long i=0,j=idx;i<n;i++,j/=10.0)
        buffer+=(double)(s[i]-'0')*(double)j;
    return buffer;
}

double convert(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}
void pieGraphMaker(char *s){
    int idx=0,strt=0;
    int n=strlen(s);
    int len=1;
    for (int i=0;i<n;i++)
        if (s[i]==',')
            len++;
    
    double * nums=(double*)actr_malloc(sizeof(double)*len);
    double total=0;
    for (int i=0;i<n;i++){
        if (s[i]==','){
            double buf=parseFloat(substr(s,strt,i-strt));
            total+=buf;
            nums[idx++]=buf;
            strt=i+1;
        }
    }
    double buf=parseFloat(substr(s,strt,n-strt));
    total+=buf;
    nums[idx++]=buf;

    for (int i=0;i<len;i++)
        nums[i]=nums[i]/total;
    double prev=0;
    for (int i = 0; i < len; i++) {
        actr_canvas2d_begin_path();
        actr_canvas2d_moveto(XSTART, YSTART);
        actr_canvas2d_lineto(actr_cos(convert(prev + nums[i])) * DIST + XSTART, actr_sin(convert(prev + nums[i])) * DIST + YSTART);
        prev += nums[i] * 360;
        actr_canvas2d_stroke();
        actr_canvas2d_close_path();
    }
    prev=0;
    for (int i=0;i<len;i++){
        actr_canvas2d_begin_path();
        actr_canvas2d_arc(XSTART, YSTART, DIST, prev, prev + nums[i] * 360, 1);
        prev += nums[i] * 360;
        actr_canvas2d_stroke();
        actr_canvas2d_close_path();
    }
    actr_free(nums);
}
struct ActrUIControlText * textbox;
struct ActrUIControlButton * button;

[[clang::export_name("actr_init")]]
void actr_init(int w, int h)
{
    actr_ui_init(w,h); // required
    textbox = actr_ui_text(100, 100, 500, 25, "Hello World");
    button = actr_ui_button(100, 200, 250, 25, "My Button");

}

[[clang::export_name("actr_pointer_tap")]]
void actr_pointer_tap(int x, int y)
{
    struct ActrUIControl *tapped = actr_ui_tap(x, y);

    if ((struct ActrUIControlButton *)tapped == button)
    {
        draw=1;
        return;
    }
    
}

[[clang::export_name("actr_pointer_move")]]
void actr_pointer_move(int x, int y)
{
    actr_ui_move(x, y);
}

[[clang::export_name("actr_key_down")]]
void actr_key_down(int key)
{
    actr_ui_key_down(key);
}

[[clang::export_name("actr_resize")]]
void actr_resize()
{
    actr_ui_invalidate(); // required
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    actr_ui_draw(delta); // required
    if (draw)
        pieGraphMaker(textbox->value);

}


