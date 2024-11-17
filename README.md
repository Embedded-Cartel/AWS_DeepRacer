# AWS_DeepRacer
Embedded SW Contest 2024 - Autonomous Driving Racing

## Team Bermudar

|이름|직책|담당|
|---|---|----------------------|
|염  훈|팀  장|ARXML Design, SOME/IP|
|강시형|팀  원|Image Preprocessing, AA(Sensor)|
|박재완|응  원|RL, AA(Calc)|
|오규상|팀  원|CI/CD, RL, AA(Calc)|
|황귀용|팀  원|AA(Actuator)|

## Directory
### AA
대회용 AA 개발 디렉토리

### Inference-workspace
- 강화학습 관련 code 및 구현한 tool history

## model
- 강화학습 보상학습 최종 코드 포함
- 본선 최종 사용모델 
    carfile.tar.gz : 모델 파일 _ for AWS local Web <br/>
    workspace : model.pb : AA_model_load <br/>
          ┗ reward_function.py - 강화학습 보상 함수
          ┗ model_metadata.json - action space 설정값
          ┗ hyperparameters.json - 강화학습 hyperparameter 설정값
    Bucket 경로 : s3://deepracer-bermuda/rl-deepracer-sagemaker-2//carfile.tar.gz <br/>


### sdk
a.k.a PARA_SDK
ARXML 기반 Generate 이후 build 진행 시 실행파일 생성되는 경로 (cd $PARA_SDK)

## Script
### build.sh
AA 개발 코드 빌드

### export.sh
PARA SDK 환경변수 설정(soure export.sh)

### install_para_sdk.sh
PARA_SDK 설치
LiDAR SDK 설치

### ps.sh
실행 중인 AA Process 확인
