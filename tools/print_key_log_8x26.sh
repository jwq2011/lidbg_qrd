
cd "$1"
rm -rf ./lidbg_log
mkdir lidbg_log
IFS=$'\n';
for i in * 
do

	echo --------------------------$i--------------------------------------------  >>  ./lidbg_log/reset_count.txt
	cat $i | grep "Linux version" >>  ./lidbg_log/reset_count.txt

	echo --------------------------$i--------------------------------------------  >>  ./lidbg_log/kill_process.txt
	cat $i | grep "killing any children in process group" >>  ./lidbg_log/kill_process.txt

	echo --------------------------$i--------------------------------------------  >>  ./lidbg_log/lowmemkill.txt
	cat $i | grep "send sigkill to" >> ./lidbg_log/lowmemkill.txt


	echo --------------------------$i--------------------------------------------  >>  ./lidbg_log/force_unlock.txt
	cat $i | grep -E "force_unlock|ftf_pm.wl" >> ./lidbg_log/force_unlock.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/acc_count.txt
	cat $i | grep "7.suspend_devices_and_enter" >>  ./lidbg_log/acc_count.txt


	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/WARNING.txt
	cat $i | grep "WARNING: at" >>  ./lidbg_log/WARNING.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/unhandled.txt
	cat $i | grep "unhandled page" >>  ./lidbg_log/unhandled.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/lidbgerr.txt
	cat $i | grep "lidbgerr" >>  ./lidbg_log/lidbgerr.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/slimbuserr.txt
	cat $i | grep "slimbus Read error" >>  ./lidbg_log/slimbuserr.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/lpc.txt
	cat $i | grep "LPC reset reason" >>  ./lidbg_log/lpc.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/Watchdog.txt
	cat $i | grep "Watchdog bark" >>  ./lidbg_log/Watchdog.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/rejecting_io.txt
	cat $i | grep "rejecting I/O to offline device" >>  ./lidbg_log/rejecting_io.txt

	echo ---------------------------$i---------------------------------------------  >> ./lidbg_log/keyword.txt
	cat $i | grep "find key word" >>  ./lidbg_log/keyword.txt

	cat $i | grep -E "send sigkill to|killing any children in process group|Linux version|force_unlock|WARNING: at|slimbus Read error|unhandled page|lidbgerr|LPC reset reason|overflow error|msm_gpio_show_resume_irq|rejecting I/O to offline device|Watchdog bark|find key word|dsi83.check.err|ftf_pm.wl" >>  ./lidbg_log/$i
done

	date >>  ./lidbg_log/finish.txt

