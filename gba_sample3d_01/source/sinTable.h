

#define TABLESIZE 256
unsigned int sinTable[TABLESIZE] = {
  0x00000001, 0x00000648, 0x00000c8f, 0x000012d5,
  0x00001917, 0x00001f56, 0x00002590, 0x00002bc4,
  0x000031f1, 0x00003817, 0x00003e33, 0x00004447,
  0x00004a50, 0x0000504d, 0x0000563e, 0x00005c22,
  0x000061f7, 0x000067bd, 0x00006d74, 0x00007319,
  0x000078ad, 0x00007e2e, 0x0000839c, 0x000088f5,
  0x00008e39, 0x00009368, 0x0000987f, 0x00009d7f,
  0x0000a267, 0x0000a736, 0x0000abeb, 0x0000b085,
  0x0000b504, 0x0000b968, 0x0000bdae, 0x0000c1d8,
  0x0000c5e4, 0x0000c9d1, 0x0000cd9f, 0x0000d14d,
  0x0000d4db, 0x0000d848, 0x0000db94, 0x0000debe,
  0x0000e1c5, 0x0000e4aa, 0x0000e76b, 0x0000ea09,
  0x0000ec83, 0x0000eed8, 0x0000f109, 0x0000f314,
  0x0000f4fa, 0x0000f6ba, 0x0000f853, 0x0000f9c7,
  0x0000fb14, 0x0000fc3b, 0x0000fd3a, 0x0000fe13,
  0x0000fec4, 0x0000ff4e, 0x0000ffb1, 0x0000ffec,
  0x00010000, 0x0000ffec, 0x0000ffb1, 0x0000ff4e,
  0x0000fec4, 0x0000fe13, 0x0000fd3a, 0x0000fc3b,
  0x0000fb14, 0x0000f9c7, 0x0000f853, 0x0000f6ba,
  0x0000f4fa, 0x0000f314, 0x0000f109, 0x0000eed8,
  0x0000ec83, 0x0000ea09, 0x0000e76b, 0x0000e4aa,
  0x0000e1c5, 0x0000debe, 0x0000db94, 0x0000d848,
  0x0000d4db, 0x0000d14d, 0x0000cd9f, 0x0000c9d1,
  0x0000c5e4, 0x0000c1d8, 0x0000bdae, 0x0000b968,
  0x0000b504, 0x0000b085, 0x0000abeb, 0x0000a736,
  0x0000a267, 0x00009d7f, 0x0000987f, 0x00009368,
  0x00008e39, 0x000088f5, 0x0000839c, 0x00007e2e,
  0x000078ad, 0x00007319, 0x00006d74, 0x000067bd,
  0x000061f7, 0x00005c22, 0x0000563e, 0x0000504d,
  0x00004a50, 0x00004447, 0x00003e33, 0x00003817,
  0x000031f1, 0x00002bc4, 0x00002590, 0x00001f56,
  0x00001917, 0x000012d5, 0x00000c8f, 0x00000648,
  0x00000000, 0xfffff9b8, 0xfffff371, 0xffffed2b,
  0xffffe6e9, 0xffffe0aa, 0xffffda70, 0xffffd43c,
  0xffffce0f, 0xffffc7e9, 0xffffc1cd, 0xffffbbb9,
  0xffffb5b0, 0xffffafb3, 0xffffa9c2, 0xffffa3de,
  0xffff9e09, 0xffff9843, 0xffff928c, 0xffff8ce7,
  0xffff8753, 0xffff81d2, 0xffff7c64, 0xffff770b,
  0xffff71c7, 0xffff6c98, 0xffff6781, 0xffff6281,
  0xffff5d99, 0xffff58ca, 0xffff5415, 0xffff4f7b,
  0xffff4afc, 0xffff4698, 0xffff4252, 0xffff3e28,
  0xffff3a1c, 0xffff362f, 0xffff3261, 0xffff2eb3,
  0xffff2b25, 0xffff27b8, 0xffff246c, 0xffff2142,
  0xffff1e3b, 0xffff1b56, 0xffff1895, 0xffff15f7,
  0xffff137d, 0xffff1128, 0xffff0ef7, 0xffff0cec,
  0xffff0b06, 0xffff0946, 0xffff07ad, 0xffff0639,
  0xffff04ec, 0xffff03c5, 0xffff02c6, 0xffff01ed,
  0xffff013c, 0xffff00b2, 0xffff004f, 0xffff0014,
  0xffff0000, 0xffff0014, 0xffff004f, 0xffff00b2,
  0xffff013c, 0xffff01ed, 0xffff02c6, 0xffff03c5,
  0xffff04ec, 0xffff0639, 0xffff07ad, 0xffff0946,
  0xffff0b06, 0xffff0cec, 0xffff0ef7, 0xffff1128,
  0xffff137d, 0xffff15f7, 0xffff1895, 0xffff1b56,
  0xffff1e3b, 0xffff2142, 0xffff246c, 0xffff27b8,
  0xffff2b25, 0xffff2eb3, 0xffff3261, 0xffff362f,
  0xffff3a1c, 0xffff3e28, 0xffff4252, 0xffff4698,
  0xffff4afc, 0xffff4f7b, 0xffff5415, 0xffff58ca,
  0xffff5d99, 0xffff6281, 0xffff6781, 0xffff6c98,
  0xffff71c7, 0xffff770b, 0xffff7c64, 0xffff81d2,
  0xffff8753, 0xffff8ce7, 0xffff928c, 0xffff9843,
  0xffff9e09, 0xffffa3de, 0xffffa9c2, 0xffffafb3,
  0xffffb5b0, 0xffffbbb9, 0xffffc1cd, 0xffffc7e9,
  0xffffce0f, 0xffffd43c, 0xffffda70, 0xffffe0aa,
  0xffffe6e9, 0xffffed2b, 0xfffff371, 0xfffff9b8,
};