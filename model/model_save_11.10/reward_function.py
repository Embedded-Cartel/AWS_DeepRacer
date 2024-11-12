# def reward_function(params):
#     '''
#     Reward function optimized for object avoidance in a track with obstacles.
#     Encourages staying in the center when no obstacles are nearby and penalizes staying in the same lane as obstacles.
#     '''
#     # Read input parameters
#     all_wheels_on_track = params['all_wheels_on_track']
#     distance_from_center = params['distance_from_center']
#     track_width = params['track_width']
#     speed = params['speed']
#     steering_angle = abs(params['steering_angle'])
#     objects_distance = params['objects_distance']
#     _, next_object_index = params['closest_objects']
#     objects_left_of_center = params['objects_left_of_center']
#     is_left_of_center = params['is_left_of_center']

#     # Initialize reward
#     reward = 1e-3  # Base reward

#     # Calculate the reward for staying on the track
#     if all_wheels_on_track and (0.5 * track_width - distance_from_center) >= 0.05:
#         reward_lane = 1.0
#     else:
#         reward_lane = 1e-3  # Heavy penalty if off-track

#     # Calculate reward for avoiding obstacles
#     reward_avoid = 1.0

#     # Distance to the closest object
#     distance_closest_object = objects_distance[next_object_index]
#     # Check if the agent and the next object are in the same lane
#     is_same_lane = objects_left_of_center[next_object_index] == is_left_of_center

#     # If the vehicle is on the same lane as the object, it should avoid it
#     if is_same_lane:
#         if 0.8 > distance_closest_object >= 0.5:
#             reward_avoid *= 0.7  # Reduce reward slightly if getting too close
#         elif 0.5 > distance_closest_object >= 0.3:
#             reward_avoid *= 0.3  # Larger penalty if very close to the object
#         elif distance_closest_object < 0.3:
#             reward_avoid = 1e-3  # Very likely to crash, give minimal reward
#         # Disable center reward when close to obstacles
#         center_reward = 0.0  # No reward for staying in the center when avoiding obstacles
#     else:
#         # If not in the same lane, encourage staying near the center
#         # Center reward only when there is no immediate obstacle on the same lane
#         center_reward = (1 - (distance_from_center / (track_width * 0.5))) ** 2

#     # Penalize sharp turns to prevent zig-zag driving
#     if steering_angle > 15:
#         reward *= 0.8  # Penalize sharp turns

#     # Speed reward: penalize slow driving but encourage maintaining a safe speed
#     SPEED_THRESHOLD = 1.0
#     if speed < SPEED_THRESHOLD:
#         speed_reward = 0.5
#     else:
#         speed_reward = 1.0

#     # Final reward calculation
#     reward = 1.0 * reward_lane + 2.0 * reward_avoid + 0.5 * center_reward + 0.5 * speed_reward

#     return float(reward)

def reward_function(params):
    '''
    Optimized reward function for object avoidance and efficient track navigation.
    Focuses on staying centered when safe, avoiding obstacles effectively, and maintaining a balanced speed.
    '''
    # Read input parameters
    all_wheels_on_track = params['all_wheels_on_track']
    distance_from_center = params['distance_from_center']
    track_width = params['track_width']
    speed = params['speed']
    steering_angle = abs(params['steering_angle'])
    objects_distance = params['objects_distance']
    _, next_object_index = params['closest_objects']
    objects_left_of_center = params['objects_left_of_center']
    is_left_of_center = params['is_left_of_center']

    # Initialize base reward
    reward = 1e-3  # Small base reward

    # Encourage staying on the track and near the center when safe
    if all_wheels_on_track and (0.5 * track_width - distance_from_center) >= 0.1:
        reward += 1.0
    else:
        reward -= 1.0  # Heavy penalty if off-track

    # Object avoidance logic
    reward_avoid = 1.0  # Base reward for obstacle avoidance
    distance_closest_object = objects_distance[next_object_index]
    is_same_lane = objects_left_of_center[next_object_index] == is_left_of_center

    # Adjust reward based on proximity to obstacle in the same lane
    if is_same_lane:
        if 0.8 > distance_closest_object >= 0.5:
            reward_avoid *= 0.5  # Moderate penalty when close
        elif 0.5 > distance_closest_object >= 0.3:
            reward_avoid *= 0.2  # Higher penalty for very close
        elif distance_closest_object < 0.3:
            reward_avoid = 1e-3  # Minimal reward due to high collision risk
        center_reward = 0.0  # Center reward disabled when avoiding obstacle
    else:
        # Encourage staying near center if no obstacles in the same lane
        center_reward = (1 - (distance_from_center / (track_width * 0.5))) ** 2

    # Penalize sharp turns for smoother driving
    if steering_angle > 15:
        reward *= 0.7  # Penalize sharp turns slightly more

    # Encourage balanced speed: avoid driving too slow or too fast
    SPEED_THRESHOLD_MIN = 1.0
    SPEED_THRESHOLD_MAX = 2.0

    if speed < SPEED_THRESHOLD_MIN:
        speed_reward = 0.5  # Penalty for going too slow
    elif speed > SPEED_THRESHOLD_MAX:
        speed_reward = 0.7  # Slight penalty for overspeeding
    else:
        speed_reward = 1.5  # Higher reward for staying within optimal speed range

    # Calculate the final reward with weights adjusted for priority
    reward = 1.2 * reward + 2.0 * reward_avoid + 0.8 * center_reward + 0.5 * speed_reward

    return float(reward)
