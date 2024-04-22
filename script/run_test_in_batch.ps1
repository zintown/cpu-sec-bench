param(
    [Parameter(Mandatory=$true)]
    [string]$csv_file,
)

if ($args.Count -ne 1) {
    Write-Host "Usage: $PSCommandPath <csv_file>"
    exit 1
}

$collect_dir = "results"

Set-Location -Path ..
Remove-Item -Path $collect_dir -Recurse -Force
New-Item -Path $collect_dir -ItemType Directory
Set-Location -Path "cpu-sec-bench"

$one_run_ind = 0
Import-Csv $csv_file | ForEach-Object {
    $prefix = $_.prefix
    $rest_columns = $_.rest_columns

    Write-Output "prefix: $prefix"
    Write-Output "rest_columns: $rest_columns"
Start-Job -ScriptBlock{
    if ($one_run_ind -ne 0) {
        $new_dir = "cpu-sec-bench-$using:one_run_ind"
        Set-Location -Path ..
        Remove-Item -Path $new_dir -Recurse -Force
        Copy-Item -Path "cpu-sec-bench" -Destination $new_dir -Recurse
        Set-Location -Path $new_dir
    }
    Import-Module .\script\start-job-function.ps1

    export_func "$using:rest_columns"
    run_test "$using:prefix"
}
    $one_run_ind++
}

Write-Host "Waiting for all processes to finish..."
Get-Process | Where-Object {$_.Name -eq "powershell"} | Wait-Process