# 프로젝트 소개  
VR, PC 플랫폼에서 함께 즐길 수 있는 Cross Play 게임을 개발했습니다.
- 프로젝트 인트로 영상 링크
https://drive.google.com/file/d/1JbYX3U96HZhw-0tanYEmCO697TrBZbtb/view?usp=sharing  

## 환경
* 기간 : 2023.04 ~ 2023.06
* 사용도구 : Unreal Engine5 , Github , C++
* 개발인원 : PC 및 인터랙션 개발 1명, VR 개발 1명, 이펙트 개발 1명 총 3명



## 게임 Concept
### Design Concept
지구온난화로 인해 해수면이 상승해 고지대의 산들이 섬이되는 디스토피아 상황..


### Play Concept


## 게임 규칙
### PC 승리 조건
##### 제한시간내에 VR코어를 파괴
- 다음과 같은 쓰레기를 수집한다.  
  
- Converter에 수집한 쓰레기를 넣어 포탄으로 변환을 한다.
  ![쓰레기변환](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/e3f9390c-93cb-412e-ae46-614994bda9b7)

  
- 변환된 포탄을 습득 후 대포에 장전을 하여 VR Core를 공격한다.  
![대포발사(게임종료위젯생성)](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/f7d9e99e-16c6-4e6e-80ed-d3f0e9ecccbd)

### VR 승리 조건
##### 제한시간 동안 PC 플레이어들이 VR코어를 파괴하지 못하도록 방해하자.



## 게임 Play
### 게임 생성 및 참가 방법
- 리슨서버를 활용해 제작했습니다. (현재 Local 연결만 되도록 설정했습니다.)
- 최대 4명까지 플레이가 가능합니다. (현재는 3명만 플레이가 가능합니다.)
- PC 플레이어만 Host를 할 수 있습니다.
#### PC 플레이어 Host

#### PC 플레이어 Join



#### VR 플레이어 Join
- VR 플레이어의 경우 방에 참가만 할 수 있습니다.

### 조작법
#### PC 플레이어
- W,A,S,D : 이동
- Q : 구르기
- L-Shift : 슬라이딩  
![구르기_슬라이딩](https://github.com/oh-bba-ya/Project_CrossBridge/assets/49023743/9fc7f682-fe70-4cf4-b2b4-5b88116d162a)  
- Space Bar : Fly  
- E : 상호작용  
- R : 쓰레기 봉투 발사  
- G : 아이템 버리기  
- Mouse Left-Click : Fire  
- Mouse Right-Click : Freeze  


  
#### VR 플레이어



