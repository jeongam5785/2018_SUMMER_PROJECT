#!/usr/bin/env python3

import dialogflow
from google.protobuf.json_format import MessageToDict
from pprint import pprint


def send_text_request(session, text):
    text_input = dialogflow.types.TextInput(text=text, language_code=language_code)
    query_input = dialogflow.types.QueryInput(text=text_input)
    return send_request(session, query_input)


def send_request(session, query_input):
    response = dialogflow.Session_Client.detect_intent(session=session, query_input=query_input)
    # response= response.replace('\302\240',' ')
    print('=' * 20)
    print('Query text: {}'.format(response.query_result.query_text))
    print('Detected intent: {} (confidence: {})'.format(
        response.query_result.intent.display_name,
        response.query_result.intent_detection_confidence))
    print('Fulfillment text: {}'.format(response.query_result.fulfillment_text))
    print('=' * 20)
    return response_toDict(response)


def response_toDict(response):
    dict_response = MessageToDict(response.query_result)
    pprint(type(dict_response))
    pprint('dict_text:' + dict_response)
    print(dict_response['parameters']['Home_appliance'])
    print(dict_response['parameters']['action'])
    return dict_response




