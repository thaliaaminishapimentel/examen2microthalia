/* SPDX-License-Identifier: MIT */
#ifndef ALARM_CORE_H
#define ALARM_CORE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Pure logic: can be tested without the FRDM board. */
typedef struct
{
    uint8_t threshold;
    uint8_t percent;
    uint16_t adc;
    bool has_sample;
    bool running;
    bool alarm;
    bool reset_decision;
} alarm_core_t;

static inline uint8_t alarm_pct16(uint16_t adc)
{
    /* Rounded 100 * ADC / 65535 with a 32-bit intermediate. */
    return (uint8_t)(((uint32_t)adc * 100U + 32767U) / 65535U);
}

static inline void alarm_init(alarm_core_t *c)
{
    c->threshold = 70U;
    c->percent = 0U;
    c->adc = 0U;
    c->has_sample = false;
    c->running = true;
    c->alarm = false;
    c->reset_decision = true;
}

static inline void alarm_restart_decision(alarm_core_t *c)
{
    if (!c->running)
    {
        c->alarm = false;
        return;
    }
    if (c->has_sample)
    {
        c->alarm = (c->percent >= c->threshold);
        c->reset_decision = false;
    }
    else
    {
        c->alarm = false;
        c->reset_decision = true;
    }
}

static inline void alarm_start(alarm_core_t *c)
{
    if (!c->running)
    {
        c->running = true;
        alarm_restart_decision(c);
    }
}

static inline void alarm_stop(alarm_core_t *c)
{
    c->running = false;
    c->alarm = false;
}

static inline void alarm_toggle(alarm_core_t *c)
{
    if (c->running) alarm_stop(c);
    else alarm_start(c);
}

static inline void alarm_set_thr(alarm_core_t *c, uint8_t thr)
{
    c->threshold = thr;
    alarm_restart_decision(c);
}

static inline void alarm_sample(alarm_core_t *c, uint16_t adc)
{
    c->adc = adc;
    c->percent = alarm_pct16(adc);
    c->has_sample = true;
    if (!c->running)
    {
        c->alarm = false;
    }
    else if (c->reset_decision)
    {
        alarm_restart_decision(c);
    }
    else if (c->percent >= c->threshold)
    {
        c->alarm = true;
    }
    else if (c->percent < (uint8_t)(c->threshold - 5U))
    {
        c->alarm = false;
    }
    /* Hold state inside the five percentage-point hysteresis band. */
}

/* Only EXACT uppercase commands; THR n: decimal 10..95, no trailing chars. */
typedef enum { ALARM_CMD_IGNORE, ALARM_CMD_STATUS, ALARM_CMD_THR,
               ALARM_CMD_START, ALARM_CMD_STOP, ALARM_CMD_ERR } alarm_cmd_t;
static inline alarm_cmd_t alarm_parse(const char *line, uint8_t *new_thr)
{
    unsigned value = 0;
    size_t i;
    if (line[0] == '\0') return ALARM_CMD_IGNORE;
    if (strcmp(line, "STATUS") == 0) return ALARM_CMD_STATUS;
    if (strcmp(line, "START") == 0) return ALARM_CMD_START;
    if (strcmp(line, "STOP") == 0) return ALARM_CMD_STOP;
    if (strncmp(line, "THR ", 4) != 0) return ALARM_CMD_ERR;
    if (line[4] == '\0') return ALARM_CMD_ERR;
    for (i = 4; line[i] != '\0'; ++i)
    {
        if (line[i] < '0' || line[i] > '9') return ALARM_CMD_ERR;
        value = value * 10U + (unsigned)(line[i] - '0');
        if (value > 95U) return ALARM_CMD_ERR;
    }
    if (value < 10U) return ALARM_CMD_ERR;
    *new_thr = (uint8_t)value;
    return ALARM_CMD_THR;
}

typedef struct
{
    bool raw;
    bool stable;
    uint32_t changed_at;
} alarm_debounce_t;

static inline void alarm_debounce_init(alarm_debounce_t *d, bool initial_raw)
{
    d->raw = initial_raw;
    d->stable = initial_raw;
    d->changed_at = 0;
}

/* Returns true ONCE per debounced press. Release must stabilize first. */
static inline bool alarm_debounce_pressed(alarm_debounce_t *d, bool pressed, uint32_t now)
{
    if (pressed != d->raw)
    {
        d->raw = pressed;
        d->changed_at = now;
    }
    if ((d->raw != d->stable) && ((uint32_t)(now - d->changed_at) >= 30U))
    {
        d->stable = d->raw;
        return d->stable;
    }
    return false;
}

#endif /* ALARM_CORE_H */
