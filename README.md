# CCDC
## 창의적종합설계경진대회(CreativeComprehensiveDesignCompetition) 출품작</br></br>



### 사용 기술 : arduino uno, exprexx(Node.js), MySQL</br>
### 2019 INU 창의적 종합 설계 : 금상 수상</br>
### 2019 INU 창업경진대회 : 최우수상 수상</br>
### 2019 창의적 종합설계 : 장려상 수상</br>
</br>
### 나의 역할
</br>
1. 빛으로 데이터를 전송하는 알고리즘 설계</br>
2. 빛 데이터를 아두이노로 수신 및 서버에 http통신을 이용한 데이터 요청 설계

</br>


<div>
<img src="https://user-images.githubusercontent.com/35087350/97012823-3dffb780-1583-11eb-885e-6c13f43f4e1c.png" width="45%"></img>
<br>

송신기와 수신기가 각각 DB로 부터 데이터를 가져와서 키 값을 비교하는 방식이다.
송신기에서 수신기로 키 값을 전달할 때, 보안을 보다 강화하고자 했다. 
이때, 보안 방법은 간단하게 서버에서 OTP처럼 주기적으로 키 값이 바뀌게 하는 것과 표준으로 많이 사용되는 AES128을 사용했다.

<img src="https://user-images.githubusercontent.com/35087350/97012828-3f30e480-1583-11eb-8124-7ff397378e62.png" width="45%"></img>
<br>

송신기는 누구나 들고다니는 스마트폰으로 했다.

사진처럼 Flutter로 앱을 하나 만들었다. 이 앱은 서버로부터 키값을 받아와서 빛 신호로 변환해주는 어플이다.

</div>
