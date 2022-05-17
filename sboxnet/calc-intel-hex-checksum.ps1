$i=0;
$f=read-host -prompt "Input filename eep: "
#$f=".\0x300-1.eep"
echo "$f"
get-content "$f" | foreach-object {
	# jede Zeile
	$array = $_.tochararray()
	# start at 1
	$index = 1
	$sum = 0
	# while index < count-1
	while ($index -lt $array.Count-2) {
		# get first char
		$c1 = $array[$index++]
		# and second
		$c2 = $array[$index++]
		# combine it to hexchar
		$h = "0x$c1$c2"
		# convert to int
		#echo $h
		$i = [int]$h
		$sum = $sum + $i
	}
	$checksum = (255-($sum % 256)+1)
	$chshex = [System.Convert]::ToString($checksum,16)
	echo $chshex
}
