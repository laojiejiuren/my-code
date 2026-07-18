#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = ( ((uint64_t)inl(RTC_ADDR + 4) << 32 ) | (inl(RTC_ADDR)) );
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 50;
  rtc->minute = 37;
  rtc->hour   = 16;
  rtc->day    = 18;
  rtc->month  = 7;
  rtc->year   = 2026;
}
