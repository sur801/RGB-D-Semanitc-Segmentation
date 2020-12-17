from rknn.api import RKNN
import numpy as np
import os
import re
import math
import random
import cv2
os.environ["CUDA_DEVICE_ORDER"]="PCI_BUS_ID"   
os.environ["CUDA_VISIBLE_DEVICES"]="1"
#create RKNN object
#rknn = RKNN()
rknn = RKNN(verbose=True)
rknn.config(channel_mean_value='127.5 127.5 127.5 127.5#0.0 0.0 0.0 100.0',
           reorder_channel='2 1 0#2 1 0',
           need_horizontal_merge=True)
# rknn config
# 입력된 이미지의 R, G, B 3채널 값에 대해 각각 127.5를 빼주고 127.5로 나눠서 정규화하는 이미지
# 전처리를 수행해주겠다는 의미


#Load model
print('--> Loading model') #로딩하는 중임을 알려주는 출력
#freeze된 pb파일, input 노드, output노드, input 사이즈 리스트 입력
#노드 확인은 tensorboard, netron을 활용

ret = rknn.load_tensorflow(tf_pb='./bisenetv2_cityscapes_frozen_new.pb',
              inputs=['input_tensor1','input_tensor2'],
              outputs=['BiseNetV2/prediction'],
              input_size_list=[[320,640,3],[320,640,3]])

if ret !=0:
  print('Load failed!')
  exit(ret)
#Load 완료시 'done'출력
print('done')

#Build model
print('--> Building model') #빌드하는 중임을 알려주는 출력
#ret = rknn.build(do_quantization=False)
ret = rknn.build(do_quantization=True, dataset='./qauntization/dataset.txt')
#오류 발생시 오류 메시지 출력
if ret !=0:
    print('Build failed!')
    exit(ret)
#Build 완료시 'done'출력
print('done')

#Export rknn model
print('--> Export RKNN model')
ret = rknn.export_rknn('./bisenetv2_new_quant.rknn') #다음과 같은 이름으로 저장됩니다.
#오류 발생시 오류 메시지 출력
if ret != 0:
    print('Export failed!')
    exit(ret)
#export 완료시 'done'출력
print('done')