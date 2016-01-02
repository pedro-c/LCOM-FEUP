#ifndef __TIMER_H
#define __TIMER_H

static int hook_rtc=NOTIFICATION_RTC;

int rtc_subscribe();

int rtc_unsubscribe();

void wait_valid_rtc(void);

void display_date();
#endif
