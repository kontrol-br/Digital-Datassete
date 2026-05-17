#pragma once
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT 0
#define A0 26
inline auto __start = std::chrono::steady_clock::now();
inline uint32_t millis(){return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-__start).count();}
inline uint32_t micros(){return (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-__start).count();}
inline void delay(uint32_t){}
inline void delayMicroseconds(uint32_t){}
inline void pinMode(uint8_t,uint8_t){}
inline void digitalWrite(uint8_t,uint8_t){}
inline int digitalRead(uint8_t){return HIGH;}
inline void tone(uint8_t,uint16_t,uint16_t){}
inline void noTone(uint8_t){}
inline void analogReadResolution(int){}
inline int analogRead(uint8_t){return 2048;}
struct SerialStub{template<class T> void print(T v){std::printf("%s", std::to_string(v).c_str());} void print(const char*s){std::printf("%s",s);} template<class T> void println(T v){print(v);std::printf("\n");} void println(const char*s){std::printf("%s\n",s);} void begin(uint32_t){} operator bool() const{return true;}};
inline SerialStub Serial;
#endif
