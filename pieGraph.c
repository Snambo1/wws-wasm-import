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

void actr_strcpy(char *s,char *dst){
    while ((*dst++=*s++)!=0);
}
double convert(double degree){
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}
void pieGraphMaker(char *s, char * d){
    //actr_canvas2d_stroke_style(0,0,0,0);
    int idx=0,strt=0;
    int n=strlen(s),n2=strlen(d);
    int len=1;
    for (int i=0;i<n;i++)
        if (s[i]==',')
            len++;
    
    double * nums=(double*)actr_malloc(sizeof(double)*len);
    char ** names=(char**)actr_malloc(sizeof(char*)*len);
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
    strt=0;
    idx=0;
    for (int i=0;i<n2;i++){
        if (d[i]==','){
            names[idx]=(char*)actr_malloc(sizeof(char)*strlen(substr(d,strt,i-strt)));
            actr_strcpy(substr(d,strt,i-strt),names[idx++]);
            strt=i+1;
        }
    }
    names[idx]=(char*)actr_malloc(sizeof(char)*strlen(substr(d,strt,n2-strt)));
    actr_strcpy(substr(d,strt,n2-strt),names[idx++]);

    for (int i=0;i<len;i++)
        nums[i]=nums[i]/total;
    double prev=0;
    for (int i = 0; i < len; i++) {
        actr_canvas2d_begin_path();
        actr_canvas2d_moveto(XSTART, YSTART);
        actr_canvas2d_lineto(actr_cos(convert(prev + nums[i])) * DIST + XSTART, actr_sin(convert(prev + nums[i])) * DIST + YSTART);
        actr_canvas2d_stroke();
        actr_canvas2d_close_path();
        actr_canvas2d_fill_text(actr_cos(convert(prev + nums[i]/2*360))*(DIST+15)+XSTART,actr_sin(convert(prev + nums[i]/2*360))*(DIST+15)+YSTART,names[i]);
        prev += nums[i] * 360;
    }
    actr_canvas2d_begin_path();
    actr_canvas2d_arc(XSTART, YSTART, DIST, 0, 2*3.14159265359, 1);
    actr_canvas2d_stroke();
    actr_canvas2d_close_path();
    for (int i=0;i<len;i++)
        actr_free(names[i]);
    actr_free(names);
    actr_free(nums);
}
struct ActrUIControlText * percents;
struct ActrUIControlText * labels;
struct ActrUIControlButton * button;
char * nInput=0;
char * lInput=0;

[[clang::export_name("actr_init")]]
void actr_init(int w, int h)
{
    actr_ui_init(w,h); // required
    percents = actr_ui_text(100, 130, 500, 25, "");
    labels=actr_ui_text(100, 100, 500, 25, "");
    button = actr_ui_button(100, 200, 250, 25, "make");

}

[[clang::export_name("actr_pointer_tap")]]
void actr_pointer_tap(int x, int y)
{
    struct ActrUIControl *tapped = actr_ui_tap(x, y);

    if ((struct ActrUIControlButton *)tapped == button)
    {
        draw=1;
        if (nInput!=0){
            actr_free(nInput);
            actr_free(lInput);
        }
        nInput=(char*)actr_malloc(strlen(percents->value)*sizeof(char));
        lInput=(char*)actr_malloc(strlen(labels->value)*sizeof(char));
        actr_strcpy(percents->value,nInput);
        actr_strcpy(labels->value,lInput);
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
        pieGraphMaker(nInput,lInput);

}


