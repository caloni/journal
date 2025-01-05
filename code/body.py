import subprocess
process = subprocess.run(['gawk', '-f', r'code/body.awk', 'private/body.txt'], check=True)
