# Turn off the PowerShell execution policy
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass

# Change directory
Set-Location ..

$i = 1
while ($i -le 3) {
    # Create a new directory name
    $new_dir = "cpu-sec-bench-$i"
    # Use Copy-Item to copy the current directory into the new directory
    Remove-Item -Recurse -Force $new_dir #-ErrorAction SilentlyContinue
    Copy-Item -Recurse "cpu-sec-bench" $new_dir
    # Increment i by 1 in each loop
    $i++
}

$currentLocation = Get-Location

$prefix = "G8-Win-none"
$job0 = Start-Job -ScriptBlock{
    Set-Location $using:currentLocation
    Set-Location "cpu-sec-bench" -ErrorAction Stop
    Import-Module .\script\start-job-function.ps1
    
    Write-Output $using:prefix

    run_test "$using:prefix"
} 

$prefix = "G8-Win-heap"
$ind = "1"
$job1 = Start-Job -ScriptBlock{
    Set-Location $using:currentLocation
    Set-Location "cpu-sec-bench-$using:ind" -ErrorAction Stop
    Import-Module .\script\start-job-function.ps1

    Write-Output $using:prefix

    $env:enable_heap_integrity = "yes"

    run_test "$using:prefix"
}

$prefix = "G8-Win-cf"
$ind = "2"
$job2 = Start-Job -ScriptBlock{
    Set-Location $using:currentLocation
    Set-Location "cpu-sec-bench-$using:ind" -ErrorAction Stop
    Import-Module .\script\start-job-function.ps1

    Write-Output $using:prefix

    $env:OPT_LEVEL = 'Od'
    $env:enable_control_flow_protection = "yes"
    $env:enable_extra_stack_protection = "yes"

    run_test "$using:prefix"
}

$prefix = "G8-Win-full"
$ind = "3"
$job3 = Start-Job -ScriptBlock{
    Set-Location $using:currentLocation
    Set-Location "cpu-sec-bench-$using:ind" -ErrorAction Stop
    Import-Module .\script\start-job-function.ps1

    Write-Output $using:prefix

    $env:OPT_LEVEL = 'Od'
    $env:enable_control_flow_protection = "yes"
    $env:enable_extra_stack_protection = "yes"
    $env:enable_heap_integrity = "yes"
    $env:enable_cet_shadow_stack = "yes"

    run_test "$using:prefix"
}

Set-Location "cpu-sec-bench"

Wait-Job -Job $job0, $job1,$job2,$job3

$output1 = Receive-Job -Job $job1
Write-Output "job 1: $output1"

$output2 = Receive-Job -Job $job2
Write-Output "job 2: $output2"

$output3 = Receive-Job -Job $job3
Write-Output "job 3: $output3"