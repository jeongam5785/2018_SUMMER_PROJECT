#!/usr/bin/env python3

import paho.mqtt.client as mqtt


def mqtt_func(topic, msg):

    mqttc = mqtt.Client("python_pub")
    mqttc.connect("192.168.0.5", 1883)
    mqttc.publish(topic, msg)
    mqttc.loop(2)


def send_mqtt_topic_msg(Dict_text):
    if Dict_text['parameters']['Home_appliance'] == ['light']:
        print('light')
        if Dict_text['parameters']['number'] != 0:
            val_light = Dict_text['parameters']['number']
            print(val_light)
            mqtt_func('home/light/brightness/set', val_light)
        if Dict_text['parameters']['default'] != 0:
            val_light_d = Dict_text['parameters']['default']
            print(val_light_d)
            mqtt_func('home/light/brightness/set', val_light_d)
        if Dict_text['parameters']['action'] == ['turn\xa0on']:
            print('turn on')
            mqtt_func('home/light/set', 'ON')
        if Dict_text['parameters']['action'] == ['turn off']:
            print('turn off')
            mqtt_func('home/light/set', 'OFF')
    if Dict_text['parameters']['Home_appliance'] == ['fan']:
        print('fan')
        if Dict_text['parameters']['number'] != 0:  # after turn on
            val_fan = Dict_text['parameters']['number']
            if type(val_fan) == float:
                mqtt_func('home/fan/speed/set', int(val_fan))
        if Dict_text['parameters']['action'] == ['turn\xa0on']:
            print('turn on')
            mqtt_func('home/fan/set', 'ON')
        if Dict_text['parameters']['action'] == ['turn off']:
            print('turn off')
            mqtt_func('home/fan/set', 'OFF')
        if Dict_text['parameters']['action'] == ['oscillate']:  # after turn on
            print('oscillate')
            mqtt_func('home/fan/oscillate/set', 'ON')
    if Dict_text['parameters']['Home_appliance'] == ['humidifier']:
        print('humidifier')
        if Dict_text['parameters']['default'] != 0:
            val_humi = Dict_text['parameters']['default']
            print(val_humi)
            mqtt_func('home/humidifier/level/set', val_humi)
        if Dict_text['parameters']['action'] == ['turn\xa0on']:
            print('turn on')
            mqtt_func('home/humidifier/set', 'ON')
        if Dict_text['parameters']['action'] == ['turn off']:
            print('turn off')
            mqtt_func('home/humidifier/set', 'OFF')
    if Dict_text['parameters']['Home_appliance'] == ['TV']:
        print('TV')
        if Dict_text['parameters']['number'] != 0:
            val_tv = Dict_text['parameters']['number']
            print(val_tv)
            # mqtt_func('home/TV/channel/set',val_tv)
        if Dict_text['parameters']['action'] == ['mute']:
            print('mute')
            # mqtt_func('home/TV/set','ON')
        if Dict_text['parameters']['action'] == ['play']:
            print('play')
            # mqtt_func('home/TV/set','ON')
        if Dict_text['parameters']['action'] == ['pause']:
            print('pause')
            # mqtt_func('home/TV/set','ON')
        if Dict_text['parameters']['action'] == ['rewind']:
            print('rewind')
            # mqtt_func('home/TV/set','ON')
        if Dict_text['parameters']['action'] == ['fastforward']:
            print('fastforward')
            # mqtt_func('home/TV/set','ON')
        if Dict_text['parameters']['action'] == ['turn\xa0up']:
            print('turn up')
            # mqtt_func('home/TV/volume/set','ON')
        if Dict_text['parameters']['action'] == ['turn\xa0down']:
            print('turn down')
            # mqtt_func('home/TV/volume/set','ON')
        if Dict_text['parameters']['action'] == ['turn\xa0on']:
            print('turn on')
            # mqtt_func('home/TV/set','ON')
        if Dict_text['parameters']['action'] == ['turn\xa0off']:
            print('turn off')
            # mqtt_func('home/TV/set','OFF')