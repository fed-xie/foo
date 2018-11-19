#!/bin/bash
if [ 0 == $# ]; then
	echo "Use argument \"help\" to get valid parameters"
	exit 0
fi
case $1 in
"start")
	iptables -F
	iptables -X
	iptables -P INPUT DROP
	iptables -P FORWARD DROP
	iptables -P OUTPUT ACCEPT
	iptables -A INPUT -s 127.0.0.1 -j ACCEPT
	/usr/local/openresty/bin/openresty
	if [ $? == 0 ]; then
		echo "Run openresty success, remember to open http port"
	fi
	;;
"conf")
	vim /usr/local/openresty/nginx/conf/nginx.conf;;
"reload")
	/usr/local/openresty/bin/openresty -s reload;;
"quit")
	/usr/local/openresty/bin/openresty -s quit;;
"http_on")
	iptables -A INPUT -p tcp --dport 80 -j ACCEPT;;
"http_off")
	iptables -D INPUT -p tcp --dport 80 -j ACCEPT;;
"https_on")
	iptables -A INPUT -p tcp --dport 443 -j ACCEPT;;
"https_off")
	iptables -D INPUT -p tcp --dport 443 -j ACCEPT;;
"ssh_on")
	systemctl start sshd
	iptables -A INPUT -p tcp --dport 22 -j ACCEPT
	iptables -A OUTPUT -p tcp --dport 22 -j ACCEPT
	;;
"ssh_off")
	systemctl stop sshd
	iptables -D INPUT -p tcp --dport 22 -j ACCEPT
	iptables -D OUTPUT -p tcp --dport 22 -j ACCEPT
	;;
"redis_on")
	redis-server /etc/redis/redis.conf;;
*)
	if [ $1 != "help" ]; then echo "Invalid argument $1"; fi
	echo "valid parameters:"
	toolcmds=(
	"start" "conf" "reload"
	"http_on" "http_off"
	"https_on" "https_off" 
	"ssh_on" "ssh_off"
	"redis_on")
	cnt=1
	for tcmd in ${toolcmds[@]}; do
		echo -e "\t${cnt}. $tcmd"
		let "++cnt"
	done
	echo "You can use 'lsof -i -P -n' to see port usage"
	echo "You may need to use \"sudo\" to get permission"
	;;
esac
