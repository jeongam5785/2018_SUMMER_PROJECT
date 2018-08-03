#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "mongoose.h"
#include "Fan.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>





namespace
{
	static const char *s_address = "192.168.0.5:1883"; //서버 주소?
	static const char *s_user_name = NULL;
	static const char *s_password = NULL;
	static const char *s_topic = "home/fan/#";
	static const char* s_clinet_id = "appiuuujnink";

	std::string toString(int value)//정수를 문자열로 변환 함수
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	static struct mg_mqtt_topic_expression s_topic_expr = { NULL, 0 };


	int count1 = 0;
	int count2 = 0;
	int stopFlag = 0;


	int past_arr[12];
	int cur_arr[12];
	int cur1_arr[12];

	int cnt_p = 0;

	int gCntStop = 0;
	int gCnt = 0;

	uint16_t state = 0b0000000000000000;
	uint16_t state_p = 0b0000000000000000;

	const int SPEED1 = 27;
	const int SPEED2 =25;
	const int SPEED3 =24;
	const int SPEED4 =23;
	const int SPEED5 =22;
	const int SPEED6 =26;
	const int SPEED7 =21;
	const int SPEED8 =28;
	const int SWING =29;

	struct mg_mgr mgr;
	struct mg_connection* connection = NULL;
}
void option_parser(int argc, char** argv);
void ev_handler(struct mg_connection *nc, int ev, void *p);
void checkSTATE(void);
int state_monitoring(void);

void notify_updates(Fan& fan, Fan::Flag update_flags)
{
	if (!connection)
		return;

	std::string topic;//토픽
	std::string payload;//

	if (update_flags & Fan::FLAG_POWER)
	{
		topic = "home/fan/power";
		payload = fan.power() ? "ON" : "OFF";
		mg_mqtt_publish(connection, topic.c_str(), 0, MG_MQTT_QOS(1), payload.c_str(), payload.size());
	}

	if (update_flags & Fan::FLAG_SPEED)
	{
		topic = "home/fan/speed";
		payload = toString(fan.speed());
		mg_mqtt_publish(connection, topic.c_str(), 0, MG_MQTT_QOS(1), payload.c_str(), payload.size());
	}

	if (update_flags & Fan::FLAG_SWING)
	{
		topic = "home/fan/swing";
		payload = fan.swing() ? "ON" : "OFF";
		mg_mqtt_publish(connection, topic.c_str(), 0, MG_MQTT_QOS(1), payload.c_str(), payload.size());
	}
	printf("%d, %d, %d\n", fan.power(), fan.speed(), fan.swing());

	/*mg_mqtt_publish(connection, topic.c_str(), 0, MG_MQTT_QOS(1), payload.c_str(), payload.size());*///토픽과 데이터 보냄
}

static void update_callback(Fan& fan, Fan::Flag update_flags)
{
	if (update_flags & Fan::FLAG_POWER)
	{
		
	}
	if (update_flags & Fan::FLAG_SPEED)
	{

	}
	if (update_flags & Fan::FLAG_SWING)
	{

	}

	notify_updates(fan, update_flags);
}

Fan fan(update_callback);

int state_monitoring(void)
{
	checkSTATE();
	if (cur_arr[0] & cur_arr[1] & cur_arr[2] & cur_arr[3] & cur_arr[4] & cur_arr[5] & cur_arr[6] & cur_arr[7]) {
		gCntStop = 1;
	}
	for (int i = 0; i < 9; i++)
		if (cur_arr[i] != past_arr[i])
		{
			if (cur1_arr[i] != cur_arr[i]) {

				for (int j = 0; j < 9; j++)
				past_arr[i] = cur1_arr[i];
				return 1;
			}for (int j = 0; j < 9; j++) {
				past_arr[i] = cur_arr[i]; 
			} return 2;
		}
	return 0;
}

void checkSTATE(void) {

	uint16_t temp = 0;
	uint16_t temp_p = 0;
	for (int i = 0; i < 9; i++)
		if(cur_arr[i] == 1)
			temp |= 1 << i;



	for (int i = 0; i < 8; i++)
		if (cur_arr[i] == 1)
			temp_p |= 1 << i;

	for (int i = 0; i < 8; i++)
		if (cur1_arr[i] == 1)
			temp_p |= 1 << i;

	state = temp;
	state_p = temp_p;




	//for (int i = 0; i < 9; i++)
	//{
	//	if (state & (1 << i))
	//	{
	//		printf("%d", 1);
	//	}
	//	else
	//	{
	//		printf("%d", 0);
	//	}

	//}
	//
	//printf("\n");
	//printf("%d\n", cnt_p);
	
		
}


int main(int argc, char **argv)
{
	option_parser(argc, argv);

	wiringPiSetup();//gpio 핀 셋업

	pinMode(SPEED1, INPUT);
	pinMode(SPEED2, INPUT);
	pinMode(SPEED3, INPUT);
	pinMode(SPEED4, INPUT);
	pinMode(SPEED5, INPUT);
	pinMode(SPEED6, INPUT);
	pinMode(SPEED7, INPUT);
	pinMode(SPEED8, INPUT);
	pinMode(SWING, INPUT);//선풍기 핀 output 설정

	mg_mgr_init(&mgr, NULL);

	connection = mg_connect(&mgr, s_address, ev_handler);
	if (connection == NULL) {
		fprintf(stderr, "mg_connect(%s) failed\n", s_address);
		exit(EXIT_FAILURE);
	}
	int past_speed;

	uint_fast16_t past_bin_state = 0;

	past_arr[0] = digitalRead(SPEED1);
	past_arr[1] = digitalRead(SPEED2);
	past_arr[2] = digitalRead(SPEED3);
	past_arr[3] = digitalRead(SPEED4);
	past_arr[4] = digitalRead(SPEED5);
	past_arr[5] = digitalRead(SPEED6);
	past_arr[6] = digitalRead(SPEED7);
	past_arr[7] = digitalRead(SPEED8);
	past_arr[8] = digitalRead(SWING);

	//공통부분 1
	for (;;)
	{
		if (!connection)
		{
			mg_connection* reconnect = mg_connect(&mgr, s_address, ev_handler);
			if (reconnect)
			{
				connection = reconnect;
				printf("connection established.");
			}
		}

		//공통부분 2        

		mg_mgr_poll(&mgr, 100);	// ???

		fan.poll();

		int cur_speed;
		int cur_bin_state;
		int speed;

		cur_arr[0] = digitalRead(SPEED1);
		cur_arr[1] = digitalRead(SPEED2);
		cur_arr[2] = digitalRead(SPEED3);
		cur_arr[3] = digitalRead(SPEED4);
		cur_arr[4] = digitalRead(SPEED5);
		cur_arr[5] = digitalRead(SPEED6);
		cur_arr[6] = digitalRead(SPEED7);
		cur_arr[7] = digitalRead(SPEED8);
		cur_arr[8] = digitalRead(SWING);

		cur1_arr[0] = digitalRead(SPEED1);
		cur1_arr[1] = digitalRead(SPEED2);
		cur1_arr[2] = digitalRead(SPEED3);
		cur1_arr[3] = digitalRead(SPEED4);
		cur1_arr[4] = digitalRead(SPEED5);
		cur1_arr[5] = digitalRead(SPEED6);
		cur1_arr[6] = digitalRead(SPEED7);
		cur1_arr[7] = digitalRead(SPEED8);
		cur1_arr[8] = digitalRead(SWING);
		

		int power_cnt = 0;
	
		switch (state_monitoring()) {
		case 1:
			if (cur_arr[8])
			fan.set_swing(0);
			   else
				   fan.set_swing(1);

			for (int i = 0; i <= 7; i++)
				if (cur_arr[i] == 0)
					speed = i + 1;

			fan.set_speed(speed);
			break;
		case 2:
			if (state_p == 0x00ff)
			fan.set_power(0);
			   else
				   fan.set_power(1);
			break;
		default: break;
		}


		/*if (state_monitoring()) {
			
				if (state_p == 0x00ff)
					fan.set_power(0);
				else
					fan.set_power(1);

				if (cur_arr[8])
					fan.set_swing(0);
				else
					fan.set_swing(1);

				for (int i = 0; i <= 7; i++)
					if (cur_arr[i] == 0)
						speed = i + 1;

				fan.set_speed(speed);
		}*/
	}
}

void ev_handler(struct mg_connection *nc, int ev, void *p)
{
	struct mg_mqtt_message *msg = (struct mg_mqtt_message *) p;
	(void)nc;

	switch (ev)
	{
	case MG_EV_CONNECT: {
		struct mg_send_mqtt_handshake_opts opts;
		memset(&opts, 0, sizeof(opts));
		opts.user_name = s_user_name;
		opts.password = s_password;

		mg_set_protocol_mqtt(nc);
		mg_send_mqtt_handshake_opt(nc, s_clinet_id, opts);
		printf("USER HANDLER GOT EVENT %d\n", ev);
	} break;
	case MG_EV_MQTT_CONNACK:
		if (msg->connack_ret_code != MG_EV_MQTT_CONNACK_ACCEPTED)
		{
			printf("Got mqtt connection error: %d\n", msg->connack_ret_code);
			exit(1);
		}
		s_topic_expr.topic = s_topic;
		printf("Subscribing to '%s'\n", s_topic);
		mg_mqtt_subscribe(nc, &s_topic_expr, 1, 0);
		fan.update_force();

		break;
	case MG_EV_MQTT_PUBACK:
		printf("Message publishing acknowledged (msg_id: %d)\n", msg->message_id);
		break;
	case MG_EV_MQTT_SUBACK:
		printf("Subscription acknowledged, forwarding to '%s'\n", s_topic);
		break;
	case MG_EV_MQTT_PUBLISH: {
		std::string topic(msg->topic.p, msg->topic.len);
		std::string payload(msg->payload.p, msg->payload.len);

		//TODO

	} break;
	case MG_EV_CLOSE:
		connection = NULL;
		printf("Connection closed\n");
	default:
		break;
	}
}

void option_parser(int argc, char** argv)//공통
{
	/* Parse command line arguments */
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			s_address = argv[++i];
			printf("-h %s\r\n", s_address);
		}
		else if (strcmp(argv[i], "-u") == 0) {
			s_user_name = argv[++i];
			printf("-u %s\r\n", s_user_name);
		}
		else if (strcmp(argv[i], "-t") == 0) {
			s_topic = argv[++i];
			printf("-t %s\r\n", s_topic);
		}
		else if (strcmp(argv[i], "-p") == 0) {
			s_password = argv[++i];
			printf("-p %s\r\n", s_password);
		}
		else if (strcmp(argv[i], "-c") == 0) {
			s_clinet_id = argv[++i];
			printf("-c %s\n", s_clinet_id);
		}
	}
}
