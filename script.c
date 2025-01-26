// #include <string.h>

extern void _actr_fillStyle(const char* value, int length);
extern void _actr_fillRect(int x, int y, int w, int h);


int main() {
  _actr_fillStyle("white", 5);
  _actr_fillRect(10, 20, 30, 40);
}

