/* Copyright by Ryan Wu 2015 */

#pragma once

Window* create_splash_window();
Window* create_timer_window (char* title, char* description, time_t duration);
Window* create_step_window(char* title, char* description);
void start_timer(time_t duration);