# Direction from a sensor towards an identified target.
# header.frame_id is the frame of the sensor that produced the measurement.

std_msgs/Header header

int32 id

# unit vector, expressed in header.frame_id
geometry_msgs/Vector3 direction
