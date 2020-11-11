#include "user_config.h" // It must be located above ros2arduino.h.
#include <ros2arduino.h>
#include <M5Stack.h>


#define XRCEDDS_PORT  Serial
#define PUBLISH_FREQUENCY 2 //hz

void publishString(std_msgs::String* msg, void* arg)
{
  (void)(arg);

  static int cnt = 0;
  sprintf(msg->data, "Hello ros2arduino %d", cnt++);
}

class StringPub : public ros2::Node
{
public:
  StringPub()
  : Node("ros2arduino_pub_node")
  {
    ros2::Publisher<std_msgs::String>* publisher_ = this->createPublisher<std_msgs::String>("arduino_chatter");
    this->createWallFreq(PUBLISH_FREQUENCY, (ros2::CallbackFunc)publishString, nullptr, publisher_);
  }
};

void setup() 
{
  // init lcd, serial, but don't init sd card
  M5.begin(true, false, true);
  M5.Power.begin();

  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.println("Button example");

  XRCEDDS_PORT.begin(115200);
  while (!XRCEDDS_PORT); 
  M5.Lcd.println("Port OK!");
  
  ros2::init(&XRCEDDS_PORT);
  M5.Lcd.println("Init OK!");
}

void loop() 
{
  static StringPub StringNode;

  ros2::spin(&StringNode);
}
