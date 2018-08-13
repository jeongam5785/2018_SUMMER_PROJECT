#!/usr/bin/env python3s


from aiy_led import Leds
import aiy.voicehat
import time
import aiy.assistant.grpc
import _WaveDump
import _WaveDump1
import RPi.GPIO as GPIO
from pytz import timezone
from datetime import datetime

RED = (0xFF, 0x00, 0x00)
GREEN = (0x00, 0xFF, 0x00)
YELLOW = (0xFF, 0xFF, 0x00)
BLUE = (0x00, 0x00, 0xFF)
PURPLE = (0xFF, 0x00, 0xFF)
CYAN = (0x00, 0xFF, 0xFF)
WHITE = (0xFF, 0xFF, 0xFF)


count = 0  # type: int


class MyRec:

    def record(self):
        global count
        now = datetime.now(timezone('Asia/Seoul'))
        y = now.strftime('%Y-%m-%d_%H-%M-%S')
        if count >= 1:
            _WaveDump1.record_to_wave('/home/pi/hellomodi_wave_file/wave_file/'+y + '.wav', 10)
        else:
            _WaveDump.record_to_wave('/home/pi/hellomodi_wave_file/wave_file/'+y + '.wav', 10)
        count += 1


def main():
    button = aiy.voicehat.get_button()
    led = Leds()
    rec = MyRec()

    time.sleep(1)
    while True:
        led.update(Leds.set_color(WHITE))

        if GPIO.input(23) == 0:

            time.sleep(1)
            rec.record()
            led.reset()


if __name__ == '__main__':
    main()