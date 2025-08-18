## Functional Level Logging
#### Concept
- 프로그램 내부 단순한 로깅 함수
- File descriptor 또는 표준 출력에 직접 기록
- `O_APPEND` + `write(2)`
#### Features
- 스레드 안전성
	- 멀티스레드 환경: `pthread_mutex` 같은 락으로 쓰기 구간 보호
- 프로세스 간 안전성
	- 멀티 프로세스 환경: `flock` 배타적으로 쓰기 보호
- 시그널 처리
	- `logrotate`: 프로세스에 `SIGHUP` 시그널 발생 $\rightarrow$ `fd` 재오픈 유도
## Dedicated Logging Thread (in-process)
#### Concept
- 로깅 전용 스레드
- Worker thread의 역할: 공용 큐에 `enqueue`만 함.
- Logging thread의 역할: 큐에서 메시지를 꺼내 순서대로 로그 기록
- 스레드 경합과 파일 I/O 부하 최소화
- 고빈도 로그가 찍히는 서버 환경에서 효과적
#### Features
- 비동기화 구조
	- Worker thread가 I/O 대기나 lock 경쟁 없이 로그를 남길 수 있음.
	- 전체 처리량에 미치는 영향이 줄어듦
- 큐 관리
	- Multi-producer-single-consumer(MPSC) 큐를 주로 씀. [MPSC-queue](MPSC-queue)
	- (optional) 락 프리 구현, 원형 버퍼 사용
- 백프레셔 정책
	- 큐가 꽉 차면: 로그 드롭/워커 일시정지/샘플링
- 종료 처리
	- 프로그램 종료 시 큐에 남은 로그 전부 flush
- 핸들러와 충돌 방지
	- 크래시 시그널에서는 전용 스레드에 의존하지 않고 `write`로 바로 출력
## Centralized Logging System (out-of-process/Infra)
#### Concept
- 여러 프로세스/서버의 로그를 한 곳에 모아 관리/검색/모니터링
- 애플리케이션은 로그를 `stdout`, `syslog`, `journald` 등에 남기고, 수집기가 이를 모아 중앙 저장소(`Elasticsearch`, `Loki`, `Splunk`)로 전송
#### Features
- 수집 방식
	- Docker/Kubernetes 환경: 애플리케이션이 `stdout`/`stderr`에 출력 $\rightarrow$ 컨테이너 런타임이 로그 수집 $\rightarrow$ 로깅 드라이버나 사이드카 에이전트 (Filebeat, Fluent Bit)가 중앙 저장소로 전송
	- Linux 호스트 환경: `syslog` API 호출 $\rightarrow$ `syslog` 데몬이 facility/priority에 따라 로그를 파일이나 네트워크로 전달
- 검색/분석
	- 중앙 저장소에서 Kibana, Grafana 같은 UI로 필터링/검색/알람
- 포맷
	- 구조화된 로그(JSON)
	- 파싱/인덱싱 용이
- 개별 로테이션 불필요