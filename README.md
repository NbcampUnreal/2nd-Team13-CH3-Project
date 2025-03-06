# Shadows of the Desert

## 🏜 프로젝트 소개

### 프로젝트 개요

**Shadows of the Desert**는 황량한 사막에서 생존하며 몰려오는 적들을 상대하는 **3D 슈팅 게임**입니다. 언리얼 엔진을 기반으로 제작되었으며, 빠른 템포의 전투와 다양한 무기 시스템을 제공합니다.

### 주요 기능 요약

- **긴장감 넘치는 슈팅 게임**: 사방에서 몰려오는 적을 상대하며 생존하는 게임 플레이
- **다양한 무기 시스템**: 여러 종류의 무기를 전략적으로 활용 가능
- **언리얼 엔진 5 기반**: 현실감 넘치는 그래픽과 최적화된 게임 플레이 경험
- **직관적인 UI**: 플레이어가 쉽게 게임을 이해하고 조작할 수 있도록 설계
- **적 AI 시스템**: AI가 순찰하고 플레이어를 추격하며 공격하는 기능

## 🛠 개발 환경

- **언리얼 엔진**: Unreal Engine 5.5.3
- **프로그래밍 언어**: C++
- **IDE 및 개발 도구**:
  - Visual Studio 2022
  - Unreal Engine Editor
- **버전 관리**: Git & GitHub
- **개발 기간**: 2025.02.17 ~ 2025.03.07

## 🔧 빌드 및 실행 방법

1. 언리얼 엔진 5.5.3을 설치합니다.
2. 프로젝트를 클론합니다:
   ```sh
   git clone https://github.com/your-repository/ShadowsOfTheDesert.git
   ```
3. 언리얼 엔진에서 `.uproject` 파일을 엽니다.
4. `Build` 버튼을 눌러 프로젝트를 빌드합니다.
5. `Play` 버튼을 눌러 게임을 실행합니다.

## 📂 프로젝트 구조

```
ShadowsOfTheDesert
 ┣ Source
 ┃ ┣ ShadowsOfTheDesert  # 메인 게임 로직
 ┃ ┃ ┣ Enemy  # 적 AI 관련 코드 (EnemyCharacterAi, EnemyAIController)
 ┃ ┃ ┣ Character  # 플레이어 캐릭터 관련 코드 (Shadow_of_the_DesertCharacter)
 ┃ ┃ ┣ Weapon  # 무기 시스템 및 탄환 관련 코드 (WeaponBase, BulletBase)
 ┃ ┃ ┣ UI  # 사용자 인터페이스 및 HUD (CustomHUD, DamageTextWidget)
 ┃ ┃ ┗ GameMode  # 게임 모드 및 상태 관리
 ┣ Config  # 게임 설정 파일
 ┣ Content  # 에셋 및 리소스
 ┣ Plugins  # 추가 플러그인
 ┗ README.md  # 프로젝트 설명
```

## 🏹 게임 플레이 방법

- **이동**: WASD 키
- **사격**: 마우스 좌클릭
- **무기 변경**: 숫자 키 (1 - 라이플, 2 - 스나이퍼, 3 - 로켓런처)
- **재장전**: R 키
- **달리기**: Shift 키
- **점프**: Space 키

## 🏗 주요 클래스 및 로직 설명

### 1. 플레이어 캐릭터 시스템

- `Shadow_of_the_DesertCharacter.cpp`
  - 기본적인 이동 및 조작 구현 (WASD 이동, 점프, 달리기)
  - 무기 변경 기능 (`Swap_Rifle`, `Swap_Sniper`, `Swap_Rocket`)
  - 무기 장착 시 탄약 상태 유지 (`EquipWeapon`)
  - 대미지 처리 (`TakeDamage`), 사망 시 게임 종료 연계
  - `Enhanced Input System`을 활용한 입력 처리 (`SetupPlayerInputComponent`)

### 2. 적 AI 시스템

- `EnemyCharacterAi.cpp`
  - 적이 플레이어를 감지하고 공격하는 로직 구현 (`OnHit`, `EnemyTakeDamage`)
  - 적이 사망 시 점수 추가 및 UI 업데이트
  - 히트마커와 킬마커 표시 기능 포함
- `EnemyAIController.cpp`
  - AI의 이동 및 상태를 관리
  - 순찰 및 추격 기능 구현
- `PatrolPath.cpp`
  - AI가 지정된 경로를 따라 이동하도록 구현

### 3. 무기 및 탄환 시스템

- `WeaponBase.cpp`
  - 모든 무기의 기본 클래스
  - 공격, 재장전, 탄환 개수 등 공통 기능 구현
- **각 무기의 특징**
  - `Rifle.cpp`: 빠른 연사 속도의 일반적인 라이플
  - `Sniper.cpp`: 강력한 단발 데미지를 가진 스나이퍼
  - `RocketLauncher.cpp`: 폭발 피해를 주는 로켓런처
- **탄환 시스템**
  - `NomalBullet.cpp`: 기본 총알
  - `PiercingBullet.cpp`: 적을 관통하는 총알
  - `ExplosiveBullet.cpp`: 일정 범위에 폭발 피해를 주는 총알

### 4. UI 및 HUD 시스템

- `CustomHUD.cpp`
  - 조준선, 히트마커, 킬마커 표시 기능 포함
  - 플레이어의 체력, 탄창 수량 등을 HUD에 출력
- `DamageTextWidget.cpp`
  - 적이 맞을 때 화면에 데미지 수치를 표시

### 5. 게임 상태 및 스코어링 시스템

- `Shadow_of_the_DesertGameInstance.cpp`
  - 플레이어가 가한 피해량, 받은 피해량, 총 점수 관리
- `Shadow_of_the_DesertGameState.cpp`
  - 라운드 점수, 웨이브 수, 적 처치 카운트 관리

## 🌟 추가 구현 사항
- **UI 및 게임 흐름 개선**:
  - **이재성 & 이경민**
    - **이경민**: 버튼 기능 연결 및 상호작용 구현
    - **이재성**: 
메인 메뉴 화면 및 버튼 추가

![메인메뉴화면](https://github.com/user-attachments/assets/8d9c0105-e512-40bd-a5a8-b1a005203ffa)

조작법 UI 구현

![조작법화면](https://github.com/user-attachments/assets/12b9b53e-adae-4550-b720-71942c40a6d8)

일시정지 화면 추가

![일시정지화면](https://github.com/user-attachments/assets/5378a473-092b-401a-811c-fe5a528e6946)

게임 클리어 & 게임 오버 화면 추가

![클리어화면](https://github.com/user-attachments/assets/e5154619-0f16-466e-8a96-97dbd4d93404)

![패배화면](https://github.com/user-attachments/assets/ce7651f2-9e68-4377-b37d-e9f8ceca8083)


## ⚠️ 주의사항

- 프로젝트는 **언리얼 엔진 5.5.3** 버전에 최적화되어 있습니다. 다른 버전에서는 정상 작동하지 않을 수 있습니다.
- 그래픽 설정이 높은 경우 낮은 사양의 PC에서는 렉이 발생할 수 있습니다.

## 👥 팀원 소개

- **김영빈**: Enemy 시스템 담당
- **문정수**: Character 시스템 담당
- **정찬효**: Weapon 시스템 담당
- **이재성**: UI 시스템 담당
- **이경민**: GameMode 담당

## 🎮 게임 스크린샷

### 메인 메뉴
![메인 메뉴](./main_menu.png)

### 게임 플레이
![게임 플레이](./gameplay.png)

### 결과 창
![결과 창](./result_screen.png)

---


![jLXVRzis47_tfn1Ge70Jjm_Rsr0KLUnayHQdXfMqZm4ZdcqY5AbHL1mtstVVaITRf29bAx2z9EBlTppUdpl-yUVACcFhGevooQeg-PKLC5S8gXm2QA5vECmiiuVnTCiCoosOPhoKR0zcgfKrMieZ_0rOgLKpEAkHiStWN46nFsf9HGYdynUAbioDyU_se0osx55eOmt0Q1GwaBn_trhVV3](https://github.com/user-attachments/assets/87752f81-1891-4b05-add9-2711da619f32)
