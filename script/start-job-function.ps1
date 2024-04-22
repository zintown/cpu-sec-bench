function run_test {
    param (
        [string]$prefix,
        [string]$result_dir
    )

    make cleanall > temp.log 2>&1
    make -e >> temp.log 2>&1
    ./run-test exhausted-run >> temp.log 2>&1
    $base_name = rename_log
    $final_name = "${prefix}_$base_name"
    Write-Output $final_name
    Move-Item -Path temp.log -Destination "$final_name.log"
    Move-Item -Path "$base_name.dat" -Destination "$final_name.dat"

    collect_results ..\$result_dir
}

function collect_results {
    param (
        [string]$result_dir
    )
    # Find and copy .dat files
    Get-ChildItem -Recurse -Filter *.dat | Copy-Item -Destination $result_dir

    # Find and copy .log files
    Get-ChildItem -Recurse -Filter *.log | Copy-Item -Destination $result_dir

}

function export_func {
    param (
        [string]$exported_vars
    )
    # Split rest_columns with comma into an array
    $columns_array = $exported_vars -split ','

    # Iterate over the columns array
    foreach ($column in $columns_array) {
        Write-Output "\$env:$column"
        $env:$column
    }
}