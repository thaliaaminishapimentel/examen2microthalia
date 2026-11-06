/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include "../src/alarm_core.h"

static uint16_t adc_for_pct(unsigned p)
{
    return (uint16_t)((p * 65535U + 50U) / 100U);
}

int main(void)
{
    alarm_core_t a;
    alarm_debounce_t d;
    uint8_t thr = 0;
    alarm_init(&a);
    assert(a.running && a.threshold == 70 && !a.alarm && !a.has_sample);
    assert(alarm_pct16(0) == 0 && alarm_pct16(65535) == 100);
    assert(alarm_pct16(32768) == 50);

    alarm_sample(&a, adc_for_pct(60)); assert(!a.alarm);
    alarm_sample(&a, adc_for_pct(72)); assert(a.alarm);
    alarm_sample(&a, adc_for_pct(67)); assert(a.alarm);
    alarm_sample(&a, adc_for_pct(65)); assert(a.alarm);
    alarm_sample(&a, adc_for_pct(64)); assert(!a.alarm);

    alarm_sample(&a, adc_for_pct(80)); assert(a.alarm);
    alarm_stop(&a); assert(!a.running && !a.alarm);
    alarm_sample(&a, adc_for_pct(90)); assert(!a.alarm);
    alarm_start(&a); assert(a.running && a.alarm);
    alarm_start(&a); assert(a.alarm); /* idempotent START */
    alarm_set_thr(&a, 95); assert(!a.alarm);
    alarm_set_thr(&a, 10); assert(a.alarm);
    alarm_stop(&a);
    alarm_set_thr(&a, 95); assert(!a.alarm);
    alarm_start(&a); assert(!a.alarm);

    assert(alarm_parse("", &thr) == ALARM_CMD_IGNORE);
    assert(alarm_parse("STATUS", &thr) == ALARM_CMD_STATUS);
    assert(alarm_parse("START", &thr) == ALARM_CMD_START);
    assert(alarm_parse("STOP", &thr) == ALARM_CMD_STOP);
    assert(alarm_parse("THR 10", &thr) == ALARM_CMD_THR && thr == 10);
    assert(alarm_parse("THR 95", &thr) == ALARM_CMD_THR && thr == 95);
    assert(alarm_parse("THR 9", &thr) == ALARM_CMD_ERR);
    assert(alarm_parse("THR 96", &thr) == ALARM_CMD_ERR);
    assert(alarm_parse("THR abc", &thr) == ALARM_CMD_ERR);
    assert(alarm_parse("THR 50x", &thr) == ALARM_CMD_ERR);
    assert(alarm_parse("THR 50 extra", &thr) == ALARM_CMD_ERR);
    assert(alarm_parse("THR 50.0", &thr) == ALARM_CMD_ERR);
    assert(alarm_parse("thr 50", &thr) == ALARM_CMD_ERR);
    assert(alarm_parse("THR 999999999999999999999", &thr) == ALARM_CMD_ERR);

    alarm_debounce_init(&d, false);
    assert(!alarm_debounce_pressed(&d, true, 100));
    assert(!alarm_debounce_pressed(&d, true, 129));
    assert(alarm_debounce_pressed(&d, true, 130));
    assert(!alarm_debounce_pressed(&d, true, 3000));
    assert(!alarm_debounce_pressed(&d, false, 3020));
    assert(!alarm_debounce_pressed(&d, false, 3049));
    assert(!alarm_debounce_pressed(&d, false, 3050));
    assert(!alarm_debounce_pressed(&d, true, 3100));
    assert(alarm_debounce_pressed(&d, true, 3130));
    puts("OK: logica de alarma, histéresis, comandos y debounce");
    return 0;
}
