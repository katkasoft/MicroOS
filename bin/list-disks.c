#include <libudev.h>
#include <stdio.h>
#include <string.h>

int main() {
    struct udev *udev = udev_new();
    if (!udev) {
        perror("list-disks: error while getting udev");
        return 1;
    }
    struct udev_enumerate *enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "block");
    udev_enumerate_scan_devices(enumerate);
    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;
    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        struct udev_device *dev = udev_device_new_from_syspath(udev, path);
        const char *type = udev_device_get_devtype(dev);
        if (type != NULL && strcmp(type, "disk") == 0) {
            const char *node = udev_device_get_devnode(dev);
            if (node) {
                printf("%s\n", node);
            }
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    return 0;
}
