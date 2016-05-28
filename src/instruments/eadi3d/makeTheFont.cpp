
#include <cstdio>

unsigned int base = 0;

#define MR2IN   0.026785714
#define IN2MR   (1.0 / MR2IN)

#if 0
#define glNewList(x, y) std::printf("%d,\n", x);
#define glBegin(x) std::printf("{ FONT_BEGIN },\n");
#define glVertex2fv(vec); std::printf("{ FONT_NEXT, %f, %f },\n", vec[0] * IN2MR, vec[1] * IN2MR);
#define glEnd() std::printf("{ FONT_END },\n");
#define glEndList() std::printf("{ FONT_ADVANCE, 6.0, 0.0 },\n\n");
#else
bool begin = false;
bool wasbegin = false;
float savex = 0.0;
float savey = 0.0;
#define glNewList(x, y) std::printf("    {\n        %d,\n", x);
#define glBegin(x) begin = true;
#define glVertex2fv(vec) \
    if (begin) { \
        std::printf("        FONT_BEGIN, %f, %f,\n", vec[0] * IN2MR, vec[1] * IN2MR); \
    begin = false; \
    wasbegin = true; \
    } else { \
    if (!wasbegin) { \
            std::printf("        FONT_NEXT, %f, %f,\n", savex * IN2MR, savey * IN2MR); \
    } \
    wasbegin = false; \
    savex = vec[0]; \
    savey = vec[1]; \
    }
#define glEnd() std::printf("        FONT_END, %f, %f,\n", savex * IN2MR, savey * IN2MR);
#define glEndList() std::printf("        FONT_ADVANCE, 6.0, 0.0\n    },\n\n");
#endif

void make_FT_ZRO(unsigned int obj)
{
    static float    vec_1[2] =
    {0.013393f, 0.013393f};
    static float    vec_2[2] =
    {0.000000f, 0.040179f};
    static float    vec_3[2] =
    {0.000000f, 0.147321f};
    static float    vec_4[2] =
    {0.013393f, 0.174107f};
    static float    vec_5[2] =
    {0.040179f, 0.187500f};
    static float    vec_6[2] =
    {0.066964f, 0.187500f};
    static float    vec_7[2] =
    {0.093750f, 0.174107f};
    static float    vec_8[2] =
    {0.107143f, 0.147321f};
    static float    vec_9[2] =
    {0.107143f, 0.040179f};
    static float    vec_10[2] =
    {0.093750f, 0.013393f};
    static float    vec_11[2] =
    {0.066964f, 0.000000f};
    static float    vec_12[2] =
    {0.040179f, 0.000000f};
    static float    vec_13[2] =
    {0.013393f, 0.013393f};
    static float    vec_14[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_14);
    glEnd();
#endif
    glEndList();
}

void make_FT_1(unsigned int obj)
{
    static float    vec_1[2] =
    {0.026786f, 0.000000f};
    static float    vec_2[2] =
    {0.080357f, 0.000000f};
    static float    vec_3[2] =
    {0.053571f, 0.000000f};
    static float    vec_4[2] =
    {0.053571f, 0.187500f};
    static float    vec_5[2] =
    {0.026786f, 0.133929f};
    static float    vec_6[2] =
    {0.080357f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glEnd();
#endif
    glEndList();
}



void make_FT_2(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.107143f, 0.000000f};
    static float    vec_3[2] =
    {0.000000f, 0.000000f};
    static float    vec_4[2] =
    {0.093750f, 0.107143f};
    static float    vec_5[2] =
    {0.107143f, 0.133929f};
    static float    vec_6[2] =
    {0.107143f, 0.147321f};
    static float    vec_7[2] =
    {0.093750f, 0.174107f};
    static float    vec_8[2] =
    {0.066964f, 0.187500f};
    static float    vec_9[2] =
    {0.040179f, 0.187500f};
    static float    vec_10[2] =
    {0.013393f, 0.174107f};
    static float    vec_11[2] =
    {0.000000f, 0.147321f};
    static float    vec_12[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_12);
    glEnd();
#endif
    glEndList();
}



void make_FT_3(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.040179f};
    static float    vec_2[2] =
    {0.013393f, 0.013393f};
    static float    vec_3[2] =
    {0.040179f, 0.000000f};
    static float    vec_4[2] =
    {0.066964f, 0.000000f};
    static float    vec_5[2] =
    {0.093750f, 0.013393f};
    static float    vec_6[2] =
    {0.107143f, 0.040179f};
    static float    vec_7[2] =
    {0.107143f, 0.066964f};
    static float    vec_8[2] =
    {0.093750f, 0.093750f};
    static float    vec_9[2] =
    {0.066964f, 0.107143f};
    static float    vec_10[2] =
    {0.053571f, 0.107143f};
    static float    vec_11[2] =
    {0.107143f, 0.187500f};
    static float    vec_12[2] =
    {0.000000f, 0.187500f};
    static float    vec_13[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_13);
    glEnd();
#endif
    glEndList();
}



void make_FT_4(unsigned int obj)
{
    static float    vec_1[2] =
    {0.093750f, 0.000000f};
    static float    vec_2[2] =
    {0.093750f, 0.187500f};
    static float    vec_3[2] =
    {0.066964f, 0.174107f};
    static float    vec_4[2] =
    {0.000000f, 0.066964f};
    static float    vec_5[2] =
    {0.107143f, 0.066964f};
    static float    vec_6[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glEnd();
#endif
    glEndList();
}



void make_FT_5(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.026786f};
    static float    vec_2[2] =
    {0.013393f, 0.013393f};
    static float    vec_3[2] =
    {0.040179f, 0.000000f};
    static float    vec_4[2] =
    {0.066964f, 0.000000f};
    static float    vec_5[2] =
    {0.093750f, 0.013393f};
    static float    vec_6[2] =
    {0.107143f, 0.040179f};
    static float    vec_7[2] =
    {0.107143f, 0.080357f};
    static float    vec_8[2] =
    {0.080357f, 0.107143f};
    static float    vec_9[2] =
    {0.026786f, 0.107143f};
    static float    vec_10[2] =
    {0.000000f, 0.080357f};
    static float    vec_11[2] =
    {0.000000f, 0.187500f};
    static float    vec_12[2] =
    {0.107143f, 0.187500f};
    static float    vec_13[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_13);
    glEnd();
#endif
    glEndList();
}



void make_FT_6(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.040179f};
    static float    vec_2[2] =
    {0.013393f, 0.013393f};
    static float    vec_3[2] =
    {0.040179f, 0.000000f};
    static float    vec_4[2] =
    {0.066964f, 0.000000f};
    static float    vec_5[2] =
    {0.093750f, 0.013393f};
    static float    vec_6[2] =
    {0.107143f, 0.040179f};
    static float    vec_7[2] =
    {0.107143f, 0.053571f};
    static float    vec_8[2] =
    {0.093750f, 0.080357f};
    static float    vec_9[2] =
    {0.066964f, 0.093750f};
    static float    vec_10[2] =
    {0.040179f, 0.093750f};
    static float    vec_11[2] =
    {0.013393f, 0.080357f};
    static float    vec_12[2] =
    {0.000000f, 0.053571f};
    static float    vec_13[2] =
    {0.000000f, 0.040179f};
    static float    vec_14[2] =
    {0.000000f, 0.093750f};
    static float    vec_15[2] =
    {0.026786f, 0.160714f};
    static float    vec_16[2] =
    {0.066964f, 0.187500f};
    static float    vec_17[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_13);
    glVertex2fv(vec_14);
    glVertex2fv(vec_15);
    glVertex2fv(vec_16);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_17);
    glEnd();
#endif
    glEndList();
}



void make_FT_7(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.107143f, 0.187500f};
    static float    vec_3[2] =
    {0.000000f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glEnd();
#endif
    glEndList();
}



void make_FT_8(unsigned int obj)
{
    static float    vec_1[2] =
    {0.013393f, 0.013393f};
    static float    vec_2[2] =
    {0.040179f, 0.000000f};
    static float    vec_3[2] =
    {0.066964f, 0.000000f};
    static float    vec_4[2] =
    {0.093750f, 0.013393f};
    static float    vec_5[2] =
    {0.107143f, 0.040179f};
    static float    vec_6[2] =
    {0.107143f, 0.053571f};
    static float    vec_7[2] =
    {0.093750f, 0.080357f};
    static float    vec_8[2] =
    {0.066964f, 0.093750f};
    static float    vec_9[2] =
    {0.040179f, 0.093750f};
    static float    vec_10[2] =
    {0.013393f, 0.107143f};
    static float    vec_11[2] =
    {0.000000f, 0.133929f};
    static float    vec_12[2] =
    {0.000000f, 0.147321f};
    static float    vec_13[2] =
    {0.013393f, 0.174107f};
    static float    vec_14[2] =
    {0.040179f, 0.187500f};
    static float    vec_15[2] =
    {0.066964f, 0.187500f};
    static float    vec_16[2] =
    {0.093750f, 0.174107f};
    static float    vec_17[2] =
    {0.107143f, 0.147321f};
    static float    vec_18[2] =
    {0.107143f, 0.133929f};
    static float    vec_19[2] =
    {0.093750f, 0.107143f};
    static float    vec_20[2] =
    {0.066964f, 0.093750f};
    static float    vec_21[2] =
    {0.040179f, 0.093750f};
    static float    vec_22[2] =
    {0.013393f, 0.080357f};
    static float    vec_23[2] =
    {0.000000f, 0.053571f};
    static float    vec_24[2] =
    {0.000000f, 0.040179f};
    static float    vec_25[2] =
    {0.013393f, 0.013393f};
    static float    vec_26[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glVertex2fv(vec_14);
    glVertex2fv(vec_15);
    glVertex2fv(vec_16);
    glVertex2fv(vec_17);
    glVertex2fv(vec_18);
    glVertex2fv(vec_19);
    glVertex2fv(vec_20);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_21);
    glVertex2fv(vec_22);
    glVertex2fv(vec_23);
    glVertex2fv(vec_24);
    glVertex2fv(vec_25);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_26);
    glEnd();
#endif
    glEndList();
}



void make_FT_9(unsigned int obj)
{
    static float    vec_1[2] =
    {0.040179f, 0.000000f};
    static float    vec_2[2] =
    {0.080357f, 0.026786f};
    static float    vec_3[2] =
    {0.107143f, 0.093750f};
    static float    vec_4[2] =
    {0.107143f, 0.147321f};
    static float    vec_5[2] =
    {0.093750f, 0.174107f};
    static float    vec_6[2] =
    {0.066964f, 0.187500f};
    static float    vec_7[2] =
    {0.040179f, 0.187500f};
    static float    vec_8[2] =
    {0.013393f, 0.174107f};
    static float    vec_9[2] =
    {0.000000f, 0.147321f};
    static float    vec_10[2] =
    {0.000000f, 0.133929f};
    static float    vec_11[2] =
    {0.013393f, 0.107143f};
    static float    vec_12[2] =
    {0.040179f, 0.093750f};
    static float    vec_13[2] =
    {0.066964f, 0.093750f};
    static float    vec_14[2] =
    {0.093750f, 0.107143f};
    static float    vec_15[2] =
    {0.107143f, 0.133929f};
    static float    vec_16[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glVertex2fv(vec_14);
    glVertex2fv(vec_15);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_16);
    glEnd();
#endif
    glEndList();
}



void make_FT_A(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.120536f};
    static float    vec_3[2] =
    {0.026786f, 0.174107f};
    static float    vec_4[2] =
    {0.053571f, 0.187500f};
    static float    vec_5[2] =
    {0.080357f, 0.174107f};
    static float    vec_6[2] =
    {0.107143f, 0.120536f};
    static float    vec_7[2] =
    {0.107143f, 0.000000f};
    static float    vec_8[2] =
    {0.107143f, 0.093750f};
    static float    vec_9[2] =
    {0.000000f, 0.093750f};
    static float    vec_10[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_10);
    glEnd();
#endif
    glEndList();
}



void make_FT_B(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.080357f, 0.000000f};
    static float    vec_3[2] =
    {0.093750f, 0.013393f};
    static float    vec_4[2] =
    {0.107143f, 0.040179f};
    static float    vec_5[2] =
    {0.107143f, 0.053571f};
    static float    vec_6[2] =
    {0.093750f, 0.080357f};
    static float    vec_7[2] =
    {0.080357f, 0.093750f};
    static float    vec_8[2] =
    {0.093750f, 0.107143f};
    static float    vec_9[2] =
    {0.107143f, 0.133929f};
    static float    vec_10[2] =
    {0.107143f, 0.147321f};
    static float    vec_11[2] =
    {0.093750f, 0.174107f};
    static float    vec_12[2] =
    {0.080357f, 0.187500f};
    static float    vec_13[2] =
    {0.000000f, 0.187500f};
    static float    vec_14[2] =
    {0.013393f, 0.187500f};
    static float    vec_15[2] =
    {0.013393f, 0.000000f};
    static float    vec_16[2] =
    {0.013393f, 0.093750f};
    static float    vec_17[2] =
    {0.080357f, 0.093750f};
    static float    vec_18[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_14);
    glVertex2fv(vec_15);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_16);
    glVertex2fv(vec_17);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_18);
    glEnd();
#endif
    glEndList();
}



void make_FT_C(unsigned int obj)
{
    static float    vec_1[2] =
    {0.013393f, 0.013393f};
    static float    vec_2[2] =
    {0.040179f, 0.000000f};
    static float    vec_3[2] =
    {0.066964f, 0.000000f};
    static float    vec_4[2] =
    {0.093750f, 0.013393f};
    static float    vec_5[2] =
    {0.107143f, 0.040179f};
    static float    vec_6[2] =
    {0.107143f, 0.147321f};
    static float    vec_7[2] =
    {0.093750f, 0.174107f};
    static float    vec_8[2] =
    {0.066964f, 0.187500f};
    static float    vec_9[2] =
    {0.040179f, 0.187500f};
    static float    vec_10[2] =
    {0.013393f, 0.174107f};
    static float    vec_11[2] =
    {0.000000f, 0.133929f};
    static float    vec_12[2] =
    {0.000000f, 0.053571f};
    static float    vec_13[2] =
    {0.013393f, 0.013393f};
    static float    vec_14[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_14);
    glEnd();
#endif
    glEndList();
}



void make_FT_D(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.066964f, 0.000000f};
    static float    vec_3[2] =
    {0.093750f, 0.013393f};
    static float    vec_4[2] =
    {0.107143f, 0.040179f};
    static float    vec_5[2] =
    {0.107143f, 0.147321f};
    static float    vec_6[2] =
    {0.093750f, 0.174107f};
    static float    vec_7[2] =
    {0.066964f, 0.187500f};
    static float    vec_8[2] =
    {0.000000f, 0.187500f};
    static float    vec_9[2] =
    {0.013393f, 0.187500f};
    static float    vec_10[2] =
    {0.013393f, 0.000000f};
    static float    vec_11[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_11);
    glEnd();
#endif
    glEndList();
}



void make_FT_E(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.107143f, 0.187500f};
    static float    vec_4[2] =
    {0.080357f, 0.093750f};
    static float    vec_5[2] =
    {0.000000f, 0.093750f};
    static float    vec_6[2] =
    {0.000000f, 0.000000f};
    static float    vec_7[2] =
    {0.107143f, 0.000000f};
    static float    vec_8[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_8);
    glEnd();
#endif
    glEndList();
}



void make_FT_F(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.107143f, 0.187500f};
    static float    vec_4[2] =
    {0.066964f, 0.107143f};
    static float    vec_5[2] =
    {0.000000f, 0.107143f};
    static float    vec_6[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glEnd();
#endif
    glEndList();
}



void make_FT_G(unsigned int obj)
{
    static float    vec_1[2] =
    {0.013393f, 0.013393f};
    static float    vec_2[2] =
    {0.040179f, 0.000000f};
    static float    vec_3[2] =
    {0.066964f, 0.000000f};
    static float    vec_4[2] =
    {0.093750f, 0.013393f};
    static float    vec_5[2] =
    {0.107143f, 0.040179f};
    static float    vec_6[2] =
    {0.107143f, 0.080357f};
    static float    vec_7[2] =
    {0.053571f, 0.080357f};
    static float    vec_8[2] =
    {0.013393f, 0.013393f};
    static float    vec_9[2] =
    {0.000000f, 0.053571f};
    static float    vec_10[2] =
    {0.000000f, 0.133929f};
    static float    vec_11[2] =
    {0.013393f, 0.174107f};
    static float    vec_12[2] =
    {0.040179f, 0.187500f};
    static float    vec_13[2] =
    {0.066964f, 0.187500f};
    static float    vec_14[2] =
    {0.093750f, 0.174107f};
    static float    vec_15[2] =
    {0.107143f, 0.147321f};
    static float    vec_16[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glVertex2fv(vec_14);
    glVertex2fv(vec_15);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_16);
    glEnd();
#endif
    glEndList();
}



void make_FT_H(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.000000f, 0.093750f};
    static float    vec_4[2] =
    {0.107143f, 0.093750f};
    static float    vec_5[2] =
    {0.107143f, 0.187500f};
    static float    vec_6[2] =
    {0.107143f, 0.000000f};
    static float    vec_7[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_7);
    glEnd();
#endif
    glEndList();
}



void make_FT_I(unsigned int obj)
{
    static float    vec_1[2] =
    {0.026786f, 0.000000f};
    static float    vec_2[2] =
    {0.080357f, 0.000000f};
    static float    vec_3[2] =
    {0.053571f, 0.000000f};
    static float    vec_4[2] =
    {0.053571f, 0.187500f};
    static float    vec_5[2] =
    {0.026786f, 0.187500f};
    static float    vec_6[2] =
    {0.080357f, 0.187500f};
    static float    vec_7[2] =
    {0.080357f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_7);
    glEnd();
#endif
    glEndList();
}



void make_FT_J(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.040179f};
    static float    vec_2[2] =
    {0.013393f, 0.013393f};
    static float    vec_3[2] =
    {0.040179f, 0.000000f};
    static float    vec_4[2] =
    {0.066964f, 0.013393f};
    static float    vec_5[2] =
    {0.080357f, 0.040179f};
    static float    vec_6[2] =
    {0.080357f, 0.187500f};
    static float    vec_7[2] =
    {0.080357f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_7);
    glEnd();
#endif
    glEndList();
}



void make_FT_K(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000};
    static float    vec_2[2] =
    {0.000000f, 0.187500};
    static float    vec_3[2] =
    {0.107143f, 0.187500};
    static float    vec_4[2] =
    {0.013393f, 0.093750};
    static float    vec_5[2] =
    {0.000000f, 0.093750};
    static float    vec_6[2] =
    {0.013393f, 0.093750};
    static float    vec_7[2] =
    {0.107143f, 0.000000};
    static float    vec_8[2] =
    {0.107143f, 0.093750};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_8);
    glEnd();
#endif
    glEndList();
}



void make_FT_L(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.107143f, 0.000000f};
    static float    vec_3[2] =
    {0.000000f, 0.000000f};
    static float    vec_4[2] =
    {0.000000f, 0.187500f};
    static float    vec_5[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_M(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.053571f, 0.107143f};
    static float    vec_4[2] =
    {0.107143f, 0.187500f};
    static float    vec_5[2] =
    {0.107143f, 0.000000f};
    static float    vec_6[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glEnd();
#endif
    glEndList();
}



void make_FT_N(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.107143f, 0.000000f};
    static float    vec_4[2] =
    {0.107143f, 0.187500f};
    static float    vec_5[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_O(unsigned int obj)
{
    static float    vec_1[2] =
    {0.026786f, 0.000000f};
    static float    vec_2[2] =
    {0.080357f, 0.000000f};
    static float    vec_3[2] =
    {0.107143f, 0.053571f};
    static float    vec_4[2] =
    {0.107143f, 0.133929f};
    static float    vec_5[2] =
    {0.080357f, 0.187500f};
    static float    vec_6[2] =
    {0.026786f, 0.187500f};
    static float    vec_7[2] =
    {0.000000f, 0.133929f};
    static float    vec_8[2] =
    {0.000000f, 0.053571f};
    static float    vec_9[2] =
    {0.026786f, 0.000000f};
    static float    vec_10[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_10);
    glEnd();
#endif
    glEndList();
}



void make_FT_P(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.066964f, 0.187500f};
    static float    vec_4[2] =
    {0.093750f, 0.174107f};
    static float    vec_5[2] =
    {0.107143f, 0.147321f};
    static float    vec_6[2] =
    {0.107143f, 0.133929f};
    static float    vec_7[2] =
    {0.093750f, 0.107143f};
    static float    vec_8[2] =
    {0.066964f, 0.093750f};
    static float    vec_9[2] =
    {0.000000f, 0.093750f};
    static float    vec_10[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_10);
    glEnd();
#endif
    glEndList();
}



void make_FT_Q(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.040179f};
    static float    vec_2[2] =
    {0.013393f, 0.013393f};
    static float    vec_3[2] =
    {0.040179f, 0.000000f};
    static float    vec_4[2] =
    {0.066964f, 0.000000f};
    static float    vec_5[2] =
    {0.093750f, 0.013393f};
    static float    vec_6[2] =
    {0.107143f, 0.040179f};
    static float    vec_7[2] =
    {0.107143f, 0.147321f};
    static float    vec_8[2] =
    {0.093750f, 0.174107f};
    static float    vec_9[2] =
    {0.066964f, 0.187500f};
    static float    vec_10[2] =
    {0.040179f, 0.187500f};
    static float    vec_11[2] =
    {0.013393f, 0.174107f};
    static float    vec_12[2] =
    {0.000000f, 0.147321f};
    static float    vec_13[2] =
    {0.000000f, 0.040179f};
    static float    vec_14[2] =
    {0.053571f, 0.053571f};
    static float    vec_15[2] =
    {0.107143f, 0.000000f};
    static float    vec_16[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_14);
    glVertex2fv(vec_15);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_16);
    glEnd();
#endif
    glEndList();
}



void make_FT_R(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.066964f, 0.187500f};
    static float    vec_4[2] =
    {0.093750f, 0.174107f};
    static float    vec_5[2] =
    {0.107143f, 0.147321f};
    static float    vec_6[2] =
    {0.107143f, 0.133929f};
    static float    vec_7[2] =
    {0.093750f, 0.107143f};
    static float    vec_8[2] =
    {0.066964f, 0.093750f};
    static float    vec_9[2] =
    {0.000000f, 0.093750f};
    static float    vec_10[2] =
    {0.053571f, 0.093750f};
    static float    vec_11[2] =
    {0.107143f, 0.000000f};
    static float    vec_12[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_12);
    glEnd();
#endif
    glEndList();
}



void make_FT_S(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.040179f};
    static float    vec_2[2] =
    {0.013393f, 0.013393f};
    static float    vec_3[2] =
    {0.040179f, 0.000000f};
    static float    vec_4[2] =
    {0.066964f, 0.000000f};
    static float    vec_5[2] =
    {0.093750f, 0.013393f};
    static float    vec_6[2] =
    {0.107143f, 0.040179f};
    static float    vec_7[2] =
    {0.107143f, 0.053571f};
    static float    vec_8[2] =
    {0.093750f, 0.080357f};
    static float    vec_9[2] =
    {0.066964f, 0.093750f};
    static float    vec_10[2] =
    {0.040179f, 0.093750f};
    static float    vec_11[2] =
    {0.013393f, 0.107143f};
    static float    vec_12[2] =
    {0.000000f, 0.133929f};
    static float    vec_13[2] =
    {0.000000f, 0.147321f};
    static float    vec_14[2] =
    {0.013393f, 0.174107f};
    static float    vec_15[2] =
    {0.040179f, 0.187500f};
    static float    vec_16[2] =
    {0.066964f, 0.187500f};
    static float    vec_17[2] =
    {0.093750f, 0.174107f};
    static float    vec_18[2] =
    {0.107143f, 0.147321f};
    static float    vec_19[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glVertex2fv(vec_10);
    glVertex2fv(vec_11);
    glVertex2fv(vec_12);
    glVertex2fv(vec_13);
    glVertex2fv(vec_14);
    glVertex2fv(vec_15);
    glVertex2fv(vec_16);
    glVertex2fv(vec_17);
    glVertex2fv(vec_18);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_19);
    glEnd();
#endif
    glEndList();
}



void make_FT_T(unsigned int obj)
{
    static float    vec_1[2] =
    {0.053571f, 0.000000};
    static float    vec_2[2] =
    {0.053571f, 0.187500};
    static float    vec_3[2] =
    {0.000000f, 0.187500};
    static float    vec_4[2] =
    {0.107143f, 0.187500};
    static float    vec_5[2] =
    {0.107143f, 0.093750};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_U(unsigned int obj)
{
    static float    vec_1[2] =
    {0.013393f, 0.013393f};
    static float    vec_2[2] =
    {0.000000f, 0.040179f};
    static float    vec_3[2] =
    {0.000000f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.187500f};
    static float    vec_5[2] =
    {0.107143f, 0.040179f};
    static float    vec_6[2] =
    {0.093750f, 0.013393f};
    static float    vec_7[2] =
    {0.066964f, 0.000000f};
    static float    vec_8[2] =
    {0.040179f, 0.000000f};
    static float    vec_9[2] =
    {0.013393f, 0.013393f};
    static float    vec_10[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_10);
    glEnd();
#endif
    glEndList();
}



void make_FT_V(unsigned int obj)
{
    static float    vec_1[2] =
    {0.053571f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.120536f};
    static float    vec_3[2] =
    {0.000000f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.187500f};
    static float    vec_5[2] =
    {0.107143f, 0.120536f};
    static float    vec_6[2] =
    {0.053571f, 0.000000f};
    static float    vec_7[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_7);
    glEnd();
#endif
    glEndList();
}



void make_FT_W(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.187500f};
    static float    vec_3[2] =
    {0.107143f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.000000f};
    static float    vec_5[2] =
    {0.053571f, 0.093750f};
    static float    vec_6[2] =
    {0.000000f, 0.000000f};
    static float    vec_7[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_7);
    glEnd();
#endif
    glEndList();
}



void make_FT_X(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.107143f, 0.187500f};
    static float    vec_3[2] =
    {0.000000f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.000000f};
    static float    vec_5[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_Y(unsigned int obj)
{
    static float    vec_1[2] =
    {0.053571f, 0.000000f};
    static float    vec_2[2] =
    {0.053571f, 0.093750f};
    static float    vec_3[2] =
    {0.000000f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.187500f};
    static float    vec_5[2] =
    {0.053571f, 0.093750f};
    static float    vec_6[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glEnd();
#endif
    glEndList();
}



void make_FT_Z(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.107143f, 0.187500f};
    static float    vec_3[2] =
    {0.000000f, 0.187500f};
    static float    vec_4[2] =
    {0.013393f, 0.093750f};
    static float    vec_5[2] =
    {0.093750f, 0.093750f};
    static float    vec_6[2] =
    {0.107143f, 0.000000f};
    static float    vec_7[2] =
    {0.000000f, 0.000000f};
    static float    vec_8[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_8);
    glEnd();
#endif
    glEndList();
}



void make_FT_AST(unsigned int obj)
{
    static float    vec_1[2] =
    {0.013393f, 0.053571f};
    static float    vec_2[2] =
    {0.093750f, 0.133929f};
    static float    vec_3[2] =
    {0.053571f, 0.147321f};
    static float    vec_4[2] =
    {0.053571f, 0.040179f};
    static float    vec_5[2] =
    {0.093750f, 0.053571f};
    static float    vec_6[2] =
    {0.013393f, 0.133929f};
    static float    vec_7[2] =
    {0.000000f, 0.093750f};
    static float    vec_8[2] =
    {0.107143f, 0.093750f};
    static float    vec_9[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_9);
    glEnd();
#endif
    glEndList();
}



void make_FT_COL(unsigned int obj)
{
    static float    vec_1[2] =
    {0.066964f, 0.026786f};
    static float    vec_2[2] =
    {0.066964f, 0.053571f};
    static float    vec_3[2] =
    {0.066964f, 0.093750f};
    static float    vec_4[2] =
    {0.066964f, 0.120536f};
    static float    vec_5[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_COM(unsigned int obj)
{
    static float    vec_1[2] =
    {0.040179f, 0.000000f};
    static float    vec_2[2] =
    {0.066964f, 0.026786f};
    static float    vec_3[2] =
    {0.066964f, 0.053571f};
    static float    vec_4[2] =
    {0.066964f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glEnd();
#endif
    glEndList();
}



void make_FT_DEG(unsigned int obj)
{
    static float    vec_1[2] =
    {0.026786f, 0.147321f};
    static float    vec_2[2] =
    {0.026786f, 0.174107f};
    static float    vec_3[2] =
    {0.040179f, 0.187500f};
    static float    vec_4[2] =
    {0.066964f, 0.187500f};
    static float    vec_5[2] =
    {0.080357f, 0.174107f};
    static float    vec_6[2] =
    {0.080357f, 0.147321f};
    static float    vec_7[2] =
    {0.066964f, 0.133929f};
    static float    vec_8[2] =
    {0.040179f, 0.133929f};
    static float    vec_9[2] =
    {0.026786f, 0.147321f};
    static float    vec_10[2] =
    {0.080357f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glVertex2fv(vec_9);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_10);
    glEnd();
#endif
    glEndList();
}



void make_FT_EQU(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.066964f};
    static float    vec_2[2] =
    {0.107143f, 0.066964f};
    static float    vec_3[2] =
    {0.107143f, 0.120536f};
    static float    vec_4[2] =
    {0.000000f, 0.120536f};
    static float    vec_5[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_GAP(unsigned int obj)
{
    static float    vec_1[2] =
    {0.053571f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glEnd();
#endif
    glEndList();
}



void make_FT_ID(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.066964f};
    static float    vec_2[2] =
    {0.107143f, 0.066964f};
    static float    vec_3[2] =
    {0.053571f, 0.013393f};
    static float    vec_4[2] =
    {0.000000f, 0.066964f};
    static float    vec_5[2] =
    {0.000000f, 0.133929f};
    static float    vec_6[2] =
    {0.053571f, 0.187500f};
    static float    vec_7[2] =
    {0.107143f, 0.133929f};
    static float    vec_8[2] =
    {0.000000f, 0.133929f};
    static float    vec_9[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glVertex2fv(vec_6);
    glVertex2fv(vec_7);
    glVertex2fv(vec_8);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_9);
    glEnd();
#endif
    glEndList();
}



void make_FT_LTS(unsigned int obj)
{
    static float    vec_1[2] =
    {0.107143f, 0.000000f};
    static float    vec_2[2] =
    {0.000000f, 0.093750f};
    static float    vec_3[2] =
    {0.107143f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glEnd();
#endif
    glEndList();
}



void make_FT_MIN(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.093750};
    static float    vec_2[2] =
    {0.107143f, 0.093750};
    static float    vec_3[2] =
    {0.107143f, 0.093750};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glEnd();
#endif
    glEndList();
}



void make_FT_PER(unsigned int obj)
{
    static float    vec_1[2] =
    {0.040179f, 0.026786f};
    static float    vec_2[2] =
    {0.053571f, 0.053571f};
    static float    vec_3[2] =
    {0.066964f, 0.026786f};
    static float    vec_4[2] =
    {0.040179f, 0.026786f};
    static float    vec_5[2] =
    {0.066964f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_PLU(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.093750f};
    static float    vec_2[2] =
    {0.107143f, 0.093750f};
    static float    vec_3[2] =
    {0.053571f, 0.147321f};
    static float    vec_4[2] =
    {0.053571f, 0.040179f};
    static float    vec_5[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glVertex2fv(vec_4);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_5);
    glEnd();
#endif
    glEndList();
}



void make_FT_RTS(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.107143f, 0.093750f};
    static float    vec_3[2] =
    {0.000000f, 0.187500f};
    static float    vec_4[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glEnd();
#endif
    glEndList();
}



void make_FT_SCL(unsigned int obj)
{
    static float    vec_1[2] =
    {0.040179f, 0.000000f};
    static float    vec_2[2] =
    {0.066964f, 0.026786f};
    static float    vec_3[2] =
    {0.066964f, 0.053571f};
    static float    vec_4[2] =
    {0.066964f, 0.093750f};
    static float    vec_5[2] =
    {0.066964f, 0.120536f};
    static float    vec_6[2] =
    {0.066964f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glVertex2fv(vec_3);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_4);
    glVertex2fv(vec_5);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_6);
    glEnd();
#endif
    glEndList();
}



void make_FT_SLH(unsigned int obj)
{
    static float    vec_1[2] =
    {0.000000f, 0.000000f};
    static float    vec_2[2] =
    {0.107143f, 0.187500f};
    static float    vec_3[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glEnd();
#endif
    glEndList();
}



void make_FT_SPC(unsigned int obj)
{
    static float    vec_1[2] =
    {0.107143f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glEnd();
#endif
    glEndList();
}



void make_FT_SQ(unsigned int obj)
{
    static float    vec_1[2] =
    {0.040179f, 0.133929f};
    static float    vec_2[2] =
    {0.066964f, 0.187500f};
    static float    vec_3[2] =
    {0.066964f, 0.093750f};

    glNewList(base+obj, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_1);
    glVertex2fv(vec_2);
    glEnd();
#if 0
    glBegin(GL_LINE_STRIP);
    glVertex2fv(vec_3);
    glEnd();
#endif
    glEndList();
}

int main()
{
    make_FT_MIN('-');
    make_FT_PER('.');
    make_FT_SLH('/');
    make_FT_ZRO('0');
    make_FT_1('1');
    make_FT_2('2');
    make_FT_3('3');
    make_FT_4('4');
    make_FT_5('5');
    make_FT_6('6');
    make_FT_7('7');
    make_FT_8('8');
    make_FT_9('9');
    make_FT_A('A');
    make_FT_B('B');
    make_FT_C('C');
    make_FT_D('D');
    make_FT_E('E');
    make_FT_F('F');
    make_FT_G('G');
    make_FT_H('H');
    make_FT_I('I');
    make_FT_J('J');
    make_FT_K('K');
    make_FT_L('L');
    make_FT_M('M');
    make_FT_N('N');
    make_FT_O('O');
    make_FT_P('P');
    make_FT_Q('Q');
    make_FT_R('R');
    make_FT_S('S');
    make_FT_T('T');
    make_FT_U('U');
    make_FT_V('V');
    make_FT_W('W');
    make_FT_X('X');
    make_FT_Y('Y');
    make_FT_Z('Z');
#if 0
    make_FT_AST('*');
    make_FT_COL(':');
    make_FT_COM(',');
    make_FT_DEG('^');
    make_FT_EQU('=');
    make_FT_GAP('!');
    make_FT_ID('A');
    make_FT_LTS('[');
    make_FT_MIN('-');
    make_FT_PER('.');
    make_FT_PLU('+');
    make_FT_RTS(']');
    make_FT_SCL('#');
    make_FT_SLH('/');
    make_FT_SPC(' ');
    make_FT_SQ('$');
#endif
}
