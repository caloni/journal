import subprocess
process = subprocess.run(['gawk', '-f', r'scripts/body.awk', 'body.txt'], check=True)
