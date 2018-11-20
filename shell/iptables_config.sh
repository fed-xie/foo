#!/bin/bash
if [ `whoami` != "root" ]; then
	echo "You need to switch to root"
	exit 1
fi
read -p "Before config iptables, do you need to backup your old configure? [yes|no]" yn
case $yn in
	y|yes)
		printf "Backup files to './backup'\n"
		if [ ! -d ".backup" ]; then
			mkdir backup
		fi
		iptables-save > backup/iptables.rules.backup
		ip6tables-seve > backup/ip6tables.rules.backup
		;;
	*)
		;;
esac

iptables_rules='*filter
:INPUT DROP [0:0]
:FORWARD DROP [0:0]
:OUTPUT ACCEPT [0:0]
-A INPUT -s 127.0.0.1/32 -j ACCEPT
-A FORWARD -s 127.0.0.1/32 -j ACCEPT
-A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
COMMIT
'
echo "$iptables_rules" > /etc/iptables.rules

ip6tables_rules='*filter
:INPUT DROP [0:0]
:FORWARD DROP [0:0]
:OUTPUT ACCEPT [0:0]
-A INPUT -s ::1/128 -j ACCEPT
-A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
-A FORWARD -s ::1/128 -j ACCEPT
COMMIT
'
echo "$ip6tables_rules" > /etc/ip6tables.rules

restore_shell='#!/bin/bash
iptables-restore < /etc/iptables.rules
'
echo "$restore_shell" > /etc/network/if-pre-up.d/iptables
chmod +x /etc/network/if-pre-up.d/iptables

restore6_shell='#!/bin/bash
ip6tables-restore < /etc/ip6tables.rules
'
echo "$restore6_shell" > /etc/network/if-pre-up.d/ip6tables
chmod +x /etc/network/if-pre-up.d/ip6tables

read -p "Apply now? [yes/no]" yn
case $yn in
	y|yes)
		iptables-restore < /etc/iptables.rules
		ip6tables-restore < /etc/ip6tables.rules
		;;
	*)
		;;
esac
