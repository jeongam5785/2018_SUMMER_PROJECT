import wave
import time
from aiy_led import Leds
import _recorder
import RPi.GPIO as GPIO

AUDIO_SAMPLE_SIZE = 2  # bytes per sample
AUDIO_SAMPLE_RATE_HZ = 16000

# Global variables. They are lazily initialized.
_voicehat_recorder = None
_voicehat_player = None
_status_ui = None
_tts_volume = 60
_tts_pitch = 130

RED = (0xFF, 0x00, 0x00)
GREEN = (0x00, 0xFF, 0x00)
YELLOW = (0xFF, 0xFF, 0x00)
BLUE = (0x00, 0x00, 0xFF)
PURPLE = (0xFF, 0x00, 0xFF)
CYAN = (0x00, 0xFF, 0xFF)
WHITE = (0xFF, 0xFF, 0xFF)


class _WaveDump(object):
    """A processor that saves recorded audio to a wave file."""

    def __init__(self, filepath, duration):
        self._wave = wave.open(filepath, 'wb')
        self._wave.setnchannels(1)
        self._wave.setsampwidth(2)
        self._wave.setframerate(16000)
        self._bytes = 0
        self._bytes_limit = int(duration * 16000) * 1 * 2

    def add_data(self, data):

        max_bytes = self._bytes_limit - self._bytes
        data = data[:max_bytes]
        self._bytes += len(data)
        if data:
            self._wave.writeframes(data)

    def is_done(self):
        return self._bytes >= self._bytes_limit

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self._wave.close()


def get_recorder():
    """Returns a driver to control the VoiceHat microphones.

    The aiy modules automatically use this recorder. So usually you do not need to
    use this.
    """
    global _voicehat_recorder
    if not _voicehat_recorder:
        _voicehat_recorder = _recorder.Recorder()
    return _voicehat_recorder


def record_to_wave(filepath, duration):
    """Records an audio for the given duration to a wave file."""
    recorder = get_recorder()
    dumper = _WaveDump(filepath, duration)
    a = dumper.is_done()
    led = Leds()
    with recorder, dumper:
        led.update(Leds.set_color(RED))
        recorder.add_processor(dumper)
        while not a:
            print(dumper.is_done())
            if GPIO.input(23) == 1:
                a = True
            time.sleep(0.1)
        print(dumper.is_done())
        recorder.remove_processor(dumper)
