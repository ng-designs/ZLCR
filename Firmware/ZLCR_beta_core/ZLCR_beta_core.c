/**
 * @file    ZLCR_beta_core.c
 * @author  TIANLAN <yitiandelan@outlook.com>
 * @date    2018-JUN-3
 * @brief   
 *
 * Copyright (c) 2016-2018, TIANLAN.tech
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ZLCR_beta_core.h"
#include "arm_math.h"

float ZLCR_prv_freq = 1000.0f;  //f = fs / 2^22 * freqREG
unsigned int ZLCR_prv_tick1;    //freqREG
unsigned int ZLCR_prv_sum1;     //phaseREG

void ZLCR_Init(void)
{
    ZLCR_Setfreq(&ZLCR_prv_freq);
}

void ZLCR_DeInit(void);

void ZLCR_IDLE(void)
{

}

void ZLCR_ISR(unsigned short *txbuf, unsigned short *rxbuf, unsigned short offset, unsigned short size)
{

}

void ZLCR_Setfreq(float *freq)
{
    float f;
    f =  (*freq > 90000.0f) ? 90000.0f : (*freq < 0.0f) ? 0.0f : *freq;
    ZLCR_prv_tick1 = 22906.5f * f;
    ZLCR_prv_freq = ZLCR_prv_tick1 / 22906.5f;
}

void ZLCR_Getfreq(float *freq)
{
    *freq = ZLCR_prv_freq;
}

const int16_t twiddleCoefQ15[] = {
    0x0000, 0x0093, 0x0127, 0x01ba, 0x024d, 0x02e0, 0x0373, 0x0407, 0x049a, 0x052d, 0x05c0, 0x0653, 0x06e6, 0x0778, 0x080b, 0x089e,
    0x0930, 0x09c3, 0x0a55, 0x0ae8, 0x0b7a, 0x0c0c, 0x0c9e, 0x0d30, 0x0dc2, 0x0e53, 0x0ee5, 0x0f76, 0x1007, 0x1098, 0x1129, 0x11ba,
    0x124a, 0x12db, 0x136b, 0x13fb, 0x148a, 0x151a, 0x15a9, 0x1639, 0x16c8, 0x1756, 0x17e5, 0x1873, 0x1901, 0x198f, 0x1a1c, 0x1aaa,
    0x1b37, 0x1bc4, 0x1c50, 0x1cdc, 0x1d68, 0x1df4, 0x1e7f, 0x1f0b, 0x1f95, 0x2020, 0x20aa, 0x2134, 0x21bd, 0x2247, 0x22d0, 0x2358,
    0x23e0, 0x2468, 0x24f0, 0x2577, 0x25fe, 0x2684, 0x270a, 0x2790, 0x2815, 0x289a, 0x291f, 0x29a3, 0x2a27, 0x2aaa, 0x2b2d, 0x2baf,
    0x2c32, 0x2cb3, 0x2d34, 0x2db5, 0x2e36, 0x2eb5, 0x2f35, 0x2fb4, 0x3032, 0x30b1, 0x312e, 0x31ab, 0x3228, 0x32a4, 0x3320, 0x339b,
    0x3416, 0x3490, 0x350a, 0x3583, 0x35fb, 0x3674, 0x36eb, 0x3762, 0x37d9, 0x384f, 0x38c4, 0x3939, 0x39ae, 0x3a21, 0x3a95, 0x3b07,
    0x3b79, 0x3beb, 0x3c5c, 0x3ccc, 0x3d3c, 0x3dab, 0x3e1a, 0x3e88, 0x3ef5, 0x3f62, 0x3fce, 0x403a, 0x40a5, 0x410f, 0x4179, 0x41e2,
    0x424b, 0x42b2, 0x431a, 0x4380, 0x43e6, 0x444b, 0x44b0, 0x4514, 0x4577, 0x45d9, 0x463b, 0x469c, 0x46fd, 0x475d, 0x47bc, 0x481a,
    0x4878, 0x48d5, 0x4932, 0x498d, 0x49e8, 0x4a43, 0x4a9c, 0x4af5, 0x4b4d, 0x4ba4, 0x4bfb, 0x4c51, 0x4ca6, 0x4cfa, 0x4d4e, 0x4da1,
    0x4df3, 0x4e45, 0x4e95, 0x4ee5, 0x4f34, 0x4f83, 0x4fd1, 0x501d, 0x5069, 0x50b5, 0x50ff, 0x5149, 0x5192, 0x51da, 0x5222, 0x5268,
    0x52ae, 0x52f3, 0x5337, 0x537b, 0x53bd, 0x53ff, 0x5440, 0x5480, 0x54c0, 0x54fe, 0x553c, 0x5579, 0x55b5, 0x55f0, 0x562b, 0x5664,
    0x569d, 0x56d5, 0x570c, 0x5742, 0x5778, 0x57ac, 0x57e0, 0x5813, 0x5845, 0x5876, 0x58a7, 0x58d6, 0x5905, 0x5932, 0x595f, 0x598b,
    0x59b7, 0x59e1, 0x5a0a, 0x5a33, 0x5a5b, 0x5a81, 0x5aa7, 0x5acd, 0x5af1, 0x5b14, 0x5b37, 0x5b58, 0x5b79, 0x5b99, 0x5bb8, 0x5bd6,
    0x5bf3, 0x5c0f, 0x5c2b, 0x5c45, 0x5c5f, 0x5c77, 0x5c8f, 0x5ca6, 0x5cbc, 0x5cd1, 0x5ce6, 0x5cf9, 0x5d0c, 0x5d1d, 0x5d2e, 0x5d3e,
    0x5d4c, 0x5d5a, 0x5d68, 0x5d74, 0x5d7f, 0x5d89, 0x5d93, 0x5d9b, 0x5da3, 0x5daa, 0x5db0, 0x5db5, 0x5db9, 0x5dbc, 0x5dbe, 0x5dc0,
    0x5dc0, 0x5dc0, 0x5dbe, 0x5dbc, 0x5db9, 0x5db5, 0x5db0, 0x5daa, 0x5da3, 0x5d9b, 0x5d93, 0x5d89, 0x5d7f, 0x5d74, 0x5d68, 0x5d5a,
    0x5d4c, 0x5d3e, 0x5d2e, 0x5d1d, 0x5d0c, 0x5cf9, 0x5ce6, 0x5cd1, 0x5cbc, 0x5ca6, 0x5c8f, 0x5c77, 0x5c5f, 0x5c45, 0x5c2b, 0x5c0f,
    0x5bf3, 0x5bd6, 0x5bb8, 0x5b99, 0x5b79, 0x5b58, 0x5b37, 0x5b14, 0x5af1, 0x5acd, 0x5aa7, 0x5a81, 0x5a5b, 0x5a33, 0x5a0a, 0x59e1,
    0x59b7, 0x598b, 0x595f, 0x5932, 0x5905, 0x58d6, 0x58a7, 0x5876, 0x5845, 0x5813, 0x57e0, 0x57ac, 0x5778, 0x5742, 0x570c, 0x56d5,
    0x569d, 0x5664, 0x562b, 0x55f0, 0x55b5, 0x5579, 0x553c, 0x54fe, 0x54c0, 0x5480, 0x5440, 0x53ff, 0x53bd, 0x537b, 0x5337, 0x52f3,
    0x52ae, 0x5268, 0x5222, 0x51da, 0x5192, 0x5149, 0x50ff, 0x50b5, 0x5069, 0x501d, 0x4fd1, 0x4f83, 0x4f34, 0x4ee5, 0x4e95, 0x4e45,
    0x4df3, 0x4da1, 0x4d4e, 0x4cfa, 0x4ca6, 0x4c51, 0x4bfb, 0x4ba4, 0x4b4d, 0x4af5, 0x4a9c, 0x4a43, 0x49e8, 0x498d, 0x4932, 0x48d5,
    0x4878, 0x481a, 0x47bc, 0x475d, 0x46fd, 0x469c, 0x463b, 0x45d9, 0x4577, 0x4514, 0x44b0, 0x444b, 0x43e6, 0x4380, 0x431a, 0x42b2,
    0x424b, 0x41e2, 0x4179, 0x410f, 0x40a5, 0x403a, 0x3fce, 0x3f62, 0x3ef5, 0x3e88, 0x3e1a, 0x3dab, 0x3d3c, 0x3ccc, 0x3c5c, 0x3beb,
    0x3b79, 0x3b07, 0x3a95, 0x3a21, 0x39ae, 0x3939, 0x38c4, 0x384f, 0x37d9, 0x3762, 0x36eb, 0x3674, 0x35fb, 0x3583, 0x350a, 0x3490,
    0x3416, 0x339b, 0x3320, 0x32a4, 0x3228, 0x31ab, 0x312e, 0x30b1, 0x3032, 0x2fb4, 0x2f35, 0x2eb5, 0x2e36, 0x2db5, 0x2d34, 0x2cb3,
    0x2c32, 0x2baf, 0x2b2d, 0x2aaa, 0x2a27, 0x29a3, 0x291f, 0x289a, 0x2815, 0x2790, 0x270a, 0x2684, 0x25fe, 0x2577, 0x24f0, 0x2468,
    0x23e0, 0x2358, 0x22d0, 0x2247, 0x21bd, 0x2134, 0x20aa, 0x2020, 0x1f95, 0x1f0b, 0x1e7f, 0x1df4, 0x1d68, 0x1cdc, 0x1c50, 0x1bc4,
    0x1b37, 0x1aaa, 0x1a1c, 0x198f, 0x1901, 0x1873, 0x17e5, 0x1756, 0x16c8, 0x1639, 0x15a9, 0x151a, 0x148a, 0x13fb, 0x136b, 0x12db,
    0x124a, 0x11ba, 0x1129, 0x1098, 0x1007, 0x0f76, 0x0ee5, 0x0e53, 0x0dc2, 0x0d30, 0x0c9e, 0x0c0c, 0x0b7a, 0x0ae8, 0x0a55, 0x09c3,
    0x0930, 0x089e, 0x080b, 0x0778, 0x06e6, 0x0653, 0x05c0, 0x052d, 0x049a, 0x0407, 0x0373, 0x02e0, 0x024d, 0x01ba, 0x0127, 0x0093,
    0x0000, 0xff6d, 0xfed9, 0xfe46, 0xfdb3, 0xfd20, 0xfc8d, 0xfbf9, 0xfb66, 0xfad3, 0xfa40, 0xf9ad, 0xf91a, 0xf888, 0xf7f5, 0xf762,
    0xf6d0, 0xf63d, 0xf5ab, 0xf518, 0xf486, 0xf3f4, 0xf362, 0xf2d0, 0xf23e, 0xf1ad, 0xf11b, 0xf08a, 0xeff9, 0xef68, 0xeed7, 0xee46,
    0xedb6, 0xed25, 0xec95, 0xec05, 0xeb76, 0xeae6, 0xea57, 0xe9c7, 0xe938, 0xe8aa, 0xe81b, 0xe78d, 0xe6ff, 0xe671, 0xe5e4, 0xe556,
    0xe4c9, 0xe43c, 0xe3b0, 0xe324, 0xe298, 0xe20c, 0xe181, 0xe0f5, 0xe06b, 0xdfe0, 0xdf56, 0xdecc, 0xde43, 0xddb9, 0xdd30, 0xdca8,
    0xdc20, 0xdb98, 0xdb10, 0xda89, 0xda02, 0xd97c, 0xd8f6, 0xd870, 0xd7eb, 0xd766, 0xd6e1, 0xd65d, 0xd5d9, 0xd556, 0xd4d3, 0xd451,
    0xd3ce, 0xd34d, 0xd2cc, 0xd24b, 0xd1ca, 0xd14b, 0xd0cb, 0xd04c, 0xcfce, 0xcf4f, 0xced2, 0xce55, 0xcdd8, 0xcd5c, 0xcce0, 0xcc65,
    0xcbea, 0xcb70, 0xcaf6, 0xca7d, 0xca05, 0xc98c, 0xc915, 0xc89e, 0xc827, 0xc7b1, 0xc73c, 0xc6c7, 0xc652, 0xc5df, 0xc56b, 0xc4f9,
    0xc487, 0xc415, 0xc3a4, 0xc334, 0xc2c4, 0xc255, 0xc1e6, 0xc178, 0xc10b, 0xc09e, 0xc032, 0xbfc6, 0xbf5b, 0xbef1, 0xbe87, 0xbe1e,
    0xbdb5, 0xbd4e, 0xbce6, 0xbc80, 0xbc1a, 0xbbb5, 0xbb50, 0xbaec, 0xba89, 0xba27, 0xb9c5, 0xb964, 0xb903, 0xb8a3, 0xb844, 0xb7e6,
    0xb788, 0xb72b, 0xb6ce, 0xb673, 0xb618, 0xb5bd, 0xb564, 0xb50b, 0xb4b3, 0xb45c, 0xb405, 0xb3af, 0xb35a, 0xb306, 0xb2b2, 0xb25f,
    0xb20d, 0xb1bb, 0xb16b, 0xb11b, 0xb0cc, 0xb07d, 0xb02f, 0xafe3, 0xaf97, 0xaf4b, 0xaf01, 0xaeb7, 0xae6e, 0xae26, 0xadde, 0xad98,
    0xad52, 0xad0d, 0xacc9, 0xac85, 0xac43, 0xac01, 0xabc0, 0xab80, 0xab40, 0xab02, 0xaac4, 0xaa87, 0xaa4b, 0xaa10, 0xa9d5, 0xa99c,
    0xa963, 0xa92b, 0xa8f4, 0xa8be, 0xa888, 0xa854, 0xa820, 0xa7ed, 0xa7bb, 0xa78a, 0xa759, 0xa72a, 0xa6fb, 0xa6ce, 0xa6a1, 0xa675,
    0xa649, 0xa61f, 0xa5f6, 0xa5cd, 0xa5a5, 0xa57f, 0xa559, 0xa533, 0xa50f, 0xa4ec, 0xa4c9, 0xa4a8, 0xa487, 0xa467, 0xa448, 0xa42a,
    0xa40d, 0xa3f1, 0xa3d5, 0xa3bb, 0xa3a1, 0xa389, 0xa371, 0xa35a, 0xa344, 0xa32f, 0xa31a, 0xa307, 0xa2f4, 0xa2e3, 0xa2d2, 0xa2c2,
    0xa2b4, 0xa2a6, 0xa298, 0xa28c, 0xa281, 0xa277, 0xa26d, 0xa265, 0xa25d, 0xa256, 0xa250, 0xa24b, 0xa247, 0xa244, 0xa242, 0xa240,
    0xa240, 0xa240, 0xa242, 0xa244, 0xa247, 0xa24b, 0xa250, 0xa256, 0xa25d, 0xa265, 0xa26d, 0xa277, 0xa281, 0xa28c, 0xa298, 0xa2a6,
    0xa2b4, 0xa2c2, 0xa2d2, 0xa2e3, 0xa2f4, 0xa307, 0xa31a, 0xa32f, 0xa344, 0xa35a, 0xa371, 0xa389, 0xa3a1, 0xa3bb, 0xa3d5, 0xa3f1,
    0xa40d, 0xa42a, 0xa448, 0xa467, 0xa487, 0xa4a8, 0xa4c9, 0xa4ec, 0xa50f, 0xa533, 0xa559, 0xa57f, 0xa5a5, 0xa5cd, 0xa5f6, 0xa61f,
    0xa649, 0xa675, 0xa6a1, 0xa6ce, 0xa6fb, 0xa72a, 0xa759, 0xa78a, 0xa7bb, 0xa7ed, 0xa820, 0xa854, 0xa888, 0xa8be, 0xa8f4, 0xa92b,
    0xa963, 0xa99c, 0xa9d5, 0xaa10, 0xaa4b, 0xaa87, 0xaac4, 0xab02, 0xab40, 0xab80, 0xabc0, 0xac01, 0xac43, 0xac85, 0xacc9, 0xad0d,
    0xad52, 0xad98, 0xadde, 0xae26, 0xae6e, 0xaeb7, 0xaf01, 0xaf4b, 0xaf97, 0xafe3, 0xb02f, 0xb07d, 0xb0cc, 0xb11b, 0xb16b, 0xb1bb,
    0xb20d, 0xb25f, 0xb2b2, 0xb306, 0xb35a, 0xb3af, 0xb405, 0xb45c, 0xb4b3, 0xb50b, 0xb564, 0xb5bd, 0xb618, 0xb673, 0xb6ce, 0xb72b,
    0xb788, 0xb7e6, 0xb844, 0xb8a3, 0xb903, 0xb964, 0xb9c5, 0xba27, 0xba89, 0xbaec, 0xbb50, 0xbbb5, 0xbc1a, 0xbc80, 0xbce6, 0xbd4e,
    0xbdb5, 0xbe1e, 0xbe87, 0xbef1, 0xbf5b, 0xbfc6, 0xc032, 0xc09e, 0xc10b, 0xc178, 0xc1e6, 0xc255, 0xc2c4, 0xc334, 0xc3a4, 0xc415,
    0xc487, 0xc4f9, 0xc56b, 0xc5df, 0xc652, 0xc6c7, 0xc73c, 0xc7b1, 0xc827, 0xc89e, 0xc915, 0xc98c, 0xca05, 0xca7d, 0xcaf6, 0xcb70,
    0xcbea, 0xcc65, 0xcce0, 0xcd5c, 0xcdd8, 0xce55, 0xced2, 0xcf4f, 0xcfce, 0xd04c, 0xd0cb, 0xd14b, 0xd1ca, 0xd24b, 0xd2cc, 0xd34d,
    0xd3ce, 0xd451, 0xd4d3, 0xd556, 0xd5d9, 0xd65d, 0xd6e1, 0xd766, 0xd7eb, 0xd870, 0xd8f6, 0xd97c, 0xda02, 0xda89, 0xdb10, 0xdb98,
    0xdc20, 0xdca8, 0xdd30, 0xddb9, 0xde43, 0xdecc, 0xdf56, 0xdfe0, 0xe06b, 0xe0f5, 0xe181, 0xe20c, 0xe298, 0xe324, 0xe3b0, 0xe43c,
    0xe4c9, 0xe556, 0xe5e4, 0xe671, 0xe6ff, 0xe78d, 0xe81b, 0xe8aa, 0xe938, 0xe9c7, 0xea57, 0xeae6, 0xeb76, 0xec05, 0xec95, 0xed25,
    0xedb6, 0xee46, 0xeed7, 0xef68, 0xeff9, 0xf08a, 0xf11b, 0xf1ad, 0xf23e, 0xf2d0, 0xf362, 0xf3f4, 0xf486, 0xf518, 0xf5ab, 0xf63d,
    0xf6d0, 0xf762, 0xf7f5, 0xf888, 0xf91a, 0xf9ad, 0xfa40, 0xfad3, 0xfb66, 0xfbf9, 0xfc8d, 0xfd20, 0xfdb3, 0xfe46, 0xfed9, 0xff6d,
};

const float twiddleCoeff32[] = {
    +0.000000e+00, +6.759538e-13, +1.351882e-12, +2.027760e-12, +2.703561e-12, +3.379260e-12, +4.054832e-12, +4.730252e-12,
    +5.405493e-12, +6.080531e-12, +6.755340e-12, +7.429894e-12, +8.104169e-12, +8.778139e-12, +9.451778e-12, +1.012506e-11,
    +1.079796e-11, +1.147046e-11, +1.214252e-11, +1.281413e-11, +1.348525e-11, +1.415587e-11, +1.482595e-11, +1.549548e-11,
    +1.616442e-11, +1.683275e-11, +1.750045e-11, +1.816749e-11, +1.883385e-11, +1.949950e-11, +2.016441e-11, +2.082857e-11,
    +2.149194e-11, +2.215450e-11, +2.281622e-11, +2.347709e-11, +2.413708e-11, +2.479615e-11, +2.545429e-11, +2.611148e-11,
    +2.676768e-11, +2.742287e-11, +2.807703e-11, +2.873013e-11, +2.938215e-11, +3.003307e-11, +3.068285e-11, +3.133148e-11,
    +3.197893e-11, +3.262518e-11, +3.327019e-11, +3.391396e-11, +3.455645e-11, +3.519763e-11, +3.583749e-11, +3.647601e-11,
    +3.711315e-11, +3.774889e-11, +3.838321e-11, +3.901608e-11, +3.964749e-11, +4.027740e-11, +4.090580e-11, +4.153266e-11,
    +4.215795e-11, +4.278166e-11, +4.340375e-11, +4.402421e-11, +4.464302e-11, +4.526014e-11, +4.587556e-11, +4.648925e-11,
    +4.710119e-11, +4.771136e-11, +4.831973e-11, +4.892629e-11, +4.953100e-11, +5.013384e-11, +5.073480e-11, +5.133385e-11,
    +5.193097e-11, +5.252613e-11, +5.311931e-11, +5.371049e-11, +5.429965e-11, +5.488677e-11, +5.547182e-11, +5.605478e-11,
    +5.663563e-11, +5.721435e-11, +5.779092e-11, +5.836531e-11, +5.893750e-11, +5.950747e-11, +6.007520e-11, +6.064067e-11,
    +6.120386e-11, +6.176474e-11, +6.232330e-11, +6.287951e-11, +6.343335e-11, +6.398481e-11, +6.453386e-11, +6.508047e-11,
    +6.562464e-11, +6.616633e-11, +6.670554e-11, +6.724223e-11, +6.777639e-11, +6.830800e-11, +6.883704e-11, +6.936349e-11,
    +6.988732e-11, +7.040853e-11, +7.092708e-11, +7.144296e-11, +7.195616e-11, +7.246664e-11, +7.297440e-11, +7.347940e-11,
    +7.398164e-11, +7.448110e-11, +7.497775e-11, +7.547158e-11, +7.596257e-11, +7.645070e-11, +7.693595e-11, +7.741830e-11,
    +7.789774e-11, +7.837424e-11, +7.884780e-11, +7.931838e-11, +7.978598e-11, +8.025058e-11, +8.071215e-11, +8.117069e-11,
    +8.162617e-11, +8.207857e-11, +8.252789e-11, +8.297410e-11, +8.341718e-11, +8.385713e-11, +8.429391e-11, +8.472753e-11,
    +8.515795e-11, +8.558517e-11, +8.600916e-11, +8.642992e-11, +8.684742e-11, +8.726166e-11, +8.767260e-11, +8.808025e-11,
    +8.848458e-11, +8.888558e-11, +8.928324e-11, +8.967753e-11, +9.006844e-11, +9.045597e-11, +9.084008e-11, +9.122078e-11,
    +9.159805e-11, +9.197186e-11, +9.234222e-11, +9.270909e-11, +9.307248e-11, +9.343236e-11, +9.378872e-11, +9.414156e-11,
    +9.449084e-11, +9.483658e-11, +9.517874e-11, +9.551731e-11, +9.585229e-11, +9.618367e-11, +9.651142e-11, +9.683553e-11,
    +9.715601e-11, +9.747282e-11, +9.778596e-11, +9.809542e-11, +9.840119e-11, +9.870326e-11, +9.900161e-11, +9.929623e-11,
    +9.958711e-11, +9.987424e-11, +1.001576e-10, +1.004372e-10, +1.007130e-10, +1.009851e-10, +1.012533e-10, +1.015177e-10,
    +1.017783e-10, +1.020351e-10, +1.022880e-10, +1.025370e-10, +1.027823e-10, +1.030236e-10, +1.032610e-10, +1.034946e-10,
    +1.037243e-10, +1.039501e-10, +1.041719e-10, +1.043899e-10, +1.046039e-10, +1.048139e-10, +1.050200e-10, +1.052222e-10,
    +1.054204e-10, +1.056146e-10, +1.058049e-10, +1.059912e-10, +1.061735e-10, +1.063517e-10, +1.065260e-10, +1.066963e-10,
    +1.068626e-10, +1.070248e-10, +1.071830e-10, +1.073372e-10, +1.074873e-10, +1.076334e-10, +1.077754e-10, +1.079134e-10,
    +1.080473e-10, +1.081771e-10, +1.083029e-10, +1.084246e-10, +1.085422e-10, +1.086557e-10, +1.087651e-10, +1.088704e-10,
    +1.089717e-10, +1.090688e-10, +1.091618e-10, +1.092507e-10, +1.093356e-10, +1.094162e-10, +1.094928e-10, +1.095652e-10,
    +1.096336e-10, +1.096978e-10, +1.097578e-10, +1.098137e-10, +1.098655e-10, +1.099132e-10, +1.099567e-10, +1.099961e-10,
    +1.100313e-10, +1.100624e-10, +1.100894e-10, +1.101122e-10, +1.101309e-10, +1.101454e-10, +1.101557e-10, +1.101620e-10,
    +1.101640e-10, +1.101620e-10, +1.101557e-10, +1.101454e-10, +1.101309e-10, +1.101122e-10, +1.100894e-10, +1.100624e-10,
    +1.100313e-10, +1.099961e-10, +1.099567e-10, +1.099132e-10, +1.098655e-10, +1.098137e-10, +1.097578e-10, +1.096978e-10,
    +1.096336e-10, +1.095652e-10, +1.094928e-10, +1.094162e-10, +1.093356e-10, +1.092507e-10, +1.091618e-10, +1.090688e-10,
    +1.089717e-10, +1.088704e-10, +1.087651e-10, +1.086557e-10, +1.085422e-10, +1.084246e-10, +1.083029e-10, +1.081771e-10,
    +1.080473e-10, +1.079134e-10, +1.077754e-10, +1.076334e-10, +1.074873e-10, +1.073372e-10, +1.071830e-10, +1.070248e-10,
    +1.068626e-10, +1.066963e-10, +1.065260e-10, +1.063517e-10, +1.061735e-10, +1.059912e-10, +1.058049e-10, +1.056146e-10,
    +1.054204e-10, +1.052222e-10, +1.050200e-10, +1.048139e-10, +1.046039e-10, +1.043899e-10, +1.041719e-10, +1.039501e-10,
    +1.037243e-10, +1.034946e-10, +1.032610e-10, +1.030236e-10, +1.027823e-10, +1.025370e-10, +1.022880e-10, +1.020351e-10,
    +1.017783e-10, +1.015177e-10, +1.012533e-10, +1.009851e-10, +1.007130e-10, +1.004372e-10, +1.001576e-10, +9.987424e-11,
    +9.958711e-11, +9.929623e-11, +9.900161e-11, +9.870326e-11, +9.840119e-11, +9.809542e-11, +9.778596e-11, +9.747282e-11,
    +9.715601e-11, +9.683553e-11, +9.651142e-11, +9.618367e-11, +9.585229e-11, +9.551731e-11, +9.517874e-11, +9.483658e-11,
    +9.449084e-11, +9.414156e-11, +9.378872e-11, +9.343236e-11, +9.307248e-11, +9.270909e-11, +9.234222e-11, +9.197186e-11,
    +9.159805e-11, +9.122078e-11, +9.084008e-11, +9.045597e-11, +9.006844e-11, +8.967753e-11, +8.928324e-11, +8.888558e-11,
    +8.848458e-11, +8.808025e-11, +8.767260e-11, +8.726166e-11, +8.684742e-11, +8.642992e-11, +8.600916e-11, +8.558517e-11,
    +8.515795e-11, +8.472753e-11, +8.429391e-11, +8.385713e-11, +8.341718e-11, +8.297410e-11, +8.252789e-11, +8.207857e-11,
    +8.162617e-11, +8.117069e-11, +8.071215e-11, +8.025058e-11, +7.978598e-11, +7.931838e-11, +7.884780e-11, +7.837424e-11,
    +7.789774e-11, +7.741830e-11, +7.693595e-11, +7.645070e-11, +7.596257e-11, +7.547158e-11, +7.497775e-11, +7.448110e-11,
    +7.398164e-11, +7.347940e-11, +7.297440e-11, +7.246664e-11, +7.195616e-11, +7.144296e-11, +7.092708e-11, +7.040853e-11,
    +6.988732e-11, +6.936349e-11, +6.883704e-11, +6.830800e-11, +6.777639e-11, +6.724223e-11, +6.670554e-11, +6.616633e-11,
    +6.562464e-11, +6.508047e-11, +6.453386e-11, +6.398481e-11, +6.343335e-11, +6.287951e-11, +6.232330e-11, +6.176474e-11,
    +6.120386e-11, +6.064067e-11, +6.007520e-11, +5.950747e-11, +5.893750e-11, +5.836531e-11, +5.779092e-11, +5.721435e-11,
    +5.663563e-11, +5.605478e-11, +5.547182e-11, +5.488677e-11, +5.429965e-11, +5.371049e-11, +5.311931e-11, +5.252613e-11,
    +5.193097e-11, +5.133385e-11, +5.073480e-11, +5.013384e-11, +4.953100e-11, +4.892629e-11, +4.831973e-11, +4.771136e-11,
    +4.710119e-11, +4.648925e-11, +4.587556e-11, +4.526014e-11, +4.464302e-11, +4.402421e-11, +4.340375e-11, +4.278166e-11,
    +4.215795e-11, +4.153266e-11, +4.090580e-11, +4.027740e-11, +3.964749e-11, +3.901608e-11, +3.838321e-11, +3.774889e-11,
    +3.711315e-11, +3.647601e-11, +3.583749e-11, +3.519763e-11, +3.455645e-11, +3.391396e-11, +3.327019e-11, +3.262518e-11,
    +3.197893e-11, +3.133148e-11, +3.068285e-11, +3.003307e-11, +2.938215e-11, +2.873013e-11, +2.807703e-11, +2.742287e-11,
    +2.676768e-11, +2.611148e-11, +2.545429e-11, +2.479615e-11, +2.413708e-11, +2.347709e-11, +2.281622e-11, +2.215450e-11,
    +2.149194e-11, +2.082857e-11, +2.016441e-11, +1.949950e-11, +1.883385e-11, +1.816749e-11, +1.750045e-11, +1.683275e-11,
    +1.616442e-11, +1.549548e-11, +1.482595e-11, +1.415587e-11, +1.348525e-11, +1.281413e-11, +1.214252e-11, +1.147046e-11,
    +1.079796e-11, +1.012506e-11, +9.451778e-12, +8.778139e-12, +8.104169e-12, +7.429894e-12, +6.755340e-12, +6.080531e-12,
    +5.405493e-12, +4.730252e-12, +4.054832e-12, +3.379260e-12, +2.703561e-12, +2.027760e-12, +1.351882e-12, +6.759538e-13,
    -3.543146e-26, -6.759538e-13, -1.351882e-12, -2.027760e-12, -2.703561e-12, -3.379260e-12, -4.054832e-12, -4.730252e-12,
    -5.405493e-12, -6.080531e-12, -6.755340e-12, -7.429894e-12, -8.104169e-12, -8.778139e-12, -9.451778e-12, -1.012506e-11,
    -1.079796e-11, -1.147046e-11, -1.214252e-11, -1.281413e-11, -1.348525e-11, -1.415587e-11, -1.482595e-11, -1.549548e-11,
    -1.616442e-11, -1.683275e-11, -1.750045e-11, -1.816749e-11, -1.883385e-11, -1.949950e-11, -2.016441e-11, -2.082857e-11,
    -2.149194e-11, -2.215450e-11, -2.281622e-11, -2.347709e-11, -2.413708e-11, -2.479615e-11, -2.545429e-11, -2.611148e-11,
    -2.676768e-11, -2.742287e-11, -2.807703e-11, -2.873013e-11, -2.938215e-11, -3.003307e-11, -3.068285e-11, -3.133148e-11,
    -3.197893e-11, -3.262518e-11, -3.327019e-11, -3.391396e-11, -3.455645e-11, -3.519763e-11, -3.583749e-11, -3.647601e-11,
    -3.711315e-11, -3.774889e-11, -3.838321e-11, -3.901608e-11, -3.964749e-11, -4.027740e-11, -4.090580e-11, -4.153266e-11,
    -4.215795e-11, -4.278166e-11, -4.340375e-11, -4.402421e-11, -4.464302e-11, -4.526014e-11, -4.587556e-11, -4.648925e-11,
    -4.710119e-11, -4.771136e-11, -4.831973e-11, -4.892629e-11, -4.953100e-11, -5.013384e-11, -5.073480e-11, -5.133385e-11,
    -5.193097e-11, -5.252613e-11, -5.311931e-11, -5.371049e-11, -5.429965e-11, -5.488677e-11, -5.547182e-11, -5.605478e-11,
    -5.663563e-11, -5.721435e-11, -5.779092e-11, -5.836531e-11, -5.893750e-11, -5.950747e-11, -6.007520e-11, -6.064067e-11,
    -6.120386e-11, -6.176474e-11, -6.232330e-11, -6.287951e-11, -6.343335e-11, -6.398481e-11, -6.453386e-11, -6.508047e-11,
    -6.562464e-11, -6.616633e-11, -6.670554e-11, -6.724223e-11, -6.777639e-11, -6.830800e-11, -6.883704e-11, -6.936349e-11,
    -6.988732e-11, -7.040853e-11, -7.092708e-11, -7.144296e-11, -7.195616e-11, -7.246664e-11, -7.297440e-11, -7.347940e-11,
    -7.398164e-11, -7.448110e-11, -7.497775e-11, -7.547158e-11, -7.596257e-11, -7.645070e-11, -7.693595e-11, -7.741830e-11,
    -7.789774e-11, -7.837424e-11, -7.884780e-11, -7.931838e-11, -7.978598e-11, -8.025058e-11, -8.071215e-11, -8.117069e-11,
    -8.162617e-11, -8.207857e-11, -8.252789e-11, -8.297410e-11, -8.341718e-11, -8.385713e-11, -8.429391e-11, -8.472753e-11,
    -8.515795e-11, -8.558517e-11, -8.600916e-11, -8.642992e-11, -8.684742e-11, -8.726166e-11, -8.767260e-11, -8.808025e-11,
    -8.848458e-11, -8.888558e-11, -8.928324e-11, -8.967753e-11, -9.006844e-11, -9.045597e-11, -9.084008e-11, -9.122078e-11,
    -9.159805e-11, -9.197186e-11, -9.234222e-11, -9.270909e-11, -9.307248e-11, -9.343236e-11, -9.378872e-11, -9.414156e-11,
    -9.449084e-11, -9.483658e-11, -9.517874e-11, -9.551731e-11, -9.585229e-11, -9.618367e-11, -9.651142e-11, -9.683553e-11,
    -9.715601e-11, -9.747282e-11, -9.778596e-11, -9.809542e-11, -9.840119e-11, -9.870326e-11, -9.900161e-11, -9.929623e-11,
    -9.958711e-11, -9.987424e-11, -1.001576e-10, -1.004372e-10, -1.007130e-10, -1.009851e-10, -1.012533e-10, -1.015177e-10,
    -1.017783e-10, -1.020351e-10, -1.022880e-10, -1.025370e-10, -1.027823e-10, -1.030236e-10, -1.032610e-10, -1.034946e-10,
    -1.037243e-10, -1.039501e-10, -1.041719e-10, -1.043899e-10, -1.046039e-10, -1.048139e-10, -1.050200e-10, -1.052222e-10,
    -1.054204e-10, -1.056146e-10, -1.058049e-10, -1.059912e-10, -1.061735e-10, -1.063517e-10, -1.065260e-10, -1.066963e-10,
    -1.068626e-10, -1.070248e-10, -1.071830e-10, -1.073372e-10, -1.074873e-10, -1.076334e-10, -1.077754e-10, -1.079134e-10,
    -1.080473e-10, -1.081771e-10, -1.083029e-10, -1.084246e-10, -1.085422e-10, -1.086557e-10, -1.087651e-10, -1.088704e-10,
    -1.089717e-10, -1.090688e-10, -1.091618e-10, -1.092507e-10, -1.093356e-10, -1.094162e-10, -1.094928e-10, -1.095652e-10,
    -1.096336e-10, -1.096978e-10, -1.097578e-10, -1.098137e-10, -1.098655e-10, -1.099132e-10, -1.099567e-10, -1.099961e-10,
    -1.100313e-10, -1.100624e-10, -1.100894e-10, -1.101122e-10, -1.101309e-10, -1.101454e-10, -1.101557e-10, -1.101620e-10,
    -1.101640e-10, -1.101620e-10, -1.101557e-10, -1.101454e-10, -1.101309e-10, -1.101122e-10, -1.100894e-10, -1.100624e-10,
    -1.100313e-10, -1.099961e-10, -1.099567e-10, -1.099132e-10, -1.098655e-10, -1.098137e-10, -1.097578e-10, -1.096978e-10,
    -1.096336e-10, -1.095652e-10, -1.094928e-10, -1.094162e-10, -1.093356e-10, -1.092507e-10, -1.091618e-10, -1.090688e-10,
    -1.089717e-10, -1.088704e-10, -1.087651e-10, -1.086557e-10, -1.085422e-10, -1.084246e-10, -1.083029e-10, -1.081771e-10,
    -1.080473e-10, -1.079134e-10, -1.077754e-10, -1.076334e-10, -1.074873e-10, -1.073372e-10, -1.071830e-10, -1.070248e-10,
    -1.068626e-10, -1.066963e-10, -1.065260e-10, -1.063517e-10, -1.061735e-10, -1.059912e-10, -1.058049e-10, -1.056146e-10,
    -1.054204e-10, -1.052222e-10, -1.050200e-10, -1.048139e-10, -1.046039e-10, -1.043899e-10, -1.041719e-10, -1.039501e-10,
    -1.037243e-10, -1.034946e-10, -1.032610e-10, -1.030236e-10, -1.027823e-10, -1.025370e-10, -1.022880e-10, -1.020351e-10,
    -1.017783e-10, -1.015177e-10, -1.012533e-10, -1.009851e-10, -1.007130e-10, -1.004372e-10, -1.001576e-10, -9.987424e-11,
    -9.958711e-11, -9.929623e-11, -9.900161e-11, -9.870326e-11, -9.840119e-11, -9.809542e-11, -9.778596e-11, -9.747282e-11,
    -9.715601e-11, -9.683553e-11, -9.651142e-11, -9.618367e-11, -9.585229e-11, -9.551731e-11, -9.517874e-11, -9.483658e-11,
    -9.449084e-11, -9.414156e-11, -9.378872e-11, -9.343236e-11, -9.307248e-11, -9.270909e-11, -9.234222e-11, -9.197186e-11,
    -9.159805e-11, -9.122078e-11, -9.084008e-11, -9.045597e-11, -9.006844e-11, -8.967753e-11, -8.928324e-11, -8.888558e-11,
    -8.848458e-11, -8.808025e-11, -8.767260e-11, -8.726166e-11, -8.684742e-11, -8.642992e-11, -8.600916e-11, -8.558517e-11,
    -8.515795e-11, -8.472753e-11, -8.429391e-11, -8.385713e-11, -8.341718e-11, -8.297410e-11, -8.252789e-11, -8.207857e-11,
    -8.162617e-11, -8.117069e-11, -8.071215e-11, -8.025058e-11, -7.978598e-11, -7.931838e-11, -7.884780e-11, -7.837424e-11,
    -7.789774e-11, -7.741830e-11, -7.693595e-11, -7.645070e-11, -7.596257e-11, -7.547158e-11, -7.497775e-11, -7.448110e-11,
    -7.398164e-11, -7.347940e-11, -7.297440e-11, -7.246664e-11, -7.195616e-11, -7.144296e-11, -7.092708e-11, -7.040853e-11,
    -6.988732e-11, -6.936349e-11, -6.883704e-11, -6.830800e-11, -6.777639e-11, -6.724223e-11, -6.670554e-11, -6.616633e-11,
    -6.562464e-11, -6.508047e-11, -6.453386e-11, -6.398481e-11, -6.343335e-11, -6.287951e-11, -6.232330e-11, -6.176474e-11,
    -6.120386e-11, -6.064067e-11, -6.007520e-11, -5.950747e-11, -5.893750e-11, -5.836531e-11, -5.779092e-11, -5.721435e-11,
    -5.663563e-11, -5.605478e-11, -5.547182e-11, -5.488677e-11, -5.429965e-11, -5.371049e-11, -5.311931e-11, -5.252613e-11,
    -5.193097e-11, -5.133385e-11, -5.073480e-11, -5.013384e-11, -4.953100e-11, -4.892629e-11, -4.831973e-11, -4.771136e-11,
    -4.710119e-11, -4.648925e-11, -4.587556e-11, -4.526014e-11, -4.464302e-11, -4.402421e-11, -4.340375e-11, -4.278166e-11,
    -4.215795e-11, -4.153266e-11, -4.090580e-11, -4.027740e-11, -3.964749e-11, -3.901608e-11, -3.838321e-11, -3.774889e-11,
    -3.711315e-11, -3.647601e-11, -3.583749e-11, -3.519763e-11, -3.455645e-11, -3.391396e-11, -3.327019e-11, -3.262518e-11,
    -3.197893e-11, -3.133148e-11, -3.068285e-11, -3.003307e-11, -2.938215e-11, -2.873013e-11, -2.807703e-11, -2.742287e-11,
    -2.676768e-11, -2.611148e-11, -2.545429e-11, -2.479615e-11, -2.413708e-11, -2.347709e-11, -2.281622e-11, -2.215450e-11,
    -2.149194e-11, -2.082857e-11, -2.016441e-11, -1.949950e-11, -1.883385e-11, -1.816749e-11, -1.750045e-11, -1.683275e-11,
    -1.616442e-11, -1.549548e-11, -1.482595e-11, -1.415587e-11, -1.348525e-11, -1.281413e-11, -1.214252e-11, -1.147046e-11,
    -1.079796e-11, -1.012506e-11, -9.451778e-12, -8.778139e-12, -8.104169e-12, -7.429894e-12, -6.755340e-12, -6.080531e-12,
    -5.405493e-12, -4.730252e-12, -4.054832e-12, -3.379260e-12, -2.703561e-12, -2.027760e-12, -1.351882e-12, -6.759538e-13,
};