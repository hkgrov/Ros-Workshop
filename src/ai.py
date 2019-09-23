#!/usr/bin/env python
from geometry_msgs.msg import Polygon
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Point32
from geometry_msgs.msg import Point
import math
import rospy


# Initialize ROS
rospy.init_node("ai_node")

boxes = []
visitedBoxes = []
got_boxes = False
def boxesCallback(msg):
    global boxes
    global got_boxes
    got_boxes = True
    boxes = msg.points
    print("Message Received AI")

posisjon = []
def poseCallback(msg):
    global posisjon
    posisjon = msg.pose.position
    #print(posisjon)

def euclideanDistance(pos, boxes):
    #print(boxes)
    minimum = 1000000000
    nearest_box = None
    for box in boxes:
        if(box in visitedBoxes):
            print("Kjorte denne")
            continue
        #print(box)
        euc = math.sqrt((box.x - pos.x)**2 + (box.y - pos.y)**2 + (box.z - pos.z)**2)
        #print(euc)
        if (euc < minimum):
            minimum = euc
            nearest_box = box
    visitedBoxes.append(nearest_box)
    return nearest_box

# Set up subscribers here

sub = rospy.Subscriber("/perception/boxes", Polygon, boxesCallback)
sub2 = rospy.Subscriber("/mavros/local_position/pose" , PoseStamped, poseCallback)

pub = rospy.Publisher("/control/position_setpoint", Point32, queue_size=1)

msg = Point32()



# Main loop
rospy.loginfo("ai is running")
rate = rospy.Rate(30)
i = 0
next_box = False
while not rospy.is_shutdown():
    if(got_boxes == True):
        print("Publishes AI Message")
        nearest_box = euclideanDistance(posisjon, boxes)
        msg = nearest_box
        pub.publish(msg)
        

   
    # Rate limiting
    rate.sleep(10)









