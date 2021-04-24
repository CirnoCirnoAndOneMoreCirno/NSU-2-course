#include <iostream>
#include "libusb.h"
#include <stdio.h>
using namespace std;
void printdev(libusb_device* dev);


int main() {
    setlocale(LC_ALL, "Russian");
    libusb_device** devs; // указатель на указатель на устройство,
    // используется для получения списка устройств
    libusb_context* ctx = NULL; // контекст сессии libusb
    int r; // для возвращаемых значений
    ssize_t cnt; // число найденных USB-устройств
    ssize_t i; // индексная переменная цикла перебора всех устройств
    // инициализировать библиотеку libusb, открыть сессию работы с libusb
    r = libusb_init(&ctx);
    if (r < 0) {
         fprintf(stderr,
                 "Ошибка: инициализация не выполнена, код: %d.\n", r);
         return 1;
    }
    // задать уровень подробности отладочных сообщений
    libusb_set_debug(ctx, 3);
    // получить список всех найденных USB- устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
         fprintf(stderr,
                 "Ошибка: список USB устройств не получен.\n", r);
         return 1;
    }
    printf("найдено устройств: %d\n", cnt);
    printf("=============================="
          "=============================\n");
    printf("класс устройства\n");
    printf("| * идентификатор производителя\n");
    printf("| | * идентификатор устройства\n");
    printf("| | | * серийный номер\n");
    printf("+--+----+----+----\n");
    for (i = 0; i < cnt; i++) { // цикл перебора всех устройств
         printdev(devs[i]); // печать параметров устройства
    }
    printf("=============================="
          "=============================\n");
    // освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx); // завершить работу с библиотекой libusb,
    // закрыть сессию работы с libusb
    return 0;
}
void printdev(libusb_device* dev) {
    libusb_device_descriptor desc; // дескриптор устройства
    libusb_config_descriptor* config; // дескриптор конфигурации объекта
    libusb_device_handle* handle = NULL;
    unsigned char serialNum[256];

    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
         fprintf(stderr,
                 "Ошибка: дескриптор устройства не получен, код: %d.\n", r);
         return;
    }
    // получить конфигурацию устройства
    libusb_get_config_descriptor(dev, 0, &config);
    printf(" %.2x %.4x %.4x",
         (int)desc.bDeviceClass,
         desc.idVendor,
         desc.idProduct
    );

    libusb_open(dev, &handle);
    if (handle && desc.iSerialNumber)
    {
         r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serialNum, sizeof(serialNum));
         printf(" %s", serialNum);
    }
    else
         printf(" no serial number");
    printf("\n");
    if (handle)
          libusb_close(handle);
    libusb_free_config_descriptor(config);
}
