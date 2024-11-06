#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import miso_in_the_soup_xiao as xiao
import time


def main() -> None:
    while True:
        xiao.setLed(4)
        # 青フワフワ
        while True:
            sensors = xiao.capture()
            print(sensors)
            if sensors[0] == 255:
                break
            time.sleep(0.1)

        xiao.setLed(1)  # 赤点灯
        while True:
            sensors = xiao.capture()
            print(sensors)
            if sensors[0] == 0:
                break
            time.sleep(0.1)


if __name__ == "__main__":
    try:
        xiao.open("/dev/ttyACM0")
        main()
    except KeyboardInterrupt:
        pass
