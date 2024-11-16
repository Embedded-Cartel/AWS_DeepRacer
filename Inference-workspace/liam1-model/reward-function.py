def reward_function(params):
    '''
    Liam test training ver1
    '''

    all_wheels_on_track = params['all_wheels_on_track']
    distance_from_center = params['distance_from_center']
    track_width = params['track_width']
    objects_distance = params['objects_distance']
    _, next_object_index = params['closest_objects']
    objects_left_of_center = params['objects_left_of_center']
    is_left_of_center = params['is_left_of_center']
    speed = params['speed']  # Adding speed to consider speed in rewards

    # Initialize reward
    reward = 1e-3

    # Reward if the agent stays inside the two borders of the track and is not too close to the edges
    if all_wheels_on_track and (0.5 * track_width - distance_from_center) >= 0.05:
        reward_lane = 1.0
    else:
        reward_lane = 1e-3  # Lower reward if off-track or too close to the edges

    # Reward for avoiding obstacles
    reward_avoid = 1.0
    distance_closest_object = objects_distance[next_object_index]
    is_same_lane = objects_left_of_center[next_object_index] == is_left_of_center

    if is_same_lane:
        if 0.5 <= distance_closest_object < 0.8:
            reward_avoid *= 0.5
        elif 0.3 <= distance_closest_object < 0.5:
            reward_avoid *= 0.2
        elif distance_closest_object < 0.3:
            reward_avoid = 1e-3  # Almost crashed, minimal reward

    # Add reward for speed: avoid being too slow
    SPEED_THRESHOLD = 2.0  # Speed threshold
    if speed >= SPEED_THRESHOLD:
        reward_speed = 1.0  # High reward if speed is above the threshold
    else:
        reward_speed = 0.5  # Lower reward if the speed is too slow

    # Reward for recovering back to track if the agent is off-track
    if not all_wheels_on_track:
        reward_recovery = 0.5  # Encourage recovery if off-track
    else:
        reward_recovery = 1.0  # Higher reward for staying on track

    # Combine rewards with appropriate weights
    reward += 1.0 * reward_lane + 2.0 * reward_avoid + 1.5 * reward_speed + 2.0 * reward_recovery

    return reward
