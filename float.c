#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "format-float.h"

static int g_pass_count = 0;
static int g_fail_count = 0;

#define DO_GFMT_TEST    1
#define DO_GFNUM_TEST   0
#define DO_GBUFTEST     0

#if DO_GFMT_TEST || DO_GFNUM_TEST
const char *gFmt[] = {
#if 0
    "%e",
    "%.1e", "%.2e", "%.3e", "%.4e",
    "%+e",
    "% e",
    "%12.4e",
    "%-12.4e",
    "%+12.4e",
    "% 12.4e",
    "%012.4e",
    "%6.3e",
#endif
    "%5.e",
#if 0
    "%.e",
    "%.1e",
    "%1e", "%2e",
    "%1.1e", "%1.2e", "%1.3e", "%1.4e",
    "%2.1e", "%2.2e", "%2.3e", "%2.4e",
    "%3.1e", "%3.2e", "%3.3e", "%3.4e",
    "%4.1e", "%4.2e", "%4.3e", "%4.4e",
    "%5.1e", "%5.2e", "%5.3e", "%5.4e",
    "%6.1e", "%6.2e", "%6.3e", "%6.4e",
    "%7.1e", "%7.2e", "%7.3e", "%7.4e",
    "%8.1e", "%8.2e", "%8.3e", "%8.4e",
#endif
};
#define NUM_FMT (sizeof(gFmt) / sizeof(gFmt[0]))

float gNum[] = {
#if 0
    0.0F,   // Placeholder for nan
    INFINITY,
    -INFINITY,
    0.0F,
    -0.0F,
    0.1F,
    1.234F,
    12.345F,
    1.23456789F,
    123456789.0F,
    -0.0F,
    -0.1F,
    -1.234F,
    -12.345F,
    1e4F,
    1e-4F,
    1e5F,
    1e-5F,
    1e6F,
    1e-6F,
    1e10F,
    1e37F,
    -1e37F,
    1e-37F,
    -1e-37F,
    1.23456e8, 1.23456e7, 1.23456e6, 1.23456e5, 1.23456e4, 1.23456e3, 1.23456e2, 1.23456e1, 1.23456e0,
    1.23456e-1, 1.23456e-2, 1.23456e-3, 1.23456e-4, 1.23456e-5, 1.23456e-6, 1.23456e-7, 1.23456e-8,
    -1.23456e8, -1.23456e7, -1.23456e6, -1.23456e5, -1.23456e4, -1.23456e3, -1.23456e2, -1.23456e1, -1.23456e0,
    -1.23456e-1, -1.23456e-2, -1.23456e-3, -1.23456e-4, -1.23456e-5, -1.23456e-6, -1.23456e-7, -1.23456e-8,

#endif

    0.09999995,
    9.99999e-6, 9.99999e-5, 9.99999e-4, 9.99999e-3, 9.99999e-2, 9.99999e-1,
    9.99999e0, 9.99999e1, 9.99999e2, 9.99999e3, 9.99999e4, 9.99999e5,
    9.99999e6,
};
#define NUM_NUM (sizeof(gNum) / sizeof(gNum[0]))
#endif // DO_GFMT_TEST

#if DO_GFNUM_TEST
// Since there is really only 6 full digits of significance in 32-bit floats,
// the f style formats need a much more limited range and still hope to compare
// so we create sets of formats and numbers which work together.

static float gFNum[] = {
    0.0F,   // Placeholder for nan
    INFINITY,
    -INFINITY,
    0.0F,
    -0.0F,
    0.0001F, 0.001F, 0.01F, 0.1F, 1.0F, 10.0F,
    0.0012F, 0.0123F, 0.1234F, 1.2345F, 12.3456F,
    -0.0001F, -0.001F, -0.01F, -0.1F, -1.0F, -10.0F,
    -0.0012F, -0.0123F, -0.1234F, -1.2345F, -12.3456F,

    0.09999995,
    9.99999e-6, 9.99999e-5, 9.99999e-4, 9.99999e-3, 9.99999e-2, 9.99999e-1,
    9.99999e0, 9.99999e1, 9.99999e2, 9.99999e3, 9.99999e4, 9.99999e5,
    9.99999e6,
};
#define NUM_FNUM (sizeof(gFNum) / sizeof(gFNum[0]))
#endif

#if DO_GBUFTEST
typedef struct {
    float num;
    char fmt_char;
    char sign;
    int prec;
    size_t buf_size;
    const char *expected;
} BufTest;

static BufTest gBufTest[] = {
    { 0.0F,       'f', '\0', 3, 1, "" },
    { 0.0F,       'f', '\0', 3, 2, "?" },
    { 0.0F,       'f', '\0', 3, 3, "?" },
    { 0.0F,       'f', '\0', 3, 4, "?" },
    { 0.0F,       'f', '\0', 3, 5, "?" },
    { 0.0F,       'f', '\0', 3, 6, "?" },

    { 0.0F,       'f', '\0', 3, 7, "0.000" },
    { 1.0F,       'f', '\0', 3, 7, "1.000" },
    { 1e1F,       'f', '\0', 3, 7, "10.000" },
    { 1e2F,       'f', '\0', 3, 7, "100.00" },
    { 1e3F,       'f', '\0', 3, 7, "1000.0" },
    { 1e4F,       'f', '\0', 3, 7, "10000" },
    { 1e5F,       'f', '\0', 3, 7, "100000" },
    { 1e6F,       'f', '\0', 3, 7, "1e+06" },

    { 0.0F,       'f', ' ', 3, 7, " 0.000" },

    { 0.0F,       'f', '+', 3, 7, "+0.000" },
    { 1.0F,       'f', '+', 3, 7, "+1.000" },
    { 1e1F,       'f', '+', 3, 7, "+10.00" },
    { 1e2F,       'f', '+', 3, 7, "+100.0" },
    { 1e3F,       'f', '+', 3, 7, "+1000" },
    { 1e4F,       'f', '+', 3, 7, "+10000" },
    { 1e5F,       'f', '+', 3, 7, "+1e+05" },
    { 1e6F,       'f', '+', 3, 7, "+1e+06" },

    { -1e1F,      'f', '\0', 3, 7, "-10.00" },
    { -1e2F,      'f', '\0', 3, 7, "-100.0" },
    { -1e3F,      'f', '\0', 3, 7, "-1000" },
    { -1e4F,      'f', '\0', 3, 7, "-10000" },
    { -1e5F,      'f', '\0', 3, 7, "-1e+05" },
    { -1e6F,      'f', '\0', 3, 7, "-1e+06" },
    { 1e-1F,      'f', '\0', 3, 7, "0.100" },
    { 1e-1F,      'f', '\0', 4, 7, "0.1000" },
    { 1e-1F,      'f', '\0', 5, 7, "0.1000" },
    { -1e-1F,     'f', '\0', 3, 7, "-0.100" },
    { -1e-1F,     'f', '\0', 4, 7, "-0.100" },

    { 1.234e-1F,  'e', '\0', 3, 7, "1e-01" },
    { 1.234e-1F,  'e', '\0', 3, 8, "1.2e-01" },
    { 1.234e-1F,  'e', '\0', 3, 9, "1.23e-01" },
    { 1.234e-1F,  'e', '\0', 3, 10, "1.234e-01" },
    { 1.234e-1F,  'e', '\0', 3, 11, "1.234e-01" },

    { 1.234e-1F,  'e', ' ', 3, 7, " 1e-01" },

    { 1.234e-1F,  'e', '+', 3, 7, "+1e-01" },
    { 1.234e-1F,  'e', '+', 3, 8, "+1e-01" },
    { 1.234e-1F,  'e', '+', 3, 9, "+1.2e-01" },
    { 1.234e-1F,  'e', '+', 3, 10, "+1.23e-01" },
    { 1.234e-1F,  'e', '+', 3, 11, "+1.234e-01" },
    { 1.234e-1F,  'e', '+', 3, 12, "+1.234e-01" },

    { -1.234e-1F, 'e', '\0', 3, 7, "-1e-01" },
    { -1.234e-1F, 'e', '\0', 3, 8, "-1e-01" },
    { -1.234e-1F, 'e', '\0', 3, 9, "-1.2e-01" },

    { 1.234e1F,   'e', '\0', 3, 7, "1e+01" },
    { 1.234e1F,   'e', '\0', 3, 8, "1.2e+01" },
    { 1.234e1F,   'e', '\0', 3, 9, "1.23e+01" },

    { -1.234e1F,  'e', '\0', 3, 7, "-1e+01" },
    { -1.234e1F,  'e', '\0', 3, 8, "-1e+01" },
    { -1.234e1F,  'e', '\0', 3, 9, "-1.2e+01" },
    { 1.234e-1F,  'g', '\0', 3, 7, "0.123" },
    { 1.234e-2F,  'g', '\0', 3, 7, "0.0123" },
    { 1.234e-3F,  'g', '\0', 3, 7, "0.0012" },
    { 1.234e-4F,  'g', '\0', 3, 7, "0.0001" },
    { 1.234e-5F,  'g', '\0', 3, 7, "1e-05" },
    { 1.234e-5F,  'g', '\0', 3, 8, "1.2e-05" },
    { 1.234e-5F,  'g', '\0', 3, 9, "1.23e-05" },
    { 1.234e-5F,  'g', '\0', 3, 10, "1.23e-05" },

    { 1.234e-1F,  'g', ' ', 3, 7, " 0.123" },

    { 1.234e-1F,  'g', '+', 3, 7, "+0.123" },
    { 1.234e-2F,  'g', '+', 3, 7, "+0.012" },
    { 1.234e-3F,  'g', '+', 3, 7, "+0.001" },
    { 1.234e-4F,  'g', '+', 3, 7, "+0" },
    { 1.234e-5F,  'g', '+', 3, 7, "+1e-05" },
    { 1.234e-5F,  'g', '+', 3, 8, "+1e-05" },
    { 1.234e-5F,  'g', '+', 3, 9, "+1.2e-05" },
    { 1.234e-5F,  'g', '+', 3, 10, "+1.23e-05" },
    { 1.234e-5F,  'g', '+', 3, 11, "+1.23e-05" },

    { -1.234e-1F, 'g', '\0', 3, 7, "-0.123" },
    { -1.234e-2F, 'g', '\0', 3, 7, "-0.012" },
    { -1.234e-3F, 'g', '\0', 3, 7, "-0.001" },
    { -1.234e-4F, 'g', '\0', 3, 7, "-0" },
    { -1.234e-5F, 'g', '\0', 3, 7, "-1e-05" },
    { -1.234e-5F, 'g', '\0', 3, 8, "-1e-05" },
    { -1.234e-5F, 'g', '\0', 3, 9, "-1.2e-05" },
    { -1.234e-5F, 'g', '\0', 3, 10, "-1.23e-05" },
    { -1.234e-5F, 'g', '\0', 3, 11, "-1.23e-05" },
};
#define NUM_BUF_TEST (sizeof(gBufTest) / sizeof(gBufTest[0]))
#endif

int test_vsnprintf(char *str, size_t size, const char *fmt, va_list ap);
int test_snprintf(char *str, size_t size, const char *fmt, ...);

void test_print(char *buf, int buf_size, const char *fmt, float f)
{
    const char *s = fmt;
    char sign = '\0';
    int left_align = 0;
    int zero_pad = 0;
    int width = -1;
    int prec = 6;
    int len;

    s++;    // skip %

    while (*s) {
        if (*s == '-') {
            left_align = 1;
        } else if (*s == '+') {
            sign = '+';
        } else if (*s == ' ') {
            sign = ' ';
        } else if (*s == '0') {
            zero_pad = 1;
        } else {
            break;
        }
        s++;
    }
    if (isdigit(*s)) {
        char *endptr;
        width = strtoul(s, &endptr, 10);
        s = endptr;
    }
    if (*s == '.') {
        s++;
        if (isdigit(*s)) {
            char *endptr;
            prec = strtoul(s, &endptr, 10);
            s = endptr;
        }
        else {
            prec = 0;
        }
    }
    len = format_float(f, buf, buf_size, *s, prec, sign);

    // buf[0] < '0' returns true if the first character is space, + or -
    // buf[1] < '9' matches a digit, and doesn't match when we get back +nan or +inf
    if (buf[0] < '0' && buf[1] <= '9' && zero_pad) {
        buf++;
        width--;
        len--;
    }
    if (*buf < '0' || *buf >= '9') {
        // For inf or nan, we don't want to zero pad.
        zero_pad = 0;
    }
    if (len >= width) {
        return;
    }
    buf[width] = '\0';
    if (left_align) {
        memset(&buf[len], ' ', width - len);
        return;
    }
    memmove(&buf[width - len], buf, len);
    if (zero_pad) {
        memset(buf, '0', width - len);
    } else {
        memset(buf, ' ', width - len);
    }
}

void DoTest(const char *fmt, float num)
{
    char str[100];
    char str2[100];

    snprintf(str, sizeof(str), fmt, num);
#if 0
    test_snprintf(str2, sizeof(str2), fmt, num);
#else
    test_print(str2, sizeof(str2), fmt, num);
#endif

    if (strcmp(str, str2) == 0) {
        fputs("\x1b[1;32mPASS: ", stdout);
        g_pass_count++;
    } else {
        fputs("\x1b[1;31mFAIL: ", stdout);
        g_fail_count++;
    }
    printf("%-8s >%s< >%s<\x1b[0m\n", fmt, str, str2);
#if 0
    if (g_fail_count) {
        exit(1);
    }
#endif
}

void DoBufTest(float num, char fmt_char, char sign, int prec, size_t buf_size, const char *expected)
{
    char buf[100];
    char *str = &buf[4];
    const char *prefix = "abc[";
    const char *suffix = "]def";
    memcpy(&buf[0], prefix, 4);
    memcpy(&buf[buf_size + 4], suffix, 4);

    format_float(num, str, buf_size, fmt_char, prec, sign);
    if (memcmp(&buf[0], prefix, 4) != 0) {
        fputs("\x1b[1;31mPREFIX FAIL: ", stdout);
        g_fail_count++;
    } else
    if (memcmp(&buf[buf_size + 4], suffix, 4) != 0) {
        fputs("\x1b[1;31mSUFFIX FAIL: ", stdout);
        g_fail_count++;
    } else
    if (strcmp(str, expected) == 0) {
        fputs("\x1b[1;32mPASS: ", stdout);
        g_pass_count++;
    } else {
        fputs("\x1b[1;31mFAIL: ", stdout);
        g_fail_count++;
    }
    printf(">%s< >%s<\x1b[0m\n", str, expected);
}

int main(int argc, char **argv) {
#if DO_GFMT_TEST
    if (gNum[0] == 0.0F) {
        gNum[0] = nanf("");
    }
#endif
#if DO_GFNUM_TEST
    if (gFNum[0] == 0.0F) {
        gFNum[0] = nanf("");
    }
#endif

#if DO_GFMT_TEST
    for (int num_idx = 0; num_idx < NUM_NUM; num_idx++) {
        for (int fmt_idx = 0; fmt_idx < NUM_FMT; fmt_idx++) {
            char *fmt = strdup(gFmt[fmt_idx]);
            char *fmt_char = strchr(fmt, 'e');
            DoTest(fmt, gNum[num_idx]);
            *fmt_char = 'E';
            DoTest(fmt, gNum[num_idx]);
            *fmt_char = 'g';
            DoTest(fmt, gNum[num_idx]);
            *fmt_char = 'G';
            DoTest(fmt, gNum[num_idx]);
            free(fmt);
        }
    }
#endif

#if DO_GFNUM_TEST
    for (int num_idx = 0; num_idx < NUM_FNUM; num_idx++) {
        for (int fmt_idx = 0; fmt_idx < NUM_FMT; fmt_idx++) {
            char *fmt = strdup(gFmt[fmt_idx]);
            char *fmt_char = strchr(fmt, 'e');
            *fmt_char = 'f';
            DoTest(fmt, gFNum[num_idx]);
            free(fmt);
        }
    }
#endif
#if DO_GBUFTEST
    for (BufTest *bt = gBufTest; bt < &gBufTest[NUM_BUF_TEST]; bt++) {
        DoBufTest(bt->num, bt->fmt_char, bt->sign, bt->prec, bt->buf_size, bt->expected);
    }
#endif

    printf("PASS: %d FAIL: %d\n", g_pass_count, g_fail_count);

    {
        float f = 9.999999e6;
        //memcpy(&f, "\xcc\xcc\xcc=", 4);
        char buf[100];
        format_float(f, buf, 9, 'E', 2, 0);
        printf("Got '%s'\n", buf);
        snprintf(buf, sizeof(buf), "%8.2E", f);
        printf("Exp '%s'\n", buf);
    }
    return 0;
}

