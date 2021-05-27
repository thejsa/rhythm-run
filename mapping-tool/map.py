from openal import *
import argparse
import time
from pynput import keyboard

parser = argparse.ArgumentParser(
    description='Create a beatmap / timing file for Rhythm Run')
parser.add_argument('musicfile')
args = parser.parse_args()

source = oalOpen(args.musicfile)
source.play()


# helper function to get the time in milliseconds, to maintain consistency
def time_ms():
    return time.time() * 1000


start_time = time_ms()
beats = []


def on_press(key):
    beat_time = time_ms() - start_time
    beats.append(round(beat_time))
    print(beat_time, file=sys.stderr)


listener = keyboard.Listener(
    on_press=on_press)

listener.start()

while source.get_state() == AL_PLAYING:
    time.sleep(1)
    print('time=', time_ms() - start_time, file=sys.stderr)
    # check input

print("\n".join([str(i) for i in beats]))
