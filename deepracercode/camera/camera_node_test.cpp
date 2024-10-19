#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

int main() {
    // 카메라에서 프레임 캡처
    VideoCapture cap1(0);
    VideoCapture cap2(2);

    if (!cap1.isOpened()) {
        std::cerr << "카메라1을 열 수 없습니다." << std::endl;
        return -1;
    }

    if (!cap2.isOpened()) {
        std::cerr << "카메라2를 열 수 없습니다." << std::endl;
        return -1;
    }

    // MJPEC 코덱 및 크기 설정
    cap1.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap1.set(CAP_PROP_FRAME_WIDTH, 160);
    cap1.set(CAP_PROP_FRAME_HEIGHT, 120);

    cap2.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap2.set(CAP_PROP_FRAME_WIDTH, 160);
    cap2.set(CAP_PROP_FRAME_HEIGHT, 120);

    
    while(true) {
        // 프레임 가져오기
        Mat frame1;
        Mat frame2;
	    std::vector<uchar> buffer1;
        std::vector<uchar> buffer2;
        cap1 >> frame1;
        cap2 >> frame2;

// 전송 모듈 코드 //////////////////////////////////////////////////////////// 
	    // 프레임 직렬화
	    imencode(".jpeg", frame1, buffer1);
        imencode(".jpeg", frame2, buffer2);
	   
	    // Publish 코드 호출
	    // ...
////////////////////////////////////////////////////////////////////////////

// 수신 모듈 코드 ////////////////////////////////////////////////////////////
	    // Subscribe 코드 호출
	    // ...
	
	    // 프레임 역직렬화
	    Mat decoded_frame1 = imdecode(buffer1, IMREAD_UNCHANGED);
        Mat decoded_frame2 = imdecode(buffer2, IMREAD_UNCHANGED);
////////////////////////////////////////////////////////////////////////////
        
// 테스트 코드 //////////////////////////////////////////////////////////////
        // 이미지 파일로로 저장
        imwrite("test1.jpg", decoded_frame1);
        imwrite("test2.jpg", decoded_frame2);
        std::cout << "프레임이 test.jpg로 저장되었습니다." << std::endl;
////////////////////////////////////////////////////////////////////////////
    }
    // 카메라 해제
    cap1.release();
    cap2.release();
    return 0;
}
