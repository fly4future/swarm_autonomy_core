# Estimated position of a target relative to the own UAV.
# header.frame_id is the localization origin frame.

std_msgs/Header header

int32 id

# [m], expressed in header.frame_id
geometry_msgs/Point position

# [m^2], row-major 3x3
float64[9] covariance
