Set Shell bash
Type "myfile is an executable that contains a file."
Sleep 1.5s
Ctrl+A
Ctrl+K

Type "EDITOR=nano ./myfile"
Sleep 1.5s
Enter
Sleep 1.5s
Type "hello world!"
Sleep 1.5s
Ctrl+O
Sleep 1s
Enter
Ctrl+X
Sleep 2s

Type "Executing the file again shows it now contains the text that was just saved."
Sleep 1.5s
Ctrl+A
Ctrl+K

Type "EDITOR=cat ./myfile"
Enter
Sleep 3s

Type "myfile is an actually portable executable, which is itself a zip file."
Sleep 1.5s
Ctrl+A
Ctrl+K

Type "unzip -vl myfile"
Enter
Sleep 2s

Type "unzip myfile x"
Enter
Sleep 1s

Type "cat x"
Enter
Sleep 3s
