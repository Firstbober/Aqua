#include "config.h"

#include <kernel/include/tty.h>
#include <kernel/include/keyboard.h>
#include <kernel/include/string.h>
#include <kernel/include/arg.h>
#include <kernel/include/cpu.h>

#include <drivers/ps2/ps2.h>
#include <drivers/cmos/cmos.h>
#include <drivers/pci/pci.h>

CPU_INFO* cpuinfo;

void kmain(void* lkc) {
	init_ps2();
	init_keyboard();
    init_pci();
    CPUInfo(cpuinfo);

    clear();
    color(12);
    printf("----------- CPU INFO ---------\n");
    CPUInfoPrint(cpuinfo);
    printf("----------- PCI INFO ---------\n");
    lspci();
}
