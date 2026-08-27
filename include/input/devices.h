#ifndef _GAS_INCLUDE_INPUT_DEVICES_H
#define _GAS_INCLUDE_INPUT_DEVICES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <libudev.h>


typedef struct devices_ctx {
    struct udev* udev;
    struct udev_monitor* monitor;
    int monitor_fd;

    int epfd;

} devices_ctx;

#endif
