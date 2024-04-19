#include "sys_math.hpp"

#define SHT_MAX 32767.0f
#define SHT_MINV (1.0f / SHT_MAX)

static f32 qNaN0x10000 = 0.0f / 0.0f;

static s16 sintable[0x400] = {
    0x0000, 0x0032, 0x0064, 0x0096, 0x00C9, 0x00FB, 0x012D, 0x0160, 0x0192,
    0x01C4, 0x01F7, 0x0229, 0x025B, 0x028E, 0x02C0, 0x02F2, 0x0324, 0x0357,
    0x0389, 0x03BB, 0x03EE, 0x0420, 0x0452, 0x0484, 0x04B7, 0x04E9, 0x051B,
    0x054E, 0x0580, 0x05B2, 0x05E4, 0x0617, 0x0649, 0x067B, 0x06AD, 0x06E0,
    0x0712, 0x0744, 0x0776, 0x07A9, 0x07DB, 0x080D, 0x083F, 0x0871, 0x08A4,
    0x08D6, 0x0908, 0x093A, 0x096C, 0x099F, 0x09D1, 0x0A03, 0x0A35, 0x0A67,
    0x0A99, 0x0ACB, 0x0AFE, 0x0B30, 0x0B62, 0x0B94, 0x0BC6, 0x0BF8, 0x0C2A,
    0x0C5C, 0x0C8E, 0x0CC0, 0x0CF2, 0x0D25, 0x0D57, 0x0D89, 0x0DBB, 0x0DED,
    0x0E1F, 0x0E51, 0x0E83, 0x0EB5, 0x0EE7, 0x0F19, 0x0F4B, 0x0F7C, 0x0FAE,
    0x0FE0, 0x1012, 0x1044, 0x1076, 0x10A8, 0x10DA, 0x110C, 0x113E, 0x116F,
    0x11A1, 0x11D3, 0x1205, 0x1237, 0x1269, 0x129A, 0x12CC, 0x12FE, 0x1330,
    0x1361, 0x1393, 0x13C5, 0x13F6, 0x1428, 0x145A, 0x148C, 0x14BD, 0x14EF,
    0x1520, 0x1552, 0x1584, 0x15B5, 0x15E7, 0x1618, 0x164A, 0x167B, 0x16AD,
    0x16DF, 0x1710, 0x1741, 0x1773, 0x17A4, 0x17D6, 0x1807, 0x1839, 0x186A,
    0x189B, 0x18CD, 0x18FE, 0x1930, 0x1961, 0x1992, 0x19C3, 0x19F5, 0x1A26,
    0x1A57, 0x1A88, 0x1ABA, 0x1AEB, 0x1B1C, 0x1B4D, 0x1B7E, 0x1BAF, 0x1BE1,
    0x1C12, 0x1C43, 0x1C74, 0x1CA5, 0x1CD6, 0x1D07, 0x1D38, 0x1D69, 0x1D9A,
    0x1DCB, 0x1DFC, 0x1E2D, 0x1E5D, 0x1E8E, 0x1EBF, 0x1EF0, 0x1F21, 0x1F52,
    0x1F82, 0x1FB3, 0x1FE4, 0x2015, 0x2045, 0x2076, 0x20A7, 0x20D7, 0x2108,
    0x2139, 0x2169, 0x219A, 0x21CA, 0x21FB, 0x222B, 0x225C, 0x228C, 0x22BD,
    0x22ED, 0x231D, 0x234E, 0x237E, 0x23AE, 0x23DF, 0x240F, 0x243F, 0x2470,
    0x24A0, 0x24D0, 0x2500, 0x2530, 0x2560, 0x2591, 0x25C1, 0x25F1, 0x2621,
    0x2651, 0x2681, 0x26B1, 0x26E1, 0x2711, 0x2740, 0x2770, 0x27A0, 0x27D0,
    0x2800, 0x2830, 0x285F, 0x288F, 0x28BF, 0x28EE, 0x291E, 0x294E, 0x297D,
    0x29AD, 0x29DD, 0x2A0C, 0x2A3C, 0x2A6B, 0x2A9B, 0x2ACA, 0x2AF9, 0x2B29,
    0x2B58, 0x2B87, 0x2BB7, 0x2BE6, 0x2C15, 0x2C44, 0x2C74, 0x2CA3, 0x2CD2,
    0x2D01, 0x2D30, 0x2D5F, 0x2D8E, 0x2DBD, 0x2DEC, 0x2E1B, 0x2E4A, 0x2E79,
    0x2EA8, 0x2ED7, 0x2F06, 0x2F34, 0x2F63, 0x2F92, 0x2FC0, 0x2FEF, 0x301E,
    0x304C, 0x307B, 0x30A9, 0x30D8, 0x3107, 0x3135, 0x3163, 0x3192, 0x31C0,
    0x31EF, 0x321D, 0x324B, 0x3279, 0x32A8, 0x32D6, 0x3304, 0x3332, 0x3360,
    0x338E, 0x33BC, 0x33EA, 0x3418, 0x3446, 0x3474, 0x34A2, 0x34D0, 0x34FE,
    0x352B, 0x3559, 0x3587, 0x35B5, 0x35E2, 0x3610, 0x363D, 0x366B, 0x3698,
    0x36C6, 0x36F3, 0x3721, 0x374E, 0x377C, 0x37A9, 0x37D6, 0x3803, 0x3831,
    0x385E, 0x388B, 0x38B8, 0x38E5, 0x3912, 0x393F, 0x396C, 0x3999, 0x39C6,
    0x39F3, 0x3A20, 0x3A4D, 0x3A79, 0x3AA6, 0x3AD3, 0x3B00, 0x3B2C, 0x3B59,
    0x3B85, 0x3BB2, 0x3BDE, 0x3C0B, 0x3C37, 0x3C64, 0x3C90, 0x3CBC, 0x3CE9,
    0x3D15, 0x3D41, 0x3D6D, 0x3D99, 0x3DC5, 0x3DF1, 0x3E1D, 0x3E49, 0x3E75,
    0x3EA1, 0x3ECD, 0x3EF9, 0x3F25, 0x3F50, 0x3F7C, 0x3FA8, 0x3FD3, 0x3FFF,
    0x402B, 0x4056, 0x4082, 0x40AD, 0x40D8, 0x4104, 0x412F, 0x415A, 0x4186,
    0x41B1, 0x41DC, 0x4207, 0x4232, 0x425D, 0x4288, 0x42B3, 0x42DE, 0x4309,
    0x4334, 0x435F, 0x4389, 0x43B4, 0x43DF, 0x4409, 0x4434, 0x445F, 0x4489,
    0x44B4, 0x44DE, 0x4508, 0x4533, 0x455D, 0x4587, 0x45B1, 0x45DC, 0x4606,
    0x4630, 0x465A, 0x4684, 0x46AE, 0x46D8, 0x4702, 0x472C, 0x4755, 0x477F,
    0x47A9, 0x47D2, 0x47FC, 0x4826, 0x484F, 0x4879, 0x48A2, 0x48CC, 0x48F5,
    0x491E, 0x4948, 0x4971, 0x499A, 0x49C3, 0x49EC, 0x4A15, 0x4A3E, 0x4A67,
    0x4A90, 0x4AB9, 0x4AE2, 0x4B0B, 0x4B33, 0x4B5C, 0x4B85, 0x4BAD, 0x4BD6,
    0x4BFE, 0x4C27, 0x4C4F, 0x4C78, 0x4CA0, 0x4CC8, 0x4CF0, 0x4D19, 0x4D41,
    0x4D69, 0x4D91, 0x4DB9, 0x4DE1, 0x4E09, 0x4E31, 0x4E58, 0x4E80, 0x4EA8,
    0x4ED0, 0x4EF7, 0x4F1F, 0x4F46, 0x4F6E, 0x4F95, 0x4FBD, 0x4FE4, 0x500B,
    0x5032, 0x505A, 0x5081, 0x50A8, 0x50CF, 0x50F6, 0x511D, 0x5144, 0x516B,
    0x5191, 0x51B8, 0x51DF, 0x5205, 0x522C, 0x5253, 0x5279, 0x52A0, 0x52C6,
    0x52EC, 0x5313, 0x5339, 0x535F, 0x5385, 0x53AB, 0x53D1, 0x53F7, 0x541D,
    0x5443, 0x5469, 0x548F, 0x54B5, 0x54DA, 0x5500, 0x5525, 0x554B, 0x5571,
    0x5596, 0x55BB, 0x55E1, 0x5606, 0x562B, 0x5650, 0x5675, 0x569B, 0x56C0,
    0x56E5, 0x5709, 0x572E, 0x5753, 0x5778, 0x579D, 0x57C1, 0x57E6, 0x580A,
    0x582F, 0x5853, 0x5878, 0x589C, 0x58C0, 0x58E5, 0x5909, 0x592D, 0x5951,
    0x5975, 0x5999, 0x59BD, 0x59E1, 0x5A04, 0x5A28, 0x5A4C, 0x5A6F, 0x5A93,
    0x5AB7, 0x5ADA, 0x5AFD, 0x5B21, 0x5B44, 0x5B67, 0x5B8B, 0x5BAE, 0x5BD1,
    0x5BF4, 0x5C17, 0x5C3A, 0x5C5D, 0x5C7F, 0x5CA2, 0x5CC5, 0x5CE7, 0x5D0A,
    0x5D2D, 0x5D4F, 0x5D71, 0x5D94, 0x5DB6, 0x5DD8, 0x5DFA, 0x5E1D, 0x5E3F,
    0x5E61, 0x5E83, 0x5EA5, 0x5EC6, 0x5EE8, 0x5F0A, 0x5F2C, 0x5F4D, 0x5F6F,
    0x5F90, 0x5FB2, 0x5FD3, 0x5FF4, 0x6016, 0x6037, 0x6058, 0x6079, 0x609A,
    0x60BB, 0x60DC, 0x60FD, 0x611E, 0x613E, 0x615F, 0x6180, 0x61A0, 0x61C1,
    0x61E1, 0x6202, 0x6222, 0x6242, 0x6263, 0x6283, 0x62A3, 0x62C3, 0x62E3,
    0x6303, 0x6323, 0x6342, 0x6362, 0x6382, 0x63A1, 0x63C1, 0x63E0, 0x6400,
    0x641F, 0x643F, 0x645E, 0x647D, 0x649C, 0x64BB, 0x64DA, 0x64F9, 0x6518,
    0x6537, 0x6556, 0x6574, 0x6593, 0x65B2, 0x65D0, 0x65EF, 0x660D, 0x662B,
    0x664A, 0x6668, 0x6686, 0x66A4, 0x66C2, 0x66E0, 0x66FE, 0x671C, 0x673A,
    0x6757, 0x6775, 0x6792, 0x67B0, 0x67CD, 0x67EB, 0x6808, 0x6825, 0x6843,
    0x6860, 0x687D, 0x689A, 0x68B7, 0x68D4, 0x68F1, 0x690D, 0x692A, 0x6947,
    0x6963, 0x6980, 0x699C, 0x69B9, 0x69D5, 0x69F1, 0x6A0E, 0x6A2A, 0x6A46,
    0x6A62, 0x6A7E, 0x6A9A, 0x6AB5, 0x6AD1, 0x6AED, 0x6B08, 0x6B24, 0x6B40,
    0x6B5B, 0x6B76, 0x6B92, 0x6BAD, 0x6BC8, 0x6BE3, 0x6BFE, 0x6C19, 0x6C34,
    0x6C4F, 0x6C6A, 0x6C84, 0x6C9F, 0x6CBA, 0x6CD4, 0x6CEF, 0x6D09, 0x6D23,
    0x6D3E, 0x6D58, 0x6D72, 0x6D8C, 0x6DA6, 0x6DC0, 0x6DDA, 0x6DF3, 0x6E0D,
    0x6E27, 0x6E40, 0x6E5A, 0x6E73, 0x6E8D, 0x6EA6, 0x6EBF, 0x6ED9, 0x6EF2,
    0x6F0B, 0x6F24, 0x6F3D, 0x6F55, 0x6F6E, 0x6F87, 0x6FA0, 0x6FB8, 0x6FD1,
    0x6FE9, 0x7002, 0x701A, 0x7032, 0x704A, 0x7062, 0x707A, 0x7092, 0x70AA,
    0x70C2, 0x70DA, 0x70F2, 0x7109, 0x7121, 0x7138, 0x7150, 0x7167, 0x717E,
    0x7196, 0x71AD, 0x71C4, 0x71DB, 0x71F2, 0x7209, 0x7220, 0x7236, 0x724D,
    0x7264, 0x727A, 0x7291, 0x72A7, 0x72BD, 0x72D4, 0x72EA, 0x7300, 0x7316,
    0x732C, 0x7342, 0x7358, 0x736E, 0x7383, 0x7399, 0x73AE, 0x73C4, 0x73D9,
    0x73EF, 0x7404, 0x7419, 0x742E, 0x7443, 0x7458, 0x746D, 0x7482, 0x7497,
    0x74AC, 0x74C0, 0x74D5, 0x74EA, 0x74FE, 0x7512, 0x7527, 0x753B, 0x754F,
    0x7563, 0x7577, 0x758B, 0x759F, 0x75B3, 0x75C7, 0x75DA, 0x75EE, 0x7601,
    0x7615, 0x7628, 0x763B, 0x764F, 0x7662, 0x7675, 0x7688, 0x769B, 0x76AE,
    0x76C1, 0x76D3, 0x76E6, 0x76F9, 0x770B, 0x771E, 0x7730, 0x7742, 0x7754,
    0x7767, 0x7779, 0x778B, 0x779D, 0x77AF, 0x77C0, 0x77D2, 0x77E4, 0x77F5,
    0x7807, 0x7818, 0x782A, 0x783B, 0x784C, 0x785D, 0x786E, 0x787F, 0x7890,
    0x78A1, 0x78B2, 0x78C3, 0x78D3, 0x78E4, 0x78F4, 0x7905, 0x7915, 0x7925,
    0x7936, 0x7946, 0x7956, 0x7966, 0x7976, 0x7985, 0x7995, 0x79A5, 0x79B5,
    0x79C4, 0x79D4, 0x79E3, 0x79F2, 0x7A02, 0x7A11, 0x7A20, 0x7A2F, 0x7A3E,
    0x7A4D, 0x7A5B, 0x7A6A, 0x7A79, 0x7A87, 0x7A96, 0x7AA4, 0x7AB3, 0x7AC1,
    0x7ACF, 0x7ADD, 0x7AEB, 0x7AF9, 0x7B07, 0x7B15, 0x7B23, 0x7B31, 0x7B3E,
    0x7B4C, 0x7B59, 0x7B67, 0x7B74, 0x7B81, 0x7B8E, 0x7B9B, 0x7BA8, 0x7BB5,
    0x7BC2, 0x7BCF, 0x7BDC, 0x7BE8, 0x7BF5, 0x7C02, 0x7C0E, 0x7C1A, 0x7C27,
    0x7C33, 0x7C3F, 0x7C4B, 0x7C57, 0x7C63, 0x7C6F, 0x7C7A, 0x7C86, 0x7C92,
    0x7C9D, 0x7CA9, 0x7CB4, 0x7CBF, 0x7CCB, 0x7CD6, 0x7CE1, 0x7CEC, 0x7CF7,
    0x7D02, 0x7D0C, 0x7D17, 0x7D22, 0x7D2C, 0x7D37, 0x7D41, 0x7D4B, 0x7D56,
    0x7D60, 0x7D6A, 0x7D74, 0x7D7E, 0x7D88, 0x7D91, 0x7D9B, 0x7DA5, 0x7DAE,
    0x7DB8, 0x7DC1, 0x7DCB, 0x7DD4, 0x7DDD, 0x7DE6, 0x7DEF, 0x7DF8, 0x7E01,
    0x7E0A, 0x7E13, 0x7E1B, 0x7E24, 0x7E2C, 0x7E35, 0x7E3D, 0x7E45, 0x7E4D,
    0x7E56, 0x7E5E, 0x7E66, 0x7E6D, 0x7E75, 0x7E7D, 0x7E85, 0x7E8C, 0x7E94,
    0x7E9B, 0x7EA3, 0x7EAA, 0x7EB1, 0x7EB8, 0x7EBF, 0x7EC6, 0x7ECD, 0x7ED4,
    0x7EDB, 0x7EE1, 0x7EE8, 0x7EEE, 0x7EF5, 0x7EFB, 0x7F01, 0x7F08, 0x7F0E,
    0x7F14, 0x7F1A, 0x7F20, 0x7F25, 0x7F2B, 0x7F31, 0x7F36, 0x7F3C, 0x7F41,
    0x7F47, 0x7F4C, 0x7F51, 0x7F56, 0x7F5B, 0x7F60, 0x7F65, 0x7F6A, 0x7F6F,
    0x7F74, 0x7F78, 0x7F7D, 0x7F81, 0x7F85, 0x7F8A, 0x7F8E, 0x7F92, 0x7F96,
    0x7F9A, 0x7F9E, 0x7FA2, 0x7FA6, 0x7FA9, 0x7FAD, 0x7FB0, 0x7FB4, 0x7FB7,
    0x7FBA, 0x7FBE, 0x7FC1, 0x7FC4, 0x7FC7, 0x7FCA, 0x7FCC, 0x7FCF, 0x7FD2,
    0x7FD4, 0x7FD7, 0x7FD9, 0x7FDC, 0x7FDE, 0x7FE0, 0x7FE2, 0x7FE4, 0x7FE6,
    0x7FE8, 0x7FEA, 0x7FEC, 0x7FED, 0x7FEF, 0x7FF1, 0x7FF2, 0x7FF3, 0x7FF5,
    0x7FF6, 0x7FF7, 0x7FF8, 0x7FF9, 0x7FFA, 0x7FFB, 0x7FFB, 0x7FFC, 0x7FFD,
    0x7FFD, 0x7FFE, 0x7FFE, 0x7FFE, 0x7FFE, 0x7FFE, 0x7FFF,
};

s16 sins(u16 angle) {
  s16 value;

  angle >>= 4;

  if (angle & 0x400) {
    value = sintable[0x3FF - (angle & 0x3FF)];
  } else {
    value = sintable[angle & 0x3FF];
  }

  if (angle & 0x800) {
    return -value;
  } else {
    return value;
  }
}

s16 coss(u16 angle) { return sins(angle + 0x4000); }

f32 Math_CosS(s16 angle) { return coss(angle) * SHT_MINV; }

f32 Math_SinS(s16 angle) { return sins(angle) * SHT_MINV; }

static u16 sAtan2Tbl[] = {
    0x0000, 0x000A, 0x0014, 0x001F, 0x0029, 0x0033, 0x003D, 0x0047, 0x0051,
    0x005C, 0x0066, 0x0070, 0x007A, 0x0084, 0x008F, 0x0099, 0x00A3, 0x00AD,
    0x00B7, 0x00C2, 0x00CC, 0x00D6, 0x00E0, 0x00EA, 0x00F4, 0x00FF, 0x0109,
    0x0113, 0x011D, 0x0127, 0x0131, 0x013C, 0x0146, 0x0150, 0x015A, 0x0164,
    0x016F, 0x0179, 0x0183, 0x018D, 0x0197, 0x01A1, 0x01AC, 0x01B6, 0x01C0,
    0x01CA, 0x01D4, 0x01DE, 0x01E9, 0x01F3, 0x01FD, 0x0207, 0x0211, 0x021B,
    0x0226, 0x0230, 0x023A, 0x0244, 0x024E, 0x0258, 0x0262, 0x026D, 0x0277,
    0x0281, 0x028B, 0x0295, 0x029F, 0x02A9, 0x02B4, 0x02BE, 0x02C8, 0x02D2,
    0x02DC, 0x02E6, 0x02F0, 0x02FB, 0x0305, 0x030F, 0x0319, 0x0323, 0x032D,
    0x0337, 0x0341, 0x034C, 0x0356, 0x0360, 0x036A, 0x0374, 0x037E, 0x0388,
    0x0392, 0x039C, 0x03A7, 0x03B1, 0x03BB, 0x03C5, 0x03CF, 0x03D9, 0x03E3,
    0x03ED, 0x03F7, 0x0401, 0x040C, 0x0416, 0x0420, 0x042A, 0x0434, 0x043E,
    0x0448, 0x0452, 0x045C, 0x0466, 0x0470, 0x047A, 0x0484, 0x048E, 0x0499,
    0x04A3, 0x04AD, 0x04B7, 0x04C1, 0x04CB, 0x04D5, 0x04DF, 0x04E9, 0x04F3,
    0x04FD, 0x0507, 0x0511, 0x051B, 0x0525, 0x052F, 0x0539, 0x0543, 0x054D,
    0x0557, 0x0561, 0x056B, 0x0575, 0x057F, 0x0589, 0x0593, 0x059D, 0x05A7,
    0x05B1, 0x05BB, 0x05C5, 0x05CF, 0x05D9, 0x05E3, 0x05ED, 0x05F7, 0x0601,
    0x060B, 0x0615, 0x061F, 0x0629, 0x0633, 0x063D, 0x0647, 0x0651, 0x065B,
    0x0665, 0x066E, 0x0678, 0x0682, 0x068C, 0x0696, 0x06A0, 0x06AA, 0x06B4,
    0x06BE, 0x06C8, 0x06D2, 0x06DC, 0x06E5, 0x06EF, 0x06F9, 0x0703, 0x070D,
    0x0717, 0x0721, 0x072B, 0x0735, 0x073E, 0x0748, 0x0752, 0x075C, 0x0766,
    0x0770, 0x077A, 0x0783, 0x078D, 0x0797, 0x07A1, 0x07AB, 0x07B5, 0x07BE,
    0x07C8, 0x07D2, 0x07DC, 0x07E6, 0x07EF, 0x07F9, 0x0803, 0x080D, 0x0817,
    0x0820, 0x082A, 0x0834, 0x083E, 0x0848, 0x0851, 0x085B, 0x0865, 0x086F,
    0x0878, 0x0882, 0x088C, 0x0896, 0x089F, 0x08A9, 0x08B3, 0x08BD, 0x08C6,
    0x08D0, 0x08DA, 0x08E3, 0x08ED, 0x08F7, 0x0901, 0x090A, 0x0914, 0x091E,
    0x0927, 0x0931, 0x093B, 0x0944, 0x094E, 0x0958, 0x0961, 0x096B, 0x0975,
    0x097E, 0x0988, 0x0992, 0x099B, 0x09A5, 0x09AE, 0x09B8, 0x09C2, 0x09CB,
    0x09D5, 0x09DE, 0x09E8, 0x09F2, 0x09FB, 0x0A05, 0x0A0E, 0x0A18, 0x0A22,
    0x0A2B, 0x0A35, 0x0A3E, 0x0A48, 0x0A51, 0x0A5B, 0x0A64, 0x0A6E, 0x0A77,
    0x0A81, 0x0A8B, 0x0A94, 0x0A9E, 0x0AA7, 0x0AB1, 0x0ABA, 0x0AC4, 0x0ACD,
    0x0AD7, 0x0AE0, 0x0AE9, 0x0AF3, 0x0AFC, 0x0B06, 0x0B0F, 0x0B19, 0x0B22,
    0x0B2C, 0x0B35, 0x0B3F, 0x0B48, 0x0B51, 0x0B5B, 0x0B64, 0x0B6E, 0x0B77,
    0x0B80, 0x0B8A, 0x0B93, 0x0B9D, 0x0BA6, 0x0BAF, 0x0BB9, 0x0BC2, 0x0BCB,
    0x0BD5, 0x0BDE, 0x0BE7, 0x0BF1, 0x0BFA, 0x0C03, 0x0C0D, 0x0C16, 0x0C1F,
    0x0C29, 0x0C32, 0x0C3B, 0x0C45, 0x0C4E, 0x0C57, 0x0C60, 0x0C6A, 0x0C73,
    0x0C7C, 0x0C86, 0x0C8F, 0x0C98, 0x0CA1, 0x0CAB, 0x0CB4, 0x0CBD, 0x0CC6,
    0x0CCF, 0x0CD9, 0x0CE2, 0x0CEB, 0x0CF4, 0x0CFD, 0x0D07, 0x0D10, 0x0D19,
    0x0D22, 0x0D2B, 0x0D34, 0x0D3E, 0x0D47, 0x0D50, 0x0D59, 0x0D62, 0x0D6B,
    0x0D74, 0x0D7D, 0x0D87, 0x0D90, 0x0D99, 0x0DA2, 0x0DAB, 0x0DB4, 0x0DBD,
    0x0DC6, 0x0DCF, 0x0DD8, 0x0DE1, 0x0DEA, 0x0DF3, 0x0DFC, 0x0E05, 0x0E0F,
    0x0E18, 0x0E21, 0x0E2A, 0x0E33, 0x0E3C, 0x0E45, 0x0E4E, 0x0E56, 0x0E5F,
    0x0E68, 0x0E71, 0x0E7A, 0x0E83, 0x0E8C, 0x0E95, 0x0E9E, 0x0EA7, 0x0EB0,
    0x0EB9, 0x0EC2, 0x0ECB, 0x0ED4, 0x0EDC, 0x0EE5, 0x0EEE, 0x0EF7, 0x0F00,
    0x0F09, 0x0F12, 0x0F1B, 0x0F23, 0x0F2C, 0x0F35, 0x0F3E, 0x0F47, 0x0F50,
    0x0F58, 0x0F61, 0x0F6A, 0x0F73, 0x0F7C, 0x0F84, 0x0F8D, 0x0F96, 0x0F9F,
    0x0FA7, 0x0FB0, 0x0FB9, 0x0FC2, 0x0FCA, 0x0FD3, 0x0FDC, 0x0FE5, 0x0FED,
    0x0FF6, 0x0FFF, 0x1007, 0x1010, 0x1019, 0x1021, 0x102A, 0x1033, 0x103B,
    0x1044, 0x104D, 0x1055, 0x105E, 0x1067, 0x106F, 0x1078, 0x1080, 0x1089,
    0x1092, 0x109A, 0x10A3, 0x10AB, 0x10B4, 0x10BC, 0x10C5, 0x10CE, 0x10D6,
    0x10DF, 0x10E7, 0x10F0, 0x10F8, 0x1101, 0x1109, 0x1112, 0x111A, 0x1123,
    0x112B, 0x1134, 0x113C, 0x1145, 0x114D, 0x1156, 0x115E, 0x1166, 0x116F,
    0x1177, 0x1180, 0x1188, 0x1191, 0x1199, 0x11A1, 0x11AA, 0x11B2, 0x11BB,
    0x11C3, 0x11CB, 0x11D4, 0x11DC, 0x11E4, 0x11ED, 0x11F5, 0x11FD, 0x1206,
    0x120E, 0x1216, 0x121F, 0x1227, 0x122F, 0x1237, 0x1240, 0x1248, 0x1250,
    0x1259, 0x1261, 0x1269, 0x1271, 0x127A, 0x1282, 0x128A, 0x1292, 0x129A,
    0x12A3, 0x12AB, 0x12B3, 0x12BB, 0x12C3, 0x12CC, 0x12D4, 0x12DC, 0x12E4,
    0x12EC, 0x12F4, 0x12FC, 0x1305, 0x130D, 0x1315, 0x131D, 0x1325, 0x132D,
    0x1335, 0x133D, 0x1345, 0x134D, 0x1355, 0x135E, 0x1366, 0x136E, 0x1376,
    0x137E, 0x1386, 0x138E, 0x1396, 0x139E, 0x13A6, 0x13AE, 0x13B6, 0x13BE,
    0x13C6, 0x13CE, 0x13D6, 0x13DE, 0x13E6, 0x13ED, 0x13F5, 0x13FD, 0x1405,
    0x140D, 0x1415, 0x141D, 0x1425, 0x142D, 0x1435, 0x143D, 0x1444, 0x144C,
    0x1454, 0x145C, 0x1464, 0x146C, 0x1473, 0x147B, 0x1483, 0x148B, 0x1493,
    0x149B, 0x14A2, 0x14AA, 0x14B2, 0x14BA, 0x14C1, 0x14C9, 0x14D1, 0x14D9,
    0x14E0, 0x14E8, 0x14F0, 0x14F8, 0x14FF, 0x1507, 0x150F, 0x1516, 0x151E,
    0x1526, 0x152D, 0x1535, 0x153D, 0x1544, 0x154C, 0x1554, 0x155B, 0x1563,
    0x156B, 0x1572, 0x157A, 0x1581, 0x1589, 0x1591, 0x1598, 0x15A0, 0x15A7,
    0x15AF, 0x15B7, 0x15BE, 0x15C6, 0x15CD, 0x15D5, 0x15DC, 0x15E4, 0x15EB,
    0x15F3, 0x15FA, 0x1602, 0x1609, 0x1611, 0x1618, 0x1620, 0x1627, 0x162F,
    0x1636, 0x163E, 0x1645, 0x164C, 0x1654, 0x165B, 0x1663, 0x166A, 0x1671,
    0x1679, 0x1680, 0x1688, 0x168F, 0x1696, 0x169E, 0x16A5, 0x16AC, 0x16B4,
    0x16BB, 0x16C2, 0x16CA, 0x16D1, 0x16D8, 0x16E0, 0x16E7, 0x16EE, 0x16F6,
    0x16FD, 0x1704, 0x170B, 0x1713, 0x171A, 0x1721, 0x1728, 0x1730, 0x1737,
    0x173E, 0x1745, 0x174C, 0x1754, 0x175B, 0x1762, 0x1769, 0x1770, 0x1778,
    0x177F, 0x1786, 0x178D, 0x1794, 0x179B, 0x17A2, 0x17AA, 0x17B1, 0x17B8,
    0x17BF, 0x17C6, 0x17CD, 0x17D4, 0x17DB, 0x17E2, 0x17E9, 0x17F0, 0x17F7,
    0x17FE, 0x1806, 0x180D, 0x1814, 0x181B, 0x1822, 0x1829, 0x1830, 0x1837,
    0x183E, 0x1845, 0x184C, 0x1853, 0x185A, 0x1860, 0x1867, 0x186E, 0x1875,
    0x187C, 0x1883, 0x188A, 0x1891, 0x1898, 0x189F, 0x18A6, 0x18AD, 0x18B3,
    0x18BA, 0x18C1, 0x18C8, 0x18CF, 0x18D6, 0x18DD, 0x18E3, 0x18EA, 0x18F1,
    0x18F8, 0x18FF, 0x1906, 0x190C, 0x1913, 0x191A, 0x1921, 0x1928, 0x192E,
    0x1935, 0x193C, 0x1943, 0x1949, 0x1950, 0x1957, 0x195D, 0x1964, 0x196B,
    0x1972, 0x1978, 0x197F, 0x1986, 0x198C, 0x1993, 0x199A, 0x19A0, 0x19A7,
    0x19AE, 0x19B4, 0x19BB, 0x19C2, 0x19C8, 0x19CF, 0x19D5, 0x19DC, 0x19E3,
    0x19E9, 0x19F0, 0x19F6, 0x19FD, 0x1A04, 0x1A0A, 0x1A11, 0x1A17, 0x1A1E,
    0x1A24, 0x1A2B, 0x1A31, 0x1A38, 0x1A3E, 0x1A45, 0x1A4B, 0x1A52, 0x1A58,
    0x1A5F, 0x1A65, 0x1A6C, 0x1A72, 0x1A79, 0x1A7F, 0x1A86, 0x1A8C, 0x1A93,
    0x1A99, 0x1A9F, 0x1AA6, 0x1AAC, 0x1AB3, 0x1AB9, 0x1AC0, 0x1AC6, 0x1ACC,
    0x1AD3, 0x1AD9, 0x1ADF, 0x1AE6, 0x1AEC, 0x1AF2, 0x1AF9, 0x1AFF, 0x1B05,
    0x1B0C, 0x1B12, 0x1B18, 0x1B1F, 0x1B25, 0x1B2B, 0x1B32, 0x1B38, 0x1B3E,
    0x1B44, 0x1B4B, 0x1B51, 0x1B57, 0x1B5D, 0x1B64, 0x1B6A, 0x1B70, 0x1B76,
    0x1B7D, 0x1B83, 0x1B89, 0x1B8F, 0x1B95, 0x1B9C, 0x1BA2, 0x1BA8, 0x1BAE,
    0x1BB4, 0x1BBA, 0x1BC1, 0x1BC7, 0x1BCD, 0x1BD3, 0x1BD9, 0x1BDF, 0x1BE5,
    0x1BEB, 0x1BF2, 0x1BF8, 0x1BFE, 0x1C04, 0x1C0A, 0x1C10, 0x1C16, 0x1C1C,
    0x1C22, 0x1C28, 0x1C2E, 0x1C34, 0x1C3A, 0x1C40, 0x1C46, 0x1C4C, 0x1C52,
    0x1C58, 0x1C5E, 0x1C64, 0x1C6A, 0x1C70, 0x1C76, 0x1C7C, 0x1C82, 0x1C88,
    0x1C8E, 0x1C94, 0x1C9A, 0x1CA0, 0x1CA6, 0x1CAC, 0x1CB2, 0x1CB8, 0x1CBE,
    0x1CC3, 0x1CC9, 0x1CCF, 0x1CD5, 0x1CDB, 0x1CE1, 0x1CE7, 0x1CED, 0x1CF3,
    0x1CF8, 0x1CFE, 0x1D04, 0x1D0A, 0x1D10, 0x1D16, 0x1D1B, 0x1D21, 0x1D27,
    0x1D2D, 0x1D33, 0x1D38, 0x1D3E, 0x1D44, 0x1D4A, 0x1D4F, 0x1D55, 0x1D5B,
    0x1D61, 0x1D66, 0x1D6C, 0x1D72, 0x1D78, 0x1D7D, 0x1D83, 0x1D89, 0x1D8E,
    0x1D94, 0x1D9A, 0x1DA0, 0x1DA5, 0x1DAB, 0x1DB1, 0x1DB6, 0x1DBC, 0x1DC2,
    0x1DC7, 0x1DCD, 0x1DD3, 0x1DD8, 0x1DDE, 0x1DE3, 0x1DE9, 0x1DEF, 0x1DF4,
    0x1DFA, 0x1DFF, 0x1E05, 0x1E0B, 0x1E10, 0x1E16, 0x1E1B, 0x1E21, 0x1E26,
    0x1E2C, 0x1E32, 0x1E37, 0x1E3D, 0x1E42, 0x1E48, 0x1E4D, 0x1E53, 0x1E58,
    0x1E5E, 0x1E63, 0x1E69, 0x1E6E, 0x1E74, 0x1E79, 0x1E7F, 0x1E84, 0x1E8A,
    0x1E8F, 0x1E94, 0x1E9A, 0x1E9F, 0x1EA5, 0x1EAA, 0x1EB0, 0x1EB5, 0x1EBA,
    0x1EC0, 0x1EC5, 0x1ECB, 0x1ED0, 0x1ED5, 0x1EDB, 0x1EE0, 0x1EE6, 0x1EEB,
    0x1EF0, 0x1EF6, 0x1EFB, 0x1F00, 0x1F06, 0x1F0B, 0x1F10, 0x1F16, 0x1F1B,
    0x1F20, 0x1F26, 0x1F2B, 0x1F30, 0x1F36, 0x1F3B, 0x1F40, 0x1F45, 0x1F4B,
    0x1F50, 0x1F55, 0x1F5A, 0x1F60, 0x1F65, 0x1F6A, 0x1F6F, 0x1F75, 0x1F7A,
    0x1F7F, 0x1F84, 0x1F8A, 0x1F8F, 0x1F94, 0x1F99, 0x1F9E, 0x1FA4, 0x1FA9,
    0x1FAE, 0x1FB3, 0x1FB8, 0x1FBD, 0x1FC3, 0x1FC8, 0x1FCD, 0x1FD2, 0x1FD7,
    0x1FDC, 0x1FE1, 0x1FE6, 0x1FEC, 0x1FF1, 0x1FF6, 0x1FFB, 0x2000,
};

u16 Math_GetAtan2Tbl(f32 y, f32 x) {
  u16 ret;

  if (x == 0.0f) {
    ret = sAtan2Tbl[0];
  } else {
    s32 tblIdx = ((y / x) * 1024.0f) + 0.5f;

    if (tblIdx >= (sizeof(sAtan2Tbl) / sizeof(sAtan2Tbl[0]))) {
      ret = sAtan2Tbl[0];
    } else {
      ret = sAtan2Tbl[tblIdx];
    }
  }

  return ret;
}

u16 Math_Atan2S(f32 x, f32 y) {
  s32 ret;

  if (y >= 0.0f) {
    if (x >= 0.0f) {
      if (y <= x) {
        ret = Math_GetAtan2Tbl(y, x);
      } else {
        ret = 0x4000 - Math_GetAtan2Tbl(x, y);
      }
    } else {
      if (-x < y) {
        ret = 0x4000 + Math_GetAtan2Tbl(-x, y);
      } else {
        ret = 0x8000 - Math_GetAtan2Tbl(y, -x);
      }
    }
  } else {
    if (x < 0.0f) {
      if (-y <= -x) {
        ret = 0x8000 + Math_GetAtan2Tbl(-y, -x);
      } else {
        ret = 0xC000 - Math_GetAtan2Tbl(-x, -y);
      }
    } else {
      if (x < -y) {
        ret = 0xC000 + Math_GetAtan2Tbl(x, -y);
      } else {
        ret = -Math_GetAtan2Tbl(-y, x);
      }
    }
  }

  return (u16)ret;
}

s32 gUseAtanContFrac;

/**
 * @param angle radians
 * @return tan(angle)
 */
f32 Math_FTanF(f32 angle) {
  f32 sin = sinf(angle);
  f32 cos = cosf(angle);

  return sin / cos;
}

f32 Math_FFloorF(f32 x) { return floorf(x); }

f32 Math_FCeilF(f32 x) { return ceilf(x); }

f32 Math_FRoundF(f32 x) { return roundf(x); }

f32 Math_FTruncF(f32 x) { return truncf(x); }

f32 Math_FNearbyIntF(f32 x) { return nearbyintf(x); }

/* Arctangent approximation using a Taylor series (one quadrant) */
f32 Math_FAtanTaylorQF(f32 x) {
  static const f32 coeffs[] = {
      -1.0f / 3,  +1.0f / 5,  -1.0f / 7,  +1.0f / 9, -1.0f / 11,
      +1.0f / 13, -1.0f / 15, +1.0f / 17, 0.0f,
  };

  f32 poly = x;
  f32 sq = SQ(x);
  f32 exp = x * sq;
  const f32 *c = coeffs;
  f32 term;

  while (true) {
    term = *c++ * exp;
    if (poly + term == poly) {
      break;
    }
    poly = poly + term;
    exp = exp * sq;
  }

  return poly;
}

/* Ditto for two quadrants */
f32 Math_FAtanTaylorF(f32 x) {
  f32 t;
  f32 q;

  if (x > 0.0f) {
    t = x;
  } else if (x < 0.0f) {
    t = -x;
  } else if (x == 0.0f) {
    return 0.0f;
  } else {
    return qNaN0x10000;
  }

  if (t <= M_SQRT2 - 1.0f) {
    return Math_FAtanTaylorQF(x);
  }

  if (t >= M_SQRT2 + 1.0f) {
    q = M_PI / 2 - Math_FAtanTaylorQF(1.0f / t);
  } else {
    q = M_PI / 4 - Math_FAtanTaylorQF((1.0f - t) / (1.0f + t));
  }

  if (x > 0.0f) {
    return q;
  } else {
    return -q;
  }
}

/* Arctangent approximation using a continued fraction */
f32 Math_FAtanContFracF(f32 x) {
  s32 sector;
  f32 z;
  f32 conv;
  f32 sq;
  s32 i;

  if (x >= -1.0f && x <= 1.0f) {
    sector = 0;
  } else if (x > 1.0f) {
    sector = 1;
    x = 1.0f / x;
  } else if (x < -1.0f) {
    sector = -1;
    x = 1.0f / x;
  } else {
    return qNaN0x10000;
  }

  sq = SQ(x);
  conv = 0.0f;
  z = 8.0f;
  for (i = 8; i != 0; i--) {
    conv = SQ(z) * sq / (2.0f * z + 1.0f + conv);
    z -= 1.0f;
  }
  conv = x / (1.0f + conv);

  if (sector == 0) {
    return conv;
  } else if (sector > 0) {
    return M_PI / 2 - conv;
  } else {
    return -M_PI / 2 - conv;
  }
}

/**
 * @return arctan(x) in radians, in (-pi/2,pi/2) range
 */
f32 Math_FAtanF(f32 x) {
  if (!gUseAtanContFrac) {
    return Math_FAtanTaylorF(x);
  } else {
    return Math_FAtanContFracF(x);
  }
}

/**
 * @return angle to (x,y) from vector (1,0) around (0,0) in radians, in (-pi,pi]
 * range
 */
f32 Math_FAtan2F(f32 y, f32 x) {
  if (x == 0.0f) {
    if (y == 0.0f) {
      return 0.0f;
    } else if (y > 0.0f) {
      return M_PI / 2;
    } else if (y < 0.0f) {
      return -M_PI / 2;
    } else {
      return qNaN0x10000;
    }
  } else if (x >= 0.0f) {
    return Math_FAtanF(y / x);
  } else if (y < 0.0f) {
    return Math_FAtanF(y / x) - M_PI;
  } else {
    return M_PI - Math_FAtanF(-(y / x));
  }
}

/**
 * @return arcsin(x) in radians, in [-pi/2,pi/2] range
 */
f32 Math_FAsinF(f32 x) { return Math_FAtan2F(x, sqrtf(1.0f - SQ(x))); }

/**
 * @return arccos(x) in radians, in [0,pi] range
 */
f32 Math_FAcosF(f32 x) { return M_PI / 2 - Math_FAsinF(x); }

f32 Math_Vec3f_DistXZ(Vec3f *a, Vec3f *b) {
  f32 dx = b->x - a->x;
  f32 dz = b->z - a->z;

  return sqrtf(SQ(dx) + SQ(dz));
}

u16 Math_Vec3f_Yaw(Vec3f *a, Vec3f *b) {
  f32 dx = b->x - a->x;
  f32 dz = b->z - a->z;

  return Math_Atan2S(dz, dx);
}

/**
 * Changes pValue by step towards target. If step is more than fraction of the
 * remaining distance, step by that instead.
 */
void Math_ApproachF(f32 *pValue, f32 target, f32 fraction, f32 step) {
  if (*pValue != target) {
    f32 stepSize = (target - *pValue) * fraction;

    if (stepSize > step) {
      stepSize = step;
    } else if (stepSize < -step) {
      stepSize = -step;
    }

    *pValue += stepSize;
  }
}

/**
 * Changes pValue by step towards zero. If step is more than fraction of the
 * remaining distance, step by that instead.
 */
void Math_ApproachZeroF(f32 *pValue, f32 fraction, f32 step) {
  f32 stepSize = *pValue * fraction;

  if (stepSize > step) {
    stepSize = step;
  } else if (stepSize < -step) {
    stepSize = -step;
  }

  *pValue -= stepSize;
}

/**
 * Changes pValue by step (scaled by the update rate) towards target, setting it
 * equal when the target is reached. Returns true when target is reached, false
 * otherwise.
 */
bool Math_ScaledStepToS(u16 *pValue, u16 target, s16 step) {
  // Rewritten to use u16 (since this is mostly used for angles) and to avoid
  // undefined behavior.
  if (step != 0) {
    f32 updateScale = 1.5f;
    u16 update = step * updateScale;

    u16 diff = *pValue - target;
    if (diff < 0x8000) {
      if (update < diff) {
        *pValue -= update;
        return false;
      } else {
        *pValue = target;
        return true;
      }
    } else {
      diff = -diff;
      if (update < diff) {
        *pValue += update;
        return false;
      } else {
        *pValue = target;
        return true;
      }
    }
  }

  return target == *pValue;
}

/**
 * Changes pValue by step towards target, setting it equal when the target is
 * reached. Returns true when target is reached, false otherwise.
 */
bool Math_StepToF(f32 *pValue, f32 target, f32 step) {
  if (step != 0.0f) {
    if (target < *pValue) {
      step = -step;
    }

    *pValue += step;

    if (((*pValue - target) * step) >= 0) {
      *pValue = target;
      return true;
    }
  } else if (target == *pValue) {
    return true;
  }

  return false;
}
