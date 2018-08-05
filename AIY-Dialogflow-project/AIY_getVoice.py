#!/usr/bin/env python3

import logging

import aiy.assistant.grpc
import aiy.audio
import aiy.voicehat
import dialogflow_getResponse
import mqtt_manager

logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s] %(levelname)s:%(name)s:%(message)s"
)

class Training:
    def __init__(self):
        self.project_id = '************'                           # dialogflow project id
        self.client_access_token = '***************************'   # dialogflow agent client token
        self.language_code = 'en'
        self.session_client = dialogflow.SessionsClient()
        self.assistant = aiy.assistant.grpc.get_assistant()
        self.status_ui = aiy.voicehat.get_status_ui()

    def main(self):
        button = aiy.voicehat.get_button()
        with aiy.audio.get_recorder():
            while True:
                session_id = ************
                session = self.session_client.session_path(self.project_id, session_id)
                while True:
                    self.status_ui.status('ready')
                    print('Press the button and speak')
                    aiy.audio.say('Welcome, press the button and speak')
                    button.wait_for_press()
                    self.status_ui.status('listening')
                    aiy.audio.play_audio('I am listening')
                    print('Listening')
                    text, audio = self.assistant.recognize()
                    if not text:
                        aiy.audio.say('I am sorry, I did not catch that.')
                        print('I am sorry, I did not catch that.')
                        continue

                    print(text)
                    print('------------wait--------------')
                    dict_response = dialogflow_getResponse.send_text_request(session, text)
                    mqtt_manager.send_mqtt_topic_msg(dict_response)


if __name__ == '__main__':
    training = Training()
    training.main()
