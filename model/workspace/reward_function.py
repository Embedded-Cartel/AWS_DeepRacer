import math

def reward_function(params):

    # 파라미터 추출
    all_wheels_on_track = params['all_wheels_on_track']
    distance_from_center = params['distance_from_center']
    track_width = params['track_width']
    steering = abs(params['steering_angle'])
    speed = params['speed']
    progress = params['progress']
    steps = params['steps']
    waypoints = params['waypoints']
    closest_waypoints = params['closest_waypoints']
    heading = params['heading']
    x = params['x']
    y = params['y']
    is_left_of_center = params['is_left_of_center']
    objects_location = params.get('objects_location', [])
    is_offtrack = params['is_offtrack']
    _, next_object_index = params.get('closest_objects', (None, None))
    objects_left_of_center = params.get('objects_left_of_center', [])

    # 초기 보상
    reward = 1e-3

    # 모든 바퀴가 트랙 위에 있는지 확인
    if not all_wheels_on_track or is_offtrack:
        return 1e-3  # 최소 보상 반환

    # 진행도에 따른 보상
    if steps > 0:
        reward_progress = progress / steps
        reward += reward_progress

    # 현재와 다음 웨이포인트 가져오기
    next_point = waypoints[closest_waypoints[1]]
    prev_point = waypoints[closest_waypoints[0]]

    # 트랙 방향 계산
    track_direction = math.degrees(math.atan2(
        next_point[1] - prev_point[1],
        next_point[0] - prev_point[0]
    ))

    # 차량의 방향과 트랙 방향의 차이 계산
    direction_diff = abs(track_direction - heading)
    if direction_diff > 180:
        direction_diff = 360 - direction_diff

    # 코너 각도에 따라 분류
    if direction_diff > 30.0:
        corner_type = 'sharp'
    elif direction_diff > 15.0:
        corner_type = 'medium'
    else:
        corner_type = 'straight'

    # 방향 차이에 따른 보상
    DIRECTION_THRESHOLD = 10.0
    if direction_diff < DIRECTION_THRESHOLD:
        reward += 1.0
    else:
        reward *= max(0.5, 1 - (direction_diff / 50.0))  # 방향 차이에 따라 보상 감소

    # 트랙 중앙에서의 거리 보상
    marker_1 = 0.1 * track_width
    marker_2 = 0.25 * track_width
    marker_3 = 0.5 * track_width

    if distance_from_center <= marker_1:
        reward += 1.0
    elif distance_from_center <= marker_2:
        reward += 0.5
    elif distance_from_center <= marker_3:
        reward += 0.1
    else:
        reward = 1e-3  # 트랙을 벗어남

    # 속도에 대한 보상
    if corner_type == 'sharp':
        # 급격한 코너에서는 낮은 속도를 유지하도록 유도
        if speed < 1.0:
            reward += 1.0
        else:
            reward *= 0.5
    elif corner_type == 'medium':
        # 중간 코너에서는 중간 속도를 유지
        if speed < 1.5:
            reward += 1.0
        else:
            reward *= 0.8
    else:
        # 직선 구간에서는 높은 속도를 유지
        if speed > 2.5:
            reward += 1.0
        else:
            reward += (speed / 3.0)

    # 조향 각도에 대한 보상
    ABS_STEERING_THRESHOLD = 15
    if steering > ABS_STEERING_THRESHOLD:
        if corner_type != 'straight':
            # 코너에서는 큰 조향 각도를 허용
            reward += 0.5
        else:
            # 직선에서는 패널티 적용
            reward *= 0.8

    # 물체 회피 보상
    if objects_location and next_object_index is not None:
        next_object_loc = objects_location[next_object_index]
        distance_closest_object = math.hypot(x - next_object_loc[0], y - next_object_loc[1])
        is_same_lane = objects_left_of_center[next_object_index] == is_left_of_center
        MAX_DISTANCE = 1.5  # 최대 고려 거리

        if is_same_lane and distance_closest_object < MAX_DISTANCE:
            # 물체가 가까이 있을 경우 회피 행동 유도
            reward *= 0.5 * (distance_closest_object / MAX_DISTANCE)
        else:
            # 물체를 성공적으로 회피한 경우 추가 보상
            if distance_closest_object > MAX_DISTANCE and is_same_lane == False:
                reward += 1.0

    return float(reward)
