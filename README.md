# AWS_DeepRacer
Embedded SW Contest 2024 - Autonomous Driving Racing

## Team Bermudar

|이름|직책|담당|
|---|---|----------------------|
|염 훈|팀  장|Adaptive AUTOSAR (Application Design, Service Instance Manifest)|
|강시형|팀  원|LiDAR (2D Live Map, Make Data Map), Camera Pipeline Stream|
|박재완|응  원|Deep Reinforcement Learning, Image Preprocessing|
|오규상|팀  원|Linear Regressing, Image Classification (SimAM)|
|황귀용|팀  원|Adaptive AUTOSAR (Machine Manifest, Execution Manifest), Stereo Depth Map|

## Directory
### AA
대회용 AA 개발 디렉토리

### AA_Code
AA 개발 디렉토리 기반 Generated 코드 (gen_ara/implementation/Machine)

### AA_Sample_Code
PopcornSAR 예제 기반 Generated 코드 (gen_ara/implementation/Machine)

### deepracercode
DeepRacer 샘플 코드
Camera, LiDAR, Servo

### model
AI Model

### sdk
a.k.a PARA_SDK
ARXML 기반 Generate 이후 build 진행 시 실행파일 생성되는 경로 (cd $PARA_SDK)

## Script
### build.sh
AA 개발 코드 빌드

### build_sample.sh
PopcornSAR Sample 코드 빌드

### export.sh
PARA SDK 환경변수 설정

### install_para_sdk
PARA_SDK 설치
