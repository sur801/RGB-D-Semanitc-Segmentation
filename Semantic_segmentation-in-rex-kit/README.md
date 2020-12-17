# Golf_Ball_Tracker_server exam

## 필요 파일
/etc/  

디렉토리에 hostapd.conf 붙여넣기  

/etc/network/  

디렉토리에 interfaces 붙여넣기


## build 방법

cd build;

cmake ..

make

## Cam , Display 설정 파일 
include/config.h

## RKNN, LABEL 설정
/AI_Example/  
폴더에(없으면 생성) Golf_Ball.rknn, Golf_label.txt 복사

## 보드내에 라이브러리가 없는경우 
lib 폴더 하위에 있는 해당 lib파일을 보드에 /lib 경로로 복사할것

## 실행
./start.sh 실행

