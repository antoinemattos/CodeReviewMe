del code.zip
del outputfile.txt

powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::CreateFromDirectory('code', 'code.zip'); }"

aws lambda update-function-code --function-name source-code-get --region us-east-1 --zip-file fileb://code.zip --publish

aws lambda invoke --invocation-type RequestResponse --function-name source-code-get --region us-east-1 --log-type Tail --payload file://payload.txt outputfile.txt

type outputfile.txt