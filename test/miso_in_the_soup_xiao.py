#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial


def open(port: str) -> None:
    global ser
    ser = serial.Serial(port, 9600, timeout=3)


def capture() -> list:
    values = None
    ser.write("capture".encode())
    while True:
        line = ser.readline().decode()
        if line == "ok\r\n":
            break
        values = line.split()

    return [int(v) for v in values]


def setLed(led_pattern: int) -> None:
    ser.write(f"setLed {led_pattern}".encode())
    line = ser.readline().decode()
    if line != "ok\r\n":
        raise IOError()


def setVibration(vibration_pattern: int) -> None:
    ser.write(f"setVibration {vibration_pattern}".encode())
    line = ser.readline().decode()
    if line != "ok\r\n":
        raise IOError()
