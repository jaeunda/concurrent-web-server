## Checklist (Function-level)
- [ ] `mutex` + `flock` + 단일 `write`
	- lock은 짧게: `vsnprintf`은 lock 밖에서, `write`만 lock 안에서
- [ ] 로그 라인 길이 상한 & 개행 보장
- [ ] Log rotation 대응 (`SIGHUP` handler: `fd` 재오픈 )
- [ ] Prefix: `timestamp`, `LOG_LEVEL`, `PID`, `TID`
- [ ] Crash 경로에서는 (signal handler) `mutex` 금지, `write`만 사용
	- X - 포맷팅/버퍼링/lock
	- 미리 준비된 간단한 문자열을 `write(fd, msg, len)` 한 번에 쓰고 `exit`
	- async-signal-unsafe라서 deadlock 또는 메모리 오염 위험
- [ ] (optional) 종료 시 `SIGTERM`/`SIGINT` 핸들러에서 `flush`/`close`
- [ ] (optional) file logging + `syslog` 하이브리드
- [ ] (optional: docker) Logging sink(목적지) 주입 가능하도록 설계
	- 개발/컨테이너: `STDOUT_FILENO` or `STDERR_FILENO`
	- host: `fd` + `logrotate` + `SIGHUP`

---

## Reference
- [Log rotation: `SIGHUP`](notes/SIGHUP-log-rotation.md)
- [Logging System](notes/logging-system.md)
