#ifndef __DATA_PACKAGE_H__
#define __DATA_PACKAGE_H__

#define PACKAGE_SIZE 4096

struct DataPackage {
    int index;
    bool isEOF;
    int length;
    void *buffer[PACKAGE_SIZE];
};

#endif
