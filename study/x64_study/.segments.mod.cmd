savedcmd_/home/runner/ctf/study/x64_study/segments.mod := printf '%s\n'   segments.o | awk '!x[$$0]++ { print("/home/runner/ctf/study/x64_study/"$$0) }' > /home/runner/ctf/study/x64_study/segments.mod