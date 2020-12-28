# RGB, Depth map Deep Fusion for Semantic Segmentation

  

**프로젝트 명 :** RGB, Depth map Fusion을 통한 Semantic Segmentation  

**프로젝트 기간 :** 2020.12.11 ~ 2020.12.17

**내용 :** City Scape Data 의 RGB 이미지와 Depth map을 딥러닝 네트워크를 통해 융합. 융합한 데이터로 
Semantic Segmentation

**개발 환경 :** 
Anaconda, Python3.6, Tensorflow1.14, OpenCV, C++, Rex (AI Computing Embedded Computer)



**사용한 모델 :**

- BiSeNet v2 모델을 기반으로 함. 
- 변형한 BiSeNet v2의 구조
  - 기존 모델의 Depth map 이미지를 입력 받는 Depth Branch를 추가.
  - RGB 이미지와 Depth map을 융합한 결과를 이용해 Semantic Segmentation.


</br>

## BiSeNet v2 모델 구조

 ![image](https://user-images.githubusercontent.com/5088280/103218089-747bf900-495d-11eb-90cc-e241424979dc.png)

<img src="https://user-images.githubusercontent.com/5088280/103217958-3252b780-495d-11eb-99ca-fbf9ed0a08a2.png" alt="image" width="500" />

- 크게 Semantic Branch와 Detail Branch로 나뉘어 있음.
- 각 Branch의 결과물을 융합한 값을 이용함. 
- 최종적인 결과물로, 각 픽셀 단위로 어떤 물체인지 분류
- Mean IoU : 약 70%


</br>

## 변형한 BiSeNet v2 모델 구조

<img src="https://user-images.githubusercontent.com/5088280/103218009-4991a500-495d-11eb-8483-49fb6c58ffcb.png" alt="image" width="500" />



- 크게 Semantic Branch, Detail Branch, Depth Branch로 나뉘어 있음.
- RGB 이미지의 Feature와 Depth map 이미지의 Feature를 Element-Wise Add로 융합 
- Detail Branch와 Depth Branch 융합 결과를 최종적으로 Semantic Branch의 결과와 융합
- Mean IoU 값이 증가하지는 않음.
- **멀리** 있는 물체나, **그림자 안에 있는 물체** 에 대해서는 Semantic Sementaion이
   조금 더 **뚜렷하게 됨**




</br>

## 기존 BiSeNet v2와, 변형한 BiSeNet v2 모델 비교 

<img src="https://user-images.githubusercontent.com/5088280/103218973-cc1b6400-495f-11eb-8277-c98d81737fbc.png" alt="image" width="400" />


기존 BiSeNet v2의 결과 		   				/ 				   		변형한 BiSeNet v2의 결과

<img src="https://user-images.githubusercontent.com/5088280/103218990-d6d5f900-495f-11eb-84a6-f755f01b14d9.png" alt="image" width="400" /><img src="https://user-images.githubusercontent.com/5088280/103219016-ece3b980-495f-11eb-9a7c-a614948bf59f.png" alt="image" width="400" />
