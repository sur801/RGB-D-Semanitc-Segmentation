# RGB-D Segmentation in rex kit


## build 방법

cd build;

cmake ..

make

## Cam , Display 설정 파일 
include/config.h

## RKNN 설정
/AI_Example/  
폴더에(없으면 생성) bisenetv2_new_quant.rknn 복사

## 보드내에 라이브러리가 없는경우 
lib 폴더 하위에 있는 해당 lib파일을 보드에 /lib 경로로 복사할것

## 실행
./실행파일_이름

