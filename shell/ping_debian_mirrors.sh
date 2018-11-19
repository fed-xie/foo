#!/bin/bash
mirror_sites=(
"ftp2.cn.debian.org"
"ftp.cn.debian.org"
"mirror.lzu.edu.cn"
"mirrors.163.com"
"mirrors.huaweicloud.com"
"mirrors.shu.edu.cn")
for site in ${mirror_sites[@]}; do
	ping $site -c 4
	echo ""
done

