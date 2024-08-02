@echo off
call blogbuild.cmd
git add --all
git commit -m "Publishing some shit."
git push
echo Source published
choice /M "Do you want to publish the PUBLIC blog to the world wide web (default yes in 10 seconds)?" /D Y /T 10
if errorlevel 2 goto :eof
pushd public\blog
git add --all
git commit -m "Here comes great news."
git push
popd
