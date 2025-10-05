// main.cpp
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;
using std::string;

typedef unsigned char Byte; // 别用 'byte'，避免和 std::byte 冲突

// ---- S-box / inverse S-box ----
static const Byte S_BOX[256] = {
  0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
  0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
  0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
  0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
  0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
  0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
  0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
  0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
  0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
  0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
  0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
  0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
  0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
  0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
  0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
  0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

static const Byte INV_S_BOX[256] = {
  0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
  0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
  0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
  0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
  0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
  0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
  0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
  0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
  0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
  0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
  0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
  0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
  0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
  0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
  0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
  0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

// Rcon
static const Byte RCON[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};

// ---- helpers ----
// 在 AES 中，状态按列优先存储：state[row][col]
// 打印 state 矩阵，便于对照教材（16 字节 -> 4x4）
void print_state(const Byte state[4][4]) {
    cout << " state (rows 0..3, cols 0..3):\n";
    for (int r = 0; r < 4; ++r) {
        cout << "  ";
        for (int c = 0; c < 4; ++c) {
            cout << std::hex << std::setw(2) << std::setfill('0') << (int)state[r][c] << " ";
        }
        cout << std::dec << "\n";
    }
}

// GF(2^8) 乘法
Byte gmul(Byte a, Byte b) {
    Byte p = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) p ^= a;
        bool hi = (a & 0x80);
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

// AddRoundKey：roundKey 为 16 字节（列主序）
void AddRoundKey(Byte state[4][4], const Byte roundKey[16]) {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            state[r][c] ^= roundKey[c*4 + r];
        }
    }
}

// SubBytes / InvSubBytes
void SubBytes(Byte state[4][4]) {
    for (int r=0;r<4;++r)
        for (int c=0;c<4;++c)
            state[r][c] = S_BOX[state[r][c]];
}
void InvSubBytes(Byte state[4][4]) {
    for (int r=0;r<4;++r)
        for (int c=0;c<4;++c)
            state[r][c] = INV_S_BOX[state[r][c]];
}

// ShiftRows / InvShiftRows
void ShiftRows(Byte state[4][4]) {
    // row 0 no shift; row i rotate left by i
    Byte tmp[4];
    for (int r=1;r<4;++r) {
        for (int c=0;c<4;++c) tmp[c] = state[r][(c + r) % 4];
        for (int c=0;c<4;++c) state[r][c] = tmp[c];
    }
}
void InvShiftRows(Byte state[4][4]) {
    Byte tmp[4];
    for (int r=1;r<4;++r) {
        for (int c=0;c<4;++c) tmp[c] = state[r][(c + 4 - r) % 4];
        for (int c=0;c<4;++c) state[r][c] = tmp[c];
    }
}

// MixColumns / InvMixColumns
void MixColumns(Byte state[4][4]) {
    for (int c=0;c<4;++c) {
        Byte a0 = state[0][c], a1 = state[1][c], a2 = state[2][c], a3 = state[3][c];
        state[0][c] = (Byte)( gmul(a0,2) ^ gmul(a1,3) ^ a2 ^ a3 );
        state[1][c] = (Byte)( a0 ^ gmul(a1,2) ^ gmul(a2,3) ^ a3 );
        state[2][c] = (Byte)( a0 ^ a1 ^ gmul(a2,2) ^ gmul(a3,3) );
        state[3][c] = (Byte)( gmul(a0,3) ^ a1 ^ a2 ^ gmul(a3,2) );
    }
}
void InvMixColumns(Byte state[4][4]) {
    for (int c=0;c<4;++c) {
        Byte a0 = state[0][c], a1 = state[1][c], a2 = state[2][c], a3 = state[3][c];
        state[0][c] = (Byte)( gmul(a0,14) ^ gmul(a1,11) ^ gmul(a2,13) ^ gmul(a3,9) );
        state[1][c] = (Byte)( gmul(a0,9) ^ gmul(a1,14) ^ gmul(a2,11) ^ gmul(a3,13) );
        state[2][c] = (Byte)( gmul(a0,13) ^ gmul(a1,9) ^ gmul(a2,14) ^ gmul(a3,11) );
        state[3][c] = (Byte)( gmul(a0,11) ^ gmul(a1,13) ^ gmul(a2,9) ^ gmul(a3,14) );
    }
}

// KeyExpansion: 输入 16 字节 key（字节按行输入，例如 ASCII 字符串），输出 176 字节轮密钥
// roundKeys: 长度至少176
void KeyExpansion(const Byte key[16], Byte roundKeys[176]) {
    // 前 16 字节直接是密钥
    for (int i=0;i<16;++i) roundKeys[i] = key[i];

    int bytesGenerated = 16;
    int rconIter = 0;
    Byte temp[4];

    while (bytesGenerated < 176) {
        for (int i=0;i<4;++i) temp[i] = roundKeys[bytesGenerated - 4 + i];

        if (bytesGenerated % 16 == 0) {
            // rotWord
            Byte t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            // subWord
            for (int i=0;i<4;++i) temp[i] = S_BOX[temp[i]];
            // Rcon
            temp[0] ^= RCON[rconIter++];
        }
        for (int i=0;i<4;++i) {
            roundKeys[bytesGenerated] = roundKeys[bytesGenerated - 16] ^ temp[i];
            ++bytesGenerated;
        }
    }
}

// AES 单块加密（16 字节）——verbose 控制是否打印逐轮状态
void AES_EncryptBlock(Byte inOut[16], const Byte roundKeys[176], bool verbose=false) {
    // 将输入装入 state（列主序）
    Byte state[4][4];
    for (int i=0;i<16;++i) state[i%4][i/4] = inOut[i];

    if (verbose) {
        cout << "\n=== Start Encryption ===\nOriginal bytes:\n";
        print_state(state);
    }

    // 初始轮
    AddRoundKey(state, roundKeys);
    if (verbose) { cout << "\nAfter AddRoundKey (round 0):\n"; print_state(state); }

    // 9 轮标准轮
    for (int round=1; round<=9; ++round) {
        SubBytes(state);
        if (verbose) { cout << "\nAfter SubBytes (round " << round << "):\n"; print_state(state); }
        ShiftRows(state);
        if (verbose) { cout << "\nAfter ShiftRows (round " << round << "):\n"; print_state(state); }
        MixColumns(state);
        if (verbose) { cout << "\nAfter MixColumns (round " << round << "):\n"; print_state(state); }
        AddRoundKey(state, roundKeys + 16*round);
        if (verbose) { cout << "\nAfter AddRoundKey (round " << round << "):\n"; print_state(state); }
    }

    // 最后一轮（不含 MixColumns）
    SubBytes(state);
    if (verbose) { cout << "\nAfter SubBytes (round 10):\n"; print_state(state); }
    ShiftRows(state);
    if (verbose) { cout << "\nAfter ShiftRows (round 10):\n"; print_state(state); }
    AddRoundKey(state, roundKeys + 160);
    if (verbose) { cout << "\nAfter AddRoundKey (round 10):\n"; print_state(state); cout << "=== End Encryption ===\n"; }

    // 写回
    for (int i=0;i<16;++i) inOut[i] = state[i%4][i/4];
}

// AES 单块解密（16 字节）
void AES_DecryptBlock(Byte inOut[16], const Byte roundKeys[176], bool verbose=false) {
    Byte state[4][4];
    for (int i=0;i<16;++i) state[i%4][i/4] = inOut[i];

    if (verbose) {
        cout << "\n=== Start Decryption ===\nOriginal bytes (cipher):\n";
        print_state(state);
    }

    // 初始轮: addRoundKey with last
    AddRoundKey(state, roundKeys + 160);
    if (verbose) { cout << "\nAfter AddRoundKey (round 10):\n"; print_state(state); }

    // 最后一轮逆（没有 InvMixColumns）
    InvShiftRows(state);
    if (verbose) { cout << "\nAfter InvShiftRows (round 10):\n"; print_state(state); }
    InvSubBytes(state);
    if (verbose) { cout << "\nAfter InvSubBytes (round 10):\n"; print_state(state); }

    // 9 轮逆
    for (int round=9; round>=1; --round) {
        AddRoundKey(state, roundKeys + 16*round);
        if (verbose) { cout << "\nAfter AddRoundKey (round " << round << "):\n"; print_state(state); }
        InvMixColumns(state);
        if (verbose) { cout << "\nAfter InvMixColumns (round " << round << "):\n"; print_state(state); }
        InvShiftRows(state);
        if (verbose) { cout << "\nAfter InvShiftRows (round " << round << "):\n"; print_state(state); }
        InvSubBytes(state);
        if (verbose) { cout << "\nAfter InvSubBytes (round " << round << "):\n"; print_state(state); }
    }

    // 初始轮逆
    AddRoundKey(state, roundKeys);
    if (verbose) { cout << "\nAfter AddRoundKey (round 0):\n"; print_state(state); cout << "=== End Decryption ===\n"; }

    for (int i=0;i<16;++i) inOut[i] = state[i%4][i/4];
}

// 辅助打印 16 字节数组为 HEX
void print_hex16(const Byte b[16]) {
    for (int i=0;i<16;++i) {
        cout << std::hex << std::setw(2) << std::setfill('0') << (int)b[i];
    }
    cout << std::dec << "\n";
}

int main() {
    cout << "AES-128 教学版（逐轮显示）\n\n";

    string plain, key;
    cout << "输入明文（可少于16字节，程序会用空格补齐）: ";
    std::getline(cin, plain);
    cout << "输入密钥（可少于16字节，程序会用空格补齐）: ";
    std::getline(cin, key);

    // 填充至 16 字节（教学方便）
    if (plain.size() < 16) plain.append(16 - plain.size(), ' ');
    if (key.size() < 16) key.append(16 - key.size(), ' ');

    Byte in[16], original[16], k[16];
    for (int i=0;i<16;++i) { in[i] = (Byte)plain[i]; original[i] = in[i]; k[i] = (Byte)key[i]; }

    cout << "\nPlain (ASCII): \"" << plain << "\"\n";
    cout << "Key   (ASCII): \"" << key << "\"\n";

    Byte roundKeys[176];
    KeyExpansion(k, roundKeys);

    cout << "\n-- 开始加密（逐轮打印） --\n";
    AES_EncryptBlock(in, roundKeys, true);

    cout << "\nCipher (hex): ";
    print_hex16(in);

    // 解密回去，验证
    cout << "\n-- 开始解密（逐轮打印） --\n";
    Byte cipher[16];
    memcpy(cipher, in, 16);
    AES_DecryptBlock(cipher, roundKeys, true);

    cout << "\nDecrypted (hex): ";
    print_hex16(cipher);

    cout << "\nDecrypted (ASCII): \"";
    for (int i=0;i<16;++i) cout << (char)cipher[i];
    cout << "\"\n";

    cout << "\nDone.\n";
    return 0;
    getchar();
}
