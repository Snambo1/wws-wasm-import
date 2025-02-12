#ifndef ACTRASYNC_H
#define ACTRASYNC_H
enum AsyncResult {
    AsyncResultSuccess = 1,
    AsyncResultFailure = 0,
    AsyncResultAPIError = -1
};
void actr_async_result(int handle, enum AsyncResult success);
#endif
