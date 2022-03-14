from time import sleep
import paho.mqtt.client as mqtt


def connect(client_name = "laptop",host_name = "10.254.223.22"):
    client =mqtt.Client(client_name)
    client.connect(host_name)
    return client


def send(client, data, topic):
    client.publish(topic, data)


def receive(client, topic):
    def on_message(client, userdata, msg):
        return msg.payload.decode(), msg.topic
    client.subscribe(topic)
    client.on_message = on_message


def send_error(ip, robot_x, robot_y, robot_rot, cube_x, cube_y, isBlue, start):
    client = connect(host_name=ip)
    print('robot_x', robot_x, 'robot_y', robot_y,'robot_rotation', robot_rot, 'cube_x', cube_x, 'cube_y', cube_y, 'blue' , isBlue , 'start' , start)
    # font = cv2.FONT_HERSHEY_SIMPLEX
    # _, _, robo_centre, robo_ang = arucodetect.detectaruco(pic)
    # cube_ang = math.atan2(cube_co[1] - robo_centre[1], cube_co[1] - robo_centre[1])
    # cube_error = cube_ang - robo_ang
    # cross_error = ((cross[0] - robo_centre[0]) ** 2 + (cross[1] - robo_centre[1]) ** 2)**0.5
    # dest1_error = ((dest1[0] - robo_centre[0]) ** 2 + (dest1[1] - robo_centre[1]) ** 2)**0.5
    # dest2_error = ((dest2[0] - robo_centre[0]) ** 2 + (dest2[1] - robo_centre[1]) ** 2)**0.5
    # red1_error = ((red1[0] - robo_centre[0]) ** 2 + (red1[1] - robo_centre[1]) ** 2)**0.5
    # red2_error = ((red2[0] - robo_centre[0]) ** 2 + (red2[1] - robo_centre[1]) ** 2)**0.5
    # blue1_error = ((blue1[0] - robo_centre[0]) ** 2 + (blue1[1] - robo_centre[1]) ** 2)**0.5
    # blue2_error = ((blue2[0] - robo_centre[0]) ** 2 + (blue2[1] - robo_centre[1]) ** 2)**0.5
    #send(client,"cube_error","arduino/cube_error")
    #send(client,"cross_error","arduino/cross_error")
    # send(client,dest1_error,"arduino/dest1_error")
    # send(client,dest2_error,"arduino/dest2_error")
    # send(client,red1_error,"arduino/red1_error")
    # send(client,red2_error,"arduino/red2_error")
    # send(client,blue1_error,"arduino/blue1_error")
    # send(client,blue2_error,"arduino/blue2_error")
    send(client, robot_x,'arduino/RobotX')
    send(client, robot_y,'arduino/RobotY')
    send(client, robot_rot,'arduino/RobotRot')
    send(client, cube_x,'arduino/CubeX')
    send(client, cube_y,'arduino/CubeY')
    send(client,isBlue,"arduino/IsBlue")
    send(client,start,"arduino/Start")


if __name__ == "__main__":
    client = connect(host_name="10.254.223.22")
    send(client,"Hello","arduino/")
    receive(client, "arduino/")