# 프로젝트 소개  
VR, PC 플랫폼에서 함께 즐길 수 있는 Cross Play 게임을 개발했습니다.
 ![메인사진](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/671c686a-2f12-4dc9-a8bc-e8ae841655e0) 
- 프로젝트 인트로 영상 링크  
https://drive.google.com/file/d/1JbYX3U96HZhw-0tanYEmCO697TrBZbtb/view?usp=sharing  

## 환경
* 기간 : 2023.04 ~ 2023.06
* 사용도구 : Unreal Engine5 , Github , C++

## 팀 소개
* 개발인원 : PC 및 인터랙션 개발 1명, VR 개발 1명, 이펙트 개발 1명 총 3명 
![02](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/c9370fbb-961d-406c-800f-39174fecf44e)


## 게임 Concept
### Design Concept
* 지구온난화로 인해 해수면이 상승해 고지대의 산들이 섬이되는 디스토피아 상황..
* 해양에서 살아가는 사람들은 섬에서 나온 쓰레기를 주워 생활하거나 바다 깊이 가라앉은 자원을 채취해 살아가며 섬을 점령하기 위해 쓰레기를 재활용한 무기를 통해 섬을 침략하는 컨셉입니다.
* 자원이 부족한 시대인 만큼 쓰레기를 재활용한 무기를 사용합니다.
![크로스브릿지 게임디자인](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/d7abe1d6-87a1-4484-93da-1abdf751e8ea)

### Play Concept
* VR 1명 vs PC 3명 최대 4명이서 즐길 수 있는 대전 게임
* VR 과 PC 플랫폼의 플레이 방식의 차이가 존재해 차이를 하나의 게임의 플레이 콘셉트로 설정하기 위해 다윗과 골리앗 , 진격의 거인과 같은 자료를 참고하여 거인 vs 소인이라는 플레이 콘셉트가 설정되었습니다. 
![플레이컨셉](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/f47fe69d-0379-4676-809b-9ea39453ff5a)

## 게임 규칙
### PC 승리 조건
##### 제한시간내에 VR코어를 파괴
- 다음과 같은 쓰레기를 수집한다.  
![쓰레기수집](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/b19055df-726b-4f10-9ee8-27bb1021aac2)
  
- Converter에 수집한 쓰레기를 넣어 포탄으로 변환을 한다.  
![쓰레기변환](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/e3f9390c-93cb-412e-ae46-614994bda9b7)

  
- 변환된 포탄을 습득 후 대포에 장전을 하여 VR Core를 공격한다.  
![대포발사(게임종료위젯생성)](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/f7d9e99e-16c6-4e6e-80ed-d3f0e9ecccbd)

### VR 승리 조건
##### 제한시간 동안 PC 플레이어들이 VR코어를 파괴하지 못하도록 방해해야 합니다.
- VR 플레이어는 스킬을 사용해 PC 플레이어들을 방해합니다.
#### <img src="https://github.com/oh-bba-ya/Project_CrossBridge/assets/121008852/ebb01f25-9f81-4213-b825-1cceb96c7977" width="35%" height="35%"/>

- VR 플레이어가 승리한다면 다음과 같이 VR Core에서 레이저가 발사되어 도시의 방어막이 파괴됩니다. 
 ![VR플레이어 Win](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/84746e31-80c4-48b9-937b-bfd74222aa15)



## 게임 Play
### 게임 생성 및 참가 방법
- 리슨서버를 활용해 제작했습니다. (현재 Local 연결만 되도록 설정했습니다.)
- 최대 4명까지 플레이가 가능합니다. (현재는 3명만 플레이가 가능합니다.)
- PC 플레이어만 Host를 할 수 있습니다.
#### PC 플레이어 Host 
![PC플레이어 Host](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/8d98bc28-1bf9-4c5f-8f8b-3b487e67ea21)
#### PC 플레이어 Join 
![PC플레이어 Join](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/a20d0477-f33a-4ad7-9d23-9008dbc7a61b)

#### VR 플레이어 Join
- VR 플레이어의 경우 방에 참가만 할 수 있습니다.

### 조작법
#### PC 플레이어
- W,A,S,D : 이동
- Space Bar : Fly  
  ![PC플레이어 Movement](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/7f032a9a-9875-4c74-9d16-118b1392606b)
- Q : 구르기
- L-Shift : 슬라이딩  
![구르기_슬라이딩](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/9fc7f682-fe70-4cf4-b2b4-5b88116d162a)

(VR 플레이어 시점에서 바라본 블랙홀)  
![블랙홀탈출](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/a2e705d7-3bcb-4815-934c-83d609ce2ab3)
- E : 상호작용 (PC 플레이어가 상호작용할 수 있는 액터들의 경우 E 위젯이 활성화 됩니다.)  
  ![PC플레이어변환기사용](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/e3d107a7-2efc-4e6b-a0d9-9c79aae03f5e)

- R : 쓰레기 봉투 발사  
  ![PC플레이어 쓰레기봉투](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/150c2e06-865c-4344-9877-d306737005bc)
- G : 장착 무기 버리기(PC 플레이어는 총 or 포탄 1개의 아이템만 장착할 수 있습니다.)  
  ![PC플레이어 무기버리기및포탄장착](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/bab8c0c1-33a7-4346-be68-45504f889fb8)
- Mouse Left-Click : Fire  
![총알발사](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/a455a04f-0edd-4e38-b689-69d6b6885e4f)

- Mouse Right-Click : Freeze  
  ![PC플레이어 Freeze](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/d1f1f5ca-6128-4820-a0ee-0ff4c514aaba)
![PC플레이어 Freeze2](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/320d2296-7bba-4050-8a3e-7a4b7ccc85ad)

- PC 플레이어는 사망시 15초 후에 다시 리스폰 됩니다.  
  ![PC플레이어 리스폰](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/0816eef3-fca0-4ed3-9132-1835f4afaf2f)

#### VR 플레이어
* VR 플레이어는 기본적으로 스킬을 사용할때마다 손의 색깔이 변화하며 컨트롤러에 진동이 있습니다.
* VR 플레이어는 특정 자세를 만족해야만 활성화되는 스킬들이 존재합니다.
* 데미지, 회복, 부활과 같이 VR 플레이어의 체력이 변화하는 경우 그에 맞춰 VR 플레이어의 머리 색도 변화합니다.
  
- BlackHole : PC 플레이어들의 이동을 방해하는 스킬입니다.    
####  ![BlackHole](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/bf41fb4e-99d5-4267-b55f-a798e9049699)
  
- Bullet : 가장 기본적인 원거리 공격입니다.
####  ![Bullet](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/51d7f3b4-51d1-4a26-8c76-dcad7407fbf0)
  
- Sword : 두 손을 나란히 하여 광선검을 활성화합니다.
#### ![Sword](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/7f8a91e0-38c5-4717-8e2a-a4e1f22adff0)
  
- Trash : 광역 공격 스킬입니다.
####  <img src="https://github.com/oh-bba-ya/Project_CrossBridge/assets/121008852/5ef0ed6e-f783-46a2-ac03-024c7bbf1439" width="40%" height="40%"/>

- Healing : 두 손을 모아 체력을 회복합니다.  
####  ![Healing](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/10a524ff-2aa9-4a59-a920-a85a53121983)

- Damage : 피해를 받으면 나타나는 시각 효과입니다.  
####  ![Damage](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/b15d1ec9-b3c6-4934-88fd-0728be3c2494)

- Revive : VR 플레이어가 사망하면 바닥으로 떨어지고 이후 부품들이 천천히 부활 위치로 이동하여 다시 부활합니다.
####  ![Revive](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/3d58aacc-a5a1-4e74-92d1-516fa8ad5aec)

- Status Widget : VR 플레이어의 체력, 스킬 쿨타임 및 남은 경기시간을 알수 있는 위젯입니다.
####  <img src="https://github.com/oh-bba-ya/Project_CrossBridge/assets/121008852/28f029de-e638-423f-8fc1-ede239d88748" width="40%" height="40%"/>

- Left Thumbstick : 이동
- Right Thumbstick : 위아래 이동 및 좌우 회전
