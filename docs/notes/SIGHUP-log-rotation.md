## Log rotation
- 일정 주기마다 로그 파일 교체(rotate)
- `/var/log/myserver.log` $\rightarrow$ `/var/log/myserver.log.1`, 새 로그는 빈 `/var/log/myserver.log`
- 자동화 도구: `logrotate`, `newsyslog`
	- 새 로그 파일 생성
	- `SIGHUP` 시그널 발생 $\rightarrow$ 파일 교체하여 새 파일에 쓰도록 유도

## `SIGHUP` 재오픈
- 운영체제는 파일 이름이 아닌 **`inode`에 대한 `fd`** 를 잡음.
- $\therefore$ `SIGHUP` 시그널을 받으면 로그 파일을 닫았다가 다시 열도록 만듦. (파일 이름은 같지만 다른 `inode`)

### Log rotation 대응
1. `logrotate`가 `mv myserver.log myserver.log.1`
2. empty `myserver.log` 생성
3. `logrotate`: `kill -HUP <pid>` 실행 (`SIGHUP` 시그널 발생 )
4. 서버 프로세스: `SIGHUP` handler에서 `close(fd)` $\rightarrow$ `open("myserver.log")` 실행
5. 이후 로그는 새 파일에 들어감

### `logrotate`
- See details: `man logrotate`
#### 역할
- 로그 파일 크기 제한
- 주기적 회전: 주기에 맞춰 새 로그 파일 시작
- 보관 개수 관리: 오래된 로그 파일 자동 삭제 (`rotate N`)
- 압축: 오래된 로그를 압축
- `postrotate`: `SIGHUP` 시그널
```
postrotate
	kill -HUP $(< /var/run/myserver.pid)
endscript
```
#### 설정
- 전역 설정: `/etc/logrotate.conf`
```
# see "man logrotate" for details

# global options do not affect preceding include directives

# rotate log files weekly
weekly

# use the adm group by default, since this is the owning group
# of /var/log/.
su root adm

# keep 4 weeks worth of backlogs
rotate 4

# create new (empty) log files after rotating old ones
create

# use date as a suffix of the rotated file
#dateext

# uncomment this if you want your log files compressed
#compress

# packages drop log rotation information into this directory
include /etc/logrotate.d

# system-specific logs may also be configured here.
```
- 개별 설정/로그 경로 지정: `/etc/logrotate.d/*`
```
$ cat /etc/logrotate.d/rsyslog
/var/log/syslog
/var/log/mail.log
/var/log/kern.log
/var/log/auth.log
/var/log/user.log
/var/log/cron.log
{
        rotate 4
        weekly
        missingok
        notifempty
        compress
        delaycompress
        sharedscripts
        postrotate
                /usr/lib/rsyslog/rsyslog-rotate
        endscript
}
```