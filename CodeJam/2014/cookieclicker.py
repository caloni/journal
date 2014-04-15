import sys

def CookieClicker(farmCost, farmIncrement, cookieTarget):
	cookiePerSecond = 2.0
	bestTime = cookieTarget / cookiePerSecond # melhor tempo soh fazendo cookies
	cookieFarmTime = cookieTarget / (cookiePerSecond + farmIncrement) # melhor tempo ja com fazenda criada
	farmTime = farmCost / cookiePerSecond + cookieFarmTime # quanto vai custar fazer a fazenda e depois fazer cookies com a fazenda
	while farmTime < bestTime: # enquanto fazer a fazenda custar menos tempo que soh fazer cookies...
		bestTime = farmTime # por enquanto melhor tempo
		cookiePerSecond = cookiePerSecond + farmIncrement # novo tempo para fazer cookies (mais uma fazenda ja criada)
		farmTime = farmTime - cookieFarmTime # tiramos o tempo de soh fazer cookies para fazer mais uma fazenda
		cookieFarmTime = cookieTarget / (cookiePerSecond + farmIncrement) # novo tempo com mais uma fazenda criada
		farmTime = farmTime + farmCost / cookiePerSecond + cookieFarmTime # agora com o novo tempo de fazer outra fazenda e soh cookies
	return bestTime

f = open(sys.argv[1])
total = int(f.readline())

for case in range(1, total + 1):
    args = [float(i) for i in f.readline().split()]
    ret = CookieClicker(args[0], args[1], args[2])
    print 'Case #' + str(case) + ': ' + '{0:.7f}'.format(ret)
