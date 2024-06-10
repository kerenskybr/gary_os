#include  "idt.h"
#include 'config.h'

struct idt_desc descriptors[GARYOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptors;