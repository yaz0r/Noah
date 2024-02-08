#include "noahLib.h"
#include "pitchTable.h"

std::array<u16, 0xC00> pitchTable2 = { {
    0x2000, 0x2002, 0x2004, 0x2006,
    0x2007, 0x2009, 0x200B, 0x200D,
    0x200F, 0x2011, 0x2013, 0x2014,
    0x2016, 0x2018, 0x201A, 0x201C,
    0x201E, 0x201F, 0x2021, 0x2023,
    0x2025, 0x2027, 0x2029, 0x202B,
    0x202C, 0x202E, 0x2030, 0x2032,
    0x2034, 0x2036, 0x2038, 0x203A,
    0x203B, 0x203D, 0x203F, 0x2041,
    0x2043, 0x2045, 0x2047, 0x2048,
    0x204A, 0x204C, 0x204E, 0x2050,
    0x2052, 0x2054, 0x2055, 0x2057,
    0x2059, 0x205B, 0x205D, 0x205F,
    0x2061, 0x2063, 0x2064, 0x2066,
    0x2068, 0x206A, 0x206C, 0x206E,
    0x2070, 0x2072, 0x2073, 0x2075,
    0x2077, 0x2079, 0x207B, 0x207D,
    0x207F, 0x2081, 0x2082, 0x2084,
    0x2086, 0x2088, 0x208A, 0x208C,
    0x208E, 0x2090, 0x2091, 0x2093,
    0x2095, 0x2097, 0x2099, 0x209B,
    0x209D, 0x209F, 0x20A1, 0x20A2,
    0x20A4, 0x20A6, 0x20A8, 0x20AA,
    0x20AC, 0x20AE, 0x20B0, 0x20B1,
    0x20B3, 0x20B5, 0x20B7, 0x20B9,
    0x20BB, 0x20BD, 0x20BF, 0x20C1,
    0x20C3, 0x20C4, 0x20C6, 0x20C8,
    0x20CA, 0x20CC, 0x20CE, 0x20D0,
    0x20D2, 0x20D4, 0x20D5, 0x20D7,
    0x20D9, 0x20DB, 0x20DD, 0x20DF,
    0x20E1, 0x20E3, 0x20E5, 0x20E7,
    0x20E8, 0x20EA, 0x20EC, 0x20EE,
    0x20F0, 0x20F2, 0x20F4, 0x20F6,
    0x20F8, 0x20FA, 0x20FB, 0x20FD,
    0x20FF, 0x2101, 0x2103, 0x2105,
    0x2107, 0x2109, 0x210B, 0x210D,
    0x210F, 0x2110, 0x2112, 0x2114,
    0x2116, 0x2118, 0x211A, 0x211C,
    0x211E, 0x2120, 0x2122, 0x2124,
    0x2125, 0x2127, 0x2129, 0x212B,
    0x212D, 0x212F, 0x2131, 0x2133,
    0x2135, 0x2137, 0x2139, 0x213B,
    0x213C, 0x213E, 0x2140, 0x2142,
    0x2144, 0x2146, 0x2148, 0x214A,
    0x214C, 0x214E, 0x2150, 0x2152,
    0x2154, 0x2155, 0x2157, 0x2159,
    0x215B, 0x215D, 0x215F, 0x2161,
    0x2163, 0x2165, 0x2167, 0x2169,
    0x216B, 0x216D, 0x216F, 0x2170,
    0x2172, 0x2174, 0x2176, 0x2178,
    0x217A, 0x217C, 0x217E, 0x2180,
    0x2182, 0x2184, 0x2186, 0x2188,
    0x218A, 0x218C, 0x218E, 0x218F,
    0x2191, 0x2193, 0x2195, 0x2197,
    0x2199, 0x219B, 0x219D, 0x219F,
    0x21A1, 0x21A3, 0x21A5, 0x21A7,
    0x21A9, 0x21AB, 0x21AD, 0x21AF,
    0x21B0, 0x21B2, 0x21B4, 0x21B6,
    0x21B8, 0x21BA, 0x21BC, 0x21BE,
    0x21C0, 0x21C2, 0x21C4, 0x21C6,
    0x21C8, 0x21CA, 0x21CC, 0x21CE,
    0x21D0, 0x21D2, 0x21D4, 0x21D6,
    0x21D7, 0x21D9, 0x21DB, 0x21DD,
    0x21DF, 0x21E1, 0x21E3, 0x21E5,
    0x21E7, 0x21E9, 0x21EB, 0x21ED,
    0x21EF, 0x21F1, 0x21F3, 0x21F5,
    0x21F7, 0x21F9, 0x21FB, 0x21FD,
    0x21FF, 0x2201, 0x2203, 0x2205,
    0x2207, 0x2208, 0x220A, 0x220C,
    0x220E, 0x2210, 0x2212, 0x2214,
    0x2216, 0x2218, 0x221A, 0x221C,
    0x221E, 0x2220, 0x2222, 0x2224,
    0x2226, 0x2228, 0x222A, 0x222C,
    0x222E, 0x2230, 0x2232, 0x2234,
    0x2236, 0x2238, 0x223A, 0x223C,
    0x223E, 0x2240, 0x2242, 0x2244,
    0x2246, 0x2248, 0x224A, 0x224C,
    0x224E, 0x2250, 0x2252, 0x2253,
    0x2255, 0x2257, 0x2259, 0x225B,
    0x225D, 0x225F, 0x2261, 0x2263,
    0x2265, 0x2267, 0x2269, 0x226B,
    0x226D, 0x226F, 0x2271, 0x2273,
    0x2275, 0x2277, 0x2279, 0x227B,
    0x227D, 0x227F, 0x2281, 0x2283,
    0x2285, 0x2287, 0x2289, 0x228B,
    0x228D, 0x228F, 0x2291, 0x2293,
    0x2295, 0x2297, 0x2299, 0x229B,
    0x229D, 0x229F, 0x22A1, 0x22A3,
    0x22A5, 0x22A7, 0x22A9, 0x22AB,
    0x22AD, 0x22AF, 0x22B1, 0x22B3,
    0x22B5, 0x22B7, 0x22B9, 0x22BB,
    0x22BD, 0x22BF, 0x22C1, 0x22C3,
    0x22C5, 0x22C7, 0x22C9, 0x22CB,
    0x22CD, 0x22CF, 0x22D1, 0x22D3,
    0x22D5, 0x22D7, 0x22D9, 0x22DB,
    0x22DD, 0x22DF, 0x22E1, 0x22E3,
    0x22E5, 0x22E7, 0x22E9, 0x22EB,
    0x22EE, 0x22F0, 0x22F2, 0x22F4,
    0x22F6, 0x22F8, 0x22FA, 0x22FC,
    0x22FE, 0x2300, 0x2302, 0x2304,
    0x2306, 0x2308, 0x230A, 0x230C,
    0x230E, 0x2310, 0x2312, 0x2314,
    0x2316, 0x2318, 0x231A, 0x231C,
    0x231E, 0x2320, 0x2322, 0x2324,
    0x2326, 0x2328, 0x232A, 0x232C,
    0x232E, 0x2330, 0x2332, 0x2334,
    0x2336, 0x2338, 0x233B, 0x233D,
    0x233F, 0x2341, 0x2343, 0x2345,
    0x2347, 0x2349, 0x234B, 0x234D,
    0x234F, 0x2351, 0x2353, 0x2355,
    0x2357, 0x2359, 0x235B, 0x235D,
    0x235F, 0x2361, 0x2363, 0x2365,
    0x2367, 0x2369, 0x236B, 0x236E,
    0x2370, 0x2372, 0x2374, 0x2376,
    0x2378, 0x237A, 0x237C, 0x237E,
    0x2380, 0x2382, 0x2384, 0x2386,
    0x2388, 0x238A, 0x238C, 0x238E,
    0x2390, 0x2392, 0x2394, 0x2397,
    0x2399, 0x239B, 0x239D, 0x239F,
    0x23A1, 0x23A3, 0x23A5, 0x23A7,
    0x23A9, 0x23AB, 0x23AD, 0x23AF,
    0x23B1, 0x23B3, 0x23B5, 0x23B7,
    0x23BA, 0x23BC, 0x23BE, 0x23C0,
    0x23C2, 0x23C4, 0x23C6, 0x23C8,
    0x23CA, 0x23CC, 0x23CE, 0x23D0,
    0x23D2, 0x23D4, 0x23D6, 0x23D9,
    0x23DB, 0x23DD, 0x23DF, 0x23E1,
    0x23E3, 0x23E5, 0x23E7, 0x23E9,
    0x23EB, 0x23ED, 0x23EF, 0x23F1,
    0x23F4, 0x23F6, 0x23F8, 0x23FA,
    0x23FC, 0x23FE, 0x2400, 0x2402,
    0x2404, 0x2406, 0x2408, 0x240A,
    0x240C, 0x240F, 0x2411, 0x2413,
    0x2415, 0x2417, 0x2419, 0x241B,
    0x241D, 0x241F, 0x2421, 0x2423,
    0x2425, 0x2428, 0x242A, 0x242C,
    0x242E, 0x2430, 0x2432, 0x2434,
    0x2436, 0x2438, 0x243A, 0x243C,
    0x243F, 0x2441, 0x2443, 0x2445,
    0x2447, 0x2449, 0x244B, 0x244D,
    0x244F, 0x2451, 0x2454, 0x2456,
    0x2458, 0x245A, 0x245C, 0x245E,
    0x2460, 0x2462, 0x2464, 0x2466,
    0x2469, 0x246B, 0x246D, 0x246F,
    0x2471, 0x2473, 0x2475, 0x2477,
    0x2479, 0x247B, 0x247E, 0x2480,
    0x2482, 0x2484, 0x2486, 0x2488,
    0x248A, 0x248C, 0x248E, 0x2491,
    0x2493, 0x2495, 0x2497, 0x2499,
    0x249B, 0x249D, 0x249F, 0x24A1,
    0x24A4, 0x24A6, 0x24A8, 0x24AA,
    0x24AC, 0x24AE, 0x24B0, 0x24B2,
    0x24B5, 0x24B7, 0x24B9, 0x24BB,
    0x24BD, 0x24BF, 0x24C1, 0x24C3,
    0x24C6, 0x24C8, 0x24CA, 0x24CC,
    0x24CE, 0x24D0, 0x24D2, 0x24D4,
    0x24D7, 0x24D9, 0x24DB, 0x24DD,
    0x24DF, 0x24E1, 0x24E3, 0x24E5,
    0x24E8, 0x24EA, 0x24EC, 0x24EE,
    0x24F0, 0x24F2, 0x24F4, 0x24F7,
    0x24F9, 0x24FB, 0x24FD, 0x24FF,
    0x2501, 0x2503, 0x2505, 0x2508,
    0x250A, 0x250C, 0x250E, 0x2510,
    0x2512, 0x2514, 0x2517, 0x2519,
    0x251B, 0x251D, 0x251F, 0x2521,
    0x2523, 0x2526, 0x2528, 0x252A,
    0x252C, 0x252E, 0x2530, 0x2532,
    0x2535, 0x2537, 0x2539, 0x253B,
    0x253D, 0x253F, 0x2542, 0x2544,
    0x2546, 0x2548, 0x254A, 0x254C,
    0x254E, 0x2551, 0x2553, 0x2555,
    0x2557, 0x2559, 0x255B, 0x255E,
    0x2560, 0x2562, 0x2564, 0x2566,
    0x2568, 0x256B, 0x256D, 0x256F,
    0x2571, 0x2573, 0x2575, 0x2577,
    0x257A, 0x257C, 0x257E, 0x2580,
    0x2582, 0x2584, 0x2587, 0x2589,
    0x258B, 0x258D, 0x258F, 0x2591,
    0x2594, 0x2596, 0x2598, 0x259A,
    0x259C, 0x259F, 0x25A1, 0x25A3,
    0x25A5, 0x25A7, 0x25A9, 0x25AC,
    0x25AE, 0x25B0, 0x25B2, 0x25B4,
    0x25B6, 0x25B9, 0x25BB, 0x25BD,
    0x25BF, 0x25C1, 0x25C4, 0x25C6,
    0x25C8, 0x25CA, 0x25CC, 0x25CE,
    0x25D1, 0x25D3, 0x25D5, 0x25D7,
    0x25D9, 0x25DC, 0x25DE, 0x25E0,
    0x25E2, 0x25E4, 0x25E6, 0x25E9,
    0x25EB, 0x25ED, 0x25EF, 0x25F1,
    0x25F4, 0x25F6, 0x25F8, 0x25FA,
    0x25FC, 0x25FF, 0x2601, 0x2603,
    0x2605, 0x2607, 0x260A, 0x260C,
    0x260E, 0x2610, 0x2612, 0x2615,
    0x2617, 0x2619, 0x261B, 0x261D,
    0x2620, 0x2622, 0x2624, 0x2626,
    0x2628, 0x262B, 0x262D, 0x262F,
    0x2631, 0x2633, 0x2636, 0x2638,
    0x263A, 0x263C, 0x263E, 0x2641,
    0x2643, 0x2645, 0x2647, 0x264A,
    0x264C, 0x264E, 0x2650, 0x2652,
    0x2655, 0x2657, 0x2659, 0x265B,
    0x265D, 0x2660, 0x2662, 0x2664,
    0x2666, 0x2669, 0x266B, 0x266D,
    0x266F, 0x2671, 0x2674, 0x2676,
    0x2678, 0x267A, 0x267D, 0x267F,
    0x2681, 0x2683, 0x2685, 0x2688,
    0x268A, 0x268C, 0x268E, 0x2691,
    0x2693, 0x2695, 0x2697, 0x2699,
    0x269C, 0x269E, 0x26A0, 0x26A2,
    0x26A5, 0x26A7, 0x26A9, 0x26AB,
    0x26AE, 0x26B0, 0x26B2, 0x26B4,
    0x26B6, 0x26B9, 0x26BB, 0x26BD,
    0x26BF, 0x26C2, 0x26C4, 0x26C6,
    0x26C8, 0x26CB, 0x26CD, 0x26CF,
    0x26D1, 0x26D4, 0x26D6, 0x26D8,
    0x26DA, 0x26DD, 0x26DF, 0x26E1,
    0x26E3, 0x26E6, 0x26E8, 0x26EA,
    0x26EC, 0x26EF, 0x26F1, 0x26F3,
    0x26F5, 0x26F8, 0x26FA, 0x26FC,
    0x26FE, 0x2701, 0x2703, 0x2705,
    0x2707, 0x270A, 0x270C, 0x270E,
    0x2710, 0x2713, 0x2715, 0x2717,
    0x2719, 0x271C, 0x271E, 0x2720,
    0x2722, 0x2725, 0x2727, 0x2729,
    0x272B, 0x272E, 0x2730, 0x2732,
    0x2735, 0x2737, 0x2739, 0x273B,
    0x273E, 0x2740, 0x2742, 0x2744,
    0x2747, 0x2749, 0x274B, 0x274D,
    0x2750, 0x2752, 0x2754, 0x2757,
    0x2759, 0x275B, 0x275D, 0x2760,
    0x2762, 0x2764, 0x2766, 0x2769,
    0x276B, 0x276D, 0x2770, 0x2772,
    0x2774, 0x2776, 0x2779, 0x277B,
    0x277D, 0x2780, 0x2782, 0x2784,
    0x2786, 0x2789, 0x278B, 0x278D,
    0x278F, 0x2792, 0x2794, 0x2796,
    0x2799, 0x279B, 0x279D, 0x27A0,
    0x27A2, 0x27A4, 0x27A6, 0x27A9,
    0x27AB, 0x27AD, 0x27B0, 0x27B2,
    0x27B4, 0x27B6, 0x27B9, 0x27BB,
    0x27BD, 0x27C0, 0x27C2, 0x27C4,
    0x27C6, 0x27C9, 0x27CB, 0x27CD,
    0x27D0, 0x27D2, 0x27D4, 0x27D7,
    0x27D9, 0x27DB, 0x27DD, 0x27E0,
    0x27E2, 0x27E4, 0x27E7, 0x27E9,
    0x27EB, 0x27EE, 0x27F0, 0x27F2,
    0x27F5, 0x27F7, 0x27F9, 0x27FB,
    0x27FE, 0x2800, 0x2802, 0x2805,
    0x2807, 0x2809, 0x280C, 0x280E,
    0x2810, 0x2813, 0x2815, 0x2817,
    0x281A, 0x281C, 0x281E, 0x2820,
    0x2823, 0x2825, 0x2827, 0x282A,
    0x282C, 0x282E, 0x2831, 0x2833,
    0x2835, 0x2838, 0x283A, 0x283C,
    0x283F, 0x2841, 0x2843, 0x2846,
    0x2848, 0x284A, 0x284D, 0x284F,
    0x2851, 0x2854, 0x2856, 0x2858,
    0x285B, 0x285D, 0x285F, 0x2862,
    0x2864, 0x2866, 0x2869, 0x286B,
    0x286D, 0x2870, 0x2872, 0x2874,
    0x2877, 0x2879, 0x287B, 0x287E,
    0x2880, 0x2882, 0x2885, 0x2887,
    0x2889, 0x288C, 0x288E, 0x2890,
    0x2893, 0x2895, 0x2897, 0x289A,
    0x289C, 0x289E, 0x28A1, 0x28A3,
    0x28A5, 0x28A8, 0x28AA, 0x28AC,
    0x28AF, 0x28B1, 0x28B4, 0x28B6,
    0x28B8, 0x28BB, 0x28BD, 0x28BF,
    0x28C2, 0x28C4, 0x28C6, 0x28C9,
    0x28CB, 0x28CD, 0x28D0, 0x28D2,
    0x28D5, 0x28D7, 0x28D9, 0x28DC,
    0x28DE, 0x28E0, 0x28E3, 0x28E5,
    0x28E7, 0x28EA, 0x28EC, 0x28EE,
    0x28F1, 0x28F3, 0x28F6, 0x28F8,
    0x28FA, 0x28FD, 0x28FF, 0x2901,
    0x2904, 0x2906, 0x2909, 0x290B,
    0x290D, 0x2910, 0x2912, 0x2914,
    0x2917, 0x2919, 0x291C, 0x291E,
    0x2920, 0x2923, 0x2925, 0x2927,
    0x292A, 0x292C, 0x292F, 0x2931,
    0x2933, 0x2936, 0x2938, 0x293A,
    0x293D, 0x293F, 0x2942, 0x2944,
    0x2946, 0x2949, 0x294B, 0x294D,
    0x2950, 0x2952, 0x2955, 0x2957,
    0x2959, 0x295C, 0x295E, 0x2961,
    0x2963, 0x2965, 0x2968, 0x296A,
    0x296D, 0x296F, 0x2971, 0x2974,
    0x2976, 0x2979, 0x297B, 0x297D,
    0x2980, 0x2982, 0x2985, 0x2987,
    0x2989, 0x298C, 0x298E, 0x2991,
    0x2993, 0x2995, 0x2998, 0x299A,
    0x299D, 0x299F, 0x29A1, 0x29A4,
    0x29A6, 0x29A9, 0x29AB, 0x29AD,
    0x29B0, 0x29B2, 0x29B5, 0x29B7,
    0x29B9, 0x29BC, 0x29BE, 0x29C1,
    0x29C3, 0x29C5, 0x29C8, 0x29CA,
    0x29CD, 0x29CF, 0x29D2, 0x29D4,
    0x29D6, 0x29D9, 0x29DB, 0x29DE,
    0x29E0, 0x29E2, 0x29E5, 0x29E7,
    0x29EA, 0x29EC, 0x29EF, 0x29F1,
    0x29F3, 0x29F6, 0x29F8, 0x29FB,
    0x29FD, 0x2A00, 0x2A02, 0x2A04,
    0x2A07, 0x2A09, 0x2A0C, 0x2A0E,
    0x2A11, 0x2A13, 0x2A15, 0x2A18,
    0x2A1A, 0x2A1D, 0x2A1F, 0x2A22,
    0x2A24, 0x2A26, 0x2A29, 0x2A2B,
    0x2A2E, 0x2A30, 0x2A33, 0x2A35,
    0x2A37, 0x2A3A, 0x2A3C, 0x2A3F,
    0x2A41, 0x2A44, 0x2A46, 0x2A49,
    0x2A4B, 0x2A4D, 0x2A50, 0x2A52,
    0x2A55, 0x2A57, 0x2A5A, 0x2A5C,
    0x2A5F, 0x2A61, 0x2A63, 0x2A66,
    0x2A68, 0x2A6B, 0x2A6D, 0x2A70,
    0x2A72, 0x2A75, 0x2A77, 0x2A79,
    0x2A7C, 0x2A7E, 0x2A81, 0x2A83,
    0x2A86, 0x2A88, 0x2A8B, 0x2A8D,
    0x2A90, 0x2A92, 0x2A95, 0x2A97,
    0x2A99, 0x2A9C, 0x2A9E, 0x2AA1,
    0x2AA3, 0x2AA6, 0x2AA8, 0x2AAB,
    0x2AAD, 0x2AB0, 0x2AB2, 0x2AB5,
    0x2AB7, 0x2AB9, 0x2ABC, 0x2ABE,
    0x2AC1, 0x2AC3, 0x2AC6, 0x2AC8,
    0x2ACB, 0x2ACD, 0x2AD0, 0x2AD2,
    0x2AD5, 0x2AD7, 0x2ADA, 0x2ADC,
    0x2ADF, 0x2AE1, 0x2AE4, 0x2AE6,
    0x2AE8, 0x2AEB, 0x2AED, 0x2AF0,
    0x2AF2, 0x2AF5, 0x2AF7, 0x2AFA,
    0x2AFC, 0x2AFF, 0x2B01, 0x2B04,
    0x2B06, 0x2B09, 0x2B0B, 0x2B0E,
    0x2B10, 0x2B13, 0x2B15, 0x2B18,
    0x2B1A, 0x2B1D, 0x2B1F, 0x2B22,
    0x2B24, 0x2B27, 0x2B29, 0x2B2C,
    0x2B2E, 0x2B31, 0x2B33, 0x2B36,
    0x2B38, 0x2B3B, 0x2B3D, 0x2B40,
    0x2B42, 0x2B45, 0x2B47, 0x2B4A,
    0x2B4C, 0x2B4F, 0x2B51, 0x2B54,
    0x2B56, 0x2B59, 0x2B5B, 0x2B5E,
    0x2B60, 0x2B63, 0x2B65, 0x2B68,
    0x2B6A, 0x2B6D, 0x2B6F, 0x2B72,
    0x2B74, 0x2B77, 0x2B79, 0x2B7C,
    0x2B7E, 0x2B81, 0x2B83, 0x2B86,
    0x2B88, 0x2B8B, 0x2B8D, 0x2B90,
    0x2B92, 0x2B95, 0x2B97, 0x2B9A,
    0x2B9C, 0x2B9F, 0x2BA1, 0x2BA4,
    0x2BA6, 0x2BA9, 0x2BAB, 0x2BAE,
    0x2BB1, 0x2BB3, 0x2BB6, 0x2BB8,
    0x2BBB, 0x2BBD, 0x2BC0, 0x2BC2,
    0x2BC5, 0x2BC7, 0x2BCA, 0x2BCC,
    0x2BCF, 0x2BD1, 0x2BD4, 0x2BD6,
    0x2BD9, 0x2BDC, 0x2BDE, 0x2BE1,
    0x2BE3, 0x2BE6, 0x2BE8, 0x2BEB,
    0x2BED, 0x2BF0, 0x2BF2, 0x2BF5,
    0x2BF7, 0x2BFA, 0x2BFD, 0x2BFF,
    0x2C02, 0x2C04, 0x2C07, 0x2C09,
    0x2C0C, 0x2C0E, 0x2C11, 0x2C13,
    0x2C16, 0x2C18, 0x2C1B, 0x2C1E,
    0x2C20, 0x2C23, 0x2C25, 0x2C28,
    0x2C2A, 0x2C2D, 0x2C2F, 0x2C32,
    0x2C35, 0x2C37, 0x2C3A, 0x2C3C,
    0x2C3F, 0x2C41, 0x2C44, 0x2C46,
    0x2C49, 0x2C4C, 0x2C4E, 0x2C51,
    0x2C53, 0x2C56, 0x2C58, 0x2C5B,
    0x2C5D, 0x2C60, 0x2C63, 0x2C65,
    0x2C68, 0x2C6A, 0x2C6D, 0x2C6F,
    0x2C72, 0x2C75, 0x2C77, 0x2C7A,
    0x2C7C, 0x2C7F, 0x2C81, 0x2C84,
    0x2C87, 0x2C89, 0x2C8C, 0x2C8E,
    0x2C91, 0x2C93, 0x2C96, 0x2C99,
    0x2C9B, 0x2C9E, 0x2CA0, 0x2CA3,
    0x2CA5, 0x2CA8, 0x2CAB, 0x2CAD,
    0x2CB0, 0x2CB2, 0x2CB5, 0x2CB8,
    0x2CBA, 0x2CBD, 0x2CBF, 0x2CC2,
    0x2CC4, 0x2CC7, 0x2CCA, 0x2CCC,
    0x2CCF, 0x2CD1, 0x2CD4, 0x2CD7,
    0x2CD9, 0x2CDC, 0x2CDE, 0x2CE1,
    0x2CE4, 0x2CE6, 0x2CE9, 0x2CEB,
    0x2CEE, 0x2CF0, 0x2CF3, 0x2CF6,
    0x2CF8, 0x2CFB, 0x2CFD, 0x2D00,
    0x2D03, 0x2D05, 0x2D08, 0x2D0A,
    0x2D0D, 0x2D10, 0x2D12, 0x2D15,
    0x2D17, 0x2D1A, 0x2D1D, 0x2D1F,
    0x2D22, 0x2D25, 0x2D27, 0x2D2A,
    0x2D2C, 0x2D2F, 0x2D32, 0x2D34,
    0x2D37, 0x2D39, 0x2D3C, 0x2D3F,
    0x2D41, 0x2D44, 0x2D46, 0x2D49,
    0x2D4C, 0x2D4E, 0x2D51, 0x2D54,
    0x2D56, 0x2D59, 0x2D5B, 0x2D5E,
    0x2D61, 0x2D63, 0x2D66, 0x2D69,
    0x2D6B, 0x2D6E, 0x2D70, 0x2D73,
    0x2D76, 0x2D78, 0x2D7B, 0x2D7E,
    0x2D80, 0x2D83, 0x2D85, 0x2D88,
    0x2D8B, 0x2D8D, 0x2D90, 0x2D93,
    0x2D95, 0x2D98, 0x2D9A, 0x2D9D,
    0x2DA0, 0x2DA2, 0x2DA5, 0x2DA8,
    0x2DAA, 0x2DAD, 0x2DB0, 0x2DB2,
    0x2DB5, 0x2DB7, 0x2DBA, 0x2DBD,
    0x2DBF, 0x2DC2, 0x2DC5, 0x2DC7,
    0x2DCA, 0x2DCD, 0x2DCF, 0x2DD2,
    0x2DD5, 0x2DD7, 0x2DDA, 0x2DDC,
    0x2DDF, 0x2DE2, 0x2DE4, 0x2DE7,
    0x2DEA, 0x2DEC, 0x2DEF, 0x2DF2,
    0x2DF4, 0x2DF7, 0x2DFA, 0x2DFC,
    0x2DFF, 0x2E02, 0x2E04, 0x2E07,
    0x2E0A, 0x2E0C, 0x2E0F, 0x2E12,
    0x2E14, 0x2E17, 0x2E1A, 0x2E1C,
    0x2E1F, 0x2E22, 0x2E24, 0x2E27,
    0x2E2A, 0x2E2C, 0x2E2F, 0x2E32,
    0x2E34, 0x2E37, 0x2E3A, 0x2E3C,
    0x2E3F, 0x2E42, 0x2E44, 0x2E47,
    0x2E4A, 0x2E4C, 0x2E4F, 0x2E52,
    0x2E54, 0x2E57, 0x2E5A, 0x2E5C,
    0x2E5F, 0x2E62, 0x2E64, 0x2E67,
    0x2E6A, 0x2E6C, 0x2E6F, 0x2E72,
    0x2E74, 0x2E77, 0x2E7A, 0x2E7D,
    0x2E7F, 0x2E82, 0x2E85, 0x2E87,
    0x2E8A, 0x2E8D, 0x2E8F, 0x2E92,
    0x2E95, 0x2E97, 0x2E9A, 0x2E9D,
    0x2E9F, 0x2EA2, 0x2EA5, 0x2EA8,
    0x2EAA, 0x2EAD, 0x2EB0, 0x2EB2,
    0x2EB5, 0x2EB8, 0x2EBA, 0x2EBD,
    0x2EC0, 0x2EC3, 0x2EC5, 0x2EC8,
    0x2ECB, 0x2ECD, 0x2ED0, 0x2ED3,
    0x2ED5, 0x2ED8, 0x2EDB, 0x2EDE,
    0x2EE0, 0x2EE3, 0x2EE6, 0x2EE8,
    0x2EEB, 0x2EEE, 0x2EF1, 0x2EF3,
    0x2EF6, 0x2EF9, 0x2EFB, 0x2EFE,
    0x2F01, 0x2F04, 0x2F06, 0x2F09,
    0x2F0C, 0x2F0E, 0x2F11, 0x2F14,
    0x2F17, 0x2F19, 0x2F1C, 0x2F1F,
    0x2F21, 0x2F24, 0x2F27, 0x2F2A,
    0x2F2C, 0x2F2F, 0x2F32, 0x2F35,
    0x2F37, 0x2F3A, 0x2F3D, 0x2F3F,
    0x2F42, 0x2F45, 0x2F48, 0x2F4A,
    0x2F4D, 0x2F50, 0x2F53, 0x2F55,
    0x2F58, 0x2F5B, 0x2F5D, 0x2F60,
    0x2F63, 0x2F66, 0x2F68, 0x2F6B,
    0x2F6E, 0x2F71, 0x2F73, 0x2F76,
    0x2F79, 0x2F7C, 0x2F7E, 0x2F81,
    0x2F84, 0x2F87, 0x2F89, 0x2F8C,
    0x2F8F, 0x2F92, 0x2F94, 0x2F97,
    0x2F9A, 0x2F9D, 0x2F9F, 0x2FA2,
    0x2FA5, 0x2FA8, 0x2FAA, 0x2FAD,
    0x2FB0, 0x2FB3, 0x2FB5, 0x2FB8,
    0x2FBB, 0x2FBE, 0x2FC0, 0x2FC3,
    0x2FC6, 0x2FC9, 0x2FCB, 0x2FCE,
    0x2FD1, 0x2FD4, 0x2FD6, 0x2FD9,
    0x2FDC, 0x2FDF, 0x2FE2, 0x2FE4,
    0x2FE7, 0x2FEA, 0x2FED, 0x2FEF,
    0x2FF2, 0x2FF5, 0x2FF8, 0x2FFA,
    0x2FFD, 0x3000, 0x3003, 0x3006,
    0x3008, 0x300B, 0x300E, 0x3011,
    0x3013, 0x3016, 0x3019, 0x301C,
    0x301F, 0x3021, 0x3024, 0x3027,
    0x302A, 0x302C, 0x302F, 0x3032,
    0x3035, 0x3038, 0x303A, 0x303D,
    0x3040, 0x3043, 0x3045, 0x3048,
    0x304B, 0x304E, 0x3051, 0x3053,
    0x3056, 0x3059, 0x305C, 0x305F,
    0x3061, 0x3064, 0x3067, 0x306A,
    0x306D, 0x306F, 0x3072, 0x3075,
    0x3078, 0x307B, 0x307D, 0x3080,
    0x3083, 0x3086, 0x3089, 0x308B,
    0x308E, 0x3091, 0x3094, 0x3097,
    0x3099, 0x309C, 0x309F, 0x30A2,
    0x30A5, 0x30A7, 0x30AA, 0x30AD,
    0x30B0, 0x30B3, 0x30B6, 0x30B8,
    0x30BB, 0x30BE, 0x30C1, 0x30C4,
    0x30C6, 0x30C9, 0x30CC, 0x30CF,
    0x30D2, 0x30D5, 0x30D7, 0x30DA,
    0x30DD, 0x30E0, 0x30E3, 0x30E5,
    0x30E8, 0x30EB, 0x30EE, 0x30F1,
    0x30F4, 0x30F6, 0x30F9, 0x30FC,
    0x30FF, 0x3102, 0x3105, 0x3107,
    0x310A, 0x310D, 0x3110, 0x3113,
    0x3116, 0x3118, 0x311B, 0x311E,
    0x3121, 0x3124, 0x3127, 0x3129,
    0x312C, 0x312F, 0x3132, 0x3135,
    0x3138, 0x313A, 0x313D, 0x3140,
    0x3143, 0x3146, 0x3149, 0x314C,
    0x314E, 0x3151, 0x3154, 0x3157,
    0x315A, 0x315D, 0x315F, 0x3162,
    0x3165, 0x3168, 0x316B, 0x316E,
    0x3171, 0x3173, 0x3176, 0x3179,
    0x317C, 0x317F, 0x3182, 0x3185,
    0x3187, 0x318A, 0x318D, 0x3190,
    0x3193, 0x3196, 0x3199, 0x319C,
    0x319E, 0x31A1, 0x31A4, 0x31A7,
    0x31AA, 0x31AD, 0x31B0, 0x31B2,
    0x31B5, 0x31B8, 0x31BB, 0x31BE,
    0x31C1, 0x31C4, 0x31C7, 0x31C9,
    0x31CC, 0x31CF, 0x31D2, 0x31D5,
    0x31D8, 0x31DB, 0x31DE, 0x31E0,
    0x31E3, 0x31E6, 0x31E9, 0x31EC,
    0x31EF, 0x31F2, 0x31F5, 0x31F8,
    0x31FA, 0x31FD, 0x3200, 0x3203,
    0x3206, 0x3209, 0x320C, 0x320F,
    0x3212, 0x3214, 0x3217, 0x321A,
    0x321D, 0x3220, 0x3223, 0x3226,
    0x3229, 0x322C, 0x322F, 0x3231,
    0x3234, 0x3237, 0x323A, 0x323D,
    0x3240, 0x3243, 0x3246, 0x3249,
    0x324C, 0x324E, 0x3251, 0x3254,
    0x3257, 0x325A, 0x325D, 0x3260,
    0x3263, 0x3266, 0x3269, 0x326C,
    0x326E, 0x3271, 0x3274, 0x3277,
    0x327A, 0x327D, 0x3280, 0x3283,
    0x3286, 0x3289, 0x328C, 0x328F,
    0x3291, 0x3294, 0x3297, 0x329A,
    0x329D, 0x32A0, 0x32A3, 0x32A6,
    0x32A9, 0x32AC, 0x32AF, 0x32B2,
    0x32B5, 0x32B7, 0x32BA, 0x32BD,
    0x32C0, 0x32C3, 0x32C6, 0x32C9,
    0x32CC, 0x32CF, 0x32D2, 0x32D5,
    0x32D8, 0x32DB, 0x32DE, 0x32E1,
    0x32E3, 0x32E6, 0x32E9, 0x32EC,
    0x32EF, 0x32F2, 0x32F5, 0x32F8,
    0x32FB, 0x32FE, 0x3301, 0x3304,
    0x3307, 0x330A, 0x330D, 0x3310,
    0x3313, 0x3316, 0x3319, 0x331B,
    0x331E, 0x3321, 0x3324, 0x3327,
    0x332A, 0x332D, 0x3330, 0x3333,
    0x3336, 0x3339, 0x333C, 0x333F,
    0x3342, 0x3345, 0x3348, 0x334B,
    0x334E, 0x3351, 0x3354, 0x3357,
    0x335A, 0x335D, 0x3360, 0x3362,
    0x3365, 0x3368, 0x336B, 0x336E,
    0x3371, 0x3374, 0x3377, 0x337A,
    0x337D, 0x3380, 0x3383, 0x3386,
    0x3389, 0x338C, 0x338F, 0x3392,
    0x3395, 0x3398, 0x339B, 0x339E,
    0x33A1, 0x33A4, 0x33A7, 0x33AA,
    0x33AD, 0x33B0, 0x33B3, 0x33B6,
    0x33B9, 0x33BC, 0x33BF, 0x33C2,
    0x33C5, 0x33C8, 0x33CB, 0x33CE,
    0x33D1, 0x33D4, 0x33D7, 0x33DA,
    0x33DD, 0x33E0, 0x33E3, 0x33E6,
    0x33E9, 0x33EC, 0x33EF, 0x33F2,
    0x33F5, 0x33F8, 0x33FB, 0x33FE,
    0x3401, 0x3404, 0x3407, 0x340A,
    0x340D, 0x3410, 0x3413, 0x3416,
    0x3419, 0x341C, 0x341F, 0x3422,
    0x3425, 0x3428, 0x342B, 0x342E,
    0x3431, 0x3434, 0x3437, 0x343A,
    0x343D, 0x3440, 0x3443, 0x3446,
    0x3449, 0x344C, 0x344F, 0x3452,
    0x3455, 0x3458, 0x345B, 0x345E,
    0x3461, 0x3464, 0x3467, 0x346A,
    0x346D, 0x3470, 0x3473, 0x3476,
    0x3479, 0x347C, 0x3480, 0x3483,
    0x3486, 0x3489, 0x348C, 0x348F,
    0x3492, 0x3495, 0x3498, 0x349B,
    0x349E, 0x34A1, 0x34A4, 0x34A7,
    0x34AA, 0x34AD, 0x34B0, 0x34B3,
    0x34B6, 0x34B9, 0x34BC, 0x34BF,
    0x34C2, 0x34C5, 0x34C8, 0x34CC,
    0x34CF, 0x34D2, 0x34D5, 0x34D8,
    0x34DB, 0x34DE, 0x34E1, 0x34E4,
    0x34E7, 0x34EA, 0x34ED, 0x34F0,
    0x34F3, 0x34F6, 0x34F9, 0x34FC,
    0x34FF, 0x3503, 0x3506, 0x3509,
    0x350C, 0x350F, 0x3512, 0x3515,
    0x3518, 0x351B, 0x351E, 0x3521,
    0x3524, 0x3527, 0x352A, 0x352D,
    0x3531, 0x3534, 0x3537, 0x353A,
    0x353D, 0x3540, 0x3543, 0x3546,
    0x3549, 0x354C, 0x354F, 0x3552,
    0x3555, 0x3559, 0x355C, 0x355F,
    0x3562, 0x3565, 0x3568, 0x356B,
    0x356E, 0x3571, 0x3574, 0x3577,
    0x357A, 0x357E, 0x3581, 0x3584,
    0x3587, 0x358A, 0x358D, 0x3590,
    0x3593, 0x3596, 0x3599, 0x359D,
    0x35A0, 0x35A3, 0x35A6, 0x35A9,
    0x35AC, 0x35AF, 0x35B2, 0x35B5,
    0x35B8, 0x35BC, 0x35BF, 0x35C2,
    0x35C5, 0x35C8, 0x35CB, 0x35CE,
    0x35D1, 0x35D4, 0x35D7, 0x35DB,
    0x35DE, 0x35E1, 0x35E4, 0x35E7,
    0x35EA, 0x35ED, 0x35F0, 0x35F3,
    0x35F7, 0x35FA, 0x35FD, 0x3600,
    0x3603, 0x3606, 0x3609, 0x360C,
    0x3610, 0x3613, 0x3616, 0x3619,
    0x361C, 0x361F, 0x3622, 0x3625,
    0x3629, 0x362C, 0x362F, 0x3632,
    0x3635, 0x3638, 0x363B, 0x363E,
    0x3642, 0x3645, 0x3648, 0x364B,
    0x364E, 0x3651, 0x3654, 0x3658,
    0x365B, 0x365E, 0x3661, 0x3664,
    0x3667, 0x366A, 0x366E, 0x3671,
    0x3674, 0x3677, 0x367A, 0x367D,
    0x3680, 0x3684, 0x3687, 0x368A,
    0x368D, 0x3690, 0x3693, 0x3696,
    0x369A, 0x369D, 0x36A0, 0x36A3,
    0x36A6, 0x36A9, 0x36AD, 0x36B0,
    0x36B3, 0x36B6, 0x36B9, 0x36BC,
    0x36C0, 0x36C3, 0x36C6, 0x36C9,
    0x36CC, 0x36CF, 0x36D3, 0x36D6,
    0x36D9, 0x36DC, 0x36DF, 0x36E2,
    0x36E6, 0x36E9, 0x36EC, 0x36EF,
    0x36F2, 0x36F5, 0x36F9, 0x36FC,
    0x36FF, 0x3702, 0x3705, 0x3708,
    0x370C, 0x370F, 0x3712, 0x3715,
    0x3718, 0x371C, 0x371F, 0x3722,
    0x3725, 0x3728, 0x372B, 0x372F,
    0x3732, 0x3735, 0x3738, 0x373B,
    0x373F, 0x3742, 0x3745, 0x3748,
    0x374B, 0x374F, 0x3752, 0x3755,
    0x3758, 0x375B, 0x375F, 0x3762,
    0x3765, 0x3768, 0x376B, 0x376F,
    0x3772, 0x3775, 0x3778, 0x377B,
    0x377F, 0x3782, 0x3785, 0x3788,
    0x378B, 0x378F, 0x3792, 0x3795,
    0x3798, 0x379B, 0x379F, 0x37A2,
    0x37A5, 0x37A8, 0x37AC, 0x37AF,
    0x37B2, 0x37B5, 0x37B8, 0x37BC,
    0x37BF, 0x37C2, 0x37C5, 0x37C8,
    0x37CC, 0x37CF, 0x37D2, 0x37D5,
    0x37D9, 0x37DC, 0x37DF, 0x37E2,
    0x37E6, 0x37E9, 0x37EC, 0x37EF,
    0x37F2, 0x37F6, 0x37F9, 0x37FC,
    0x37FF, 0x3803, 0x3806, 0x3809,
    0x380C, 0x3810, 0x3813, 0x3816,
    0x3819, 0x381D, 0x3820, 0x3823,
    0x3826, 0x3829, 0x382D, 0x3830,
    0x3833, 0x3836, 0x383A, 0x383D,
    0x3840, 0x3843, 0x3847, 0x384A,
    0x384D, 0x3850, 0x3854, 0x3857,
    0x385A, 0x385D, 0x3861, 0x3864,
    0x3867, 0x386B, 0x386E, 0x3871,
    0x3874, 0x3878, 0x387B, 0x387E,
    0x3881, 0x3885, 0x3888, 0x388B,
    0x388E, 0x3892, 0x3895, 0x3898,
    0x389B, 0x389F, 0x38A2, 0x38A5,
    0x38A9, 0x38AC, 0x38AF, 0x38B2,
    0x38B6, 0x38B9, 0x38BC, 0x38BF,
    0x38C3, 0x38C6, 0x38C9, 0x38CD,
    0x38D0, 0x38D3, 0x38D6, 0x38DA,
    0x38DD, 0x38E0, 0x38E4, 0x38E7,
    0x38EA, 0x38ED, 0x38F1, 0x38F4,
    0x38F7, 0x38FB, 0x38FE, 0x3901,
    0x3904, 0x3908, 0x390B, 0x390E,
    0x3912, 0x3915, 0x3918, 0x391C,
    0x391F, 0x3922, 0x3925, 0x3929,
    0x392C, 0x392F, 0x3933, 0x3936,
    0x3939, 0x393D, 0x3940, 0x3943,
    0x3947, 0x394A, 0x394D, 0x3950,
    0x3954, 0x3957, 0x395A, 0x395E,
    0x3961, 0x3964, 0x3968, 0x396B,
    0x396E, 0x3972, 0x3975, 0x3978,
    0x397C, 0x397F, 0x3982, 0x3985,
    0x3989, 0x398C, 0x398F, 0x3993,
    0x3996, 0x3999, 0x399D, 0x39A0,
    0x39A3, 0x39A7, 0x39AA, 0x39AD,
    0x39B1, 0x39B4, 0x39B7, 0x39BB,
    0x39BE, 0x39C1, 0x39C5, 0x39C8,
    0x39CB, 0x39CF, 0x39D2, 0x39D5,
    0x39D9, 0x39DC, 0x39DF, 0x39E3,
    0x39E6, 0x39EA, 0x39ED, 0x39F0,
    0x39F4, 0x39F7, 0x39FA, 0x39FE,
    0x3A01, 0x3A04, 0x3A08, 0x3A0B,
    0x3A0E, 0x3A12, 0x3A15, 0x3A18,
    0x3A1C, 0x3A1F, 0x3A22, 0x3A26,
    0x3A29, 0x3A2D, 0x3A30, 0x3A33,
    0x3A37, 0x3A3A, 0x3A3D, 0x3A41,
    0x3A44, 0x3A47, 0x3A4B, 0x3A4E,
    0x3A52, 0x3A55, 0x3A58, 0x3A5C,
    0x3A5F, 0x3A62, 0x3A66, 0x3A69,
    0x3A6D, 0x3A70, 0x3A73, 0x3A77,
    0x3A7A, 0x3A7D, 0x3A81, 0x3A84,
    0x3A88, 0x3A8B, 0x3A8E, 0x3A92,
    0x3A95, 0x3A98, 0x3A9C, 0x3A9F,
    0x3AA3, 0x3AA6, 0x3AA9, 0x3AAD,
    0x3AB0, 0x3AB4, 0x3AB7, 0x3ABA,
    0x3ABE, 0x3AC1, 0x3AC5, 0x3AC8,
    0x3ACB, 0x3ACF, 0x3AD2, 0x3AD6,
    0x3AD9, 0x3ADC, 0x3AE0, 0x3AE3,
    0x3AE7, 0x3AEA, 0x3AED, 0x3AF1,
    0x3AF4, 0x3AF8, 0x3AFB, 0x3AFE,
    0x3B02, 0x3B05, 0x3B09, 0x3B0C,
    0x3B0F, 0x3B13, 0x3B16, 0x3B1A,
    0x3B1D, 0x3B20, 0x3B24, 0x3B27,
    0x3B2B, 0x3B2E, 0x3B32, 0x3B35,
    0x3B38, 0x3B3C, 0x3B3F, 0x3B43,
    0x3B46, 0x3B4A, 0x3B4D, 0x3B50,
    0x3B54, 0x3B57, 0x3B5B, 0x3B5E,
    0x3B62, 0x3B65, 0x3B68, 0x3B6C,
    0x3B6F, 0x3B73, 0x3B76, 0x3B7A,
    0x3B7D, 0x3B80, 0x3B84, 0x3B87,
    0x3B8B, 0x3B8E, 0x3B92, 0x3B95,
    0x3B98, 0x3B9C, 0x3B9F, 0x3BA3,
    0x3BA6, 0x3BAA, 0x3BAD, 0x3BB1,
    0x3BB4, 0x3BB8, 0x3BBB, 0x3BBE,
    0x3BC2, 0x3BC5, 0x3BC9, 0x3BCC,
    0x3BD0, 0x3BD3, 0x3BD7, 0x3BDA,
    0x3BDD, 0x3BE1, 0x3BE4, 0x3BE8,
    0x3BEB, 0x3BEF, 0x3BF2, 0x3BF6,
    0x3BF9, 0x3BFD, 0x3C00, 0x3C04,
    0x3C07, 0x3C0B, 0x3C0E, 0x3C11,
    0x3C15, 0x3C18, 0x3C1C, 0x3C1F,
    0x3C23, 0x3C26, 0x3C2A, 0x3C2D,
    0x3C31, 0x3C34, 0x3C38, 0x3C3B,
    0x3C3F, 0x3C42, 0x3C46, 0x3C49,
    0x3C4D, 0x3C50, 0x3C54, 0x3C57,
    0x3C5A, 0x3C5E, 0x3C61, 0x3C65,
    0x3C68, 0x3C6C, 0x3C6F, 0x3C73,
    0x3C76, 0x3C7A, 0x3C7D, 0x3C81,
    0x3C84, 0x3C88, 0x3C8B, 0x3C8F,
    0x3C92, 0x3C96, 0x3C99, 0x3C9D,
    0x3CA0, 0x3CA4, 0x3CA7, 0x3CAB,
    0x3CAE, 0x3CB2, 0x3CB5, 0x3CB9,
    0x3CBC, 0x3CC0, 0x3CC3, 0x3CC7,
    0x3CCA, 0x3CCE, 0x3CD1, 0x3CD5,
    0x3CD8, 0x3CDC, 0x3CE0, 0x3CE3,
    0x3CE7, 0x3CEA, 0x3CEE, 0x3CF1,
    0x3CF5, 0x3CF8, 0x3CFC, 0x3CFF,
    0x3D03, 0x3D06, 0x3D0A, 0x3D0D,
    0x3D11, 0x3D14, 0x3D18, 0x3D1B,
    0x3D1F, 0x3D22, 0x3D26, 0x3D2A,
    0x3D2D, 0x3D31, 0x3D34, 0x3D38,
    0x3D3B, 0x3D3F, 0x3D42, 0x3D46,
    0x3D49, 0x3D4D, 0x3D50, 0x3D54,
    0x3D58, 0x3D5B, 0x3D5F, 0x3D62,
    0x3D66, 0x3D69, 0x3D6D, 0x3D70,
    0x3D74, 0x3D77, 0x3D7B, 0x3D7F,
    0x3D82, 0x3D86, 0x3D89, 0x3D8D,
    0x3D90, 0x3D94, 0x3D97, 0x3D9B,
    0x3D9F, 0x3DA2, 0x3DA6, 0x3DA9,
    0x3DAD, 0x3DB0, 0x3DB4, 0x3DB7,
    0x3DBB, 0x3DBF, 0x3DC2, 0x3DC6,
    0x3DC9, 0x3DCD, 0x3DD0, 0x3DD4,
    0x3DD8, 0x3DDB, 0x3DDF, 0x3DE2,
    0x3DE6, 0x3DE9, 0x3DED, 0x3DF1,
    0x3DF4, 0x3DF8, 0x3DFB, 0x3DFF,
    0x3E03, 0x3E06, 0x3E0A, 0x3E0D,
    0x3E11, 0x3E14, 0x3E18, 0x3E1C,
    0x3E1F, 0x3E23, 0x3E26, 0x3E2A,
    0x3E2E, 0x3E31, 0x3E35, 0x3E38,
    0x3E3C, 0x3E40, 0x3E43, 0x3E47,
    0x3E4A, 0x3E4E, 0x3E52, 0x3E55,
    0x3E59, 0x3E5C, 0x3E60, 0x3E64,
    0x3E67, 0x3E6B, 0x3E6E, 0x3E72,
    0x3E76, 0x3E79, 0x3E7D, 0x3E80,
    0x3E84, 0x3E88, 0x3E8B, 0x3E8F,
    0x3E92, 0x3E96, 0x3E9A, 0x3E9D,
    0x3EA1, 0x3EA5, 0x3EA8, 0x3EAC,
    0x3EAF, 0x3EB3, 0x3EB7, 0x3EBA,
    0x3EBE, 0x3EC2, 0x3EC5, 0x3EC9,
    0x3ECC, 0x3ED0, 0x3ED4, 0x3ED7,
    0x3EDB, 0x3EDF, 0x3EE2, 0x3EE6,
    0x3EE9, 0x3EED, 0x3EF1, 0x3EF4,
    0x3EF8, 0x3EFC, 0x3EFF, 0x3F03,
    0x3F07, 0x3F0A, 0x3F0E, 0x3F11,
    0x3F15, 0x3F19, 0x3F1C, 0x3F20,
    0x3F24, 0x3F27, 0x3F2B, 0x3F2F,
    0x3F32, 0x3F36, 0x3F3A, 0x3F3D,
    0x3F41, 0x3F45, 0x3F48, 0x3F4C,
    0x3F50, 0x3F53, 0x3F57, 0x3F5A,
    0x3F5E, 0x3F62, 0x3F65, 0x3F69,
    0x3F6D, 0x3F70, 0x3F74, 0x3F78,
    0x3F7B, 0x3F7F, 0x3F83, 0x3F86,
    0x3F8A, 0x3F8E, 0x3F91, 0x3F95,
    0x3F99, 0x3F9C, 0x3FA0, 0x3FA4,
    0x3FA8, 0x3FAB, 0x3FAF, 0x3FB3,
    0x3FB6, 0x3FBA, 0x3FBE, 0x3FC1,
    0x3FC5, 0x3FC9, 0x3FCC, 0x3FD0,
    0x3FD4, 0x3FD7, 0x3FDB, 0x3FDF,
    0x3FE2, 0x3FE6, 0x3FEA, 0x3FEE,
    0x3FF1, 0x3FF5, 0x3FF9, 0x3FFC
} };