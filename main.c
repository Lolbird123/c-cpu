#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

byte mem[256];
byte pc;
byte acc;
byte flags;

void reset() {
  pc = 0;
  acc = 0;
  flags = 0;
}

byte fetch() {
  if(pc > 254) {
    pc = 0;
    return mem[255];
  } else {
    byte data = mem[pc];
    pc++;
    return data;
  }
}

void step() {
  byte inst = fetch();
  switch(inst) {
    case 0x01: { // ldi <data> load accumulator immediate
      acc = fetch();
    } break;

    case 0x02: { // lda <addr> load accumulator
      byte addr = fetch();
      acc = mem[addr];
    } break;

    case 0x03: { // sta <addr> store accumulator
      byte addr = fetch();
      mem[addr] = acc;
    } break;

    case 0x04: { // ldf load accumulator with flags
      acc = flags;
    } break;

    case 0x05: { // stf store accumulator to flags
      flags = acc;
    } break;

    case 0x10: { // cmp <data> compare accumulator with data (zero flag set for matches)
      byte data = fetch();
      if(data == acc) {
        flags |= 0x1;
      } else {
        flags &= ~0x1;
      }
    } break;

    case 0x11: { // adi <data> add data to accumulator immediate (affects zero flag)
      acc += fetch();
      if(acc == 0) {
        flags |= 0x1;
      } else {
        flags &= ~0x1;
      }
    } break;

    case 0x12: { // add <addr> add data to accumulator (affects zero flag)
      byte addr = fetch();
      acc += mem[addr];
      if(acc == 0) {
        flags |= 0x1;
      } else {
        flags &= ~0x1;
      }
    } break;

    case 0x13: { // sui <data> subtract data from accumulator immediate (affects zero flag)
      acc += fetch();
      if(acc == 0) {
        flags |= 0x1;
      } else {
        flags &= ~0x1;
      }
    } break;

    case 0x14: { // sub <addr> subtract data from accumulator (affects zero flag)
      byte addr = fetch();
      acc -= mem[addr];
      if(acc == 0) {
        flags |= 0x1;
      } else {
        flags &= ~0x1;
      }
    } break;

    case 0x20: { // ldp load accumulator with program counter
      acc = pc;
    } break;

    case 0x21: { // jmi <addr> unconditional immediate jump
      pc = fetch();
    } break;

    case 0x22: { // jmp <addr> unconditional jump
      byte addr = fetch();
      pc = mem[addr];
    } break;

    case 0x23: { // jiz <addr> immediate jump if zero flag set
      if(flags & 0x1) {
        pc = fetch();
      } else {
        fetch();
      }
    } break;

    case 0x24: { // jmz <addr> jump if zero flag set
      if(flags & 0x1) {
        byte addr = fetch();
        pc = mem[addr];
      } else {
        fetch();
      }
    } break;

    case 0x25: { // jin <addr> immediate jump if zero flag clear
      if(!(flags & 0x1)) {
        pc = fetch();
      } else {
        fetch();
      }
    } break;

    case 0x26: { // jmn <addr> jump if zero flag clear
      if(!(flags & 0x1)) {
        byte addr = fetch();
        pc = mem[addr];
      } else {
        fetch();
      }
    } break;
  }
}

int main() {
  while(1) {
    char input[256];
    printf("> ");
    fgets(input, 256, stdin);
    input[strlen(input)-1] = '\0';

    if(!strcmp(input, "stat") || !strcmp(input, "state") || !strcmp(input, "status")) {
      printf("pc    : 0x%02x  0b%08b\n", pc, pc);
      printf("acc   : 0x%02x  0b%08b\n", acc, acc);
      printf("flags : 0x%02x  0b%08b\n", flags, flags);
    } else if(!strcmp(input, "reset")) {
      reset();
    } else if(!strcmp(input, "read") || !strcmp(input, "peek")) {
      printf("addr> ");
      fgets(input, 4, stdin);
      input[strlen(input)-1] = '\0';

      unsigned long l = strtoul(input, 0, 16);
      byte* bytes = (byte*)&l;
      byte addr = bytes[0];

      printf("0x%02x  0b%08b\n", mem[addr], mem[addr]);
    } else if(!strcmp(input, "write") || !strcmp(input, "poke")) {
      printf("addr> ");
      fgets(input, 4, stdin);
      input[strlen(input)-1] = '\0';

      unsigned long l1 = strtoul(input, 0, 16);
      byte* bytes1 = (byte*)&l1;
      byte addr = bytes1[0];

      printf("data> ");
      fgets(input, 4, stdin);
      input[strlen(input)-1] = '\0';

      unsigned long l2 = strtoul(input, 0, 16);
      byte* bytes2 = (byte*)&l2;
      byte data = bytes2[0];

      mem[addr] = data; // totally safe
    } else if(!strcmp(input, "step")) {
      step();
    } else if(!strcmp(input, "exit") || !strcmp(input, "quit")) {
      break;
    } else if(!strcmp(input, "")) {
      //very important
    } else {
      printf("unknown command\n");
    }
  }

  return 0;
}
